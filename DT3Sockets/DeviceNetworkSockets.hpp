#ifndef DT3_DEVICENETWORKSOCKETS
#define DT3_DEVICENETWORKSOCKETS
//==============================================================================
///	
///	File: DeviceNetworkSockets.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Devices/DeviceNetwork.hpp"

#include <string>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
//#include <dns_sd.h>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class NetworkContext;
class NetworkConnection;
class BinaryBufferStream;

//==============================================================================
//==============================================================================

class DeviceNetworkSockets: public DeviceNetwork {
		/// POSIX socket network driver.
    public:
        DEFINE_TYPE(DeviceNetworkSockets,DeviceNetwork)
		DEFINE_CREATE_AND_CLONE
         
									DeviceNetworkSockets		(void);	
	private:
									DeviceNetworkSockets		(const DeviceNetworkSockets &rhs)	{}
        DeviceNetworkSockets &		operator =					(const DeviceNetworkSockets &rhs)	{	return *this;	}
    public:
        virtual						~DeviceNetworkSockets		(void);
    		
	public:
		/// Called to initialize the object
		void						initialize					(void);
	
        /// Converts a host and port to a NetworkAddress
		/// \param ip Returned Network Address
		/// \param host Host
		/// \param port Port
		/// \return Error Code
        virtual DTerr               resolve_host                (NetworkAddress &ip, const std::string &host, DTushort port);
    
		/// Attempts to determine the MAC address of an adapter
		/// \param mac Returned MAC address
		/// \param adapter Adapter to return
		/// \return Error Code
        virtual DTerr               mac_address                 (   std::string &mac,
                                                                    std::string adapter = "en0");

		/// Opens UDP socket
		/// \param socket Returned socket
		/// \param port Port
		/// \return Error Code
        virtual DTerr               udp_open                    (NetworkSocket &socket, DTushort port);

		/// Closes UDP Socket
		/// \param socket Socket
		/// \return Error Code
        virtual DTerr               udp_close                   (NetworkSocket &socket);

		/// Recieves data on a UDP socket
		/// \param packet Network Packet
		/// \param socket Socket to recieve data on
		/// \return Error Code
        virtual DTerr               udp_recv                    (NetworkPacket &packet, const NetworkSocket &socket);

		/// Sends data on a UDP socket
		/// \param packet Network Packet
		/// \param socket Socket to send data on
		/// \return Error Code
        virtual DTerr               udp_send                    (const NetworkPacket &packet, const NetworkSocket &socket);

 
		/// Opens TCP socket
		/// \param socket Returned socket
		/// \param ip IP Address
		/// \param is_server If true, then it open a listening socket
		/// \return Error Code
        virtual DTerr               tcp_open                    (NetworkSocket &socket, const NetworkAddress &ip, DTboolean is_server = false);

		/// Closes TCP socket
		/// \param socket Socket
		/// \return Error Code
        virtual DTerr               tcp_close                   (NetworkSocket &socket);

		/// Accepts an incoming connection on listening TCP socket and returns a new socket
		/// \param connection_socket Returned connected socket
		/// \param ip Returned ip address of incoming connection
		/// \param listen_socket Listening socket
		/// \return Error Code
        virtual DTerr               tcp_accept                  (NetworkSocket &connection_socket, NetworkAddress &ip, const NetworkSocket &listen_socket);

		/// Recieves data on TCP Socket
		/// \param packet Network Packet
		/// \param socket Socket to recieve data on
 		/// \return Error Code
        virtual DTerr               tcp_recv                    (NetworkPacket &packet, const NetworkSocket &socket);

		/// Sends data on a TCP socket
		/// \param packet Network Packet
		/// \param socket Socket to send data on
		/// \return Error Code
        virtual DTerr               tcp_send                    (const NetworkPacket &packet, const NetworkSocket &socket);

	private:
		std::string					address_to_string           (const struct sockaddr *sa);
		struct sockaddr_storage     string_to_address           (const std::string &ip);
		struct sockaddr_storage     string_to_address           (const std::string &ip, DTushort port);
};

//==============================================================================
//==============================================================================

} // DT3

#endif
