#ifndef DT3_NETWORKPACKET
#define DT3_NETWORKPACKET
//==============================================================================
///	
///	File: NetworkPacket.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Network/NetworkAddress.hpp"
#include "DT3Core/Types/Utility/TimerLores.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class NetworkPacket: public BaseClass {
    public:
        DEFINE_TYPE(NetworkPacket, BaseClass)
		DEFINE_CREATE_AND_CLONE
         
									NetworkPacket			(void);	
									NetworkPacket			(const NetworkPacket &rhs);
									NetworkPacket			(NetworkPacket &&rhs);
        NetworkPacket&              operator =				(const NetworkPacket &rhs);
        NetworkPacket&              operator =				(NetworkPacket &&rhs);
        virtual                     ~NetworkPacket          (void);
		
	public:

		/// Set the address of packet. Sent to or recieve from address.
		/// \param ip_address address
		void						set_network_address     (const NetworkAddress& ip_address);

		/// Returns address of packet
		/// \return address of pacet
		const NetworkAddress &      network_address         (void) const;
		

		/// Sets the packet data
		/// \param data Packet data
		void						set_data				(const std::vector<DTubyte> &data);

		/// Sets the packet data
		/// \param data Packet data
		void						set_data				(const std::string &data);

		/// Sets the packet data
		/// \param b base pointer of raw data
		/// \param size of raw data
		void						set_data				(const void *b, DTsize size);

		/// Sets the packet data size
		/// \param size of raw data
		void						set_data_size           (DTsize size)           {   _data.resize(size,0);                   }


		/// Raw pointer to data
		/// \return pointer to data
		DTubyte*					data                    (void) const            {	return (DTubyte*) &_data[0];            }

		/// Raw pointer to data
		/// \return pointer to data
		std::string					data_as_string          (void) const;
    
		/// Returns logical size of data
		/// \return logical size of data
		DTsize						data_size               (void) const            {   return _data.size();                    }
			
		/// Returns time since packet was sent
		/// \return Time since packet was sent
		DTdouble                    time_since_sent         (void) const            {	return _time_since_sent.abs_time();     }

		/// Resets the time since sent
		void						reset_time_since_sent	(void)                  {	_time_since_sent.reset_abs_time();      }


		/// Returns time since packet was recieved
		/// \return Time since packet was recieved
		DTdouble                    time_since_recv         (void) const            {	return _time_since_recv.abs_time();     }

		/// Resets the time since recieved
		void						reset_time_since_recv   (void)                  {	_time_since_recv.reset_abs_time();      }
	
	private:
		NetworkAddress              _ip_address;
        
		TimerLores					_time_since_sent;
		TimerLores					_time_since_recv;
		  
		std::vector<DTubyte>        _data;

};

//==============================================================================
//==============================================================================


} // DT3

#endif
