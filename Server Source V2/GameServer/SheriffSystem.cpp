#include "stdafx.h"
#include "MagicInstance.h"

void CGameServerDlg::WantedEventManuelStart(uint8 Type)
{
	switch (Type)
	{
	case 0:
	{
		for (int i = ZONE_RONARK_LAND; i <= ZONE_RONARK_LAND_BASE; i++)
		{
			_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(i);
			if (pMain == nullptr)
				return;

			pMain->ActiveEvent = false;
			pMain->ActiveQuery = false;
			pMain->ActiveInfoList = false;
			pMain->EventisPassive = true;
			pMain->RemainKarusUser = 0;
			pMain->RemainElmoradUser = 0;
			pMain->EventStartTime = 60;
			pMain->EventActiveTime = 0;
			pMain->ActiveQueryTime = 0;
			pMain->EventUserListInfoTime = 0;
			pMain->EventDangerControl = false;

			m_WantedEventSelectedUserArray.DeleteAllData();
		}
	}
	break;
	case 1:
	{
		for (int i = ZONE_RONARK_LAND; i <= ZONE_RONARK_LAND_BASE; i++)
		{
			_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(i);
			if (pMain == nullptr)
				return;

			pMain->ActiveEvent = false;
			pMain->ActiveQuery = false;
			pMain->ActiveInfoList = false;
			pMain->EventisPassive = false;
			pMain->RemainKarusUser = 0;
			pMain->RemainElmoradUser = 0;
			pMain->EventStartTime = 60;
			pMain->EventActiveTime = 0;
			pMain->ActiveQueryTime = 0;
			pMain->EventUserListInfoTime = 0;
			pMain->EventDangerControl = false;

			m_WantedEventSelectedUserArray.DeleteAllData();
		}
	}
	break;
	}
}

void CGameServerDlg::WantedEventMainTimer()
{
	for (int i = ZONE_RONARK_LAND; i <= ZONE_RONARK_LAND_BASE; i++)
	{
		_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(i);
		if (pMain == nullptr 
			|| pMain->EventisPassive)
			continue;

		if (!pMain->ActiveEvent)
		{
			if (pMain->ActiveQuery)
			{
				if (pMain->ActiveQueryTime > 0)
					pMain->ActiveQueryTime--;

				if (pMain->ActiveQueryTime < 0)
					pMain->ActiveQueryTime = 0;

				if (pMain->ActiveQueryTime <= 0)
				{
					if (pMain->RemainElmoradUser == 0 && pMain->RemainKarusUser == 0)
						pMain->EventStartTime = 1 * MINUTE;

					pMain->ActiveQuery = false;
				}
			}
			else
			{
				if (pMain->EventStartTime != 0)
				{
					if (pMain->EventStartTime > 0)
						pMain->EventStartTime--;

					if (pMain->EventStartTime < 0)
						pMain->EventStartTime = 0;
				}
				else
				{
					if (pMain->EventStartTime <= 0 && !pMain->EventDangerControl)
					{
						pMain->EventDangerControl = true;
						WantedEventSelectingSystem(pMain->EventZoneID);
					}
				}
			}
		}
		else
		{
			if (pMain->ActiveEvent)
			{
				if (pMain->ActiveQuery)
				{
					if (pMain->ActiveQueryTime > 0)
						pMain->ActiveQueryTime--;

					if (pMain->ActiveQueryTime < 0)
						pMain->ActiveQueryTime = 0;

					if (pMain->ActiveQueryTime <= 0)
						pMain->ActiveQuery = false;
				}

				if (pMain->ActiveInfoList)
				{
					if (pMain->EventUserListInfoTime > 0)
						pMain->EventUserListInfoTime--;

					if (pMain->EventUserListInfoTime < 0)
						pMain->EventUserListInfoTime = 0;

					if (pMain->EventUserListInfoTime <= 0)
					{
						pMain->ActiveInfoList = false;
						WantedEventTesterVoid(pMain->EventZoneID, Nation::ELMORAD);
						WantedEventTesterVoid(pMain->EventZoneID, Nation::KARUS);
					}
				}

				if (pMain->EventActiveTime > 0)
				{
					pMain->EventActiveTime--;

					if (pMain->EventActiveTime < 0)
						pMain->EventActiveTime = 0;
				}
				else
				{
					if (pMain->EventActiveTime <= 0 && pMain->ActiveEvent)
					{
						if (pMain->RemainElmoradUser > 0 
							|| pMain->RemainKarusUser > 0)
						{
							/*Ödül Avcýsý Etkinliði Bitiþ Yeri..*/
							SessionMap sessMap = m_socketMgr.GetActiveSessionMap();
							foreach(itr, sessMap)
							{
								CUser* pUser = TO_USER(itr->second);
								if (pUser == nullptr 
									|| !pUser->isInGame()
									|| !pUser->isInPKZone())
									continue;

								_WANTED_EVENT_SELECTED_USER* pSelectedUser = g_pMain->m_WantedEventSelectedUserArray.GetData(pUser->GetID());
								if (pSelectedUser == nullptr)
									continue;

								pUser->m_sWantedCount++;

								pUser->GiveItem(914053879, 1, true, 3);
								pUser->SendLoyaltyChange(50);

								if (pUser->m_sWantedCount == 100)
								{
									pUser->m_sWantedCount = 0; pUser->m_sWantedTotalAc = 90;
								}
							}
						}
						WantedEventManuelStart(KARUS);
					}
				}
			}
		}
	}
}

