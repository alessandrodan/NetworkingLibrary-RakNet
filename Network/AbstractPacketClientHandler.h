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

            virtual void ProcessPacketError(EProcessPacketError errorType, NetPacket* packet) = 0;

            CAbstractPacketClientHandler()
            {
                m_packetHeader = std::make_unique<PacketManager>();
            }

            void InitializePacketHandler()
            {
                static_cast<TClientDerived*>(this)->LoadPacketHeaders();
            }

            void Process(NetPacket* packet) override
            {
                ProcessPacket(static_cast<TClientDerived*>(this), *m_packetHeader, packet);
            }

        protected:
            std::unique_ptr<PacketManager> m_packetHeader;
    };
}
