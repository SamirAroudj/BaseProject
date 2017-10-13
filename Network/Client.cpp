/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Client.h"
#include "ClientStateConnected.h"
#include "ClientStateConnecting.h"
#include "ClientStateDisconnected.h"
#include "ClientStateDisconnecting.h"
#include "ClientStateReadyToUse.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "Session.h"

using namespace Network;
using namespace ExceptionHandling;

Client::Client()
{
	mState = new ClientStateDisconnected();
}

Client::~Client()
{
	delete mState;
	if (Session::exists())
		delete Session::getSingletonPointer();
}

void Client::addTCPMessage(MessageType messageIdentifier,
						   const Patterns::ISerializable *serializables, uint32 numOfSerializables)
{
	assert(ClientState::READY_TO_USE == mState->getState());
	assert(numOfSerializables > 0);

	((ClientStateReadyToUse *) (mState))->addTCPMessage(
		messageIdentifier, serializables, numOfSerializables);
}

bool Client::addUDPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables,
	uint32 numOfSerializables, UDPPacket::Delivery delivery, IPacketLossListener *listener)
{
	assert(ClientState::READY_TO_USE == mState->getState());
	assert(serializables);
	assert(numOfSerializables > 0);

	return ((ClientStateReadyToUse *) mState)->addUDPMessage(
		messageIdentifier, serializables, numOfSerializables, delivery, listener);
}

// try to connect to the given IP address and port
// if the update function does not receive an answer after timeout seconds then the request fails
void Client::connect(const ApplicationAddress &serverAddress, const std::string &password, Real timeoutSeconds)
{
	assert(ClientState::DISCONNECTED == mState->getState());
	delete mState;

	try
	{
		mState = new ClientStateConnecting(serverAddress, password, timeoutSeconds);
	}
	catch(NetworkException &exception)
	{
		mState = new ClientStateDisconnected();
		throw exception;
	}
}

void Client::disconnect()
{
	if (ClientState::CONNECTED == mState->getState())													// release previous state, switch to disconnecting state
	{
		mState = new ClientStateDisconnecting(((ClientStateConnected *) mState));
		return;
	}
	else if (ClientState::READY_TO_USE == mState->getState())
	{
		mState = new ClientStateDisconnecting(((ClientStateReadyToUse *) mState));
		return;
	}

	assert(false);
}

MessageType Client:: getCurrentTCPMessageIdentifier() const
{
	assert(ClientState::READY_TO_USE == mState->getState());

	return ((ClientStateReadyToUse *) mState)->getCurrentTCPMessageIdentifier();
}

MessageType Client::getCurrentUDPMessageIdentifier(uint16 &senderIdentifier) const
{
	assert(ClientState::READY_TO_USE == mState->getState());

	return ((ClientStateReadyToUse *) mState)->getCurrentUDPMessageIdentifier(senderIdentifier);
}

void Client::getTCPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
{
	assert(ClientState::READY_TO_USE == mState->getState());
	assert(numOfSerializables > 0);

	return ((ClientStateReadyToUse *) mState)->getTCPMessage(serializables, numOfSerializables);
}

void Client::getUDPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
{
	assert(ClientState::READY_TO_USE == mState->getState());
	assert(numOfSerializables > 0);

	return ((ClientStateReadyToUse *) mState)->getUDPMessage(serializables, numOfSerializables);
}

Real Client::getUDPMessageNetworkTime() const
{
	assert(ClientState::READY_TO_USE == mState->getState());
	return ((ClientStateReadyToUse *) mState)->getUDPMessageNetworkTime();
}

void Client::kick(uint16 identifier)
{
	assert(ClientState::READY_TO_USE == mState->getState());
	assert(identifier != 0);

	((ClientStateReadyToUse *) mState)->kick(identifier);
}

// sending is only possible with a connection and can fail if the connection is broken
void Client::send()
{
	assert(ClientState::READY_TO_USE == mState->getState());
	try
	{
		((ClientStateReadyToUse *) mState)->send();
	}
	catch(ExceptionHandling::ClosedConnectionException &exception)				// broken connection
	{
		delete mState;
		mState = new ClientStateDisconnected();
		throw exception;
	}
}

void Client::update()
{
	try																			// the connection could be closed suddenly, that should be caught
	{
		switch(mState->update())													// what is the next state?
		{
		case ClientState::DISCONNECTED:
			switchToDisconnectedState();
			break;

		case ClientState::CONNECTED:												// connection is built -> negotiate network session acceptance
			switchToConnectedState();
			break;

		case ClientState::READY_TO_USE:												// accepted for the network session
			assert(ClientState::CONNECTED == mState->getState());
			mState = new ClientStateReadyToUse((ClientStateConnected *)mState);			// no memory leak since ClientStateConnected() frees the previous state
			break;

		case ClientState::DISCONNECTING:
			switchToDisconnectingState();
			break;

		case ClientState::NO_STATE_CHANGE:
			break;
		
		case ClientState::CONNECTING:												// this should not be returned since the client switches to the
		default:																	//  connecting state when connect() is called
			assert(false);
		}
	}
	catch(ExceptionHandling::ClosedConnectionException &exception)					// broken connection
	{
		delete mState;
		mState = new ClientStateDisconnected();
		throw exception;
	}
}

void Client::switchToConnectedState()
{
	assert(ClientState::CONNECTING == mState->getState());
	try
	{
		mState = new ClientStateConnected((ClientStateConnecting *) mState);			// no memory leak since ClientStateConnected() frees the previous state
	}
	catch(NetworkException &exception)
	{
		mState = new ClientStateDisconnected();
		throw exception;
	}
}

void Client::switchToDisconnectedState()
{
	if (ClientState::CONNECTING == mState->getState())									// connection attempt failed due to timeout
	{
		ClientStateConnecting *state = (ClientStateConnecting *) mState;

		ConnectionCreationException *creationException = new ConnectionCreationException(
			"Connection creation timeout occured.",
			ConnectionCreationException::TIME_OUT, CONNECTION_CREATION_ERROR_CODE,
			convertAddressHostByteOrderToASCII(state->getServerAddress().getIPAddress()),
			state->getServerAddress().getPortNumber());
			
		delete mState;
		mState = new ClientStateDisconnected();
			
		throw creationException;
	}
	else if (ClientState::DISCONNECTING == mState->getState())							// connection finished closing
	{
		delete mState;
		mState = new ClientStateDisconnected();
		return;
	}

	assert(false);
}

void Client::switchToDisconnectingState()
{
	if (ClientState::CONNECTED == mState->getState())									// not allowed to join the network session: wrong password or session is full
	{
		ClientStateConnected * state = (ClientStateConnected *) mState;

		ConnectionCreationException *creationException = new ConnectionCreationException(
			"Connection request was rejected by the server.",
			state->getRejectionReason(), CONNECTION_CREATION_ERROR_CODE,
			convertAddressHostByteOrderToASCII(state->getServerAddress().getIPAddress()),
			state->getServerAddress().getPortNumber(), state->getPassword());
				
		mState = new ClientStateDisconnecting(state);										// no memory leak, the constructor frees the old state

		throw creationException;
	}
	else if (ClientState::READY_TO_USE == mState->getState())							// shut connection down
	{
		mState = new ClientStateDisconnecting((ClientStateReadyToUse *) mState);			// no memory leak, the constructor frees the old state
		return;
	}

	assert(false);
}
