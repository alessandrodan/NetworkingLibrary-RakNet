#pragma once

#include "Definition.h"

namespace Net
{
	class CNetDevice
	{
		public:
			static void Destroy();
			static bool IsConnectedToSystem(const AddressOrGUID systemIdentifier);
			static void CloseConnection(const AddressOrGUID systemIdentifier, bool sendDisconnectionNotification);
			static void CloseClientConnection();
			static bool Create();

			static StartResult Startup(unsigned int maxConnections, Socket* socketDescriptors, unsigned socketDescriptorCount);
			static ConnectionAttemptResult Connect(const char* host, unsigned short remotePort, const char* passwordData, int passwordDataLength, SLNet::PublicKey* publicKey = 0, unsigned connectionSocketIndex = 0, unsigned sendConnectionAttemptCount = 12, unsigned timeBetweenSendConnectionAttemptsMS = 500, SLNet::TimeMS timeoutTime = 0);
			static void SetClientConfigs();
			static void SetServerConfigs();
			static SLNet::Packet* Receive();
			static void DeallocatePacket(SLNet::Packet* packet);
			static uint32_t Send(const Message* msg, PacketPriority priority, PacketReliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, uint32_t forceReceiptNumber = 0);

		private:
			static Interface* peer;
	};
}
