#pragma once

#include <Network/AbstractEntityClient.hpp>
#include <Network/AbstractPacketClientHandler.h>
#include <Network/PacketManager.hpp>

class Client : public Net::CAbstractEntityClient, public Net::CAbstractPacketClientHandler<Client>
{
	public:
		Client();
		~Client() = default;

	protected:
		void __OnInitFail(int errorCode) override;
		void __OnInitSuccess() override;
		void __OnConnectFail(int errorCode) override;
		void __OnConnectSuccess() override;

	public:
		void ProcessNet() override;
		void LoadPacketHeaders() override;
		void ProcessPacketError(Net::EProcessPacketError errorType, SLNet::Packet* packet) override;

	public:
		bool IsConnected();

		void SendHandshake(uint32_t dwHandshake, uint32_t dwTime, long lDelta);
		void SendLogin();
		void TestSend();
		bool RecvPhase(SLNet::Packet* packet);
		bool RecvHandshake(SLNet::Packet* packet);
		bool TestRecv(SLNet::Packet* packet);

	private:
		bool isConnected;
};
