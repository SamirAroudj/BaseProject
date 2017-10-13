/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _WINDOWS
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif // _WINDOWS

#include "Platform/ApplicationTimer.h"
#include "LANInfo.h"
#include "Network.h"
#include "Server.h"
#include "ServerTimeManager.h"
#include "Session.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"

#ifdef _DEBUG
#include <iostream>
#endif

using namespace ExceptionHandling;
using namespace Network;
using namespace std;

Server::Server(const std::string &password, uint32 maxNumOfSessionClients, uint32 IPAddress) : mUDPServer(NULL)
{
	if (0 == IPAddress)	// user didn't specify an IP address, so bind the listening socket to the IP address of the primary network interface
	{
		vector<LANInfo> LANs;
		getLANsMachineIsConnectedTo(LANs);
		IPAddress = LANs.front().getInterfaceIPAddress();
	}

	sockaddr_in serverAddress;	// address for socket binding; primary or specified IP address, port: is arbitrary since it is published by lan server advertiser or other mechanism
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(IPAddress);
	serverAddress.sin_port = htons(0);
	memset(serverAddress.sin_zero, 0, 8);

	if (INVALID_SOCKET == (mListeningSocket = socket(PF_INET, SOCK_STREAM, 0))) // create the listening socket, bind it to the given port and make it nonblocking
		NetworkExceptionFactory::throwNetworkException("Unable to create a TCP server socket for listening.", getError());
	if (SOCKET_ERROR == bind(mListeningSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(sockaddr_in)))
		NetworkExceptionFactory::throwSocketBindException("Error, unable to bind the server socket to a port.", getError(), 0);
	changeToNonblockingMode(mListeningSocket);
	
	mUDPServer = new UDPServerSocket(IPAddress);	// create the UDPServer, network session and ServerTimeManager
	Session *session = new Session(0, maxNumOfSessionClients, Platform::ApplicationTimer::getSingleton().getSystemTime(), password);
	session->addMember(IPAddress, mUDPServer->getPortNumber(), 0, mUDPServer->getMulticastSupport());
	session->addListener(mUDPServer);
	ServerTimeManager *timeManager = new ServerTimeManager();

	FD_ZERO(&mReadingSockets);// init reading / writing status of the sockets
	FD_ZERO(&mWritingSockets);
	FD_SET(mListeningSocket, &mReadingSockets);
	FD_SET(mUDPServer->getSocketHandle(), &mReadingSockets);
	FD_SET(mUDPServer->getSocketHandle(), &mWritingSockets);
	mMaxSocketHandle = (mListeningSocket > mUDPServer->getSocketHandle()) ? mListeningSocket : mUDPServer->getSocketHandle();
}

Server::~Server()
{
	closeSocket(mListeningSocket);
	delete ServerTimeManager::getSingletonPointer();
	delete Session::getSingletonPointer();
	delete mUDPServer;

	TCPServerEnd::destroyAllPendingPackets();	// free pending packets which have not been sent yet

	for(uint32 i = 0; i < mConnections.size(); ++i) // and delete all connections
		delete mConnections[i];
	FD_ZERO(&mReadingSockets);
	FD_ZERO(&mWritingSockets);
}

void Server::registerServerEnd(TCPServerEnd *serverEnd)
{
	mConnections.push_back(serverEnd);
	registerForSending(serverEnd);
	registerForReceiving(serverEnd);
}

void Server::registerForSending(const EventListener *listener)
{
	FD_SET(listener->getSocketHandle(), &mWritingSockets);
	if (mMaxSocketHandle < listener->getSocketHandle())
		mMaxSocketHandle = listener->getSocketHandle();
}

void Server::registerForReceiving(const EventListener *listener)
{
	FD_SET(listener->getSocketHandle(), &mReadingSockets);
	if (mMaxSocketHandle < listener->getSocketHandle())
		mMaxSocketHandle = listener->getSocketHandle();
}

void Server::listen(int32 maxNumOfPendingConnectionRequests)
{
	if (SOCKET_ERROR == ::listen(mListeningSocket, maxNumOfPendingConnectionRequests))
		NetworkExceptionFactory::throwNetworkException("Unable to start listening for connection requests.", getError());
}

void Server::addTCPMessageToAllExceptOne(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
										 uint16 excludedServerEndIdentifier)
{
	assert(serializables && numOfSerializables > 0);
	vector<TCPServerEnd *> *receivers = new vector<TCPServerEnd *>();
	computeSessionMemberReceivers(excludedServerEndIdentifier, receivers);
	TCPServerEnd::addToSeveralServerEnds(messageIdentifier, serializables, numOfSerializables, receivers);
}

