#include "StdAfx.h"
#include "ServerHandshake.h"
#include "Packet.h"
#include <iostream>
#include <Network/NetDevice.h>
#include "Peer.h"
#include <Network/PacketIO.hpp>
#include "PeerManager.h"

using namespace Net;

ServerHandshake::ServerHandshake()
{
	InitializePacketHandler();
}

void ServerHandshake::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_HANDSHAKE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGHandshake), &ServerHandshake::RecvHandshake));
}

void ServerHandshake::ProcessPacketError(Net::EProcessPacketError errorType, NetPacket* packet, Net::CAbstractPeer* peer)
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

bool ServerHandshake::RecvHandshake(NetPacket* packet, Net::CAbstractPeer* peer)
{
	CPeer* d = CPeerManager::ValidPeer(peer);
	if (!d)
		return false;

	TPacketCGHandshake p;
	if (!CPacketIO::ReadPacketData(packet, p))
		return false;

	if (d->GetHandshake() != p.dwHandshake)
	{
		std::cerr << "Invalid Handshake on" << d->GetGUID().ToString() << std::endl;
		return false;
	}

	if (d->IsPhase(PHASE_HANDSHAKE))
	{
		if (d->HandshakeProcess(p.dwTime, p.lDelta))
			d->SetPhase(PHASE_AUTH);
	}

	return true;
}
