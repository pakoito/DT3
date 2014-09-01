//==============================================================================
///	
///	File: NetworkPacket.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Network/NetworkPacket.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/Endian.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(NetworkPacket)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

NetworkPacket::NetworkPacket (void)
{

}

NetworkPacket::NetworkPacket (const NetworkPacket &rhs)
	:	_ip_address         (rhs._ip_address),
		_data               (rhs._data)
{
	
}

NetworkPacket::NetworkPacket (NetworkPacket &&rhs)
	:	_ip_address         (std::move(rhs._ip_address)),
		_data               (std::move(rhs._data))
{
	
}

NetworkPacket& NetworkPacket::operator = (const NetworkPacket &rhs)	
{	
	_ip_address = rhs._ip_address;
	_data = rhs._data;

	return *this;
}

NetworkPacket& NetworkPacket::operator = (NetworkPacket &&rhs)
{	
	_ip_address = std::move(rhs._ip_address);
	_data = std::move(rhs._data);

	return *this;
}
			
NetworkPacket::~NetworkPacket (void)
{

}

//==============================================================================
//==============================================================================

void NetworkPacket::set_network_address (const NetworkAddress& ip_address)
{
    _ip_address = ip_address;
}

const NetworkAddress & NetworkPacket::network_address (void) const
{
	return _ip_address;
}

//==============================================================================
//==============================================================================

void NetworkPacket::set_data (const std::vector<DTubyte> &data)
{
    set_data (&data[0], data.size());
}

void NetworkPacket::set_data (const std::string &data)
{
    set_data (&data[0], data.size());
}

void NetworkPacket::set_data (const void *b, DTsize size)
{
    _data.resize(size);
    
	// copy data
	::memcpy(&_data[0], b, (size_t) size);
}


//==============================================================================
//==============================================================================

std::string NetworkPacket::data_as_string (void) const
{
    std::string s;
    
    s.resize(_data.size());
    ::memcpy(&s[0], &_data[0], _data.size());
    
    return s;
}

//==============================================================================
//==============================================================================

} // DT3
