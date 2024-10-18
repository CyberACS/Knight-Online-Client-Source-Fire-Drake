#include "stdafx.h"

void CGameServerDlg::GameInfoNoticeTimer()
{
	if (m_GameInfo1Time > 0)
		m_GameInfo1Time--;

	if (m_GameInfo1Time <= 0)
	{
		m_GameInfo1Time = 1800;
		GameInfo1Packet();
	}

	if (m_GameInfo2Time > 0)
		m_GameInfo2Time--;

	if (m_GameInfo2Time <= 0)
	{
		m_GameInfo2Time = 3600;
		GameInfo2Packet();
	}
}

void CGameServerDlg::GameInfo1Packet()
{
	Packet result;
	std::string notice;
	GetServerResource(IDS_NOTICE_INFO_1, &notice);
	ChatPacket::Construct(&result, (uint8)PUBLIC_CHAT, &notice);
	Send_All(&result, nullptr, Nation::ALL);
}

void CGameServerDlg::GameInfo2Packet()
{
	Packet result;
	std::string notice;
	GetServerResource(IDS_NOTICE_INFO_2, &notice);
	ChatPacket::Construct(&result, (uint8)PUBLIC_CHAT, &notice);
	Send_All(&result, nullptr, Nation::ALL);
}
