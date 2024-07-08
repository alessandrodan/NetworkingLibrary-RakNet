#pragma once

#include <map>
#include "PacketDefinition.h"

namespace Net
{
	class CAbstractPacketHeaderMap
	{
		public:
			typedef struct SPacketType
			{
				SPacketType(int iSize = 0, bool bFlag = false)
				{
					iPacketSize = iSize;
				}

				int iPacketSize;
			} TPacketType;

		public:
			CAbstractPacketHeaderMap() = default;
			virtual ~CAbstractPacketHeaderMap() = default;

			void Set(PacketCGHeader header, const TPacketType& packetType);
			void Set(PacketGCHeader header, const TPacketType& packetType);
			bool Get(TPacketHeader header, TPacketType* pPacketType);

		protected:
			std::map<TPacketHeader, TPacketType> m_headerMap;

		private:
			void Set(TPacketHeader header, const TPacketType& rPacketType);
	};
}