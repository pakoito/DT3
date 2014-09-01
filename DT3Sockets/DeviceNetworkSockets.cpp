//==============================================================================
///	
///	File: DeviceNetworkSockets.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Sockets/DeviceNetworkSockets.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Network/NetworkAddress.hpp"
#include "DT3Core/Types/Network/NetworkSocket.hpp"
#include "DT3Core/Types/Network/NetworkPacket.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

#include <unistd.h>

#if DT3_OS != DT3_ANDROID
    #include <net/if.h>
    #include <net/if_dl.h>
    #include <sys/sysctl.h>
#endif

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceNetworkSockets)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceNetworkSockets::DeviceNetworkSockets (void)
{

}
			
DeviceNetworkSockets::~DeviceNetworkSockets (void)
{

}

//==============================================================================
//==============================================================================

void DeviceNetworkSockets::initialize (void)
{
	DeviceNetwork::initialize();

}

//==============================================================================
//==============================================================================

DTerr DeviceNetworkSockets::resolve_host (NetworkAddress &ip, const std::string &host, DTushort port)
{
	std::string real_host = Globals::substitute_global(host);

	struct addrinfo hints, *res;

	// get address info
	::bzero(&hints, sizeof(hints));
#if defined(DT3_USE_IPV6) && DT3_USE_IPV6
	hints.ai_family = AF_UNSPEC;
#else
	hints.ai_family = AF_INET;
#endif
	hints.ai_socktype = SOCK_STREAM;

	DTint err = ::getaddrinfo( (host.size() == 0) ? NULL : real_host.c_str(), MoreStrings::cast_to_string(port).c_str(), &hints, &res);
	if (err != 0) {
		return DT3_ERR_NET_UNKNOWN;
	}

	// save the results for freeing later
	if (res != NULL) {
		ip.set_network_address(address_to_string(res->ai_addr));
        
		::freeaddrinfo(res);
		return DT3_ERR_NONE;
	} else {
        ::freeaddrinfo(res);
        return DT3_ERR_NET_UNKNOWN;
	}
	
}

//==============================================================================
//==============================================================================

DTerr DeviceNetworkSockets::mac_address (std::string &mac, const std::string adapter)
{
#if DT3_OS == DT3_ANDROID
    return DT3_ERR_NET_UNKNOWN;
    
#else
    int                 mgmtInfoBase[6];
    void                *msgBuffer = NULL;
    size_t              length;
    struct if_msghdr    *interfaceMsgStruct;
    struct sockaddr_dl  *socketStruct;

    // Setup the management Information Base (mib)
    mgmtInfoBase[0] = CTL_NET;        // Request network subsystem
    mgmtInfoBase[1] = AF_ROUTE;       // Routing table info
    mgmtInfoBase[2] = 0;              
    mgmtInfoBase[3] = AF_LINK;        // Request link layer information
    mgmtInfoBase[4] = NET_RT_IFLIST;  // Request all configured interfaces

    // With all configured interfaces requested, get handle index
    if ((mgmtInfoBase[5] = ::if_nametoindex(adapter.c_str())) == 0)
        return DT3_ERR_NET_UNKNOWN;

    // Get the size of the data available (store in len)
    if (::sysctl(mgmtInfoBase, 6, NULL, &length, NULL, 0) < 0)
        return DT3_ERR_NET_UNKNOWN;

    // Alloc memory based on above call
    if ((msgBuffer = ::malloc(length)) == NULL)
        return DT3_ERR_NET_UNKNOWN;

    // Get system information, store in buffer
    if (::sysctl(mgmtInfoBase, 6, msgBuffer, &length, NULL, 0) < 0) {
        ::free(msgBuffer);
        return DT3_ERR_NET_UNKNOWN;
    }
        
    // Map msgbuffer to interface message structure
    interfaceMsgStruct = (struct if_msghdr *) msgBuffer;

    // Map to link-level socket structure
    socketStruct = (struct sockaddr_dl *) (interfaceMsgStruct + 1);

    // Copy link layer address data in socket structure to an array
    DTubyte macAddress[6];
    memcpy(&macAddress, socketStruct->sdl_data + socketStruct->sdl_nlen, 6);
    
    DTcharacter buffer[18];
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);

    mac = buffer;

    return DT3_ERR_NONE;
