#include "stdafx.h"
#include "DBAgent.h"
using std::string;
using std::vector;

void CGameServerDlg::SendEventRemainingTime(bool bSendAll, CUser* pUser, uint8 ZoneID)
{
	Packet result(WIZ_BIFROST, uint8(BIFROST_EVENT));
	uint16 nRemainingTime = 0;

	if (ZoneID == ZONE_BATTLE4 || ZoneID == ZONE_BATTLE5)
		nRemainingTime = m_byNereidsIslandRemainingTime / 2;

	result << nRemainingTime;

	if (pUser)
		pUser->Send(&result);
	else if (bSendAll)
	{
		if (ZoneID == ZONE_BATTLE4)
			Send_All(&result, nullptr, 0, ZONE_BATTLE4);
		else if (ZoneID == ZONE_BATTLE5)
			Send_All(&result, nullptr, 0, ZONE_BATTLE5);
		else
		{
			Send_All(&result, nullptr, 0, ZONE_RONARK_LAND);
			Send_All(&result, nullptr, 0, ZONE_BIFROST);
		}
	}
	if (ZoneID == ZONE_BATTLE4)
	{
		result.clear();
		result.Initialize(WIZ_MAP_EVENT);
		result << uint8(0) << uint8(7);

		for (int i = 0; i < 7; i++)
			result << m_sNereidsIslandMonuArray[i];

		if (pUser)
			pUser->Send(&result);

		result.clear();
		result.Initialize(WIZ_MAP_EVENT);
		result << uint8(2) << uint16(m_sElmoMonumentPoint) << uint16(m_sKarusMonumentPoint);

		if (pUser)
			pUser->Send(&result);
	}
}

void CUser::TempleProcess(Packet &pkt)
{
	uint8 opcode = pkt.read<uint8>();

	switch (opcode)
	{
	case MONSTER_STONE:
		MonsterStoneProcess(pkt);
		break;
	case TEMPLE_EVENT_JOIN:
		TempleJoinEvent(g_pMain->pTempleEvent.ActiveEvent);
		break;
	case TEMPLE_EVENT_DISBAND:
		TempleDisbandEvent(g_pMain->pTempleEvent.ActiveEvent);
		break;
	case TEMPLE_DRAKI_TOWER_ENTER:
		DrakiTowerTempleEnter(pkt);
		break;
	case TEMPLE_DRAKI_TOWER_LIST:
		DrakiTowerList();
		break;
	case TEMPLE_DRAKI_TOWER_TOWN:
		DrakiTowerTown();
	case TEMPLE_EVENT_DUNGEON_SIGN:
		DungeonDefenceSign();
		break;
	case TEMPLE_EVENT_REWARDS:
		{
		if (g_pMain->pTempleEvent.ActiveEvent == TEMPLE_EVENT_CHAOS)
		{
			Packet items(PL_EVENT);
			items << uint8(19);
			items << g_pMain->m_sChaosFinishItem << uint32(1);
			Send(&items);
		}
		else if (g_pMain->pTempleEvent.ActiveEvent == TEMPLE_EVENT_BORDER_DEFENCE_WAR)
		{
			Packet items(PL_EVENT);
			items << uint8(19);
			items << g_pMain->m_sBorderFinishItem << uint32(1);
			items << g_pMain->m_sBorderFinishItem1 << uint32(1);
			items << g_pMain->m_sBorderFinishItem2 << uint32(1);
			items << g_pMain->m_sBorderFinishItem3 << uint32(1);
			Send(&items);

		}
		else if (g_pMain->pTempleEvent.ActiveEvent == TEMPLE_EVENT_JURAD_MOUNTAIN)
		{
			Packet items(PL_EVENT);
			items << uint8(19);
			items << g_pMain->m_sJuraidFinishItem << uint32(1);
			items << g_pMain->m_sJuraidFinishItem1 << uint32(1);
			Send(&items);
		}
		else if (g_pMain->pTempleEvent.ActiveEvent == DEATHMATCH)
		{
			Packet items(PL_EVENT);
			items << uint8(19);
			items << g_pMain->m_DeathMatchItem1 << uint32(1);
			items << g_pMain->m_DeathMatchItem2 << uint32(1);
			items << g_pMain->m_DeathMatchItem3 << uint32(1);
			Send(&items);
		}



		}
		break;
	default:
		printf("Temple Process unhandled packets %d \n", opcode);
		TRACE("Temple Process unhandled packets %d \n", opcode);
		break;

	}
}

