#pragma once

#include "StdAfx.h"
#include "AbstractEntity.h"
#include "NetDevice.h"
#include <slikenet/types.h>
#include "Definition.h"

namespace Net
{
	class CAbstractEntityServer : public CAbstractEntity
	{
		public:
			CAbstractEntityServer() = default;
			virtual ~CAbstractEntityServer() = default;

			bool Start(const char* c_szAddr, int port) override
			{
				if (!__Initialize(c_szAddr, port))
					return false;

				__Configure();

				return true;
			}

		protected:
			bool __Initialize(const char* c_szAddr, int port) override
			{
				SLNet::SocketDescriptor socketDescriptor;
				strcpy_s(socketDescriptor.hostAddress, c_szAddr);
				socketDescriptor.port = static_cast<unsigned short>(port);
				socketDescriptor.socketFamily = AF_INET;

				const SLNet::StartupResult startupResult = CNetDevice::peer->Startup(SERVER_MAX_CONNECTIONS, &socketDescriptor, 1);
				if (startupResult != SLNet::RAKNET_STARTED)
				{
					__OnInitFail(startupResult);
					return false;
				}

				__OnInitSuccess();
				return true;
			}

			void __Configure()
			{
				CNetDevice::peer->SetMaximumIncomingConnections(SERVER_MAX_INCOMING_CONNECTIONS);
				CNetDevice::peer->SetIncomingPassword(0, 0);
				CNetDevice::peer->SetTimeoutTime(10000, SLNet::UNASSIGNED_SYSTEM_ADDRESS);
				CNetDevice::peer->SetOccasionalPing(true);
				CNetDevice::peer->SetUnreliableTimeout(1000);
			}
	};
}
