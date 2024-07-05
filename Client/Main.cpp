#include <iostream>
#include <Network/NetDevice.h>
#include "Client.h"

int main()
{
	if (!Net::CNetDevice::Create())
	{
		std::cerr << "Failed to create rak Instance" << std::endl;
		system("pause");
		return 0;
	}

	Client client;
    if (client.Initialize("localhost", 8080))
    {
        while (true)
        {
            client.Process();
        }
    }

	Net::CNetDevice::Destroy();

	system("pause");
	return 0;
}
