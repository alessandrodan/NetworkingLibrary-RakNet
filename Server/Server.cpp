#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.cpp>
#include <Network/Definition.h>
#include "Server.h"

using namespace Net;

bool Server::Initialize(const char* c_szAddr, int port)
{
	SLNet::SocketDescriptor socketDescriptor;
	strcpy(socketDescriptor.hostAddress, c_szAddr);
	socketDescriptor.port = static_cast<unsigned short>(port);
	socketDescriptor.socketFamily = AF_INET;

	const SLNet::StartupResult startupResult = CNetDevice::peer->Startup(SERVER_MAX_CONNECTIONS, &socketDescriptor, 1);
	if (startupResult != SLNet::RAKNET_STARTED)
	{
		std::cout << "Failed to start server! Quitting - error code: " << startupResult << std::endl;
		return false;
	}

	CNetDevice::peer->SetMaximumIncomingConnections(SERVER_MAX_INCOMING_CONNECTIONS);

	std::cout << "Socket Listening..." << std::endl;

    return true;
}

void Server::Process()
{
}