/**
* @brief Handles join requests from user to the ongoing event.
*
*/
bool CUser::TempleJoinEvent(int16 sEvent)
{
	if (isEventUser() /*|| isGM()*/ || GetZoneID() == ZONE_PRISON)
		return false;

	int16 nActiveEvent = g_pMain->pTempleEvent.ActiveEvent;

	if (nActiveEvent == -1
		|| sEvent == -1
		|| nActiveEvent != sEvent
		|| !g_pMain->pTempleEvent.bAllowJoin)
		return false;

	bool isAdded = false;

	switch (sEvent)
	{
	case TEMPLE_EVENT_JURAD_MOUNTAIN:
	{
		Packet result(PL_EVENT, uint8(TEMPLE_EVENT_JOIN));
		uint8 bResult = 1;

		if (bResult == 1 && g_pMain->AddEventUser(this))
		{
			result << uint8(bResult) << nActiveEvent;
			Send(&result);

			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount++ : g_pMain->pTempleEvent.ElMoradUserCount++;
			g_pMain->pTempleEvent.AllUserCount = (g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount);
			isAdded = true;
			m_sJoinedEvent = TEMPLE_EVENT_JURAD_MOUNTAIN;

			g_pMain->TemplEventJuraidSendJoinScreenUpdate();
			g_pMain->TemplEventJuraidSendJoinScreenUpdate(this);
		}
		else
		{
			result << uint8(bResult) << nActiveEvent;
			Send(&result);
		}
	}
	break;
	case TEMPLE_EVENT_BORDER_DEFENCE_WAR:
	{
		Packet result(PL_EVENT, uint8(TEMPLE_EVENT_JOIN));
		uint8 bResult = 1;

		if (bResult == 1 && g_pMain->AddEventUser(this))
		{
			result << uint8(1) << nActiveEvent;
			Send(&result);

			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount++ : g_pMain->pTempleEvent.ElMoradUserCount++;
			g_pMain->pTempleEvent.AllUserCount = (g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount);
			isAdded = true;
			m_sJoinedEvent = TEMPLE_EVENT_BORDER_DEFENCE_WAR;
			g_pMain->TemplEventBDWSendJoinScreenUpdate();
			g_pMain->TemplEventBDWSendJoinScreenUpdate(this);
		}
		else
		{
			result << uint8(bResult) << nActiveEvent;
			Send(&result);
		}
	}
	break;
	case TEMPLE_EVENT_CHAOS:
	{
		Packet result(PL_EVENT, uint8(TEMPLE_EVENT_JOIN));
		uint8 bResult = 1;

		if (CheckExistItem(CHAOS_MAP, 1))
			bResult = 1;
		else if (m_sItemArray[RIGHTHAND].nNum == MATTOCK || m_sItemArray[RIGHTHAND].nNum == GOLDEN_MATTOCK || isMining())
			bResult = 4;
		else
			bResult = 3;

		if (bResult == 1 && g_pMain->AddEventUser(this))
		{
			result << bResult << nActiveEvent;
			Send(&result);

			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount++ : g_pMain->pTempleEvent.ElMoradUserCount++;
			g_pMain->pTempleEvent.AllUserCount = (g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount);
			isAdded = true;
			m_sJoinedEvent = TEMPLE_EVENT_CHAOS;
			g_pMain->TemplEventChaosSendJoinScreenUpdate();
			g_pMain->TemplEventChaosSendJoinScreenUpdate(this);
		}
		else
		{
			result << uint8(bResult) << nActiveEvent;
			Send(&result);
		}
	}
	break;
	case DEATHMATCH:
	{
		Packet result(PL_EVENT, uint8(TEMPLE_EVENT_JOIN));
		uint8 bResult = 1;

		if (bResult == 1 && g_pMain->AddEventUser(this))
		{
			result << bResult << nActiveEvent;
			Send(&result);

			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount++ : g_pMain->pTempleEvent.ElMoradUserCount++;
			g_pMain->pTempleEvent.AllUserCount = (g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount);
			isAdded = true;
			m_sJoinedEvent = DEATHMATCH;
			g_pMain->TemplEventDeathMatchSendJoinScreenUpdate();
			g_pMain->TemplEventDeathMatchSendJoinScreenUpdate(this);
		}
		else
		{
			result << uint8(bResult) << nActiveEvent;
			Send(&result);
		}
	}
	break;
	default:
		break;
	}

	return isAdded;
}

