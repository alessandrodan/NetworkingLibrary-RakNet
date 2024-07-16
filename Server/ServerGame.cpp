#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <Network/NetDevice.cpp>
#include <Network/Definition.h>
#include "ServerGame.h"
#include <Network/PacketDefinition.h>

using namespace Net;

ServerGame::ServerGame()
{
	m_packetHeader = std::make_unique<PacketManager>();
	__LoadPacketHeaders();
}

bool ServerGame::Initialize(const char* c_szAddr, int port)
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

void ServerGame::__LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_ACTION1, PacketManager::TPacketType(sizeof(TPacketCGAction1), &ServerGame::TestRecv));
}

void ServerGame::Process()
{
	for (SLNet::Packet* packet = CNetDevice::peer->Receive(); packet; CNetDevice::peer->DeallocatePacket(packet), packet = CNetDevice::peer->Receive())
	{
		PacketManager::TPacketType packetType;
		if (m_packetHeader->Get(packet->data[0], &packetType))
		{
			if (packet->length == packetType.iPacketSize)
				(this->*packetType.packetHandler)(packet);
			else
				std::cout << "Packet size mismatch for header " << (unsigned)packet->data[0] << std::endl;
		}
		else
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
}

bool ServerGame::TestRecv(SLNet::Packet* packet)
{
	if (!packet)
		return false;

	TPacketCGAction1 action1;
	if (packet->length != sizeof(action1))
		return false;

	SLNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.Read((char*)&action1, sizeof(action1));

	std::cout << "HEADER_CG_ACTION1 receved. num = " << action1.numIntero << std::endl;

	TestSend(packet);
	return true;
}

bool ServerGame::TestSend(SLNet::Packet* packet)
{
	TPacketGCResponse response;

	SLNet::BitStream bsOut(sizeof(response));
	bsOut.Write((char*)&response, sizeof(response));
	CNetDevice::peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

	return true;
}
