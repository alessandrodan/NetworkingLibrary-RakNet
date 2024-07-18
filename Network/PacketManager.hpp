#pragma once

#include <map>
#include <memory>
#include "PacketDefinition.h"
#include "AbstractPeer.h"

namespace Net
{
    template <typename T>
    class CPacketManagerBase
    {
        public:
            virtual ~CPacketManagerBase() = default;

            typedef struct SPacketType
            {
                int iPacketSize;
                virtual bool Handle(T* instance, SLNet::Packet* packet, Net::CAbstractPeer* peer) = 0;

                SPacketType(int iSize = 0) : iPacketSize(iSize) {}
                virtual ~SPacketType() = default;
            } TPacketType;

            void Set(TPacketHeader header, std::unique_ptr<TPacketType> packetType)
            {
                m_headerMap[header] = std::move(packetType);
            }

            bool Get(TPacketHeader header, TPacketType*& pPacketType)
            {
                const auto it = m_headerMap.find(header);
                if (it == m_headerMap.end())
                    return false;

                pPacketType = it->second.get();
                return true;
            }

        protected:
            std::map<TPacketHeader, std::unique_ptr<TPacketType>> m_headerMap;
    };


    template <typename T>
    class CPacketManagerClient : public CPacketManagerBase<T>
    {
        public:
            using PacketHandler = bool (T::*)(SLNet::Packet*);

            typedef struct SPacketType : public CPacketManagerBase<T>::TPacketType
            {
                PacketHandler packetHandler;

                SPacketType(int iSize = 0, PacketHandler handler = nullptr)
                    : CPacketManagerBase<T>::TPacketType(iSize), packetHandler(handler)
                {
                }

                bool Handle(T* instance, SLNet::Packet* packet, Net::CAbstractPeer* peer) override
                {
                    return (instance->*packetHandler)(packet);
                }
            } TPacketType;
    };

    template <typename T>
    class CPacketManagerServer : public CPacketManagerBase<T>
    {
        public:
            using PacketHandler = bool (T::*)(SLNet::Packet*, Net::CAbstractPeer*);

            typedef struct SPacketType : public CPacketManagerBase<T>::TPacketType
            {
                PacketHandler packetHandler;

                SPacketType(int iSize = 0, PacketHandler handler = nullptr)
                    : CPacketManagerBase<T>::TPacketType(iSize), packetHandler(handler)
                {
                }

                bool Handle(T* instance, SLNet::Packet* packet, Net::CAbstractPeer* peer) override
                {
                    return (instance->*packetHandler)(packet, peer);
                }
            } TPacketType;
    };
}
