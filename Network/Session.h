/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SESSION_H_
#define _SESSION_H_

#include <cassert>
#include <string>
#include <vector>
#include "Platform/DataTypes.h"
#include "ISessionListener.h"
#include "Patterns/Singleton.h"

namespace Network
{
	class Session : public Patterns::Singleton<Session>
	{
	public:
		Session(uint16 ownIdentifier, uint16 maxNumOfSessionClients, Real startTime, const std::string &password = "");
		virtual ~Session();

		void addMember(uint32 IPAddress, uint16 udpPortNumber, uint16 identifier, bool multicastSupport);
		void addListener(ISessionListener *listener);

		/// user must free the created array
		void getClientIdentifiers(uint16 *&identifiers) const;
		uint16 getMaxNumOfClients() const { return mMaxNumOfClients; }
		const SessionMember *getMember(uint16 identifier) const;
		const std::vector<const SessionMember *> &getMembers() const { return mMembers; }
		uint16 getNextFreeIdentifier() const;
		uint16 getNumOfClients() const { return mMembers.size() - 1; }
		const SessionMember *getOwnMemberData() const { return getMember(mOwnIdentifier); }
		uint16 getOwnIdentifier() const { return mOwnIdentifier; }
		const std::string &getPassword() const { return mPassword; }
		const Real getStartTime() const { return mStartTime; }

		bool isFull() const { return (0 == mMaxNumOfClients) ? false : mMaxNumOfClients == (mMembers.size() - 1); }
		bool isMember(const uint32 IPAddress, const uint16 portNumber, uint16 &identifier) const;
		bool isPasswordProtected() const { return !mPassword.empty(); }

		void removeMember(uint16 identifier);
		void removeListener(ISessionListener *listener);
		bool verifyPassword(const std::string &passwordGuess) { return mPassword.empty() ? true : passwordGuess == mPassword; }

	private:
		void sort();

		std::vector<ISessionListener *> mListeners;
		std::vector<const SessionMember *> mMembers;
		std::string mPassword;
		const Real mStartTime;
		uint16 mOwnIdentifier;
		uint16 mMaxNumOfClients;

		/// forbidden
		Session(const Session &copy) : mStartTime(0) { assert(false); }
		/// forbidden
		Session &operator =(const Session &rhs) { assert(false); return *this; }
	};
}

#endif // _SESSION_H_

