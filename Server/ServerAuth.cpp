#include "StdAfx.h"
#include "ServerAuth.h"
#include "Packet.h"
#include <slikenet/BitStream.h>
#include <iostream>
#include <Network/NetDevice.h>

using namespace Net;

ServerAuth::ServerAuth()
{
	Initialize();
}

void ServerAuth::LoadPacketHeaders()
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
