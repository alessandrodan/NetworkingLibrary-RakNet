#pragma once

#include <map>
#include "PacketDefinition.h"

namespace Net
{
	template <typename T>
	class CPacketManager
	{
		public:
			typedef bool (T::* PacketHandler)(SLNet::Packet*);

			typedef struct SPacketType
			{
				int iPacketSize;
				PacketHandler packetHandler;

				SPacketType(int iSize = 0, PacketHandler handler = nullptr)
					: iPacketSize(iSize), packetHandler(handler)
				{
				}
			} TPacketType;

			CPacketManager() = default;
			virtual ~CPacketManager() = default;

			void Set(TPacketHeader header, const TPacketType& packetType)
			{
				SetInternal(header, packetType);
			}

			bool Get(TPacketHeader header, TPacketType* pPacketType)
			{
				const auto it = m_headerMap.find(header);
				if (it == m_headerMap.end())
					return false;

				*pPacketType = it->second;
				return true;
			}

		protected:
			void SetInternal(TPacketHeader header, const TPacketType& rPacketType)
			{
				m_headerMap[header] = rPacketType;
			}

		private:
			std::map<TPacketHeader, TPacketType> m_headerMap;
	};
}
