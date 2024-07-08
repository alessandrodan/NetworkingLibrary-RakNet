#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.cpp>
#include "Client.h"
#include <Network/Definition.h>
#include <Network/PacketDefinition.h>

using namespace Net;

Client::Client()
{
	isConnected = false;
	m_packetHeader = std::make_unique<CClientPacketHeaderMap>();
}

bool Client::Initialize(const char* c_szAddr, int port)
{
	SLNet::SocketDescriptor socketDescriptor;
	const auto startupResult = CNetDevice::peer->Startup(CLIENT_MAX_CONNECTIONS, &socketDescriptor, 1);
	if (startupResult != SLNet::RAKNET_STARTED)
	{
		std::cout << "Failed to start client! Quitting - error code: "<< startupResult << std::endl;
		return false;
	}

	const auto connectionAttemptResult = CNetDevice::peer->Connect(c_szAddr, port, 0, 0, 0, 0, CLIENT_CONNECTION_ATTEMPT_COUNT, CLIENT_TIME_BTW_SEND_CONNECTION_ATTEMPT_COUNT);
	if (connectionAttemptResult != SLNet::CONNECTION_ATTEMPT_STARTED)
	{
		std::cout << "Failed to connect client! Quitting - error code: " << connectionAttemptResult << std::endl;
		return false;
	}

	CNetDevice::peer->SetOccasionalPing(true);

	std::cout << "socket connected to the server" << std::endl;

    return true;
}

void Client::Process()
{
	for (SLNet::Packet* packet = CNetDevice::peer->Receive(); packet; CNetDevice::peer->DeallocatePacket(packet), packet = CNetDevice::peer->Receive())
	{
		Net::CAbstractPacketHeaderMap::TPacketType packetType;
		if (m_packetHeader->Get(packet->data[0], &packetType))
		{
			if (packet->length == packetType.iPacketSize)
			{
				switch (packet->data[0])
				{
					case PacketGCHeader::HEADER_GC_RESPONSE:
						std::cout << "HEADER_GC_RESPONSE" << std::endl;
						break;

					default:
						std::cout << "Wrong packet. id " << (unsigned)packet->data[0] << " packet length " << packet->length << " from " << packet->systemAddress.ToString() << std::endl;
						CNetDevice::peer->CloseConnection(packet->systemAddress, true);
				}
			}
			else
			{
				std::cout << "Packet size mismatch for header " << (unsigned)packet->data[0] << std::endl;
			}
		}
		else
		{
			switch (packet->data[0])
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
					std::cout << "Our connection request has been accepted" << std::endl;
					isConnected = true;
					break;

				case ID_CONNECTION_ATTEMPT_FAILED:
					std::cout << "Our connection request has been FAILED" << std::endl;
					isConnected = false;
					break;

				case ID_DISCONNECTION_NOTIFICATION:
					std::cout << "Server Remote Disconnected" << std::endl;
					break;

				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					std::cout << "Server Disconnected" << std::endl;
					break;

				case ID_CONNECTION_LOST:
					std::cout << "Server Connection lost" << std::endl;
					break;

				case ID_REMOTE_CONNECTION_LOST:
					std::cout << "Server Connection Remote lost" << std::endl;
					break;

				default:
					std::cout << "Wrong packet. id " << (unsigned)packet->data[0] << " packet length " << packet->length << " from " << packet->systemAddress.ToString() << std::endl;
					CNetDevice::peer->CloseConnection(packet->systemAddress, true);
			}
		}
	}
}

bool Client::IsConnected()
{
	return isConnected;
}

void Client::TestSend()
{
	TPacketCGAction1 packet;
	packet.numIntero = 5;

	SLNet::BitStream bsOut(sizeof(packet));
	bsOut.Write((char*)&packet, sizeof(packet));
	CNetDevice::peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
