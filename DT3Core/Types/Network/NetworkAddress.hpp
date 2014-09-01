#ifndef DT3_NETWORKADDRESS
#define DT3_NETWORKADDRESS
//==============================================================================
///	
///	File: NetworkAddress.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;

//==============================================================================
/// Class
//==============================================================================

class NetworkAddress {
    public:		
        DEFINE_TYPE_SIMPLE_BASE(NetworkAddress)
    
								NetworkAddress			(void);	
								NetworkAddress			(const std::string &address);
								NetworkAddress			(const NetworkAddress &rhs);
								NetworkAddress			(NetworkAddress &&rhs);
        NetworkAddress &        operator =              (const NetworkAddress &rhs);
        NetworkAddress &        operator =              (NetworkAddress &&rhs);
								~NetworkAddress			(void);
		
	public:
	
		/// Set the network address
		/// \param address address
		void                    set_network_address     (const std::string &address){	_address = address;             }

		/// Returns the network address
		/// \return Network address
		const std::string &     network_address         (void) const				{	return _address;                }

		/// Clear the network address
		void					clear                   (void)						{	_address.clear();				}

		/// Checks if the network address is empty or not
		/// \return Address is empty
		DTboolean				is_empty                (void) const				{	return _address.size() == 0;	}

	private:
		friend int operator == (const NetworkAddress& a, const NetworkAddress& b);
		friend int operator != (const NetworkAddress& a, const NetworkAddress& b);
		friend int operator < (const NetworkAddress& a, const NetworkAddress& b);

		friend Stream& operator <<(Stream &s, const NetworkAddress&v);
		friend Stream& operator >>(Stream &s, NetworkAddress&v);

		std::string             _address;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const NetworkAddress&v);
Stream& operator >>(Stream &s, NetworkAddress&v);

//==============================================================================
//==============================================================================

inline int operator == (const NetworkAddress& a, const NetworkAddress& b)
{
	return (a._address == b._address);
}

inline int operator != (const NetworkAddress& a, const NetworkAddress& b)
{
	return (a._address != b._address);
}

inline int operator < (const NetworkAddress& a, const NetworkAddress& b)
{
	return (a._address < b._address);
}

//==============================================================================
//==============================================================================

} // DT3

#endif
