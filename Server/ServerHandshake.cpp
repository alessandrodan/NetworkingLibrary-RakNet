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
	InitializeBase();
}

void ServerHandshake::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_HANDSHAKE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGHandshake), &ServerHandshake::RecvHandshake));
}

bool ServerHandshake::RecvHandshake(SLNet::Packet* packet, Net::CAbstractPeer* peer)
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
