#pragma once

#include <slikenet/BitStream.h>
#include "NetDevice.h"

namespace Net
{
    class CPacketIO
    {
        public:
            template <typename PacketStruct>
            static bool ReadPacketData(SLNet::Packet* packet, PacketStruct& outData)
            {
                if (!packet)
                    return false;

                SLNet::BitStream bsIn(packet->data, packet->length, false);
                return bsIn.Read((char*)&outData, sizeof(outData));
            }

            //static SLNet::BitStream* WritePacketData(const void* c_pvData, int iSize)
            //{
            //    SLNet::BitStream bsOut(iSize);
            //    bsOut.Write((const char*)c_pvData, iSize);
            //    return bsOut.GetInstance();
            //}

            static void SendPacket(const void* c_pvData, int iSize, const SLNet::AddressOrGUID systemIdentifier, bool broadcast = false)
            {
                SLNet::BitStream bsOut(iSize);
                bsOut.Write((const char*)c_pvData, iSize);

                CNetDevice::peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, systemIdentifier, broadcast);
            }
    };
}