void Server::computeSessionMemberReceivers(uint16 excludedServerEndIdentifier, std::vector<TCPServerEnd *> *receivers)
{
	for(uint32 i = 0; i < mConnections.size(); ++i)
		if (!mConnections[i]->isPending() && excludedServerEndIdentifier != mConnections[i]->getIdentifier())	// don't send to pending connections which are still negotiated
			receivers->push_back(mConnections[i]);
}

void Server::send()
{
	TCPServerEnd::send();
	if (mUDPServer)
		mUDPServer->send();
}

void Server::update()
{
	timeval immediately = { 0, 0 }; // don't wait until something is readable or writable, just check the state
	fd_set readableSockets = mReadingSockets;	// which socket can be read or written?
	fd_set writableSockets = mWritingSockets;
	int32 numOfActiveSockets = select(mMaxSocketHandle + 1, &readableSockets, &writableSockets, NULL, &immediately);
	if (0 == numOfActiveSockets)
		return;

	if (FD_ISSET(mListeningSocket, &readableSockets))	// connection request occured
	{
		acceptConnection();	// negotiate the rest with session messages (password, session full?...)
		--numOfActiveSockets;
	}

	if (FD_ISSET(mUDPServer->getSocketHandle(), &readableSockets)) // update UDPServerSocket
		mUDPServer->notifyReceivingIsPossible();
	if (FD_ISSET(mUDPServer->getSocketHandle(), &writableSockets))
		mUDPServer->notifySendingIsPossible();
	while(mUDPServer->isPacketAvailable())
	{
		UDPPacket *packet = mUDPServer->getPacket();
		mMessageProvider.add(packet);
	}
	mUDPServer->update();

	updateConnections(readableSockets, writableSockets, numOfActiveSockets);
}

void Server::acceptConnection()
{
	// accept the request, create and register the server end of this connection
	socklen_t addressLength = sizeof(sockaddr);
	int32 socketHandle = accept(mListeningSocket, NULL, &addressLength);
	if (INVALID_SOCKET == socketHandle)
		NetworkExceptionFactory::throwNetworkException("Could not accept a connection request.", getError());

	registerServerEnd(new TCPServerEnd(socketHandle));
}

void Server::updateConnections(const fd_set &readableSockets, const fd_set &writableSockets, int32 &numOfActiveSockets)
{
	for(uint32 i = 0; 0 != numOfActiveSockets && i < mConnections.size(); )	// inform server ends that there is data to receive or there is new free sending buffer space
	{
		if (FD_ISSET(mConnections[i]->getSocketHandle(), &writableSockets))	// send something?
		{
			mConnections[i]->notifySendingIsPossible();
			--numOfActiveSockets;
		}
		if (FD_ISSET(mConnections[i]->getSocketHandle(), &readableSockets))	// receive something?
		{
			--numOfActiveSockets;
			try
			{
				if (!mConnections[i]->notifyReceivingIsPossible())
				{
					processClosedConnection(i);	// remote side closed connection
					continue; // do not increment i since the connection at index i was removed
				}
				while(mConnections[i]->isPacketAvailable())
					processReceivedPacket(mConnections[i]);
			}
			catch(ExceptionHandling::ClosedConnectionException &exception)	// unexpected closing of this connection ->> free it
			{
#ifdef _DEBUG
				cout << exception << endl;
#endif
				processClosedConnection(i);
				continue; // do not increment i since the connection at index i was removed
			}
		}
		++i;
	}
}

void Server::processReceivedPacket(TCPServerEnd *serverEnd)
{
	TCPPacketReader reader;
	TCPPacket *packet = serverEnd->getPacket();
	reader.beginReading(packet);

	while(true)	// go through the messages
	{
		if (reader.getCurrentMessageType() >= MIN_USER_TCP_MESSAGE_IDENTIFIER)
		{
			assert(!serverEnd->isPending());// a connection that is not fully accepted should not send only game relevant data
			reader.chopOffReadMessages();	// remove the data which is only interesting for the network management
			packet = reader.endReading();
			mMessageProvider.add(packet);
			return;
		}
		switch(reader.getCurrentMessageType())
		{
		case TCPPacket::SESSION_REMOVE_MEMBER:
			processSessionRemoveMemberMessage(reader);
			break;

		case TCPPacket::SESSION_REQUEST:
			assert(serverEnd->isPending());
			processSessionRequestMessage(reader, serverEnd);
			break;

		case TCPPacket::NO_MESSAGE:	// whole packet has been read
			reader.endReadingAndDestroyPacket();
			return;

		default:
			assert(false);
		}
	}
}

void Server::processSessionRemoveMemberMessage(TCPPacketReader &reader)
{
	uint16 identifier;
	reader.readSessionRemoveMemberMessage(identifier);
	assert(SERVER_IDENTIFIER != identifier);

	for(uint32 i = 0; i < mConnections.size(); ++i)
	{
		if (mConnections[i]->getIdentifier() == identifier)
		{
			mConnections[i]->stopSending();
			return;
		}
	}
	assert(false);
}

