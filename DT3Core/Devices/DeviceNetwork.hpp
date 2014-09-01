#ifndef DT3_DEVICENETWORK
#define DT3_DEVICENETWORK
//==============================================================================
///	
///	File: DeviceNetwork.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Devices/DeviceBase.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class NetworkPacket;
class NetworkSocket;
class NetworkAddress;

//==============================================================================
/// Class
//==============================================================================

class DeviceNetwork: public DeviceBase {
		/// Base class for Network Drivers.

    public:
        DEFINE_TYPE(DeviceNetwork,DeviceBase)
		DEFINE_CREATE
         
                                        DeviceNetwork   (void);
	private:
                                        DeviceNetwork   (const DeviceNetwork &rhs)	{}
        DeviceNetwork &                 operator =      (const DeviceNetwork &rhs)	{	return *this;	}
    public:
        virtual                         ~DeviceNetwork  (void);
    		
	public:
		/// Called to initialize the object
		void							initialize      (void);

        /// Converts a host and port to a NetworkAddress
		/// \param ip Returned Network Address
		/// \param host Host
		/// \param port Port
		/// \return Error Code
        virtual DTerr                   resolve_host    (   NetworkAddress &ip,
                                                            const std::string &host,
                                                            DTushort port)                  {   return DT3_ERR_NONE;    }
    
		/// Attempts to determine the MAC address of an adapter
		/// \param mac Returned MAC address
		/// \param adapter Adapter to return
		/// \return Error Code
        virtual DTerr                   mac_address     (   std::string &mac,
                                                            std::string adapter = "en0")    {   return DT3_ERR_NONE;    }

		/// Opens udp socket
		/// \param socket Returned socket
		/// \param port Port
		/// \return Error Code
        virtual DTerr					udp_open        (   NetworkSocket &socket,
                                                            DTushort port)                  {   return DT3_ERR_NONE;    }

		/// Closes udp Socket
		/// \param socket Socket
		/// \return Error Code
        virtual DTerr                   udp_close       (   NetworkSocket &socket)          {   return DT3_ERR_NONE;    }


		/// Recieves data on a udp socket
		/// \param packet Network Packet
		/// \param socket Socket to recieve data on
		/// \return Error Code
        virtual DTerr                   udp_recv        (   NetworkPacket &packet,
                                                            const NetworkSocket &socket)    {   return DT3_ERR_NONE;    }

		/// Sends data on a udp socket
		/// \param packet Network Packet
		/// \param socket Socket to send data on
		/// \return Error Code
        virtual DTerr                   udp_send        (   const NetworkPacket &packet,
                                                            const NetworkSocket &socket)    {   return DT3_ERR_NONE;    }



		/// Opens tcp socket
		/// \param socket Returned socket
		/// \param ip IP Address
		/// \param is_server If true, then it open a listening socket
		/// \return Error Code
        virtual DTerr                   tcp_open        (   NetworkSocket &socket,
                                                            const NetworkAddress &ip,
                                                            DTboolean is_server = false)    {   return DT3_ERR_NONE;    }

		/// Closes tcp socket
		/// \param socket Socket
		/// \return Error Code
        virtual DTerr                   tcp_close       (   NetworkSocket &socket)          {   return DT3_ERR_NONE;    }

		/// Accepts an incoming connection on listening tcp socket and returns a new socket
		/// \param connection_socket Returned connected socket
		/// \param ip Returned ip address of incoming connection
		/// \param listen_socket Listening socket
		/// \return Error Code
        virtual DTerr                   tcp_accept      (   NetworkSocket &connection_socket,
                                                            NetworkAddress &ip,
                                                            const NetworkSocket &listen_socket) {   return DT3_ERR_NONE;    }

		/// Recieves data on tcp Socket
		/// \param packet Network Packet
		/// \param socket Socket to recieve data on
 		/// \return Error Code
        virtual DTerr                   tcp_recv        (   NetworkPacket &packet,
                                                            const NetworkSocket &socket)    {   return DT3_ERR_NONE;    }

		/// Sends data on a tcp socket
		/// \param packet Network Packet
		/// \param socket Socket to send data on
		/// \return Error Code
        virtual DTerr                   tcp_send        (   const NetworkPacket &packet,
                                                            const NetworkSocket &socket)    {   return DT3_ERR_NONE;    }
};

//==============================================================================
//==============================================================================

} // DT3

#endif
