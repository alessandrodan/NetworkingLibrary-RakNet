#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.h>
#include <Network/Definition.h>
#include "ServerGame.h"
#include "Packet.h"
#include <Network/PacketIO.hpp>
#include "Peer.h"
#include "PeerManager.h"

using namespace Net;

ServerGame::ServerGame()
{
	InitializePacketHandler();
}

void ServerGame::LoadPacketHeaders()
{
	m_packetHeader->Set(PacketCGHeader::HEADER_CG_ACTION1, std::make_unique<PacketManager::TPacketType>(sizeof(TPacketCGAction1), &ServerGame::TestRecv));
}

bool ServerGame::TestRecv(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	CPeer* d = CPeerManager::ValidPeer(peer);
	if (!d)
		return false;

	TPacketCGAction1 action1;
	if (!CPacketIO::ReadPacketData(packet, action1))
		return false;

	std::cout << "HEADER_CG_ACTION1 receved. num = " << action1.numIntero << std::endl;

	TestSend(packet, peer);
	return true;
}

bool ServerGame::TestSend(SLNet::Packet* packet, Net::CAbstractPeer* peer)
{
	if (!peer)
		return false;

	TPacketGCResponse response;
	peer->Packet(&response, sizeof(response));

	return true;
}
