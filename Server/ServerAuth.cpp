#include "StdAfx.h"
#include "ServerAuth.h"
#include "Packet.h"
#include <slikenet/BitStream.h>
#include <iostream>
#include <Network/NetDevice.h>

using namespace Net;

ServerAuth::ServerAuth()
{
	m_packetHeader = std::make_unique<PacketManager>();
	__LoadPacketHeaders();
}

void ServerAuth::Process(Net::CAbstractPeer* peer, SLNet::Packet* packet)
{
	CPacketManagerBase<ServerAuth>::TPacketType* packetType;
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

void ServerAuth::__LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_AUTH_REQUEST, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGAuthRequest), &ServerAuth::TestRecv));
}

bool ServerAuth::TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	if (!packet)
		return false;

	TPacketCGAuthRequest authRequest;
	if (packet->length != sizeof(authRequest))
		return false;

	SLNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.Read((char*)&authRequest, sizeof(authRequest));

	std::cout << "HEADER_CG_AUTH_REQUEST receved. username = " << authRequest.username << "\t password = " << authRequest.password << std::endl;

	return true;
}
