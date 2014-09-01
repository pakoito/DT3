//==============================================================================
///	
///	File: URL.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Network/URL.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Globals.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

URL::URL (void)
{

}

URL::URL (const URL &rhs)
    :   _url(rhs._url)
{
    
}

URL::URL (URL &&rhs)
    :   _url(std::move(rhs._url))
{

}
		
URL& URL::operator = (const URL &rhs)
{	
	_url = rhs._url;
    return (*this);
}	

URL& URL::operator = (URL &&rhs)
{	
	_url = std::move(rhs._url);
    return (*this);
}	

URL::~URL (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const URL&v)
{
	s << v.full_url();
	return s;
}

Stream& operator >>(Stream &s, URL&v)
{
	std::string original_path;
	s >> original_path;
	v.set_full_url(original_path);
	
	return s;
}


//==============================================================================
//==============================================================================

void URL::set_full_url(const std::string &url)
{	
    _url = Globals::substitute_global(url);
}			


//==============================================================================
//==============================================================================

std::string URL::protocol (void) const
{
	std::string protocol = _url;
	
	// Check for protocol and delete it
    std::string::size_type colon_slash_slash = protocol.find("://");
	if (colon_slash_slash == std::string::npos)
        return "";

	protocol.erase(colon_slash_slash);
	
	return protocol;
}

std::string URL::hostname (void) const
{
	std::string domain = _url;
	
	// Check for protocol and delete it
    std::string::size_type colon_slash_slash = domain.find("://");
	if (colon_slash_slash == std::string::npos)
        return "";

	domain.erase(0,colon_slash_slash+3);
	
	// Get index of slash or colon and erase up to it
    std::string::size_type colon_slash = domain.find_first_of(":/");
	if (colon_slash != std::string::npos)
        domain.erase(colon_slash);
	
	return domain;
}

std::string URL::path (void) const
{
	std::string domain = _url;
	
	// Check for protocol and delete it
    std::string::size_type colon_slash_slash = domain.find("://");
	if (colon_slash_slash == std::string::npos)
        return "";

	domain.erase(0,colon_slash_slash+3);

	// find next slash
	std::string::size_type separator = domain.find_first_of('/');
	if (separator == std::string::npos)
		return "/";
	domain.erase(0,separator);
	
	return domain;
}

std::string URL::strip_protocol		(void) const
{
	std::string no_protocol = _url;
	
	// Check for protocol and delete it
    std::string::size_type colon_slash_slash = no_protocol.find("://");
	if (colon_slash_slash == std::string::npos)
        return "";

	no_protocol.erase(0,colon_slash_slash+3);
	
	return no_protocol;
}

std::string URL::strip_protocol_and_parameters (void) const
{
	std::string no_protocol = _url;
	
	// Check for protocol and delete it
    std::string::size_type colon_slash_slash = no_protocol.find("://");
	if (colon_slash_slash == std::string::npos)
        return "";

	no_protocol.erase(0,colon_slash_slash+3);
    
    // Check for parameters and delete them
    return no_protocol.substr(0, no_protocol.find('?'));
}

DTushort URL::port (void) const
{
	std::string domain = _url;
	
	// Check for protocol and delete it
    std::string::size_type colon_slash_slash = domain.find("://");
	if (colon_slash_slash == std::string::npos)
        return 0;

	domain.erase(0,colon_slash_slash+3);

	// find port separator
	std::string::size_type separator = domain.find_first_of(':');
	if (separator == std::string::npos)
		return 0;
	domain.erase(0,separator + 1);
	
	// get index of slash or colon and erase up to it
	std::string::size_type length = domain.find_first_of('/');
	if (length != std::string::npos)
		domain.erase(length);
	
	return MoreStrings::cast_from_string<DTushort>(domain);
}

std::string URL::parameters (void) const
{
    return _url.substr(_url.find('?')+1);
}

//==============================================================================
// See http://en.wikipedia.org/wiki/Query_string
//==============================================================================

std::string URL::encode_URL (const std::string &s)
{
    std::string out;
    
    for (DTuint i = 0; i < s.size(); ++i) {
        
        if ((s[i] >= 'A' && s[i] <= 'Z') || 
            (s[i] >= 'a' && s[i] <= 'z') ||
            s[i] == '.' ||
            s[i] == '-' ||
            s[i] == '~' ||
            s[i] == '_') {
            
            out += s[i];
            
        } else {
            out += "%" + MoreStrings::to_hex_string ( &s[i], 1);
        }
    
    }

    return out;
}

std::string URL::decode_URL (const std::string &s)
{
    std::string out;
    
    for (DTuint i = 0; i < s.size(); ) {
    
        if (s[i] == '%' && i < (s.size() - 2) ) {
            std::string hex;
            hex += s[i+1];
            hex += s[i+2];
            
            DTcharacter character;
            MoreStrings::from_hex_string(hex, &character, 1);
            
            out += character;
            i += 3;
            
        } else {
            out += s[i];
            i += 1;
        }
    
    }
    
    return out;
}

//==============================================================================
//==============================================================================

DTboolean URL::is_URL (const std::string &s)
{
	// Check for protocol and delete it
    std::string::size_type colon_slash_slash = s.find("://");
	if (colon_slash_slash == std::string::npos)
        return false;
    else
        return true;
}

//==============================================================================
//==============================================================================

} // DT3