void CUser::VanGuardProcess(Packet& pkt)
{
	uint8 VanGuardPacket;
	pkt >> VanGuardPacket;

	switch (VanGuardPacket)
	{
	case 1:
	{
		_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(GetZoneID());
		if (pMain == nullptr
			|| !pMain->ActiveQuery)
			return;

		_WANTED_EVENT_SELECTED_USER* pSelectedUser = g_pMain->m_WantedEventSelectedUserArray.GetData(GetID());
		if (pSelectedUser != nullptr)
		{
			pSelectedUser->UserGetNation = GetNation();
			pSelectedUser->UserGetZoneID = GetZoneID();
			pSelectedUser->UserisWanted = true;

			pMain->ActiveEvent = true;
			GetNation() == Nation::ELMORAD ? pMain->RemainElmoradUser++ : pMain->RemainKarusUser++;

			MagicInstance instance;
			instance.sCasterID = GetID();
			instance.sTargetID = GetID();
			instance.nSkillID = 302166;
			instance.Run();

			m_WantedSelectingUser = true;
			m_WantedRunTime = (uint32)UNIXTIME + (10 * MINUTE);
			return;
		}

		_WANTED_EVENT_SELECTED_USER* pData = new _WANTED_EVENT_SELECTED_USER;
		pData->UserGetID = GetID();
		pData->UserGetNation = GetNation();
		pData->UserGetZoneID = GetZoneID();
		pData->UserisWanted = true;

		if (!g_pMain->m_WantedEventSelectedUserArray.PutData(pData->UserGetID, pData))
		{
			delete pData;
			return;
		}

		pMain->ActiveEvent = true;
		GetNation() == Nation::ELMORAD ? pMain->RemainElmoradUser++ : pMain->RemainKarusUser++;

		MagicInstance instance;
		instance.sCasterID = GetID();
		instance.sTargetID = GetID();
		instance.nSkillID = 302166;
		instance.Run();

		m_WantedSelectingUser = true;
		m_WantedRunTime = (uint32)UNIXTIME + (10 * MINUTE);
	}
	break;
	default:
		printf("VanGuard unhandled Packet %d", VanGuardPacket);
		break;
	}
}


