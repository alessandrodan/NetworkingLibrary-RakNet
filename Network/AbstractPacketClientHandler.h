#pragma once

#include "AbstractPacketHandler.h"
#include "PacketManager.hpp"

namespace Net
{
    template<typename TClientDerived>
    class CAbstractPacketClientHandler : public CAbstractPacketClientHandlerBase
    {
        public:
            using PacketManager = CPacketManagerClient<TClientDerived>;

            CAbstractPacketClientHandler()
            {
                m_packetHeader = std::make_unique<PacketManager>();
            }

            void InitializePacketHandler()
            {
                static_cast<TClientDerived*>(this)->LoadPacketHeaders();
            }

            void Process(SLNet::Packet* packet) override
            {
                ProcessPacket(static_cast<TClientDerived*>(this), *m_packetHeader, packet);
            }

        protected:
            std::unique_ptr<PacketManager> m_packetHeader;
    };
}
