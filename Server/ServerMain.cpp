#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <Network/NetDevice.h>
#include <Network/Definition.h>
#include "ServerMain.h"
#include <Network/PacketDefinition.h>

using namespace Net;

bool ServerMain::Initialize(const char* c_szAddr, int port)
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

	CNetDevice::peer->SetIncomingPassword(0, 0);
	CNetDevice::peer->SetTimeoutTime(10000, SLNet::UNASSIGNED_SYSTEM_ADDRESS);
	CNetDevice::peer->SetOccasionalPing(true);
	CNetDevice::peer->SetUnreliableTimeout(1000);

	std::cout << "Socket Listening..." << std::endl;

	return true;
}

void ServerMain::Process()
{
	for (SLNet::Packet* packet = CNetDevice::peer->Receive(); packet; CNetDevice::peer->DeallocatePacket(packet), packet = CNetDevice::peer->Receive())
	{
		switch (packet->data[0])
		{
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "New incoming connection: " << packet->systemAddress.ToString() << std::endl;
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "Client Disconnected: " << packet->systemAddress.ToString() << std::endl;
				break;

			case ID_CONNECTION_LOST:
				std::cout << "Client Connection lost: " << packet->systemAddress.ToString() << std::endl;
				break;

			default:
				std::cout << "Wrong packet. id " << (unsigned)packet->data[0] << " packet length " << packet->length << " from " << packet->systemAddress.ToString() << std::endl;
				CNetDevice::peer->CloseConnection(packet->systemAddress, true);
		}
	}
}

void ServerMain::__LoadPacketHeaders()
{
}
