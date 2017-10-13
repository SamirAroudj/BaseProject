/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <string>
#include "ApplicationAddress.h"
#include "Network.h"
#include "TCPEnd.h"

namespace Network
{
	class TCPClientEnd : public TCPEnd
	{
	public:
		TCPClientEnd(int32 socketHandle);
		virtual ~TCPClientEnd();
		void connect(const ApplicationAddress &serverAddress);

		virtual bool notifyReceivingIsPossible();
		uint32 getNumberOfPendingBytesToSend() const { return mNumberOfPendingBytes; }
		void addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables);
		void addPacket(const TCPPacket *&packet, bool appendToFront = false);
		virtual void send();

	private:
		const TCPPacket *mPendingPacket;
		const TCPPacket *mPartialSentPacket;
		uint32 mNumberOfPendingBytes;

		// forbidden
		TCPClientEnd(const TCPClientEnd &copy) : TCPEnd(0) { assert(false); }
		// forbidden
		TCPClientEnd &operator =(const TCPClientEnd &rhs) { assert(false); return *this; }
	};
}

#endif // _TCP_CLIENT_H_

