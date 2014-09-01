//==============================================================================
///	
///	File: NetworkAddress.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Network/NetworkAddress.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

NetworkAddress::NetworkAddress (void)
{

}

NetworkAddress::NetworkAddress (const std::string &address)
{
    set_network_address(address);
}

NetworkAddress::NetworkAddress (const NetworkAddress &rhs)
	:	_address(rhs._address)
{
	
}

NetworkAddress::NetworkAddress (NetworkAddress &&rhs)
	:	_address(std::move(rhs._address))
{
	
}
		
NetworkAddress& NetworkAddress::operator = (const NetworkAddress &rhs)
{	
	_address = rhs._address;
    return (*this);
}	

NetworkAddress& NetworkAddress::operator = (NetworkAddress &&rhs)
{	
	_address = std::move(rhs._address);
    return (*this);
}	

NetworkAddress::~NetworkAddress (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const NetworkAddress &v)
{
    s << v.network_address();
	return s;
}

Stream& operator >>(Stream &s, NetworkAddress &v)
{
    std::string ss;
    s >> ss;
    v.set_network_address(ss);
	return s;
}

//==============================================================================
//==============================================================================

} // DT3