/**
* @brief Handles the disband request from the join request sent events.
*
*/
void CUser::TempleDisbandEvent(int16 sEvent)// þimdi bana býrak inceleyim. sen test ettiðini emin misi ? evet göstereyim reward eklemedim paketlerini yazmadým tm 1sn
{
	if (!isEventUser())
		return;

	uint16 nActiveEvent = g_pMain->pTempleEvent.ActiveEvent;

	// no active event or wrong request to handle
	if (nActiveEvent == -1
		|| sEvent == -1
		|| nActiveEvent != sEvent)
		return;

	switch (sEvent)
	{
	case TEMPLE_EVENT_JURAD_MOUNTAIN:
	{
		if (GetEventRoom() > 0)
		{
			_JURAID_ROOM_INFO* pRoomInfo = g_pMain->m_TempleEventJuraidRoomList.GetData(GetEventRoom());
			if (pRoomInfo != nullptr)
			{
				if (GetNation() == Nation::KARUS)
					pRoomInfo->m_KarusUserList.DeleteData(GetName());
				else
					pRoomInfo->m_ElmoradUserList.DeleteData(GetName());
			}
		}
		else
		{
			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount-- : g_pMain->pTempleEvent.ElMoradUserCount--;
			g_pMain->pTempleEvent.AllUserCount = g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount;
			g_pMain->RemoveEventUser(this);

			Packet result(PL_EVENT, uint8(TEMPLE_EVENT_DISBAND));
			result << uint8(1) << nActiveEvent;
			Send(&result);

			g_pMain->TemplEventJuraidSendJoinScreenUpdate();
			g_pMain->TemplEventJuraidSendJoinScreenUpdate(this);
		}
	}
	break;
	case TEMPLE_EVENT_BORDER_DEFENCE_WAR:
	{
		if (GetEventRoom() > 0)
		{
			_BDW_ROOM_INFO* pRoomInfo = g_pMain->m_TempleEventBDWRoomList.GetData(GetEventRoom());
			if (pRoomInfo != nullptr)
			{
				if (GetNation() == Nation::KARUS)
					pRoomInfo->m_KarusUserList.DeleteData(GetName());
				else
					pRoomInfo->m_ElmoradUserList.DeleteData(GetName());
			}
		}
		else
		{
			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount-- : g_pMain->pTempleEvent.ElMoradUserCount--;
			g_pMain->pTempleEvent.AllUserCount = g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount;
			g_pMain->RemoveEventUser(this);

			Packet result(PL_EVENT, uint8(TEMPLE_EVENT_DISBAND));
			result << uint8(1) << nActiveEvent;
			Send(&result);

			g_pMain->TemplEventBDWSendJoinScreenUpdate();
			g_pMain->TemplEventBDWSendJoinScreenUpdate(this);
		}

	}
	break;
	case TEMPLE_EVENT_CHAOS:
	{
		if (GetEventRoom() > 0)
		{
			_CHAOS_ROOM_INFO* pRoomInfo = g_pMain->m_TempleEventChaosRoomList.GetData(GetEventRoom());
			if (pRoomInfo != nullptr)
				pRoomInfo->m_UserList.DeleteData(GetName());
		}
		else
		{
			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount-- : g_pMain->pTempleEvent.ElMoradUserCount--;
			g_pMain->pTempleEvent.AllUserCount = g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount;
			g_pMain->RemoveEventUser(this);

			Packet result(PL_EVENT, uint8(TEMPLE_EVENT_DISBAND));
			result << uint8(1) << nActiveEvent;
			Send(&result);

			g_pMain->TemplEventChaosSendJoinScreenUpdate();
			g_pMain->TemplEventChaosSendJoinScreenUpdate(this);
		}
	}
	break;
	case DEATHMATCH:
	{
		if (GetEventRoom() > 0)
		{
			_CHAOS_ROOM_INFO* pRoomInfo = g_pMain->m_TempleEventChaosRoomList.GetData(GetEventRoom());
			if (pRoomInfo != nullptr)
				pRoomInfo->m_UserList.DeleteData(GetName());
		}
		else
		{
			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount-- : g_pMain->pTempleEvent.ElMoradUserCount--;
			g_pMain->pTempleEvent.AllUserCount = g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount;
			g_pMain->RemoveEventUser(this);

			Packet result(PL_EVENT, uint8(TEMPLE_EVENT_DISBAND));
			result << uint8(1) << nActiveEvent;
			Send(&result);
			
			g_pMain->TemplEventDeathMatchSendJoinScreenUpdate();
			g_pMain->TemplEventDeathMatchSendJoinScreenUpdate(this);
		}
	}
	break;
	default:
		break;
	}
}


bool CGameServerDlg::AddEventUser(CUser *pUser)
{
	if (pUser == nullptr || !pUser->isInGame())
	{
		TRACE("#### AddEventUser : pUser == nullptr ####\n");
		return false;
	}
	_TEMPLE_EVENT_USER *pEventUser = new _TEMPLE_EVENT_USER();
	pEventUser->strUserID = pUser->GetName();
	pEventUser->m_iJoinOrder = m_TempleEventLastUserOrder++;

	if (!g_pMain->m_TempleEventUserMap.PutData(pEventUser->m_iJoinOrder, pEventUser))
	{
		delete pEventUser;
		return false;
	}

	pUser->m_iEventJoinOrder = pEventUser->m_iJoinOrder;
	pUser->m_sJoinedEvent = g_pMain->pTempleEvent.ActiveEvent;
	return true;
}

