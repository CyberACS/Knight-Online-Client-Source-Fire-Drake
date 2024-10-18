#include "stdafx.h"

#pragma region CNpc::BifrostMonumentProcess(CUser* pKillerUser)
void CNpc::BifrostMonumentProcess(CUser* pKillerUser)
{
	if (pKillerUser == nullptr
		|| g_pMain->pBeefEvent.isMonumentDead
		|| !g_pMain->pBeefEvent.isActive)
		return;

	_BEEF_EVENT_PLAY_TIMER* pPlayInfo = g_pMain->m_BeefEventPlayTimerArray.GetData(EventLocalID::BeefEvent);
	if (pPlayInfo == nullptr)
		return;

	g_pMain->pBeefEvent.isAttackable = false;
	g_pMain->pBeefEvent.isMonumentDead = true;
	g_pMain->pBeefEvent.WictoryNation = pKillerUser->GetNation();
	g_pMain->pBeefEvent.BeefSendTime = 120 * MINUTE;
	g_pMain->pBeefEvent.BeefMonumentStartTime = 0;
	g_pMain->pBeefEvent.LoserNationSignTime = (uint32)UNIXTIME + (pPlayInfo->LoserSignTime * MINUTE);
	g_pMain->pBeefEvent.BeefFarmingPlayTime = (uint32)UNIXTIME + (pPlayInfo->FarmingTime * MINUTE);
	g_pMain->pBeefEvent.isFarmingPlay = true;
	g_pMain->BeefEventUpdateTime();
	g_pMain->BeefEventSendNotice(3);
}
#pragma endregion

#pragma region CGameServerDlg::ResetBeefEvent()
void CGameServerDlg::ResetBeefEvent()
{
	pBeefEvent.isActive = false;
	pBeefEvent.isAttackable = false;
	pBeefEvent.isMonumentDead = false;
	pBeefEvent.isFarmingPlay = false;
	pBeefEvent.WictoryNation = 0;
	pBeefEvent.BeefMonumentStartTime = 0;
	pBeefEvent.BeefFarmingPlayTime = 0;
	pBeefEvent.LoserNationSignTime = 0;
	pBeefEvent.isLoserSign = false;

	int nLocalHour = g_localTime.tm_hour;
	int nLocalMinute = g_localTime.tm_min;
	int nLocalSecond = g_localTime.tm_sec;

	if (nLocalHour > 12) { nLocalHour = nLocalHour % 12; }

	nLocalHour = nLocalHour * 3600;
	nLocalMinute = nLocalMinute * 60;

	int GüncelTime = nLocalHour + nLocalMinute + nLocalSecond;
	int32 EventTime[EVENT_START_TIMES] = { 0,0,0,0,0 };
	int32 EventRemainingTime[EVENT_START_TIMES] = { 0,0,0,0,0 };

	_EVENT_SCHEDULE* pSchedule = g_pMain->m_EventScheduleArray.GetData(EventLocalID::BeefEvent);
	if (pSchedule != nullptr)
	{
		for (int x = 0; x < EVENT_START_TIMES; x++)
		{
			if (pSchedule->TimeActive[x] != 1
				|| pSchedule->EventStartMinutes[x] > 59
				|| pSchedule->EventStartHour[x] > 23)
				continue;

			int32 BeefHour = pSchedule->EventStartHour[x];
			if (BeefHour > 12) { BeefHour = BeefHour % 12; }

			BeefHour = pSchedule->EventStartHour[x] * 3600;

			int32 BeefMinute = pSchedule->EventStartMinutes[x] * 60;

			int32 lastTime = BeefHour + BeefMinute + 0;
			if (lastTime <= 0) continue;

			EventTime[x] = lastTime;

			if (EventTime[x] <= 0) { EventTime[x] = 0; }
		}
	}

	for (int a = 0; a < EVENT_START_TIMES; a++)
	{
		int32 LastRemainingTime = EventTime[a] - GüncelTime;

		if (LastRemainingTime <= 0) { LastRemainingTime = 999999999; }

		EventRemainingTime[a] = LastRemainingTime;
		if (EventRemainingTime[a] <= 0) { EventRemainingTime[a] = 0; }
	}

	int32 min = EventRemainingTime[0];
	int32 n = sizeof(EventRemainingTime) / sizeof(EventRemainingTime[0]);
	for (int i = 0; i < n; i++)
	{
		if (EventRemainingTime[i] <= 0) continue;

		if (EventRemainingTime[i] < min)
			min = EventRemainingTime[i];
	}
	pBeefEvent.BeefSendTime = min;
}
#pragma endregion

#pragma region CGameServerDlg::BeefEventSendNotice(uint8 NoticeType)
void CGameServerDlg::BeefEventSendNotice(uint8 NoticeType)
{
	if (NoticeType <= 0)
		return;

	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach(itr, sessMap)
	{
		CUser* pUser = TO_USER(itr->second);
		if (pUser == nullptr
			|| !pUser->isInGame())
			continue;

		if (pUser->GetZoneID() != ZONE_BIFROST
			&& pUser->GetZoneID() != ZONE_RONARK_LAND)
			continue;

		pUser->BeefEventGetNotice(NoticeType);
	}
}
#pragma endregion

