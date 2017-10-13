/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <climits>
#include "Session.h"

using namespace Network;
using std::string;

Session::Session(uint16 ownIdentifier, uint16 maxNumOfSessionClients, Real startTime, const string &password) :
	mStartTime(startTime), mPassword(password), mOwnIdentifier(ownIdentifier), mMaxNumOfClients(maxNumOfSessionClients)
{

}

Session::~Session()
{
	for(uint32 i = 0; i < mMembers.size(); ++i)
		delete mMembers[i];
}

void Session::addMember(uint32 IPAddress, uint16 UDPPortNumber, uint16 identifier, bool multicastSupport)
{
#ifdef _DEBUG
	for(uint32 i = 0; i < mMembers.size(); ++i)
		assert(identifier != mMembers[i]->getIdentifier());
#endif

	SessionMember *member = new SessionMember(IPAddress, UDPPortNumber, identifier, multicastSupport);
	mMembers.push_back(member);
	sort();

	for(uint32 i = 0; i < mListeners.size(); ++i)
		mListeners[i]->notifyNewMember(*member);
}

void Session::addListener(ISessionListener *listener)
{
	for(uint32 i = 0; i < mMembers.size(); ++i)
		listener->notifyNewMember(*(mMembers[i]));
	mListeners.push_back(listener);
}

void Session::removeMember(uint16 identifier)
{
	uint32 numOfMembers = mMembers.size();
	for(uint32 i = 0; i < numOfMembers; ++i)
	{
		if (mMembers[i]->getIdentifier() == identifier)
		{
			uint32 numOfListeners = mListeners.size();
			for(uint32 j = 0; j < numOfListeners; ++j)
				mListeners[j]->notifyRemovedMember(identifier);

			delete mMembers[i];
			mMembers[i] = mMembers.back();
			mMembers.pop_back();
			sort();

			return;
		}
	}
	assert(false);
}

void Session::sort()
{
	// bubble sort the members according to their identifiers in ascending order (bubble sort since there won't be much changes)
	bool change = false;

	for(uint32 i = 1; i < mMembers.size(); ++i)
	{
		for(uint32 j = 0; j < mMembers.size() - i; ++j)
		{
			if (mMembers[j]->getIdentifier() > mMembers[j + 1]->getIdentifier())
			{
				change = true;
				const SessionMember *temp = mMembers[j + 1];
				mMembers[j + 1] = mMembers[j];
				mMembers[j] = temp;
			}
		}

		if (!change)
			return;
		change = false;
	}
}

void Session::removeListener(ISessionListener *listener)
{
	for(uint32 i = 0; i < mListeners.size(); ++i)
	{
		if (mListeners[i] == listener)
		{
			mListeners[i] = mListeners.back();
			mListeners.pop_back();
			return;
		}
	}
}

bool Session::isMember(const uint32 IPAddress, const uint16 portNumber, uint16 &identifier) const
{
	for(uint32 i = 0; i < mMembers.size(); ++i)	// check for a member with this IP address and port number
	{
		if (mMembers[i]->getIPAddress() == IPAddress && mMembers[i]->getUDPPortNumber() == portNumber)
		{
			identifier = mMembers[i]->getIdentifier();
			return true;
		}
	}
	
	identifier = USHRT_MAX;	// no session member
	return false;
}

const SessionMember *Session::getMember(uint16 identifier) const
{
	for(uint32 i = 0; i < mMembers.size(); ++i)
		if (mMembers[i]->getIdentifier() == identifier)
			return mMembers[i];
	assert(false);
	return NULL;
}

void Session::getClientIdentifiers(uint16 *&identifiers) const
{
	if (mMembers.size() == 1)
	{
		identifiers = NULL;
		return;
	}

	identifiers = new uint16[mMembers.size() - 1];
	for(uint32 i = 1; i < mMembers.size(); ++i)
		identifiers[i - 1] = mMembers[i]->getIdentifier();
}

uint16 Session::getNextFreeIdentifier() const
{
	uint16 newIdentifier = 0;
	for(uint32 i = 0; i < mMembers.size(); ++i)
	{
		if (newIdentifier < mMembers[i]->getIdentifier())
			return newIdentifier;
		++newIdentifier;
	}

	return newIdentifier;
}