#include "StdAfx.h"
#include <iostream>
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

	for (auto packet = ReceivePacket(); packet; DeallocatePacket(packet), packet = ReceivePacket())
	{
		NetPacket netPacket;
		ConvertToNetPacket(packet, &netPacket);

		switch (netPacket.header)
		{
			case ID_NEW_INCOMING_CONNECTION:
			{
				m_peerManager->AcceptPeer(netPacket.guid);
				std::cout << "New incoming connection: " << netPacket.systemAddress.ToString() << "\t. GUID (String): " << netPacket.guid.ToString() << "GUID (Int): " << netPacket.guid.ToUint32(netPacket.guid) << std::endl;
			}
			break;

			case ID_DISCONNECTION_NOTIFICATION:
			{
				std::cout << "Client Disconnected: " << netPacket.systemAddress.ToString() << std::endl;

				const auto peer = m_peerManager->GetPeer(netPacket.guid);
				if (!peer)
				{
					if (CNetDevice::IsConnectedToSystem(netPacket.systemAddress))
					{
						std::cerr << "ID_DISCONNECTION_NOTIFICATION - Peer not recognized. String: " << netPacket.guid.ToString() << "Int: " << netPacket.guid.ToUint32(netPacket.guid) << std::endl;
						CNetDevice::CloseConnection(netPacket.systemAddress, false);
					}

					return;
				}

				peer->SetPhase(PHASE_CLOSE);
			}
			break;

			case ID_CONNECTION_LOST:
			{
				std::cout << "Client Connection lost: " << netPacket.systemAddress.ToString() << std::endl;

				const auto peer = m_peerManager->GetPeer(netPacket.guid);
				if (!peer)
				{
					if (CNetDevice::IsConnectedToSystem(netPacket.systemAddress))
					{
						std::cerr << "ID_CONNECTION_LOST - Peer not recognized. String: " << netPacket.guid.ToString() << "Int: " << netPacket.guid.ToUint32(netPacket.guid) << std::endl;
						CNetDevice::CloseConnection(netPacket.systemAddress, false);
					}

					return;
				}

				peer->SetPhase(PHASE_CLOSE);
			}
			break;

			default:
			{
				const auto peer = m_peerManager->GetPeer(netPacket.guid);
				if (!peer)
				{
					std::cerr << "default - Peer not recognized. String: " << netPacket.guid.ToString() << "Int: " << netPacket.guid.ToUint32(netPacket.guid) << std::endl;
					CNetDevice::CloseConnection(netPacket.systemAddress, false);
					return;
				}

				peer->ProcessRecv(&netPacket);
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
