#include "StdAfx.h"
#include "ServerAuth.h"
#include "Packet.h"
#include <iostream>
#include <Network/NetDevice.h>
#include <Network/PacketIO.hpp>
#include "Peer.h"
#include "PeerManager.h"

using namespace Net;

ServerAuth::ServerAuth()
{
	InitializePacketHandler();
}

void ServerAuth::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_AUTH_REQUEST, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGAuthRequest), &ServerAuth::TestRecv));
}

void ServerAuth::ProcessPacketError(EProcessPacketError errorType, NetPacket* packet, CAbstractPeer* peer)
{
	switch (errorType)
	{
		case EProcessPacketError::HEADER_NOT_FOUND:
			std::cerr << "Header not found: " << packet->header << std::endl;
			peer->SetPhase(PHASE_CLOSE);
			break;

		case Net::EProcessPacketError::SIZE_MISMATCH:
			std::cerr << "Size mismatch for header: " << packet->header << std::endl;
			peer->SetPhase(PHASE_CLOSE);
			break;

		case Net::EProcessPacketError::HANDLE_FAILED:
			std::cerr << "Failed to handle packet with header: " << packet->header << std::endl;
			break;
	}
}

bool ServerAuth::TestRecv(NetPacket* packet, Net::CAbstractPeer* peer)
{
	CPeer* d = CPeerManager::ValidPeer(peer);
	if (!d)
		return false;

	TPacketCGAuthRequest authRequest;
	if (!CPacketIO::ReadPacketData(packet, authRequest))
		return false;

	if (!strcmp(authRequest.username, "username") && !strcmp(authRequest.password, "password123"))
	{
		std::cout << "HEADER_CG_AUTH_REQUEST receved. correct" << std::endl;
		d->SetPhase(PHASE_GAME);
	}
	else
	{
		std::cout << "HEADER_CG_AUTH_REQUEST receved. not correct. username = " << authRequest.username << "\t password = " << authRequest.password << std::endl;
	}

	return true;
}
