#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.h>
#include "Client.h"
#include <Network/Definition.h>
#include "Packet.h"
#include <Network/PacketIO.hpp>

using namespace Net;

Client::Client()
{
	InitializePacketHandler();
	isConnected = false;

	srand(static_cast<unsigned int>(time(0)));
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

void Client::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketGCHeader::HEADER_GC_PHASE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketGCPhase), &Client::RecvPhase));
	m_packetHeader->Set(PacketGCHeader::HEADER_GC_HANDSHAKE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketGCHandshake), &Client::RecvHandshake));
	m_packetHeader->Set(PacketGCHeader::HEADER_GC_RESPONSE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketGCResponse), &Client::TestRecv));
}

void Client::ProcessNet()
{
	for (SLNet::Packet* packet = CNetDevice::peer->Receive(); packet; CNetDevice::peer->DeallocatePacket(packet), packet = CNetDevice::peer->Receive())
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
				Process(packet);

				//std::cout << "Wrong packet. id " << (unsigned)packet->data[0] << " packet length " << packet->length << " from " << packet->systemAddress.ToString() << std::endl;
				//CNetDevice::peer->CloseConnection(packet->systemAddress, true);
		}
	}
}

bool Client::IsConnected()
{
	return isConnected;
}

void Client::SendHandshake(uint32_t dwHandshake, uint32_t dwTime, long lDelta)
{
	TPacketCGHandshake packet;

	packet.bHeader = HEADER_CG_HANDSHAKE;
	packet.dwHandshake = dwHandshake;
	packet.dwTime = dwTime;
	packet.lDelta = lDelta;

	Net::CPacketIO::SendPacket(&packet, sizeof(packet), SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::SendLogin()
{
	// user login simulation

	TPacketCGAuthRequest packet;
	strcpy(packet.username, "username");
	const char* passwords[] = { "password1", "password12", "password123" };
	strcpy(packet.password, passwords[rand() % 3]);

	Net::CPacketIO::SendPacket(&packet, sizeof(packet), SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::TestSend()
{
	TPacketCGAction1 packet;
	packet.numIntero = 5;

	Net::CPacketIO::SendPacket(&packet, sizeof(packet), SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

bool Client::RecvPhase(SLNet::Packet* packet)
{
	TPacketGCPhase phase;
	if (!CPacketIO::ReadPacketData(packet, phase))
		return false;

	switch (phase.phase)
	{
		case PHASE_AUTH:
		{
		}
		break;
	}

	return true;
}

bool Client::RecvHandshake(SLNet::Packet* packet)
{
	TPacketGCHandshake handshake;
	if (!CPacketIO::ReadPacketData(packet, handshake))
		return false;

	std::cout << "HANDSHAKE RECV" << handshake.dwTime << "\t" << handshake.lDelta << std::endl;

	std::cout << "HANDSHAKE SEND" << std::endl;
	SendHandshake(handshake.dwHandshake, handshake.dwTime, handshake.lDelta);

	return true;
}

bool Client::TestRecv(SLNet::Packet* packet)
{
	TPacketGCResponse response;
	if (!CPacketIO::ReadPacketData(packet, response))
		return false;

	std::cout << "HEADER_GC_RESPONSE" << std::endl;

	return true;
}
