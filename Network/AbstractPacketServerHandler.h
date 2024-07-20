#pragma once

#include "AbstractPacketHandler.h"
#include "PacketManager.hpp"

namespace Net
{
    template<typename TDerived>
    class CAbstractPacketServerHandler : public CAbstractPacketServerHandlerBase
    {
        public:
            using PacketManager = CPacketManagerServer<TDerived>;

            CAbstractPacketServerHandler()
            {
                m_packetHeader = std::make_unique<PacketManager>();
            }

            void Initialize()
            {
                static_cast<TDerived*>(this)->LoadPacketHeaders();
            }

            void Process(CAbstractPeer* peer, SLNet::Packet* packet) override
            {
                ProcessPacket(static_cast<TDerived*>(this), *m_packetHeader, packet, peer);
            }

        protected:
            std::unique_ptr<PacketManager> m_packetHeader;
    };
}
