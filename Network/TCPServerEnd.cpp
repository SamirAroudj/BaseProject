/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "MagicNetworkConstants.h"
#include "TCPServerEnd.h"

using namespace Network;
using namespace std;

deque<PendingPacket *> TCPServerEnd::msPendingPackets;

TCPServerEnd::TCPServerEnd(int32 socketHandle) : TCPEnd(socketHandle)
{

}

TCPServerEnd::~TCPServerEnd()
{	// if this server connection end is listed as a sender of a pending packet, then remove it from the sender list of this packet
	for(uint32 i = 0; i < msPendingPackets.size(); ++i)
	{
		for(uint32 j = 0; j < msPendingPackets[i]->getSenders().size(); ++j)
		{
			if (this == msPendingPackets[i]->getSenders()[j])
			{
				msPendingPackets[i]->getSenders()[j] = msPendingPackets[i]->getSenders().back();
				msPendingPackets[i]->getSenders().pop_back();
			}
		}
	}
}

void TCPServerEnd::destroyAllPendingPackets()
{
	for(uint32 i = 0; i < msPendingPackets.size(); ++i)
		delete msPendingPackets[i];
	msPendingPackets.clear();
}

void TCPServerEnd::addTo(TCPPacket *packet, TCPServerEnd *sender)
{
	assert(packet && sender);
	vector<TCPServerEnd *> *senders = new vector<TCPServerEnd *>();
	senders->push_back(sender);
	addToSeveralServerEnds(packet, senders);
}

void TCPServerEnd::addToSeveralServerEnds(TCPPacket *packet, std::vector<TCPServerEnd *> *&senders)
{
	assert(packet && senders);
	if (senders->empty())
	{
		delete packet;
		delete senders;
		senders = NULL;
		return;
	}

	msPendingPackets.push_back(new PendingPacket(packet, senders));
}

void TCPServerEnd::addToSeveralServerEnds(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
										  vector<TCPServerEnd *> *senders)
{
	assert(serializables && senders && numOfSerializables > 0);
	TCPPacket *packet = createTCPPacket(messageIdentifier, serializables, numOfSerializables, SERVER_IDENTIFIER);
	msPendingPackets.push_back(new PendingPacket(packet, senders));
}

void TCPServerEnd::send()
{
	uint32 sentBytes = 0;

	while(!msPendingPackets.empty()) // try to send every pending packet
	{
		vector<TCPServerEnd *> &senders = msPendingPackets[0]->getSenders(); // take the first pending packet in the queue -> senders + actual packet
		const TCPPacket &packet = msPendingPackets[0]->getPacket();

		for(uint32 i = 0; i < senders.size();) // dispatch this packet by all required senders
		{
			if (senders[i]->sendOnePacket(packet, sentBytes))
			{
				senders[i] = senders.back();  // this sender dispatched the packet
				senders.pop_back();	// so remove him from the list
			}
			else
			{
				 ++i;
			}
		}

		if (senders.empty()) // release this data packet since it was sent to all required destinations
		{
			delete msPendingPackets[0];
			msPendingPackets.pop_front();
		}
		else // there is a sender which could not finish sending this packet
		{
			return;
		}
	}
}