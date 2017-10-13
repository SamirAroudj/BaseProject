/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Iphlpapi.h>
#elif _LINUX
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/types.h>
#endif // _WINDOWS

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include "MagicNetworkConstants.h"
#include "Network.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace std;

void Network::changeToNonblockingMode(int32 socketHandle)
{
#ifdef _WINDOWS
	u_long enabled = true;
	if (SOCKET_ERROR == ioctlsocket(socketHandle, FIONBIO, &enabled))
		NetworkExceptionFactory::throwNetworkException("Unable to change the operation mode of a socket to nonblocking.", getError()); 
#else
	if (SOCKET_ERROR == fcntl(socketHandle, F_SETFL, O_NONBLOCK))
		NetworkExceptionFactory::throwNetworkException("Unable to change the operation mode of socket to nonblocking.", getError());
#endif // _WINDOWS
}

void Network::closeSocket(int32 socketHandle)
{
#ifdef _WINDOWS
	if (SOCKET_ERROR == closesocket(socketHandle))
		NetworkExceptionFactory::throwNetworkException("Unable to shut down a socket.", getError());
#else
	if (SOCKET_ERROR == close(socketHandle))
		NetworkExceptionFactory::throwNetworkException("Unable to shut down a socket.", getError());
#endif // _WINDOWS
}

char *Network::convertAddressHostByteOrderToASCII(uint32 IPAddress)
{
	in_addr address;
	address.s_addr = htonl(IPAddress);
	return inet_ntoa(address);
}

int32 Network::getError()
{
#ifdef _WINDOWS
	return WSAGetLastError();
#else
	return errno;
#endif // _WINDOWS
}

void Network::getLANsMachineIsConnectedTo(std::vector<LANInfo> &LANs)
{
#ifdef _WINDOWS
	ULONG size = sizeof(MIB_IPADDRTABLE);
	int32 errorCode;

	MIB_IPADDRTABLE *IPAddressTable = reinterpret_cast<MIB_IPADDRTABLE *>(new char[size]);	// get the IP address table form the OS
	errorCode = GetIpAddrTable(IPAddressTable, &size, false);
	if (ERROR_INSUFFICIENT_BUFFER == errorCode)	// try to reallocate enough memory to contain the whole table
	{
		delete [] (reinterpret_cast<char *>(IPAddressTable));
		IPAddressTable = reinterpret_cast<MIB_IPADDRTABLE *>(new char[size]);
		errorCode = GetIpAddrTable(IPAddressTable, &size, false);
	}
	if (NO_ERROR != errorCode)
	{
		delete [] (reinterpret_cast<char *>(IPAddressTable));
		NetworkExceptionFactory::throwNetworkException("Unable to obtain information about LANs the machine is connected to.", errorCode);
	}

	for(uint32 i = 0; i < IPAddressTable->dwNumEntries; ++i)	// get all LANs the machine is connected to (so exclude disconnected network interfaces)
	{
		if (IPAddressTable->table[i].wType & MIB_IPADDR_DISCONNECTED || IPAddressTable->table[i].wType & MIB_IPADDR_DELETED)
			continue;
		uint32 IPAddress = ntohl(IPAddressTable->table[i].dwAddr);
		uint32 netMask = ntohl(IPAddressTable->table[i].dwMask);
		LANs.push_back(LANInfo(IPAddress, netMask));

		if (IPAddressTable->table[i].wType & MIB_IPADDR_PRIMARY) // the LAN the primary network interface connects to is always the first
		{
			LANInfo temp = LANs.front();
			LANs[0] = LANs.back();
			LANs[LANs.size() - 1] = temp;
		}
	}

	delete [] (reinterpret_cast<char *>(IPAddressTable));
#elif _LINUX
    ifaddrs *addressList;
    if (0 != getifaddrs(&addressList))
        NetworkExceptionFactory::throwNetworkException("could not list network interfaces.", Network::getError());
    
    for(ifaddrs *iterator = addressList; iterator != NULL; iterator = iterator->ifa_next)
    {
        if (iterator->ifa_addr->sa_family != AF_INET || iterator->ifa_netmask->sa_family != AF_INET)
            continue;
        
        uint32 IPAddress = ntohl(reinterpret_cast<sockaddr_in *>(iterator->ifa_addr)->sin_addr.s_addr);
        uint32 netMask = ntohl(reinterpret_cast<sockaddr_in *>(iterator->ifa_netmask)->sin_addr.s_addr);
        LANs.push_back(LANInfo(IPAddress, netMask)); 
    
        if (0 == strncmp("eth0", iterator->ifa_name, 4) || 0 == strncmp("wlan0", iterator->ifa_name, 5)) // the LAN the primary network interface connects to is always the first
        {
            LANInfo temp = LANs.front();
            LANs[0] = LANs.back();
            LANs[LANs.size() - 1] = temp;
        }
    }
    
    freeifaddrs(addressList);
#endif // _WINDOWS

	if (LANs.size() == 1)/// the loopback interface is only contained when there is no other interface available otherwise ambiguity issues can arise
		return;
	for(uint32 i = 0; i < LANs.size(); ++i)
	{
		if (LANs[i].getInterfaceIPAddress() != LOOPBACK_INTERFACE_IP_ADDRESS)
			continue;

		LANs[i] = LANs.back();
		LANs.pop_back();
		break;
	}
}

bool Network::getMulticastSupport(uint32 socketHandle, uint32 interfaceIPAddress)
{
	ip_mreq multicastRequest;
	multicastRequest.imr_interface.s_addr = htonl(interfaceIPAddress);
	multicastRequest.imr_multiaddr.s_addr = htonl(MULTICAST_ADDRESS);

	if (SOCKET_ERROR == setsockopt(socketHandle, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&multicastRequest, sizeof(multicastRequest)))
	{
		if (getError() != NO_MULTICAST_SUPPORT)
			NetworkExceptionFactory::throwNetworkException("Could not detect multicast support!", getError());
		return false;
	}

	setsockopt(socketHandle, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&multicastRequest, sizeof(multicastRequest));
	return true;
}

void Network::getSocketState(int32 socketHandle, bool &readable, bool &writable)
{
	readable = writable = false;
	timeval immediately = { 0, 0 };
	fd_set readableSockets, writableSockets;
	FD_ZERO(&readableSockets);
	FD_ZERO(&writableSockets);
	FD_SET(socketHandle, &readableSockets);
	FD_SET(socketHandle, &writableSockets);
	select(socketHandle + 1, &readableSockets, &writableSockets, NULL, &immediately);

	if (FD_ISSET(socketHandle, &writableSockets))
		writable = true;
	if (FD_ISSET(socketHandle, &readableSockets))
		readable = true;
}

void Network::initializeNetwork()
{
#ifdef _WINDOWS
	const WORD versionNumber = 0x0202; // version 2.2
	WSADATA wsaData;

	if (WSAStartup(versionNumber, &wsaData)) // returns 0 on success
		NetworkExceptionFactory::throwNetworkException("Could not start winsock.", WSAGetLastError());
	if (versionNumber != wsaData.wVersion)
		NetworkExceptionFactory::throwNetworkException(" There is no sufficient implementation of winsock available, version 2.2 is required.", WINSOCK_VERSION_TOO_LOW);
#endif // _WINDOWS
}

void Network::shutDownNetwork()
{
#ifdef _WINDOWS
	if (WSACleanup()) // release network resources, returns 0 on success
		NetworkExceptionFactory::throwNetworkException("Could not cleanup the winsock implementation.", WSAGetLastError());
#endif // _WINDOWS
}
