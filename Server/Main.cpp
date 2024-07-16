#include <iostream>
#include <conio.h>
#include <Network/NetDevice.h>
#include "ServerGame.h"

int main()
{
	if (!Net::CNetDevice::Create())
	{
		std::cerr << "Failed to create winsock" << std::endl;
		system("pause");
		return 0;
	}

	ServerGame server;
	if (server.Initialize("localhost", 8080))
	{
		while (true)
		{
			server.Process();

			if (_kbhit())
			{
				if (_getch() == 27) // 'Esc' key
					break;
			}
		}
	}

	Net::CNetDevice::Destroy();

	system("pause");
	return 0;
}
