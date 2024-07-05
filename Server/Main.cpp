#include <iostream>
#include <Network/NetDevice.h>
#include "Server.h"

int main()
{
	if (!Net::CNetDevice::Create())
	{
		std::cerr << "Failed to create winsock" << std::endl;
		system("pause");
		return 0;
	}

	Server server;
	if (server.Initialize("localhost", 8080))
	{
		while (true)
			server.Process();
	}

	Net::CNetDevice::Destroy();

	system("pause");
	return 0;
}
