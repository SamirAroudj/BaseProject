/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Network.h"
#include "TCPPacketWriter.h"

using namespace Network;
using namespace std;

TCPPacketWriter::TCPPacketWriter() : PacketWriter(TCP_PACKET_WRITER_BUFFER_SIZE)
{
}

TCPPacketWriter::~TCPPacketWriter()
{

}

bool TCPPacketWriter::addSessionIsFullMessage()
{
	if (sizeof(MessageType) > mMaxPacketSize - mPacketSize)
		return false;

	write(static_cast<MessageType>(TCPPacket::SESSION_IS_FULL));
	return true;
}

bool TCPPacketWriter::addSessionRequestMessage(uint32 IPAddress, uint16 ownUDPPortNumber, const std::string &password, bool multicastSupport)
{
	uint32 messageLength = sizeof(MessageType) + sizeof(IPAddress) + sizeof(ownUDPPortNumber) +
		sizeof(char) * (1 + password.length()) + sizeof(multicastSupport);
	if (messageLength > mMaxPacketSize - mPacketSize)
		return false;	// there is not enough free space left for this message

	write(static_cast<MessageType>(TCPPacket::SESSION_REQUEST));
	write(IPAddress);
	write(ownUDPPortNumber);
	write(password);
	write(multicastSupport);
	return true;
}

bool TCPPacketWriter::addSessionResponseMessage(Session *session, uint16 remoteIdentifier)
{
	vector<const SessionMember *> members = session->getMembers();// see the things below which are written to the packet to understand the message size
	uint16 messageSize = sizeof(MessageType) + sizeof(remoteIdentifier) + sizeof(uint16) + sizeof(char) * (1 + session->getPassword().length()) +
		sizeof(Real) + sizeof(uint16) +	members.size() * (sizeof(uint32) + sizeof(uint16) + sizeof(uint16) + sizeof(bool));
	if (messageSize > mMaxPacketSize - mPacketSize)
		return false;	// there is not enough free space left for this message

	write(static_cast<MessageType>(TCPPacket::SESSION_POSITIVE_RESPONSE));
	write(remoteIdentifier);
	write(session->getMaxNumOfClients());
	write(session->getPassword());
	write(session->getStartTime());
	write(static_cast<uint16>(members.size()));

	for(uint32 i = 0; i < members.size(); ++i)
	{
		write(members[i]->getIPAddress());
		write(members[i]->getUDPPortNumber());
		write(members[i]->getIdentifier());
		write(members[i]->getMulticastSupport());
	}
	return true;
}

bool TCPPacketWriter::addSessionNewMemberMessage(const SessionMember *member)
{
	if ((sizeof(MessageType) + sizeof(uint32) + sizeof(uint16) + sizeof(uint16) + sizeof(bool)) > mMaxPacketSize - mPacketSize)
		return false;	// there is not enough free space left for this message

	write(static_cast<MessageType>(TCPPacket::SESSION_NEW_MEMBER));
	write(member->getIPAddress());
	write(member->getUDPPortNumber());
	write(member->getIdentifier());
	write(member->getMulticastSupport());

	return true;
}

bool TCPPacketWriter::addSessionRemoveMemberMessage(uint16 identifier)
{
	if ((sizeof(MessageType) + sizeof(uint16)) > mMaxPacketSize - mPacketSize)
		return false;	// there is not enough free space left for this message

	write(static_cast<MessageType>(TCPPacket::SESSION_REMOVE_MEMBER));
	write(identifier);
	return true;
}

bool TCPPacketWriter::addSessionWrongPasswordMessage()
{
	if (sizeof(MessageType) > mMaxPacketSize - mPacketSize)
		return false;

	write(static_cast<MessageType>(TCPPacket::SESSION_WRONG_PASSWORD));
	return true;
}

void TCPPacketWriter::beginPacketAssembly()
{
	assert(0 == mPacketSize); // there is no packet which is being assembled at the moment
}

TCPPacket *TCPPacketWriter::endPacketAssembly(uint16 senderIdentifier)
{
	if (0 == mPacketSize)
		return NULL;

	// create the packet with size <= mMaxPacketSize and data from the working buffer
	TCPPacket *packet = new TCPPacket(mPacketSize, mWorkingBuffer, senderIdentifier);
	mPacketSize = 0;
	return packet;
}