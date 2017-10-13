/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <conio.h>
#include <iostream>
#include "Platform/ApplicationTimer.h"
#include "ClientTimeManager.h"
#include "IPacketLossListener.h"
#include "LANServerAdvertiser.h"
#include "Platform/ResourceManagement/ResourceManagement.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "Server.h"
#include "TCPPacketReader.h"
#include "Platform/TimePeriod.h"
#include "UDPPacketReader.h"
#include "Math/Vector2.h"


#ifdef _WINDOWS
#include "MagicGameConstants.h"
#include "Tank.h"

using namespace GamePlay;
#endif // _WINDOWS

using namespace ExceptionHandling;
using namespace Math;
using namespace ResourceManagement;
using namespace Network;
using namespace Platform;
using namespace std;

// Memory management
const uint32 ResourceManagement::DEFAULT_POOL_BUCKET_NUMBER = 5;
const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_CAPACITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 1024, 1024, 1024, 1024, 1024 };
const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_GRANULARITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 16, 32, 64, 128, 256 };

#ifdef _LINUX
#define Sleep(x) usleep(1000 * x)
#endif // _LINUX

class LossListener : public IPacketLossListener
{
public:
	virtual ~LossListener() { }
	virtual void notifyAboutPacketLoss()
	{
		int x = 4;
	}
};

// minimum number of milliseconds the one main loop iteration should take
// if the processing time is shorter then sleep is invoked to free the cpu
const uint32 MIN_LOOP_TIME = 5u;

int32 main()
{
	LossListener lossListener;
	ApplicationTimer *timer = new ApplicationTimer();
	LANServerAdvertiser *serverAdvertiser = NULL;
	Server *server = NULL;
	ApplicationAddress serverAddress;
	uint16 identifier;
#ifdef _WINDOWS
	Tank tank(0, Vector2(0.0f, 0.0f), 0.0f);
#endif // _WINDOWS

#ifdef _DEBUG
	unsigned int receivedUDPBytesLastPeriod = 0;
	Platform::TimePeriod *timeForUDPStatistics = new Platform::TimePeriod(10.0f);
	cout << "This is a console application to test the server." << endl;
#endif // _DEBUG

	try
	{
		Network::initializeNetwork();
		server = new Server("0", 3, 0);
		server->listen(10);
		server->getAddress(serverAddress);
		serverAdvertiser = new LANServerAdvertiser(serverAddress, 3722, 0);

		while(true)
		{
			if (_kbhit())
				if ('e' == _getch())
					break;
			Real loopStartTime = timer->getSystemTime();

			timer->update();
			server->update();
			serverAdvertiser->update();

			while(TCPPacket::NO_MESSAGE != server->getCurrentTCPMessageIdentifier(identifier))
			{	
#ifdef _DEBUG
				cout << "Server received a TCP packet." << endl;
#endif // _DEBUG
				/*switch(server->getCurrentTCPMessageIdentifier(identifier))
				{
				default:
					assert(false);
				}

#ifdef _DEBUG
				cout << "Packet is dipatched to the clients. (except the sender)" << endl;
#endif // _DEBUG
				server->addTCPMessageToAllExceptOne(CHAT_MESSAGES_IDENTIFIER, messages, identifier);*/
			}
			
			const vector<const SessionMember *> &members = Session::getSingleton().getMembers();
			while(UDPPacket::NO_MESSAGE != server->getCurrentUDPMessageIdentifier(identifier))
			{
				switch(server->getCurrentUDPMessageIdentifier(identifier))
				{
#ifdef _WINDOWS
				case TANK_UPDATE_MESSAGE:
				{
					Real latency = ApplicationTimer::getSingleton().getSystemTime() - server->getUDPMessageNetworkTime();
					latency = (latency < 0.0f) ? 0.0f : latency;
					server->getUDPMessage(&tank, 1);
					tank.update(latency);

					/// send data back to the clients
					server->addUDPMessage(TANK_UPDATE_MESSAGE, &tank, 1, UDPPacket::LOSS_INFO, &lossListener);
#ifdef _DEBUG
					receivedUDPBytesLastPeriod += 1 + tank.getRequiredSerializationMemory();
#endif // _DEBUG
					break;
				}
#endif // _WINDOWS

				default:
					assert(false);
				}
			}

#ifdef _DEBUG
			if (timeForUDPStatistics->hasExpired())
			{
				timeForUDPStatistics->reset();
				cout << "Mean of received UDP tank data bytes per 20ms during last " << timeForUDPStatistics->getLength() << " seconds:\n";
				Real mean = (receivedUDPBytesLastPeriod / timeForUDPStatistics->getLength()) / 50.0f;
				cout << mean << " bytes\n";
				cout << "Total number of tank data bytes received during last " << timeForUDPStatistics->getLength() << " seconds:\n";
				cout << receivedUDPBytesLastPeriod << " bytes" << endl;
				receivedUDPBytesLastPeriod = 0;
			}
#endif // _DEBUG

			server->send();
			serverAdvertiser->sendResponses();

			uint32 spentTime = static_cast<uint32>(0.5f + 1000.0f * (timer->getSystemTime() - loopStartTime));
			if (spentTime < MIN_LOOP_TIME)
				Sleep(MIN_LOOP_TIME - spentTime);

		} // main while loop

	} // try block around network functionality that can fail
	catch(SocketBindException &exception)
	{
		cout << exception << endl;
	}
	catch(NetworkException &exception)
	{
		cout << exception << endl;
	}

	delete serverAdvertiser;
	delete server;
	shutDownNetwork();
	#ifdef _DEBUG
		delete timeForUDPStatistics;
	#endif // _DEBUG
	delete timer;
	MemoryManager::shutDown();
	return 0;
}