#pragma region CUser::BeefEventGetNotice(uint8 NoticeType)
void CUser::BeefEventGetNotice(uint8 NoticeType)
{
	switch (NoticeType)
	{
		//Beef Event Start
	case 1:
	{
		Packet x;
		std::string notice;

		g_pMain->GetServerResource(IDS_BEEF_ROAST_START, &notice);

		// Wrap it in a "#### NOTICE : ####" block.
		g_pMain->GetServerResource(IDP_ANNOUNCEMENT, &notice, notice.c_str());

		// Construct & send the chat/announcement packet
		ChatPacket::Construct(&x, WAR_SYSTEM_CHAT, &notice);
		Send(&x);
	}
	break;
	//Beef Event Draw
	case 2:
	{
		Packet x;
		std::string notice;

		g_pMain->GetServerResource(IDS_BEEF_ROAST_DRAW, &notice);

		// Wrap it in a "#### NOTICE : ####" block.
		g_pMain->GetServerResource(IDP_ANNOUNCEMENT, &notice, notice.c_str());

		// Construct & send the chat/announcement packet
		ChatPacket::Construct(&x, WAR_SYSTEM_CHAT, &notice);
		Send(&x);
	}
	break;
	//Wictory Nation
	case 3:
	{
		Packet x;
		std::string notice;

		g_pMain->GetServerResource(g_pMain->pBeefEvent.WictoryNation == ELMORAD ? IDS_BEEF_ROAST_VICTORY_ELMORAD : IDS_BEEF_ROAST_VICTORY_KARUS, &notice);

		// Wrap it in a "#### NOTICE : ####" block.
		g_pMain->GetServerResource(IDP_ANNOUNCEMENT, &notice, notice.c_str());

		// Construct & send the chat/announcement packet
		ChatPacket::Construct(&x, WAR_SYSTEM_CHAT, &notice);
		Send(&x);
	}
	break;
	//Finish
	case 4:
	{
		Packet x;
		std::string notice;

		g_pMain->GetServerResource(IDS_BEEF_ROAST_FINISH, &notice);

		// Wrap it in a "#### NOTICE : ####" block.
		g_pMain->GetServerResource(IDP_ANNOUNCEMENT, &notice, notice.c_str());

		// Construct & send the chat/announcement packet
		ChatPacket::Construct(&x, WAR_SYSTEM_CHAT, &notice);
		Send(&x);
	}
	break;
	//Loser Team Login Notice
	case 5:
	{
		Packet x;
		std::string notice;

		g_pMain->GetServerResource(g_pMain->pBeefEvent.WictoryNation == ELMORAD ? IDS_BEEF_ROAST_VICTORY_KARUS : IDS_BEEF_ROAST_VICTORY_ELMORAD, &notice);

		// Wrap it in a "#### NOTICE : ####" block.
		g_pMain->GetServerResource(IDP_ANNOUNCEMENT, &notice, notice.c_str());

		// Construct & send the chat/announcement packet
		ChatPacket::Construct(&x, WAR_SYSTEM_CHAT, &notice);
		Send(&x);
	}
	break;
	default:
		printf("Beef Event Send Notice error type %d \n", NoticeType);
		TRACE("Beef Event Send Notice error type %d \n", NoticeType);
		break;
	}
}
#pragma endregion

#pragma region CGameServerDlg::BeefEventUpdateTime()
void CGameServerDlg::BeefEventUpdateTime()
{
	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach(itr, sessMap)
	{
		CUser* pUser = TO_USER(itr->second);
		if (pUser == nullptr
			|| !pUser->isInGame())
			continue;

		if (pUser->GetZoneID() != ZONE_BIFROST
			&& pUser->GetZoneID() != ZONE_RONARK_LAND)
			continue;

		pUser->BeefEventGetTime();
	}
}
#pragma endregion

#pragma region CUser::BeefEventGetTime()
void CUser::BeefEventGetTime()
{
	Packet result(WIZ_BIFROST, uint8(BIFROST_EVENT));
	result << uint32(g_pMain->pBeefEvent.BeefSendTime);
	Send(&result);
}
#pragma endregion

#pragma region CUser::BeefEventLogin()
bool CUser::BeefEventLogin()
{
	if (g_pMain->pBeefEvent.isActive && g_pMain->pBeefEvent.isFarmingPlay)
	{
		if (g_pMain->pBeefEvent.WictoryNation != 0)
		{
			if (GetNation() != g_pMain->pBeefEvent.WictoryNation)
			{
				if (g_pMain->pBeefEvent.isLoserSign)
					return true;
			}

			if (GetNation() == g_pMain->pBeefEvent.WictoryNation)
				return true;
		}
	}
	return false;
}
#pragma endregion