#pragma once

class Client
{
	public:
		Client();
		~Client() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();

		bool IsConnected();

	private:
		bool isConnected;
};
