/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "TCPPacketReader.h"

using namespace Network;
using namespace std;

TCPPacketReader::TCPPacketReader()
{
}


TCPPacketReader::~TCPPacketReader()
{
}

void TCPPacketReader::beginReading(TCPPacket *&packet)
{
	PacketReader::beginReading(packet);
	readNextMessageType();
	packet = NULL;
}

void TCPPacketReader::readSessionIsFullMessage()
{
	assert(TCPPacket::SESSION_IS_FULL == mCurrentMessageType);
	readNextMessageType();
}

void TCPPacketReader::readSessionRequestMessage(uint32 &IPAddress, uint16 &remoteUDPPortNumber, std::string &password, bool &multicastSupport)
{
	assert(TCPPacket::SESSION_REQUEST == mCurrentMessageType);
	IPAddress = readUInt32();
	remoteUDPPortNumber = readUInt16();
	readString(password);
	multicastSupport = readBool();

	readNextMessageType();
}

void TCPPacketReader::readSessionResponseMessageAndCreateSession()
{
	assert(TCPPacket::SESSION_POSITIVE_RESPONSE == mCurrentMessageType);
	string password = "";
	uint16 myIdentifier = readUInt16();
	uint16 maxNumOfClients = readUInt16();
	readString(password);
	Real sessionStartTime = readFloat();
	uint16 numOfSessionMembers = readUInt16();

	Session *session = new Session(myIdentifier, maxNumOfClients, sessionStartTime, password);
	for(uint16 i = 0; i < numOfSessionMembers; ++i)
	{
		uint32 ipAddress = readUInt32();
		uint16 portNumber = readUInt16();
		uint16 identifier = readUInt16();
		bool multicastSupport = readBool();
		session->addMember(ipAddress, portNumber, identifier, multicastSupport);
	}

	readNextMessageType();
}

void TCPPacketReader::readSessionNewMemberMessage(uint32 &IPAddress, uint16 &UDPPortNumber, uint16 &identifier, bool &multicastSupport)
{
	assert(TCPPacket::SESSION_NEW_MEMBER == mCurrentMessageType);

	IPAddress = readUInt32();
	UDPPortNumber = readUInt16();
	identifier = readUInt16();
	multicastSupport = readBool();

	readNextMessageType();
}

void TCPPacketReader::readSessionRemoveMemberMessage(uint16 &identifier)
{
	assert(TCPPacket::SESSION_REMOVE_MEMBER == mCurrentMessageType);
	identifier = readUInt16();

	readNextMessageType();
}

void TCPPacketReader::readSessionWrongPasswordMessage()
{
	assert(TCPPacket::SESSION_WRONG_PASSWORD == mCurrentMessageType);

	readNextMessageType();
}