#endif

}

//==============================================================================
//==============================================================================

std::string DeviceNetworkSockets::address_to_string (const struct sockaddr *sa)
{
    char portstr[8];
    char str[INET6_ADDRSTRLEN];
    
	switch (sa->sa_family) {
    
        case AF_INET: {
            struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                return "";
                
            if (ntohs(sin->sin_port) != 0) {
                snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
                strcat(str, portstr);
            }
            return str;
        }

        case AF_INET6: {
            struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

            str[0] = '[';
            if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
                return "";
            if (ntohs(sin6->sin6_port) != 0) {
                snprintf(portstr, sizeof(portstr), "]:%d", ntohs(sin6->sin6_port));
                strcat(str, portstr);
                return str;
            }
            return (str + 1);
        }
        
	}
    return "";

}

struct sockaddr_storage DeviceNetworkSockets::string_to_address (const std::string &ip)
{
    // IP4: "255.255.255.255:12345"
    // IP6: "[::FFFF:255.255.255.255]:12345
    
    std::string ipcopy = ip;
        
    // Extract port number
    std::string port("0");
    std::size_t last_colon = ipcopy.find_last_of(':');
    if (last_colon != std::string::npos && last_colon > ipcopy.find_last_of('.')) {
        port = ipcopy.substr(last_colon+1);
        ipcopy = ipcopy.substr(0, last_colon);
    }
        
    return string_to_address (ipcopy, MoreStrings::cast_from_string<DTushort>(port));
}

struct sockaddr_storage DeviceNetworkSockets::string_to_address (const std::string &ip, DTushort port)
{
#if defined(DT3_USE_IPV6) && DT3_USE_IPV6
    // Determine if the address is IP4 or IP6
    DTboolean is_ip6 = ip.find_first_of(':') < ip.find_first_of('.');

    // IP4
    if (!is_ip6) {
#endif
		union {
			struct sockaddr_in          sa;
			struct sockaddr_storage		sa_res;
		};
		::memset(&sa, 0, sizeof(sa_res));
        
		if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr)) {
#if DT3_OS != DT3_ANDROID
			sa.sin_len = sizeof(sa);
#endif
			sa.sin_family = AF_INET;
			sa.sin_port = htons(port);
			return sa_res;
		}
               
#if defined(DT3_USE_IPV6) && DT3_USE_IPV6
   // IP6
    } else {

		union {
			struct sockaddr_in6         sa;
			struct sockaddr_storage		sa_res;
		};
		::memset(&sa, 0, sizeof(sa_res));

		if (inet_pton(AF_INET6, ip.c_str(), &sa.sin6_addr)) {
			sa.sin6_len = sizeof(sa);
			sa.sin6_family = AF_INET6;
			sa.sin6_port = htons(port);
			return sa_res;
		}
                
    }
#endif

	struct sockaddr_storage sa_res_null;
	::memset(&sa_res_null, 0, sizeof(sa_res_null));

	return sa_res_null;

}

//==============================================================================
//==============================================================================

DTerr DeviceNetworkSockets::udp_open (NetworkSocket &socket, DTushort port)
{
    // Build the socket
    DTint socket_raw = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_raw < 0) {      
        socket_raw = -1;
        return DT3_ERR_NET_UNKNOWN;
    }
        
    // TODO: Not IPv6
    if (port > 0) {
        struct sockaddr_in sa;
        
        ::memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        sa.sin_port = htons(port);
    
        DTint err = ::bind (socket_raw, (const sockaddr *) &sa, sizeof(sa));
        if (err != 0) {
            ::close(socket_raw);
            socket_raw = -1;
            return DT3_ERR_NET_UNKNOWN;
        }
    
    }
    
	// set up non-blocking reads and writes
	DTint val = ::fcntl(socket_raw, F_GETFL, 0);
	::fcntl(socket_raw, F_SETFL, val | O_NONBLOCK);
	
	socket.set_network_socket_data((DTubyte*) &socket_raw, sizeof(socket_raw));
    
	return DT3_ERR_NONE;
}

