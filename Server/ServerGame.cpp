#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <Network/NetDevice.h>
#include <Network/Definition.h>
#include "ServerGame.h"
#include "Packet.h"

using namespace Net;

ServerGame::ServerGame()
{
	m_packetHeader = std::make_unique<PacketManager>();
	__LoadPacketHeaders();
}

void ServerGame::__LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_ACTION1, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGAction1), &ServerGame::TestRecv));
}

void ServerGame::Process(Net::CAbstractPeer* peer, SLNet::Packet* packet)
{
	CPacketManagerBase<ServerGame>::TPacketType* packetType;
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

bool ServerGame::TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	if (!packet)
		return false;

	TPacketCGAction1 action1;
	if (packet->length != sizeof(action1))
		return false;

	SLNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.Read((char*)&action1, sizeof(action1));

	std::cout << "HEADER_CG_ACTION1 receved. num = " << action1.numIntero << std::endl;

	TestSend(packet);
	return true;
}

bool ServerGame::TestSend(SLNet::Packet* packet)
{
	TPacketGCResponse response;

	SLNet::BitStream bsOut(sizeof(response));
	bsOut.Write((char*)&response, sizeof(response));
	CNetDevice::peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

	return true;
}