void Server::processSessionRequestMessage(TCPPacketReader &reader, TCPServerEnd *serverEnd)
{
	TCPPacketWriter writer;
	uint32 remoteIPAddress;
	uint16 remoteUDPPortNumber;
	string password;
	bool multicastSupport;
	reader.readSessionRequestMessage(remoteIPAddress, remoteUDPPortNumber, password, multicastSupport);

	if (Session::getSingleton().isFull())	// inform this client that the session is already full
	{
		writer.beginPacketAssembly();
		writer.addSessionIsFullMessage();
		TCPServerEnd::addTo(writer.endPacketAssembly(SERVER_IDENTIFIER), serverEnd);
		return;
	}

	if (!Session::getSingleton().verifyPassword(password))	// inform this client that the password is wrong
	{
		writer.beginPacketAssembly();
		writer.addSessionWrongPasswordMessage();
		TCPServerEnd::addTo(writer.endPacketAssembly(SERVER_IDENTIFIER), serverEnd);
		return;
	}
	
	confirmConnection(serverEnd, writer, remoteIPAddress, remoteUDPPortNumber, multicastSupport);
}

void Server::confirmConnection(TCPServerEnd *serverEnd, TCPPacketWriter &writer, uint32 remoteIPAddress, uint16 remoteUDPPortNumber, bool multicastSupport)
{
	uint16 identifier = Session::getSingleton().getNextFreeIdentifier(); /// update the network session object
	Session::getSingleton().addMember(remoteIPAddress, remoteUDPPortNumber, identifier, multicastSupport);
	serverEnd->setIdentifier(identifier);
	
	writer.beginPacketAssembly(); /// inform client: accepted for the session
	writer.addSessionResponseMessage(Session::getSingletonPointer(), identifier);
	TCPServerEnd::addTo(writer.endPacketAssembly(SERVER_IDENTIFIER), serverEnd);

	writer.beginPacketAssembly(); /// inform all other clients: new session member
	writer.addSessionNewMemberMessage(Session::getSingleton().getMember(serverEnd->getIdentifier()));
	vector<TCPServerEnd *> *receivers = new vector<TCPServerEnd *>();
	computeSessionMemberReceivers(serverEnd->getIdentifier(), receivers);
	TCPServerEnd::addToSeveralServerEnds(writer.endPacketAssembly(SERVER_IDENTIFIER), receivers);
}

void Server::processClosedConnection(uint32 connectionIndex)
{
	uint16 identifier = mConnections[connectionIndex]->getIdentifier();
	TCPPacketWriter writer;

	deregisterAndDestroyServerEnd(connectionIndex);
	if (0 == identifier)	// is the member with this connection part of the game session?
		return;	// just a pending connection

	// update the session object and inform other members about disconnect
	Session::getSingleton().removeMember(identifier);
	writer.beginPacketAssembly();
	writer.addSessionRemoveMemberMessage(identifier);
	vector<TCPServerEnd *> *receivers = new vector<TCPServerEnd *>();
	computeSessionMemberReceivers(0, receivers);
	TCPServerEnd::addToSeveralServerEnds(writer.endPacketAssembly(SERVER_IDENTIFIER), receivers);
}

void Server::deregisterAndDestroyServerEnd(uint32 connectionIndex)
{
	TCPServerEnd *serverEnd = mConnections[connectionIndex];

	// erase the connection from the connections list, deregister and delete it
	mConnections[connectionIndex] = mConnections.back();
	mConnections.pop_back();
	deregisterForSending(serverEnd);
	deregisterForReceiving(serverEnd);
	delete serverEnd;
}

void Server::deregisterForSending(const EventListener *listener)
{
	FD_CLR(listener->getSocketHandle(), &mWritingSockets);
	if (listener->getSocketHandle() == mMaxSocketHandle)
		computeMaxSocketHandle();
}

void Server::deregisterForReceiving(const EventListener *listener)
{
	FD_CLR(listener->getSocketHandle(), &mReadingSockets);
	if (listener->getSocketHandle() == mMaxSocketHandle)
		computeMaxSocketHandle();
}

void Server::computeMaxSocketHandle()
{
	mMaxSocketHandle = (mListeningSocket > mUDPServer->getSocketHandle()) ? mListeningSocket : mUDPServer->getSocketHandle();
	for(uint32 i = 0; i < mConnections.size(); ++i)
		if (mMaxSocketHandle < mConnections[i]->getSocketHandle())
			mMaxSocketHandle = mConnections[i]->getSocketHandle();
}
