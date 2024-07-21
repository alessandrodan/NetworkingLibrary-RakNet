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
	InitializeBase();
}

void ServerAuth::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_AUTH_REQUEST, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGAuthRequest), &ServerAuth::TestRecv));
}

bool ServerAuth::TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	CPeer* d = CPeerManager::ValidPeer(peer);
	if (!d)
		return false;

	TPacketCGAuthRequest authRequest;
	if (!CPacketIO::ReadPacketData(packet, authRequest))
		return false;

	std::cout << "HEADER_CG_AUTH_REQUEST receved. username = " << authRequest.username << "\t password = " << authRequest.password << std::endl;

	return true;
}