void CGameServerDlg::WantedEventSelectingSystem(uint8 ZoneID)
{
	if (ZoneID == ZONE_RONARK_LAND)
	{
		struct WantedListElMoradRonarkLand { uint16 nIndex, UserGetID; };
		struct WantedListElKarusRonarkLand { uint16 nIndex, UserGetID; };

		std::vector<WantedListElMoradRonarkLand> nSelectingRonarkLandElmoradUser;
		std::vector<WantedListElKarusRonarkLand> nSelectingRonarkLandKarusdUser;

		uint16 RonarkLandElmoradIndex = 0, RonarkLandKarusIndex = 0;
		_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(ZONE_RONARK_LAND);
		if (pMain == nullptr
			|| pMain->ActiveEvent == true)
			return;

		SessionMap sessMap = m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser* pUser = TO_USER(itr->second);
			if (pUser == nullptr || !pUser->isInGame())
				continue;

			pUser->m_WantedSelectingUser = false;
			pUser->m_WantedRunTime = 0;

			if (!pUser->isInPKZone()
				|| pMain->EventZoneID != pUser->GetZoneID()
				|| pUser->GetZoneID() != ZONE_RONARK_LAND)
				continue;

			if (pUser->GetNation() == Nation::ELMORAD)
			{
				RonarkLandElmoradIndex++;
				nSelectingRonarkLandElmoradUser.push_back({ RonarkLandElmoradIndex, pUser->GetID() });
			}
			else
			{
				RonarkLandKarusIndex++;
				nSelectingRonarkLandKarusdUser.push_back({ RonarkLandKarusIndex, pUser->GetID() });
			}
		}

		if ((nSelectingRonarkLandElmoradUser.empty() && nSelectingRonarkLandKarusdUser.empty())
			|| RonarkLandElmoradIndex == 0 && RonarkLandKarusIndex == 0)
		{
			pMain->ActiveQuery = false;
			pMain->ActiveInfoList = false;
			pMain->RemainKarusUser = 0;
			pMain->RemainElmoradUser = 0;
			pMain->EventStartTime = 1 * MINUTE;
			pMain->EventActiveTime = 0;
			pMain->ActiveQueryTime = 0;
			pMain->EventUserListInfoTime = 0;
			pMain->EventDangerControl = false;
			return;
		}

		int RLandSelectingElmoradUser[MAX_SELECTING_USER] = {};
		if (!nSelectingRonarkLandElmoradUser.empty())
		{
			for (int i = 0; i < MAX_SELECTING_USER; i++)
			{
				if (RonarkLandElmoradIndex < MAX_SELECTING_USER)
				{
					if (RonarkLandElmoradIndex <= i)
						break;
				}

				RLandSelectingElmoradUser[i] = myrand(1, RonarkLandElmoradIndex);
				for (int kontrol = 0; kontrol < i; kontrol++)
				{
					if (RLandSelectingElmoradUser[kontrol] == RLandSelectingElmoradUser[i])
					{
						i--;
						break;
					}
				}
			}

			foreach(itr, nSelectingRonarkLandElmoradUser)
			{
				CUser* pSelect = g_pMain->GetUserPtr(itr->UserGetID);
				if (pSelect == nullptr
					|| !pSelect->isInGame()
					|| pSelect->GetZoneID() != ZONE_RONARK_LAND)
					continue;

				for (int i = 0; i < MAX_SELECTING_USER; i++)
				{
					if (RLandSelectingElmoradUser[i] == itr->nIndex)
					{
						Packet result(WIZ_VANGUARD, uint8(0x01));
						result << uint8(0x01) << uint8(0x01);;
						pSelect->Send(&result);
					}
				}
			}
		}

		int RLandSelectingKarusUser[MAX_SELECTING_USER] = {};
		if (!nSelectingRonarkLandKarusdUser.empty())
		{
			for (int i = 0; i < MAX_SELECTING_USER; i++)
			{
				if (RonarkLandKarusIndex < MAX_SELECTING_USER)
				{
					if (RonarkLandKarusIndex <= i)
						break;
				}

				RLandSelectingKarusUser[i] = myrand(1, RonarkLandKarusIndex);
				for (int kontrol = 0; kontrol < i; kontrol++)
				{
					if (RLandSelectingKarusUser[kontrol] == RLandSelectingKarusUser[i])
					{
						i--;
						break;
					}
				}
			}

			foreach(itr, nSelectingRonarkLandKarusdUser)
			{
				CUser* pSelect = g_pMain->GetUserPtr(itr->UserGetID);
				if (pSelect == nullptr
					|| !pSelect->isInGame()
					|| pSelect->GetZoneID() != ZONE_RONARK_LAND)
					continue;

				for (int i = 0; i < MAX_SELECTING_USER; i++)
				{
					if (RLandSelectingKarusUser[i] == itr->nIndex)
					{
						Packet result(WIZ_VANGUARD, uint8(0x01));
						result << uint8(0x01) << uint8(0x01);;
						pSelect->Send(&result);
					}
				}
			}
		}
		pMain->ActiveQuery = true;
		pMain->ActiveQueryTime = 11;
		pMain->EventActiveTime = 10 * MINUTE;
		pMain->RemainElmoradUser = 0;
		pMain->RemainKarusUser = 0;
		pMain->EventUserListInfoTime = 30;
		pMain->ActiveInfoList = true;
		pMain->EventDangerControl = false;
	}
	else if (ZoneID == ZONE_RONARK_LAND_BASE)
	{
		struct WantedListElMoradRLB { uint16 nIndex, UserGetID; };
		struct WantedListElKarusRLB { uint16 nIndex, UserGetID; };

		std::vector<WantedListElMoradRLB> nSelectingRBSElmoradUser;
		std::vector<WantedListElKarusRLB> nSelectingRBSKarusdUser;

		uint16 RBSElmoradIndex = 0, RBSKarusIndex = 0;
		_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(ZONE_RONARK_LAND_BASE);
		if (pMain == nullptr
			|| pMain->ActiveEvent == true)
			return;

		SessionMap sessMap = m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser* pUser = TO_USER(itr->second);
			if (pUser == nullptr || !pUser->isInGame())
				continue;

			pUser->m_WantedSelectingUser = false;
			pUser->m_WantedRunTime = 0;

			if (!pUser->isInPKZone()
				|| pMain->EventZoneID != pUser->GetZoneID()
				|| pUser->GetZoneID() != ZONE_RONARK_LAND_BASE)
				continue;

			if (pUser->GetNation() == Nation::ELMORAD)
			{
				RBSElmoradIndex++;
				nSelectingRBSElmoradUser.push_back({ RBSElmoradIndex, pUser->GetID() });
			}
			else
			{
				RBSKarusIndex++;
				nSelectingRBSKarusdUser.push_back({ RBSKarusIndex, pUser->GetID() });
			}
		}
		if ((nSelectingRBSElmoradUser.empty() && nSelectingRBSKarusdUser.empty())
			|| RBSElmoradIndex == 0 && RBSKarusIndex == 0)
		{
			pMain->ActiveQuery = false;
			pMain->ActiveInfoList = false;
			pMain->RemainKarusUser = 0;
			pMain->RemainElmoradUser = 0;
			pMain->EventStartTime = 1 * MINUTE;
			pMain->EventActiveTime = 0;
			pMain->ActiveQueryTime = 0;
			pMain->EventUserListInfoTime = 0;
			pMain->EventDangerControl = false;
			return;
		}

		int RLandSelectingElmoradUser[MAX_SELECTING_USER] = {};
		if (!nSelectingRBSElmoradUser.empty())
		{
			for (int i = 0; i < MAX_SELECTING_USER; i++)
			{
				if (RBSElmoradIndex < MAX_SELECTING_USER)
				{
					if (RBSElmoradIndex <= i)
						break;
				}

				RLandSelectingElmoradUser[i] = myrand(1, RBSElmoradIndex);
				for (int kontrol = 0; kontrol < i; kontrol++)
				{
					if (RLandSelectingElmoradUser[kontrol] == RLandSelectingElmoradUser[i])
					{
						i--;
						break;
					}
				}
			}

			foreach(itr, nSelectingRBSElmoradUser)
			{
				CUser* pSelect = g_pMain->GetUserPtr(itr->UserGetID);
				if (pSelect == nullptr
					|| !pSelect->isInGame()
					|| pSelect->GetZoneID() != ZONE_RONARK_LAND_BASE)
					continue;

				for (int i = 0; i < MAX_SELECTING_USER; i++)
				{
					if (RLandSelectingElmoradUser[i] == itr->nIndex)
					{
						Packet result(WIZ_VANGUARD, uint8(0x01));
						result << uint8(0x01) << uint8(0x01);;
						pSelect->Send(&result);
					}
				}
			}
		}

		int RLandSelectingKarusUser[MAX_SELECTING_USER] = {};
		if (!nSelectingRBSKarusdUser.empty())
		{
			for (int i = 0; i < MAX_SELECTING_USER; i++)
			{
				if (RBSKarusIndex < MAX_SELECTING_USER)
				{
					if (RBSKarusIndex <= i)
						break;
				}

				RLandSelectingKarusUser[i] = myrand(1, RBSKarusIndex);
				for (int kontrol = 0; kontrol < i; kontrol++)
				{
					if (RLandSelectingKarusUser[kontrol] == RLandSelectingKarusUser[i])
					{
						i--;
						break;
					}
				}
			}

			foreach(itr, nSelectingRBSKarusdUser)
			{
				CUser* pSelect = g_pMain->GetUserPtr(itr->UserGetID);
				if (pSelect == nullptr
					|| !pSelect->isInGame()
					|| pSelect->GetZoneID() != ZONE_RONARK_LAND_BASE)
					continue;

				for (int i = 0; i < MAX_SELECTING_USER; i++)
				{
					if (RLandSelectingKarusUser[i] == itr->nIndex)
					{
						Packet result(WIZ_VANGUARD, uint8(0x01));
						result << uint8(0x01) << uint8(0x01);;
						pSelect->Send(&result);
					}
				}
			}
		}

		pMain->ActiveQuery = true;
		pMain->ActiveQueryTime = 11;
		pMain->EventActiveTime = 10 * MINUTE;
		pMain->RemainElmoradUser = 0;
		pMain->RemainKarusUser = 0;
		pMain->EventUserListInfoTime = 30;
		pMain->ActiveInfoList = true;
		pMain->EventDangerControl = false;
	}
	else if (ZoneID == ZONE_ARDREAM)
	{

	}
	else
		return;
}

