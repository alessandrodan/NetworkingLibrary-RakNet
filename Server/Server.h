#pragma once

class Server
{
	public:
		Server() = default;
		~Server() = default;

		bool Initialize(const char* c_szAddr, int port);
		void Process();
};
