#include "StdAfx.h"
#include <iostream>
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

void Client::__OnInitFail(int errorCode)
{
	std::cout << "Failed to start client! Quitting - error code: " << errorCode << std::endl;
}

void Client::__OnInitSuccess()
{
}

void Client::__OnConnectFail(int errorCode)
{
	std::cout << "Failed to connect client! Quitting - error code: " << errorCode << std::endl;
}

void Client::__OnConnectSuccess()
{
	std::cout << "Connection request was sent successfully" << std::endl;
}

void Client::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketGCHeader::HEADER_GC_PHASE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketGCPhase), &Client::RecvPhase));
	m_packetHeader->Set(PacketGCHeader::HEADER_GC_HANDSHAKE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketGCHandshake), &Client::RecvHandshake));
	m_packetHeader->Set(PacketGCHeader::HEADER_GC_RESPONSE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketGCResponse), &Client::TestRecv));
}

void Client::ProcessPacketError(Net::EProcessPacketError errorType, NetPacket* packet)
{
	switch (errorType)
	{
		case EProcessPacketError::HEADER_NOT_FOUND:
			std::cerr << "Header not found: " << packet->header << std::endl;
			CNetDevice::CloseConnection(packet->systemAddress, true);
			break;

		case Net::EProcessPacketError::SIZE_MISMATCH:
			std::cerr << "Size mismatch for header: " << packet->header << std::endl;
			CNetDevice::CloseConnection(packet->systemAddress, true);
			break;

		case Net::EProcessPacketError::HANDLE_FAILED:
			std::cerr << "Failed to handle packet with header: " << packet->header << std::endl;
			break;
	}
}

void Client::ProcessNet()
{
	for (SLNet::Packet* packet = CNetDevice::peer->Receive(); packet; CNetDevice::peer->DeallocatePacket(packet), packet = CNetDevice::peer->Receive())
	{
		NetPacket netPacket;
		ConvertToNetPacket(packet, &netPacket);

		switch (netPacket.header)
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
				Process(&netPacket);
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

	Net::CPacketIO::SendPacket(&packet, sizeof(packet), Net::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::SendLogin()
{
	// user login simulation

	TPacketCGAuthRequest packet;
	strcpy(packet.username, "username");
	const char* passwords[] = { "password1", "password12", "password123" };
	strcpy(packet.password, passwords[rand() % 3]);

	Net::CPacketIO::SendPacket(&packet, sizeof(packet), Net::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::TestSend()
{
	TPacketCGAction1 packet;
	packet.numIntero = 5;

	Net::CPacketIO::SendPacket(&packet, sizeof(packet), Net::UNASSIGNED_SYSTEM_ADDRESS, true);
}

bool Client::RecvPhase(NetPacket* packet)
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

bool Client::RecvHandshake(NetPacket* packet)
{
	TPacketGCHandshake handshake;
	if (!CPacketIO::ReadPacketData(packet, handshake))
		return false;

	std::cout << "HANDSHAKE RECV" << handshake.dwTime << "\t" << handshake.lDelta << std::endl;

	std::cout << "HANDSHAKE SEND" << std::endl;
	SendHandshake(handshake.dwHandshake, handshake.dwTime, handshake.lDelta);

	return true;
}

bool Client::TestRecv(NetPacket* packet)
{
	TPacketGCResponse response;
	if (!CPacketIO::ReadPacketData(packet, response))
		return false;

	std::cout << "HEADER_GC_RESPONSE" << std::endl;

	return true;
}
