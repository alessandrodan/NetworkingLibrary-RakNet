#pragma once

class Client
{
	public:
		Client() = default;
		~Client() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();
};
