/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _NETWORK_MAGIC_CONSTANTS_H_
#define _NETWORK_MAGIC_CONSTANTS_H_

#include "Network/Network.h"

namespace Network
{
        extern const int32 WOULD_BLOCK;
        extern const int32 NO_MULTICAST_SUPPORT;
    
#ifdef _LINUX
	const int32 INVALID_SOCKET = -1;
	const uint32 SD_SEND = 1;
	const int32 SOCKET_ERROR = -1;
#endif // _LINUX

	/// default maximum time to wait for a response for a connection request
	const Real DEFAULT_CONNECTION_REQUEST_TIMOUT_SECONDS = 1.0f;

	/// maximum size of a network packet
	const uint32 TCP_PACKET_WRITER_BUFFER_SIZE = 10000u;

	/// minimum header which is appended to data which is carried by a UDP datagram
	const uint16 MIN_DATAGRAM_OVERHEAD = 28u;

	/// this is the default maximum size of a datagram which won't be split up into fragments
	/// 576 bytes ist the minimum MTU which must be supported by every physical device (ethernet, modem etc.) - (IP + UDP header) - some other header (e.g. broadband header)
	const uint16 MAX_DATAGRAM_PAYLOAD_SIZE = 576u - MIN_DATAGRAM_OVERHEAD - 48u;

	/// the identifier of the server for the game session
	const uint16 SERVER_IDENTIFIER = 0;

	/// the smallest message identifier a user of the network library can use for tcp messages
	const MessageType MIN_USER_TCP_MESSAGE_IDENTIFIER = TCPPacket::NUM_OF;

	/// the smallest message identifier a user of the network library can use for udp messages
	const MessageType MIN_USER_UDP_MESSAGE_IDENTIFIER = UDPPacket::NUM_OF;

	/// the IP address of the loopback network interface in host byte order which is always provided by the OS = 127.0.0.1
	const uint32 LOOPBACK_INTERFACE_IP_ADDRESS = 2130706433;

	// the address of the multicast group the server is sending to (host byte order)
	const uint32 MULTICAST_ADDRESS = 3871970249; // "230.201.147.201" as dotted decimal string 

	// every UDP client socket which supports explicit multicast messages is bound to this port (host byte order)
	const uint16 MULTICAST_PORT_NUMBER = 35917;


	/// <summary> The maximum length of a string containing an IPv4 address </summary>
	const uint32 MAX_IP_ADDRESS_LENGTH = 16;


	//////////////////// bandwidth usage control constants

	/// length of the period in seconds in which not more than MAX_BYTES_PER_SEND_PERIOD can be sent with UDP to one receiver
	const Real SEND_PERIOD_LENGTH = 0.050f;

	/// maximum number of bytes which can be sent via UDP to a server during a send period the length of which is SEND_PERIOD_LENGTH seconds
	const uint16 MAX_BYTES_PER_SEND_PERIOD_TO_SERVER = 500u;

	/// maximum number of bytes which can be sent via UDP to a client during a send period the length of which is SEND_PERIOD_LENGTH seconds
	const uint16 MAX_BYTES_PER_SEND_PERIOD_TO_CLIENT = 3000u;


	//////////////////// Network time management

	/// number of latest time periods that describe how long it took a packet to get from the client to the server and back
	const uint32 MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES = 32u;

	/// client sends time request messages to start checking and updating of it's own clock when REQUEST_TIME_PERIOD seconds have passed
	const Real REQUEST_TIME_PERIOD = 3.0f;

	/// <summary> The default bad round trip time is used at the beginning when there have not been enough time round trip time measurements. </summary>
	const Real DEFAULT_BAD_ROUND_TRIP_TIME = 1.0f;

	/// round-trip times from 0 to LAST_INDEX_BEST_ROUND_TRIP_TIMES (inclusive) in an array of round-trip times in ascending order are quite good and can be used for clock synchronisation
	const uint32 LAST_INDEX_BEST_ROUND_TRIP_TIMES = static_cast<uint32>(MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES * 0.2f);

	/// round-trip times from FIRST_INDEX_BAD_ROUND_TRIP_TIMES to MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES are very long round-trip times from an array of round-trip times in ascending order
	/// if an acknowledgement of a reliable packet hasn't been received after oderedRoundTripTimes[FIRST_INDEX_BAD_ROUND_TRIP_TIMES] than it should be considered as late or lost
	const uint32 FIRST_INDEX_BAD_ROUND_TRIP_TIMES = static_cast<uint32>(MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES * 0.8f) + 1;

	/// maximum deviation of the client clock from server's clock in milliseconds
	const uint32 MAX_CLIENT_CLOCK_DEVIATION = 7;

	/// <summary> A UDP packet is resent if no acknowledgement has been received since packet dispatch and
	/// 		  if (UDP_PACKET_RESEND_FACTOR * bad round trip time) seconds have passed </summary>
	const Real UDP_PACKET_RESEND_FACTOR = 1.5f;

	//////////////////// error codes

	/// error code that identifiers a closed connection
	const int32 CLOSED_CONNECTION_ERROR_CODE = 0;

	/// error code that identifies a failure caused by a failed attempt to create a connection
	const int32 CONNECTION_CREATION_ERROR_CODE = 2;

	/// error code that identifies a situation in which the winsock version is too old
	const int32 WINSOCK_VERSION_TOO_LOW = 1;
}

#endif // _NETWORK_MAGIC_CONSTANTS_H_
