#pragma once

#include "StdAfx.h"
#include "AbstractEntity.h"
#include "NetDevice.h"
#include <slikenet/types.h>
#include "Definition.h"

namespace Net
{
	class CAbstractEntityClient : public CAbstractEntity
	{
		public:
			CAbstractEntityClient() = default;
			virtual ~CAbstractEntityClient() = default;

		protected:
			virtual void __OnConnectFail(int errorCode) = 0;
			virtual void __OnConnectSuccess() = 0;

		public:
			bool Start(const char* c_szAddr, int port) override
			{
				if (!__Initialize(c_szAddr, port))
					return false;

				return __Connect(c_szAddr, port);
			}

		protected:
			bool __Initialize(const char* c_szAddr, int port) override
			{
				SLNet::SocketDescriptor socketDescriptor;
				const auto startupResult = CNetDevice::peer->Startup(CLIENT_MAX_CONNECTIONS, &socketDescriptor, 1);
				if (startupResult != SLNet::RAKNET_STARTED)
				{
					__OnInitFail(startupResult);
					return false;
				}

				__OnInitSuccess();
				return true;
			}

			bool __Connect(const char* c_szAddr, int port)
			{
				const auto connectionAttemptResult = CNetDevice::peer->Connect(c_szAddr, port, 0, 0, 0, 0, CLIENT_CONNECTION_ATTEMPT_COUNT, CLIENT_TIME_BTW_SEND_CONNECTION_ATTEMPT_COUNT);
				if (connectionAttemptResult != SLNet::CONNECTION_ATTEMPT_STARTED)
				{
					__OnConnectFail(connectionAttemptResult);
					return false;
				}

				CNetDevice::peer->SetOccasionalPing(true);

				__OnConnectSuccess();
				return true;
			}
	};
}
