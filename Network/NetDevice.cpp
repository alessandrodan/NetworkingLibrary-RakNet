#include "StdAfx.h"
#include "NetDevice.h"

namespace Net
{
	Interface* CNetDevice::peer = nullptr;

	void CNetDevice::Destroy()
	{
		peer->Shutdown(1000, 0, IMMEDIATE_PRIORITY);
		Interface::DestroyInstance(peer);
	}

	void CNetDevice::CloseConnection(const AddressOrGUID systemIdentifier, bool sendDisconnectionNotification)
	{
		if (peer->GetConnectionState(systemIdentifier) == ConnectionState::IS_CONNECTED)
		{
			peer->CloseConnection(
				systemIdentifier,
				sendDisconnectionNotification,
				0,
				sendDisconnectionNotification ? HIGH_PRIORITY : LOW_PRIORITY
			);
		}
	}

	void CNetDevice::CloseClientConnection()
	{
		CloseConnection(peer->GetSystemAddressFromIndex(0), true);
	}

	bool CNetDevice::IsConnectedToSystem(const AddressOrGUID systemIdentifier)
	{
		return peer->GetConnectionState(systemIdentifier) == ConnectionState::IS_CONNECTED;
	}

	bool CNetDevice::Create()
	{
		peer = Interface::GetInstance();
		return peer;
	}

	ConnectionAttemptResult CNetDevice::Connect(const char* host, unsigned short remotePort, const char* passwordData, int passwordDataLength, SLNet::PublicKey* publicKey, unsigned connectionSocketIndex, unsigned sendConnectionAttemptCount, unsigned timeBetweenSendConnectionAttemptsMS, SLNet::TimeMS timeoutTime)
	{
		return peer->Connect(host, remotePort, 0, 0, 0, 0, sendConnectionAttemptCount, timeBetweenSendConnectionAttemptsMS);
	}

	void CNetDevice::SetClientConfigs()
	{
		peer->SetOccasionalPing(true);
	}

	SLNet::Packet* CNetDevice::Receive()
	{
		return peer->Receive();
	}

	void CNetDevice::DeallocatePacket(SLNet::Packet* packet)
	{
		peer->DeallocatePacket(packet);
	}

	uint32_t CNetDevice::Send(const Message* msg, PacketPriority priority, PacketReliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, uint32_t forceReceiptNumber)
	{
		return peer->Send(msg, priority, reliability, orderingChannel, systemIdentifier, broadcast);
	}

	void CNetDevice::SetServerConfigs()
	{
		peer->SetMaximumIncomingConnections(SERVER_MAX_INCOMING_CONNECTIONS);
		peer->SetIncomingPassword(0, 0);
		peer->SetTimeoutTime(10000, Net::UNASSIGNED_SYSTEM_ADDRESS);
		peer->SetOccasionalPing(true);
		peer->SetUnreliableTimeout(1000);
	}

	StartResult CNetDevice::Startup(unsigned int maxConnections, Socket* socketDescriptors, unsigned socketDescriptorCount)
	{
		return peer->Startup(maxConnections, socketDescriptors, socketDescriptorCount);
	}
}
