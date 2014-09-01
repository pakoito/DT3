//==============================================================================
///	
///	File: NetworkSocket.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Network/NetworkSocket.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(NetworkSocket)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

NetworkSocket::NetworkSocket (void)
{

}

NetworkSocket::NetworkSocket (const DTubyte *socket_structure, DTuint length)
{
    set_network_socket_data(socket_structure, length);
}

NetworkSocket::NetworkSocket (const NetworkSocket &rhs)
	:	_data(rhs._data)
{
	
}
		
NetworkSocket::NetworkSocket (NetworkSocket &&rhs)
	:	_data(std::move(rhs._data))
{
	
}
		
NetworkSocket& NetworkSocket::operator = (const NetworkSocket &rhs)
{	
	_data = rhs._data;
    return (*this);
}	

NetworkSocket& NetworkSocket::operator = (NetworkSocket &&rhs)
{	
	_data = std::move(rhs._data);
    return (*this);
}	

NetworkSocket::~NetworkSocket (void)
{

}

//==============================================================================
//==============================================================================

void NetworkSocket::set_network_socket_data (const DTubyte *socket_structure, DTuint length)
{
	_data.resize(length);
    ::memcpy(&_data[0], socket_structure, length);
}

//==============================================================================
//==============================================================================

} // DT3
