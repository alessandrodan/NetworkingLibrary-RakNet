#pragma once

#include "Definition.h"
#include "NetDevice.h"

namespace Net
{
    class CPacketIO
    {
        public:
            template <typename PacketStruct>
            static bool ReadPacketData(NetPacket* packet, PacketStruct& outData)
            {
                if (!packet)
                    return false;

                Message msgIn(packet->data, packet->length, false);
                return msgIn.Read((char*)&outData, sizeof(outData));
            }

            //static Message* WritePacketData(const void* c_pvData, int iSize)
            //{
            //    Message msgOut(iSize);
            //    msgOut.Write((const char*)c_pvData, iSize);
            //    return msgOut.GetInstance();
            //}

            static void SendPacket(const void* c_pvData, int iSize, const AddressOrGUID systemIdentifier, bool broadcast = false)
            {
                Message msgOut(iSize);
                msgOut.Write((const char*)c_pvData, iSize);

                CNetDevice::peer->Send(&msgOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, systemIdentifier, broadcast);
            }
    };
}
