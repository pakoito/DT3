//==============================================================================
///	
///	File: DTPortalNetwork.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DTPortalSDK/DTPortalLib/DTPortalNetwork.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"
#include "DTPortalSDK/DTPortalLib/DTPortalCommon.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

extern "C" {
    #include "tomcrypt.h"
};

#define NEXT_16(x) ((x + 15) & ~15)

//==============================================================================
//==============================================================================

namespace DTPortal {

//==============================================================================
//==============================================================================

void DTPortalNetwork::initialize (void)
{

}
    
void DTPortalNetwork::uninitialize (void)
{

}
    
//==============================================================================
//==============================================================================

DTPerror DTPortalNetwork::post (std::string host, std::string port, std::string location, std::string contents, std::string &response)
{
	struct addrinfo hints, *res;

    //
	// Resolve address info
    //
    
	::bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int err = ::getaddrinfo( host.c_str(), port.c_str(), &hints, &res);
	if (err != 0) {
		return DTP_NO_CONNECTION;
	}
    
    //
    // Open the TCP/IP connection to the web server
    //
        
    // Build the socket
    int socket_raw = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_raw < 0) {     
        ::freeaddrinfo(res);
        return DTP_NO_CONNECTION;
    }
        
    err = ::connect(socket_raw, res->ai_addr, res->ai_addrlen);
    if (err != 0) {
        ::freeaddrinfo(res);
        ::close(socket_raw);
        return DTP_NO_CONNECTION;
    }
        

    //
    // Send the request
    //
    
    std::string url_encoded = "request=" + url_encode(contents);
    
    // We aren't using a library because we're doing simple things
    std::stringstream request;
    request << "POST /" << location << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    request << "Connection: close\r\n";
    request << "Content-Type: application/x-www-form-urlencoded\r\n";
    request << "Content-Length: " << url_encoded.size() << "\r\n";
    request << "\r\n";
    request << url_encoded;
    
    ssize_t size = ::send(socket_raw, request.str().c_str(), request.str().size(), 0);
    if (size < 0) {
        ::freeaddrinfo(res);
        ::close(socket_raw);
        return DTP_NO_CONNECTION;
    }


    //
    // Receive the response
    //
    
    response.clear();
        
    std::string buffer;
    buffer.resize(1024);
    
    
    while (1) {
        ssize_t size = ::recv(socket_raw, &buffer[0], buffer.size(), 0);
        
        if (size == 0)
            break;
        
        if (size < 0) {
            ::freeaddrinfo(res);
            ::close(socket_raw);
            return DTP_NO_CONNECTION;
        }
        
        response += buffer.substr(0,size);
    }
    
    // Clip off the header
    response = response.substr(response.find("\r\n\r\n") + 4);

    //
    // Close the connection
    //

    ::freeaddrinfo(res);
    ::close(socket_raw);

    return DTP_NO_ERROR;
}

//==============================================================================
//==============================================================================

void DTPortalNetwork::encode (std::string decoded, std::string &checksum, std::string &encoded)
{
    //
    // Hash
    //
    
    hash_state md;
    unsigned char tmp[32];

    // Generate hash
    sha256_init(&md);
    sha256_process(&md, (unsigned char*)&decoded[0], decoded.size());
    sha256_done(&md, tmp);

    // Convert to Base64
    unsigned long outlen = sizeof(tmp) + sizeof(tmp)/3 + 16;
    checksum.resize(outlen, 0);
    
    ::base64_encode(tmp, sizeof(tmp), (unsigned char*)&checksum[0], &outlen);
    checksum.resize(outlen);

    
    //
    // Encode
    //
    
    // Encrypt
    symmetric_key k;
    aes_setup(DTPortalCommon::shared_secret_as_bytes(), 128/8, 0, &k);
    
    // Pad up to next 8 bytes
    std::size_t next_size = NEXT_16(decoded.size());
    decoded.resize(next_size, 0);

    std::string encrypted;
    encrypted.resize(decoded.size(),0);
    
    for (DTPuint i = 0; i < encrypted.size(); i+=16) {
        aes_ecb_encrypt((DTPubyte*) &decoded[i], (DTPubyte*) &encrypted[i], &k);
    }
    
    aes_done(&k);
    
    outlen = encrypted.size() + encrypted.size()/3 + 16;
    encoded.resize(outlen, 0);
    
    ::base64_encode((unsigned char*) &encrypted[0], encrypted.size(), (unsigned char*) &encoded[0], &outlen);
    encoded.resize(outlen, 0);

}

DTPerror DTPortalNetwork::decode (std::string encoded, std::string checksum, std::string &decoded)
{

    //
    // Decode
    //
    
    unsigned long outlen = encoded.size();
    
    decoded.resize(outlen, 0);
    
    ::base64_decode((unsigned char*) &encoded[0], encoded.size(), (unsigned char*) &decoded[0], &outlen);
    decoded.resize(outlen, 0);
    
    // Make sure decoded length is multiple of blocksize
    if (outlen != NEXT_16(outlen))
        return DTP_DECODING_ERROR;
   
    // Decrypt
    symmetric_key k;
    if (aes_setup(DTPortalCommon::shared_secret_as_bytes(), 128/8, 0, &k) != CRYPT_OK)
        return DTP_DECODING_ERROR;
    
    std::string decrypted;
    decrypted.resize(decoded.size(), 0);
    
    for (DTPuint i = 0; i < decrypted.size(); i+=16) {
        if (aes_ecb_decrypt((DTPubyte*) &decoded[i], (DTPubyte*) &decrypted[i], &k) != CRYPT_OK) {
            aes_done(&k);
            return DTP_DECODING_ERROR;
        }
    }
    
    aes_done(&k);

    //
    // Hash
    //
    
    hash_state md;
    unsigned char tmp[32];

    // Generate hash
    sha256_init(&md);
    sha256_process(&md, (unsigned char*)&decrypted[0], decrypted.size());
    sha256_done(&md, tmp);

    // Convert to Base64
    outlen = sizeof(tmp) + sizeof(tmp)/3 + 16;
    
    std::string test_checksum;
    test_checksum.resize(outlen, 0);
    
    ::base64_encode(tmp, sizeof(tmp), (unsigned char*)&test_checksum[0], &outlen);
    test_checksum.resize(outlen);
    
    if (test_checksum != checksum)
        return DTP_DECODING_ERROR;
    
    decoded = decrypted;

    return DTP_NO_ERROR;
}

//==============================================================================
// From http://www.geekhideout.com/urlcode.shtml
//==============================================================================

std::string DTPortalNetwork::url_encode(const std::string &value) {

    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        }
        else if (c == ' ')  {
            escaped << '+';
        }
        else {
            escaped << '%' << std::setw(2) << ((int) c) << std::setw(0);
        }
    }

    return escaped.str();
}

//==============================================================================
//==============================================================================

} // DTPortal
