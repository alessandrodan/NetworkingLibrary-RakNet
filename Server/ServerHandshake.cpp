#include "StdAfx.h"
#include "ServerHandshake.h"
#include "Packet.h"
#include <slikenet/BitStream.h>
#include <iostream>
#include <Network/NetDevice.h>
#include "Peer.h"

using namespace Net;

ServerHandshake::ServerHandshake()
{
	m_packetHeader = std::make_unique<PacketManager>();
	__LoadPacketHeaders();
}

void ServerHandshake::Process(Net::CAbstractPeer* peer, SLNet::Packet* packet)
{
	ProcessPacket(this, *m_packetHeader, packet, peer);
}

void ServerHandshake::__LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_HANDSHAKE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGHandshake), &ServerHandshake::RecvHandshake));
}

bool ServerHandshake::RecvHandshake(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	CPeer* realPeer = dynamic_cast<CPeer*>(peer);
	if (realPeer)
	{
		// Ora puoi usare i metodi specifici di CPeer
		realPeer->SendHandshake(11, 22);
	}


	std::cout << "HEADER_CG_HANDSHAKE" << std::endl;
	return true;
}
