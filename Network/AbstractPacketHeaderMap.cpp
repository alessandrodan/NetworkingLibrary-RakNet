#include "StdAfx.h"
#include "AbstractPacketHeaderMap.h"

namespace Net
{
	void CAbstractPacketHeaderMap::Set(PacketCGHeader header, const TPacketType& packetType)
	{
		Set(static_cast<TPacketHeader>(header), packetType);
	}

	void CAbstractPacketHeaderMap::Set(PacketGCHeader header, const TPacketType& packetType)
	{
		Set(static_cast<TPacketHeader>(header), packetType);
	}

	void CAbstractPacketHeaderMap::Set(TPacketHeader header, const TPacketType& rPacketType)
	{
		m_headerMap[header] = rPacketType;
	}

	bool CAbstractPacketHeaderMap::Get(TPacketHeader header, TPacketType* pPacketType)
	{
		auto f = m_headerMap.find(header);
		if (m_headerMap.end() == f)
			return false;

		*pPacketType = f->second;

		return true;
	}
}