#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.cpp>
#include "Client.h"
#include <Network/Definition.h>

using namespace Net;

bool Client::Initialize(const char* c_szAddr, int port)
{
	SLNet::SocketDescriptor socketDescriptor;
	const auto startupResult = CNetDevice::peer->Startup(CLIENT_MAX_CONNECTIONS, &socketDescriptor, 1);
	if (startupResult != SLNet::RAKNET_STARTED)
	{
		std::cout << "Failed to start client! Quitting - error code: "<< startupResult << std::endl;
		return false;
	}

	const auto connectionAttemptResult = CNetDevice::peer->Connect(c_szAddr, port, 0, 0, 0, CLIENT_CONNECTION_ATTEMPT_COUNT, CLIENT_TIME_BTW_SEND_CONNECTION_ATTEMPT_COUNT);
	if (connectionAttemptResult != SLNet::CONNECTION_ATTEMPT_STARTED)
	{
		std::cout << "Failed to connect client! Quitting - error code: " << connectionAttemptResult << std::endl;
		return false;
	}

	std::cout << "socket connected to the server" << std::endl;

    return true;
}

void Client::Process()
{
}
