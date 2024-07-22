#include "StdAfx.h"
#include <iostream>
#include <slikenet/types.h>
#include <slikenet/MessageIdentifiers.h>
#include <Network/NetDevice.h>
#include <Network/Definition.h>
#include "ServerMain.h"
#include "Packet.h"

using namespace Net;

void ServerMain::__OnInitFail(int errorCode)
{
	std::cout << "Failed to start server! Quitting - error code: " << errorCode << std::endl;
}

void ServerMain::__OnInitSuccess()
{
	m_peerManager = std::make_unique<CPeerManager>();
	std::cout << "Socket Listening..." << std::endl;
}

void ServerMain::ProcessNet()
{
	m_peerManager->DestroyClosed();

	for (SLNet::Packet* packet = CNetDevice::peer->Receive(); packet; CNetDevice::peer->DeallocatePacket(packet), packet = CNetDevice::peer->Receive())
	{
		switch (packet->data[0])
		{
			case ID_NEW_INCOMING_CONNECTION:
			{
				m_peerManager->AcceptPeer(packet->guid);
				std::cout << "New incoming connection: " << packet->systemAddress.ToString() << std::endl;
			}
			break;

			case ID_DISCONNECTION_NOTIFICATION:
			{
				const auto peer = m_peerManager->GetPeer(packet->guid);
				if (!peer)
					return;

				peer->SetPhase(PHASE_CLOSE);
				std::cout << "Client Disconnected: " << packet->systemAddress.ToString() << std::endl;
			}
			break;

			case ID_CONNECTION_LOST:
			{
				const auto peer = m_peerManager->GetPeer(packet->guid);
				if (!peer)
					return;

				peer->SetPhase(PHASE_CLOSE);
				std::cout << "Client Connection lost: " << packet->systemAddress.ToString() << std::endl;
			}
			break;

			default:
			{
				const auto peer = m_peerManager->GetPeer(packet->guid);
				if (!peer)
					return;

				peer->ProcessRecv(packet);

				//std::cout << "Wrong packet. id " << (unsigned)packet->data[0] << " packet length " << packet->length << " from " << packet->systemAddress.ToString() << std::endl;
				//CNetDevice::peer->CloseConnection(packet->systemAddress, true);
			}
		}
	}
}

void ServerMain::DisconnectAll()
{
	if (m_peerManager)
		m_peerManager->Destroy();
}

void ServerMain::DisconnectFirstPeer()
{
	if (m_peerManager)
	{
		auto firstPeer = m_peerManager->GetFirstPeer();
		if (firstPeer)
			m_peerManager->DestroyDesc(firstPeer.get());
	}
}
