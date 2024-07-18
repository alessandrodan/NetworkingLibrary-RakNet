#include "StdAfx.h"
#include "ServerHandshake.h"
#include "Packet.h"
#include <slikenet/BitStream.h>
#include <iostream>
#include <Network/NetDevice.h>

using namespace Net;

ServerHandshake::ServerHandshake()
{
	m_packetHeader = std::make_unique<PacketManager>();
	__LoadPacketHeaders();
}

void ServerHandshake::Process(Net::CAbstractPeer* peer, SLNet::Packet* packet)
{
	CPacketManagerBase<ServerHandshake>::TPacketType* packetType;
	if (m_packetHeader->Get(packet->data[0], packetType))
	{
		if (packet->length == packetType->iPacketSize)
		{
			if (!packetType->Handle(this, packet, peer))
			{
				std::cerr << "Failed to handle packet with header " << (unsigned)packet->data[0] << std::endl;
				//peer->SetPhase(PHASE_CLOSE);
			}
		}
		else
			std::cerr << "Packet size mismatch for header " << (unsigned)packet->data[0] << std::endl;
	}
	else
	{
		std::cerr << "Wrong packet. id " << (unsigned)packet->data[0] << " packet length " << packet->length << " from " << packet->systemAddress.ToString() << std::endl;
		//peer->SetPhase(PHASE_CLOSE);
	}
}

void ServerHandshake::__LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_HANDSHAKE, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGHandshake), &ServerHandshake::RecvHandshake));
}

bool ServerHandshake::RecvHandshake(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	// TODO

	std::cout << "HEADER_CG_HANDSHAKE" << std::endl;
	return true;
}