void CGameServerDlg::WantedEventTesterVoid(uint8 GetZoneID, uint8 GetNation)
{
	_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(GetZoneID);
	if (pMain == nullptr
		|| !pMain->ActiveEvent)
		return;

	Packet result(WIZ_VANGUARD, uint8(0x02));
	result << uint8(0x01);

	uint8 counter = 0, nation = 0;
	result << counter;
	foreach_stlmap(itr, g_pMain->m_WantedEventSelectedUserArray)
	{
		_WANTED_EVENT_SELECTED_USER* pSelectedUser = itr->second;
		if (pSelectedUser == nullptr)
			continue;

		CUser* pSelect = g_pMain->GetUserPtr(pSelectedUser->UserGetID);
		if (pSelect == nullptr
			|| !pSelect->isInGame()
			|| pSelect->GetZoneID() != GetZoneID
			|| pSelect->GetNation() != GetNation)
			continue;

		result << pSelect->GetName();
		counter++;
	}
	GetNation == Nation::ELMORAD ? nation = KARUS : nation = ELMORAD;
	result.put(2, counter);
	Send_Zone(&result, GetZoneID, nullptr, nation);
}

void CUser::WantedEventUserisUpdate()
{
	_WANTED_EVENT_SELECTED_USER* pSelectedUser = g_pMain->m_WantedEventSelectedUserArray.GetData(GetID());
	if (pSelectedUser == nullptr
		|| pSelectedUser->UserGetZoneID != GetZoneID())
		return;

	if (isWantedUser())
	{
		if ((uint32)UNIXTIME >= m_WantedRunTime && m_WantedRunTime != 0)
		{
			m_WantedSelectingUser = false;
			m_WantedRunTime = 0;
		}
	}
}