DTerr DeviceNetworkSockets::udp_close (NetworkSocket &socket)
{	
	if (socket.is_empty())
		return DT3_ERR_NONE;

	DTint socket_raw = *reinterpret_cast<DTint*>(socket.network_socket_data());
	
    ::close(socket_raw);
    socket.clear();
    
    return DT3_ERR_NONE;
}

DTerr DeviceNetworkSockets::udp_recv (NetworkPacket &packet, const NetworkSocket &socket)
{    
	if (socket.is_empty())
		return DT3_ERR_NONE;

	DTint socket_raw = *reinterpret_cast<DTint*>(socket.network_socket_data());

    struct sockaddr from;
    socklen_t       from_size = sizeof(from);

    DTsize size = ::recvfrom(   socket_raw,
                                packet.data(),
                                (DTuint) packet.data_size(),
                                0, 
                                &from,
                                &from_size
                            );
                            
    if (size > 0) {
        packet.set_network_address( address_to_string(&from) );
        packet.set_data_size(size);
    
	} else {	
		switch (errno) {
			case EWOULDBLOCK:	return DT3_ERR_NET_WOULD_BLOCK;
			default:			return DT3_ERR_NET_UNKNOWN;
		};
	}

	return DT3_ERR_NONE;

}

DTerr DeviceNetworkSockets::udp_send (const NetworkPacket &packet, const NetworkSocket &socket)
{
	if (socket.is_empty())
		return DT3_ERR_NONE;

	DTint socket_raw = *reinterpret_cast<DTint*>(socket.network_socket_data());
	
	struct sockaddr_storage sast = string_to_address(packet.network_address().network_address());
    struct sockaddr *sa = (struct sockaddr *) &sast;

#if DT3_OS == DT3_ANDROID
    ::sendto(   socket_raw, 
                packet.data(),
                packet.data_size(),
                0, 
                sa,
                sizeof(sockaddr));
#else
    ::sendto(   socket_raw, 
                packet.data(),
                (DTuint) packet.data_size(),
                0, 
                sa,
                sa->sa_len);
                
#endif

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

DTerr DeviceNetworkSockets::tcp_open (NetworkSocket &socket, const NetworkAddress &ip, DTboolean is_server)
{
	struct sockaddr_storage sast = string_to_address(ip.network_address());
    struct sockaddr *sa = (struct sockaddr *) &sast;
    socklen_t sas;
    
    // Build the socket
    DTint socket_raw = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_raw < 0) {      
        socket_raw = -1;
        LOG_MESSAGE << "TCPOpen: socket: " << strerror(errno) << " (" << (DTuint) errno << ")";
        return DT3_ERR_NET_UNKNOWN;
    }
        
	switch (sa->sa_family) {
        case AF_INET: {
                struct sockaddr_in	*sin = (struct sockaddr_in *) sa;
#if DT3_OS == DT3_ANDROID
                sas = sizeof(sockaddr);
#else
                sas = sin->sin_len;
#endif

            } break;

        case AF_INET6: {
                struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;
#if DT3_OS == DT3_ANDROID
                sas = sizeof(sockaddr_in6);
#else
                sas = sin6->sin6_len;
#endif
            } break;
	}
    

    if (is_server) {
        
        DTerr err = ::bind(socket_raw, (const sockaddr *) sa, sas);
        if (err != 0) {
            ::close(socket_raw);
            LOG_MESSAGE << "TCPOpen: bind: " << strerror(errno) << " (" << (DTuint) errno << ")";
            return DT3_ERR_NET_UNKNOWN;
        }
            
        err = ::listen(socket_raw, 5);    // Arbitrary backlog value
        if (err != 0) {
            ::close(socket_raw);
            LOG_MESSAGE << "TCPOpen: listen: " << strerror(errno) << " (" << (DTuint) errno << ")";
            return DT3_ERR_NET_UNKNOWN;
        }
    
    } else {
        DTint err = ::connect(socket_raw, (const sockaddr *) sa, sas);
        if (err != 0) {
            ::close(socket_raw);
            LOG_MESSAGE << "TCPOpen: connect: " << strerror(errno) << " (" << (DTuint) errno << ")";
            return DT3_ERR_NET_UNKNOWN;
        }
        
    }

	// set up non-blocking reads and writes
	DTint val = ::fcntl(socket_raw, F_GETFL, 0);
	::fcntl(socket_raw, F_SETFL, val | O_NONBLOCK);

	socket.set_network_socket_data((DTubyte*) &socket_raw, sizeof(socket_raw));

	return DT3_ERR_NONE;
}

