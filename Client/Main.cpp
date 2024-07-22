#include <iostream>
#include <conio.h>
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
    if (client.Start("localhost", 8080))
    {
        while (true)
        {
            client.ProcessNet();

            if (client.IsConnected() && _kbhit())
            {
                std::string input;

                char ch = _getch();
                while (ch != '\r')
                {
                    input += ch;
                    std::cout << ch;
                    ch = _getch();
                }

                std::cout << std::endl;

                if (input == "ok")
                    client.TestSend();
                else if (input == "login")
                    client.SendLogin();
                else if (input == "q")
                    break;
            }
        }

		Net::CNetDevice::CloseConnection();
    }

	Net::CNetDevice::Destroy();

	system("pause");
	return 0;
}