void CUser::WantedEventUserisMove()
{
	_WANTED_EVENT_SELECTED_USER* pSelectedUser = g_pMain->m_WantedEventSelectedUserArray.GetData(GetID());
	if (pSelectedUser == nullptr
		|| pSelectedUser->UserGetZoneID != GetZoneID())
		return;

	_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(GetZoneID());
	if (pMain == nullptr)
		return;

	if (isWantedUser() 
		&& g_pMain->m_WantedSystemMapShowTime != 0)
	{
		if (UNIXTIME - g_pMain->m_WantedSystemMapShowTime > 60)
		{
			Packet Move(WIZ_VANGUARD, uint8(0x02));
			Move << uint8(0x02) << uint8(0x01) << uint8(0x00) << uint16(GetX()) << uint16(GetZ()) << GetName();

			if (GetNation() == Nation::ELMORAD)
				g_pMain->Send_Zone(&Move, GetZoneID(), nullptr, Nation::KARUS);
			else
				g_pMain->Send_Zone(&Move, GetZoneID(), nullptr, Nation::ELMORAD);

			g_pMain->m_WantedSystemMapShowTime = (uint32)UNIXTIME;
		}

		if (UNIXTIME - g_pMain->m_WantedSystemTownControlTime > 15)
		{
			if (GetNation() == Nation::ELMORAD)
			{
				if (((GetX() < 699.0f && GetX() > 540.0f) && ((GetZ() < 984.0f && GetZ() > 875.0f))))
					Warp(10080, 10103);
			}
			else
			{
				if (((GetX() < 1444.0f && GetX() > 1314.0f) && ((GetZ() < 1140.0f && GetZ() > 1020.0f))))
					Warp(10080, 10103);
			}
			g_pMain->m_WantedSystemTownControlTime = (uint32)UNIXTIME;
		}
	}
}

