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

void ServerHandshake::Process()
{
	std::cout << "ServerHandshake::Process" << std::endl;
}

void ServerHandshake::__LoadPacketHeaders()
{

}
