#pragma once

#include "StdAfx.h"
#include "AbstractEntity.h"
#include "NetDevice.h"
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
				Socket socketDescriptor;
				strcpy_s(socketDescriptor.hostAddress, c_szAddr);
				socketDescriptor.port = static_cast<unsigned short>(port);
				socketDescriptor.socketFamily = AF_INET;

				const auto startupResult = CNetDevice::Startup(SERVER_MAX_CONNECTIONS, &socketDescriptor, 1);
				if (startupResult != StartResult::RAKNET_STARTED)
				{
					__OnInitFail(startupResult);
					return false;
				}

				__OnInitSuccess();
				return true;
			}

			void __Configure()
			{
				CNetDevice::SetServerConfigs();
			}
	};
}