void CUser::WantedEventDead()
{
	_WANTED_EVENT_SELECTED_USER* pSelectedUser = g_pMain->m_WantedEventSelectedUserArray.GetData(GetID());
	if (pSelectedUser == nullptr
		|| !pSelectedUser->UserisWanted
		|| !isWantedUser())
		return;

	pSelectedUser->UserisWanted = false;
	m_WantedSelectingUser = false;
	m_WantedRunTime = 0;

	_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(pSelectedUser->UserGetZoneID);
	if (pMain == nullptr)
		return;

	GetNation() == Nation::ELMORAD ? pMain->RemainElmoradUser-- : pMain->RemainKarusUser--;

	if (pMain->RemainElmoradUser < 0)
		pMain->RemainElmoradUser = 0;

	if (pMain->RemainKarusUser < 0)
		pMain->RemainKarusUser = 0;

	if (pMain->RemainElmoradUser <= 0
		&& pMain->RemainKarusUser <= 0)
	{
		pMain->ActiveEvent = false;
		pMain->ActiveQuery = false;
		pMain->ActiveQueryTime = 0;
		pMain->EventStartTime = 1 * MINUTE;
		pMain->EventActiveTime = 10 * MINUTE;
		pMain->RemainElmoradUser = 0;
		pMain->RemainKarusUser = 0;
	}

	g_pMain->m_WantedEventSelectedUserArray.DeleteData(GetID());
}

void CUser::WantedEventKilled()
{
	Packet result(WIZ_VANGUARD, uint8(0x02));
	uint8 sRand = myrand(1, 2);
	switch (sRand)
	{
	case 1:
		GiveItem(914053879, 1, true, 3);
		break;
	case 2:
		GiveItem(914054880, 1, true, 3);
		break;
	default:
		break;
	}

	m_sWantedKillerCount++;

	if (m_sWantedKillerCount == 100)
	{
		m_sWantedKillerCount = 0; m_sWantedTotalHit = 30;
	}

	if (isInParty())
	{
		_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());
		if (pParty == nullptr)
			return;

		short partyUsers[MAX_PARTY_USERS];
		for (int i = 0; i < MAX_PARTY_USERS; i++)
			partyUsers[i] = pParty->uid[i];

		for (int i = 0; i < MAX_PARTY_USERS; i++)
		{
			CUser* PartyUser = g_pMain->GetUserPtr(partyUsers[i]);
			if (PartyUser == nullptr
				|| PartyUser->isDead())
				continue;

			PartyUser->SendLoyaltyChange(80);
		}
	}
	result << uint8(0x01) << uint8(0x00) << GetName();
	g_pMain->Send_Zone(&result, GetZoneID());
}

void CUser::WantedEventisExits()
{
	_WANTED_EVENT_SELECTED_USER* pSelectedUser = g_pMain->m_WantedEventSelectedUserArray.GetData(GetID());
	if (pSelectedUser == nullptr
		|| !pSelectedUser->UserisWanted
		|| !isWantedUser())
		return;

	pSelectedUser->UserisWanted = false;
	m_WantedSelectingUser = false;
	m_WantedRunTime = 0;

	_WANTED_MAIN_CONTROL* pMain = g_pMain->m_WantedEventMainControlArray.GetData(pSelectedUser->UserGetZoneID);
	if (pMain == nullptr)
		return;

	GetNation() == Nation::ELMORAD ? pMain->RemainElmoradUser-- : pMain->RemainKarusUser--;

	if (pMain->RemainElmoradUser < 0)
		pMain->RemainElmoradUser = 0;

	if (pMain->RemainKarusUser < 0)
		pMain->RemainKarusUser = 0;

	if (pMain->RemainElmoradUser <= 0
		&& pMain->RemainKarusUser <= 0)
	{
		pMain->ActiveEvent = false;
		pMain->ActiveQuery = false;
		pMain->ActiveQueryTime = 0;
		pMain->EventStartTime = 1 * MINUTE;
		pMain->EventActiveTime = 10 * MINUTE;
		pMain->RemainElmoradUser = 0;
		pMain->RemainKarusUser = 0;
	}

	g_pMain->m_WantedEventSelectedUserArray.DeleteData(GetID());
}