void CGameServerDlg::RemoveEventUser(CUser *pUser)
{
	if (pUser == nullptr)
	{
		TRACE("#### RemoveEventUser : pUser == nullptr ####\n");
		return;
	}

	g_pMain->m_TempleEventUserMap.DeleteData(pUser->m_iEventJoinOrder);
	pUser->ResetTempleEventData();
}

#pragma region 	CUser::isEventUser()
/**
* @brief Returns true if the user joined an event.
*/
bool CUser::isEventUser()
{
	return m_sJoinedEvent > 0;
}
#pragma endregion

#pragma region 	Event System Lua Kontrol
uint8 CUser::GetMonsterChallengeTime() {
	if (g_pMain->m_bForgettenTempleIsActive
		&& g_pMain->m_nForgettenTempleLevelMin != 0
		&& g_pMain->m_nForgettenTempleLevelMax != 0
		&& GetLevel() >= g_pMain->m_nForgettenTempleLevelMin
		&& GetLevel() <= g_pMain->m_nForgettenTempleLevelMax
		&& !g_pMain->m_bForgettenTempleSummonMonsters)
		return g_pMain->m_nForgettenTempleChallengeTime;
	return 0;
}

uint8 CUser::GetMonsterChallengeUserCount() { return (uint8)g_pMain->m_nForgettenTempleUsers.size(); }

bool CUser::GetUnderTheCastleOpen() { return g_pMain->m_bUnderTheCastleIsActive; }

uint16 CUser::GetUnderTheCastleUserCount() { return (uint16)g_pMain->m_nUnderTheCastleUsers.size(); }

bool CUser::GetJuraidMountainTime()
{
	if (g_pMain->pTempleEvent.ActiveEvent == TEMPLE_EVENT_JURAD_MOUNTAIN)
	{
		if (g_pMain->pTempleEvent.bAllowJoin)
			return true;
	}
	return false;
}
#pragma endregion

#pragma region 	Juraid , Border , Chaos Kontrol 
/**
* @brief Sends the Join Screen counter update.
*
*/
void CGameServerDlg::TemplEventJuraidSendJoinScreenUpdate(CUser* pUser /*= nullptr*/)
{
	if (pTempleEvent.bAllowJoin == false)
		return;

	Packet result(PL_EVENT, uint8(TEMPLE_EVENT_COUNTER));
	result << uint16(TEMPLE_EVENT_JURAD_MOUNTAIN) << g_pMain->pTempleEvent.KarusUserCount << g_pMain->pTempleEvent.ElMoradUserCount;

	if (pUser)
		pUser->Send(&result);
	else
		g_pMain->Send_All(&result, nullptr, Nation::ALL, 0, false, 0);
}


void CGameServerDlg::TemplEventBDWSendJoinScreenUpdate(CUser* pUser /*= nullptr*/)
{
	if (pTempleEvent.bAllowJoin == false)
		return;

	Packet result(PL_EVENT, uint8(TEMPLE_EVENT_COUNTER));
	result << uint16(TEMPLE_EVENT_BORDER_DEFENCE_WAR)
		<< g_pMain->pTempleEvent.KarusUserCount << g_pMain->pTempleEvent.ElMoradUserCount;

	if (pUser)
		pUser->Send(&result);
	else
		g_pMain->Send_All(&result, nullptr, Nation::ALL, 0, false, 0);
}

void CGameServerDlg::TemplEventChaosSendJoinScreenUpdate(CUser* pUser /*= nullptr*/)
{
	if (pTempleEvent.bAllowJoin == false)
		return;

	Packet result(PL_EVENT, uint8(TEMPLE_EVENT_COUNTER));
	result << uint16(TEMPLE_EVENT_CHAOS) << g_pMain->pTempleEvent.AllUserCount;

	if (pUser)
		pUser->Send(&result);
	else
		g_pMain->Send_All(&result, nullptr, Nation::ALL, 0, false, 0);
}

void CGameServerDlg::TemplEventDeathMatchSendJoinScreenUpdate(CUser* pUser /*= nullptr*/)
{
	if (pTempleEvent.bAllowJoin == false)
		return;

	Packet result(PL_EVENT, uint8(TEMPLE_EVENT_COUNTER));
	result << uint16(DEATHMATCH) << g_pMain->pTempleEvent.AllUserCount;
	

	if (pUser)
		pUser->Send(&result);
	else
		g_pMain->Send_All(&result, nullptr, Nation::ALL, 0, false, 0);
}
#pragma endregion