#pragma once

#include <Network/AbstractEntity.h>
#include <Network/AbstractPacketHandler.h>

class ServerMain : public Net::CAbstractEntity, Net::CAbstractPacketHandler
{
	public:
		ServerMain() = default;
		~ServerMain() = default;

		bool Initialize(const char* c_szAddr, int port) override;
		void Process() override;

	protected:
		void __LoadPacketHeaders() override;
};
