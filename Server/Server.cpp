#include <iostream>
#include <Network/NetDevice.h>

int main()
{
	if (!Net::CNetDevice::Create())
	{
		std::cerr << "Failed to create winsock" << std::endl;
		system("pause");
		return 0;
	}

	Net::CNetDevice::Destroy();

	system("pause");
	return 0;
}