DTerr DeviceNetworkSockets::tcp_close (NetworkSocket &socket)
{
	if (socket.is_empty())
		return DT3_ERR_NONE;

	DTint socket_raw = *reinterpret_cast<DTint*>(socket.network_socket_data());
	
    ::close(socket_raw);
    socket.clear();
    
    return DT3_ERR_NONE;
}

DTerr DeviceNetworkSockets::tcp_accept (NetworkSocket &connection_socket, NetworkAddress &ip, const NetworkSocket &listen_socket)
{
	if (listen_socket.is_empty())
		return DT3_ERR_NONE;

	DTint listen_socket_raw = *reinterpret_cast<DTint*>(listen_socket.network_socket_data());

	struct sockaddr_storage sast;
    struct sockaddr *sa = (struct sockaddr *) &sast;
    socklen_t sas = sizeof(sast);

    DTint connection_socket_raw = ::accept(listen_socket_raw, (sockaddr *) sa, &sas);
    if (connection_socket_raw < 0) {
        if (errno == EWOULDBLOCK) {
            return DT3_ERR_NET_WOULD_BLOCK;
        }
        
        LOG_MESSAGE << "TCPAccept: accept: " << strerror(errno) << " (" << (DTuint) errno << ")";
        return DT3_ERR_NET_UNKNOWN;
    }
    
    // set up non-blocking reads and writes
    DTint val = ::fcntl(connection_socket_raw, F_GETFL, 0);
    ::fcntl(connection_socket_raw, F_SETFL, val | O_NONBLOCK);

    connection_socket.set_network_socket_data((DTubyte*) &connection_socket_raw, sizeof(connection_socket_raw));
    
    ip.set_network_address(address_to_string( (const struct sockaddr *) sa));

	return DT3_ERR_NONE;
}

DTerr DeviceNetworkSockets::tcp_recv (NetworkPacket &packet, const NetworkSocket &socket)
{
	if (socket.is_empty())
		return DT3_ERR_NONE;

	DTint socket_raw = *reinterpret_cast<DTint*>(socket.network_socket_data());

    ssize_t size = ::recv(  socket_raw,
                            packet.data(),
                            (DTuint) packet.data_size(),
                            0
                        );
                            

    if (size < 0) {
        packet.set_data_size(0);

        switch (errno) {
            //case EAGAIN:  // Same as EWOULDBLOCK
			case EWOULDBLOCK:	
                return DT3_ERR_NET_WOULD_BLOCK;
			default:			
                LOG_MESSAGE << "TCPRecv: recv: " << strerror(errno) << " (" << (DTuint) errno << ")";
                return DT3_ERR_NET_UNKNOWN;
		};
	} else {
        packet.set_data_size(size);
    }

	return DT3_ERR_NONE;
}

DTerr DeviceNetworkSockets::tcp_send (const NetworkPacket &packet, const NetworkSocket &socket)
{
	if (socket.is_empty())
		return DT3_ERR_NONE;

	DTint socket_raw = *reinterpret_cast<DTint*>(socket.network_socket_data());
	
    ::send(   socket_raw, 
                packet.data(),
                (DTuint) packet.data_size(),
                0
            );

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3
