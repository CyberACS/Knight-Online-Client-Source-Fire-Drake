#include "stdafx.h"
#include "KingSystem.h"
#include "DBAgent.h"
#include <iostream>

uint32 CGameServerDlg::Timer_UpdateGameTime(void * lpParam)
{
	while (g_bRunning)
	{
		g_pMain->UpdateGameTime();
		sleep(1 * SECOND);
	}
	return 0;
}

void CGameServerDlg::UpdateGameTime()
{
	DateTime now(&g_localTime);

	if (m_sSec != now.GetSecond())
	{
		// Check timed King events.
		foreach_stlmap(itr, m_KingSystemArray)
			itr->second->CheckKingTimer();

		uint16 m_bOnlineCount = 0, m_bMerchantCount = 0, m_bMiningCount = 0, m_bGenieCount = 0, m_bFishingCount = 0;
		std::vector<CBot*> deleted;

		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser *pUser = TO_USER(itr->second);
			if (pUser == nullptr 
				|| !pUser->isInGame())
				continue;

			if (pUser->isMerchanting())
				m_bMerchantCount++;

			if (pUser->isMining())
				m_bMiningCount++;

			if (pUser->isFishing())
				m_bFishingCount++;

			if (pUser->isGenieActive())
				m_bGenieCount++;

			m_bOnlineCount++;
		}
		m_BotArray.m_lock.lock();
		foreach_stlmap_nolock(itr, m_BotArray)
		{
			CBot* pUser = itr->second;

			if (pUser == nullptr)
				continue;

			if (!pUser->isInGame())
				continue;

			if (pUser->LastWarpTime < UNIXTIME)
				deleted.push_back(pUser);

			if ((pUser->m_bResHpType == USER_MINING
				|| pUser->m_bResHpType == USER_FLASHING)
				&& pUser->m_iGold + 15 < uint32(UNIXTIME))
			{
				Packet result(WIZ_MINING, uint8(MiningAttempt));
				uint16 resultCode = MiningResultSuccess, Random = myrand(0, 10000);
				uint16 sEffect = 0;

				if (Random > 4000
					|| pUser->m_bResHpType == USER_SITDOWN) // EXP
					sEffect = 13082; // "XP" effect
				else
					sEffect = 13081; // "Item" effect

				result << resultCode << pUser->GetID() << sEffect;
				pUser->SendToRegion(&result);
				pUser->m_iGold = uint32(UNIXTIME);
			}

			if (pUser->isMerchanting())
				m_bMerchantCount++;

			if (pUser->m_bResHpType == USER_MINING)
				m_bMiningCount++;

			if (pUser->m_bResHpType == USER_FLASHING)
				m_bFishingCount++;

			if (pUser->isGenieActive())
				m_bGenieCount++;

			m_bOnlineCount++;
		}
		m_BotArray.m_lock.unlock();
		g_pMain->m_checktime -= 1 * SECOND;

		if (g_pMain->m_checktime <= 0)
		{
			g_pMain->m_checktime = 60 * SECOND;

			MEMORYSTATUSEX statex;

			statex.dwLength = sizeof(statex);

			GlobalMemoryStatusEx(&statex);

			std::string texts;
			texts = string_format("CyberACS.com [Online :%d - Merchant :%d - Mining :%d - Fishing :%d - Genie :%d] || Ram :%d ", m_bOnlineCount, m_bMerchantCount, m_bMiningCount, m_bFishingCount, m_bGenieCount, statex.dwMemoryLoad);
			SetConsoleTitle(texts.c_str());
		}

		foreach(itr, deleted) {
			if ((*itr)->isSlaveMerchant())
			{
				CUser* pSlaveUser = g_pMain->GetUserPtr((*itr)->m_bSlaveUserID);
				if (pSlaveUser != nullptr && pSlaveUser->isSlave <= 0)
					pSlaveUser->GiveSlaveMerchantItems();
			}

			CUser* pSlaveUser = g_pMain->GetUserPtr((*itr)->m_bSlaveUserID);

			if (!(*itr)->isSlaveMerchant())
				(*itr)->UserInOut(INOUT_OUT);
			else if ((*itr)->isSlaveMerchant() && pSlaveUser->isSlave <= 0)
				pSlaveUser->UserInOut(INOUT_OUT);
		}
	}

	// Every minute
	if (m_sMin != now.GetMinute())
	{

		m_sMonMin++;
		if (m_sMonMin == 60) {
			m_sMonMin = 0;
		}

		m_ReloadKnightAndUserRanksMinute++;

		if (m_ReloadKnightAndUserRanksMinute == RELOAD_KNIGHTS_AND_USER_RATING)
		{
			m_ReloadKnightAndUserRanksMinute = 0;
			ReloadKnightAndUserRanks();
		}

		// Player Ranking Rewards
		std::list<std::string> vargs = StrSplit(m_sPlayerRankingsRewardZones, ",");
		uint8 nZones = (uint8)vargs.size();
		if (nZones > 0)
		{
			uint8 nZoneID = 0;
			for (int i = 0; i < nZones; i++)
			{
				nZoneID = atoi(vargs.front().c_str());
				SetPlayerRankingRewards(nZoneID);
				vargs.pop_front();
			}
		}

		/* ARDREAM EVENT MELANOR*/
		if (ArdreamEventGameServerSatus)
		{
			ArdreamEventFinishTime--;

			if (ArdreamEventFinishTime == 30 * 60 || ArdreamEventFinishTime == 28 * 60 || ArdreamEventFinishTime == 25 * 60 ||
				ArdreamEventFinishTime == 21 * 60 || ArdreamEventFinishTime == 17 * 60 || ArdreamEventFinishTime == 13 * 60 ||
				ArdreamEventFinishTime == 10 * 60 || ArdreamEventFinishTime == 5 * 60 || ArdreamEventFinishTime == 4 * 60 ||
				ArdreamEventFinishTime == 3 * 60 || ArdreamEventFinishTime == 2 * 60 || ArdreamEventFinishTime == 1 * 60)
			{
				LogosYolla("[Ardream Event]", string_format("Event %d Dakika Sonra Bitecektir", ArdreamEventFinishTime / 60), 1, 240, 1);
			}

			if (ArdreamEventFinishTime == 0)
			{
				ArdreamEventZoneClose();
			}
		}
		
		/* DeathMatch EVENT */
		if (DeathMatchEventGameServerSatus)
		{
			DeathMatchEventFinishTime--;

			if (DeathMatchEventFinishTime == 30 || DeathMatchEventFinishTime == 28 || DeathMatchEventFinishTime == 25 ||
				DeathMatchEventFinishTime == 21 || DeathMatchEventFinishTime == 17 || DeathMatchEventFinishTime == 13 ||
				DeathMatchEventFinishTime == 10 || DeathMatchEventFinishTime == 5 || DeathMatchEventFinishTime == 4 ||
				DeathMatchEventFinishTime == 3 || DeathMatchEventFinishTime == 2 || DeathMatchEventFinishTime == 1)
			{
				LogosYolla("", string_format("DeathMatch Event %d Dakika Sonra Bitecektir", DeathMatchEventFinishTime), 1, 240, 1);
			}

			if (DeathMatchEventFinishTime == 0)
			{
				DeathMatchEventFinished();
			}
		}
		/*DeathMatch EVENT */


	}

	

	// Every hour
	if (m_sHour != now.GetHour())
	{
		Packet result1(WIZ_SIEGE, uint8(5));
		toplamclan = g_DBAgent.TotalCount(result1);
		active = g_DBAgent.TournumentLoadUpdate();

		ResetPlayerRankings();
		UpdateWeather();
		SetGameTime();

		if (m_bSantaOrAngel)
			SendFlyingSantaOrAngel();

		Packet result(WIZ_KNIGHTS_PROCESS, uint8(KNIGHTS_SAVE));
		g_pMain->AddDatabaseRequest(result);
	}

	// Every day
	if (m_sDate != now.GetDay())
	{
		for (int i = 0; i < 30; i++) {
			g_pMain->m_DailyFinishTime[i] = (60 * 60 * 24);
		}
		UpdateDailyInfoTime();
		g_DBAgent.ClearDailyQuestUser();

		_KNIGHTS_SIEGE_WARFARE *pSiegeWarFare = GetSiegeMasterKnightsPtr(Aktive);

		if (pSiegeWarFare != nullptr)
			g_DBAgent.UpdateSiegeWarfareDB(pSiegeWarFare->nMoradonTax, pSiegeWarFare->nDellosTax, pSiegeWarFare->nDungeonCharge);

		foreach_stlmap(itr, m_KingSystemArray)
			g_DBAgent.UpdateKingSystemDB(itr->second->m_byNation, itr->second->m_nNationalTreasury, itr->second->m_nTerritoryTax);
	}

	// Every month
	if (m_sMonth != now.GetMonth())
	{
		// Reset monthly NP.
		ResetLoyaltyMonthly();
	}

	// Update the server time
	m_sYear = now.GetYear();
	m_sMonth = now.GetMonth();
	m_sDate = now.GetDay();
	m_sHour = now.GetHour();
	m_sMin = now.GetMinute();
	m_sSec = now.GetSecond();
}

#pragma region CGameServerDlg::Timer_UpdateSessions(void * lpParam)
uint32 CGameServerDlg::Timer_UpdateSessions(void * lpParam)
{
	while (g_bRunning)
	{
		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser * pUser = TO_USER(itr->second);
			if (pUser == nullptr)
				continue;

#ifndef _DEBUG // ignore timeouts in debug builds, as we'll probably be pausing it with the debugger.
			ULONGLONG timeout = KOSOCKET_TIMEOUT;

			// User has authed, but isn't in-game yet (creating a character, or is waiting for the game to load).
			if (!pUser->m_strAccountID.empty() && !pUser->isInGame())
				timeout = KOSOCKET_LOADING_TIMEOUT;

			// Disconnect timed out sessions
			if ((UNIXTIME2 - pUser->GetLastResponseTime()) >= timeout)
			{
				if (pUser->GetName().length() > 0)
					printf("[%s] disconnected due to long term inactivity. \n", pUser->GetName().c_str());

				pUser->Disconnect();
				continue;
			}
#endif
			// Update the player, and hence any skill effects while we're here.
			pUser->Update();

			if (pUser->m_bWaitingOrder && pUser->m_tOrderRemain <= UNIXTIME)
				pUser->LoadingLogin((Packet)(WIZ_LOADING_LOGIN));

			if (pUser->m_PettingOn != nullptr)
			{
				if (pUser->m_PettingOn->sAttackStart)
				{
					CNpc* pNpc = g_pMain->GetNpcPtr(pUser->m_PettingOn->sAttackTargetID, pUser->GetZoneID());
					if (pNpc == nullptr)
					{
						pUser->m_PettingOn->sAttackStart = false;
						pUser->m_PettingOn->sAttackTargetID = - 1;
						continue;
					}
					else
					{
						if (pNpc->isDead())
						{
							pUser->m_PettingOn->sAttackStart = false;
							pUser->m_PettingOn->sAttackTargetID = -1;
							continue;
						}
						CNpc* pPet = g_pMain->GetPetPtr(pUser->GetSocketID(), pUser->GetZoneID());
						if (pPet == nullptr)
						{
							pUser->m_PettingOn->sAttackStart = false;
							pUser->m_PettingOn->sAttackTargetID = -1;
							continue;
						}
						else
						{
							if (!pPet->isInRangeSlow(pNpc, RANGE_50M))
							{
								float	warp_x = pNpc->GetX() - pPet->GetX(),
									warp_z = pNpc->GetZ() - pPet->GetZ();

								// Unable to work out orientation, so we'll just fail (won't be necessary with m_sDirection).
								float	distance = sqrtf(warp_x * warp_x + warp_z * warp_z);
								if (distance == 0.0f)
									continue;

								warp_x /= distance; warp_z /= distance;
								warp_x *= 2; warp_z *= 2;
								warp_x += pNpc->GetX(); warp_z += pNpc->GetZ();

								pPet->SendMoveResult(warp_x, 0, warp_z, distance);
								continue;
							}
							uint8 bResult = ATTACK_FAIL;
							int damage = pPet->GetDamage(pNpc);

							if (damage > 0)
							{
								pNpc->HpChange(-(damage), pPet);
								if (pNpc->isDead())
									bResult = ATTACK_TARGET_DEAD;
								else
									bResult = ATTACK_SUCCESS;
							}

							Packet result(WIZ_ATTACK, uint8(LONG_ATTACK));
							result << bResult << pPet->GetID() << pNpc->GetID();
							pPet->SendToRegion(&result);
						}
					}
				}
			}
		}
		Sleep(1 * SECOND);
	}
	return 0;
}

#pragma endregion 

#pragma region CGameServerDlg::Timer_UpdateConcurrent(void * lpParam)

uint32 CGameServerDlg::Timer_UpdateConcurrent(void * lpParam)
{
	while (g_bRunning)
	{
		g_pMain->ReqUpdateConcurrent();
		sleep(120 * SECOND);
	}
	return 0;
}

#pragma endregion 

#pragma region CGameServerDlg::Timer_NpcThreadHandleRequests(void * lpParam)
uint32 CGameServerDlg::Timer_NpcThreadHandleRequests(void * lpParam)
{
	while (g_bRunning)
	{
		g_pMain->NpcThreadHandleRequests();
		sleep(500);
	}
	return 0;
}
#pragma endregion 

#pragma region CGameServerDlg::NpcThreadHandleRequests()
void CGameServerDlg::NpcThreadHandleRequests()
{
	foreach_stlmap(itr, g_pMain->m_arNpcThread)
	{
		CNpcThread* pNpcThread = itr->second;
		if (pNpcThread)
			pNpcThread->AddRequest(new Signal(NpcThreadSignalType::NpcHandle));
	}
}
#pragma endregion 

#pragma region CGameServerDlg::ReqUpdateConcurrent()
void CGameServerDlg::ReqUpdateConcurrent()
{
	uint32 sCount = ((uint32)m_socketMgr.GetActiveSessionMap().size() + (uint32)g_pMain->m_BotcharacterNameMap.size());

	Packet result(WIZ_ZONE_CONCURRENT);
	result << uint32(m_nServerNo)
		<< sCount;
	AddDatabaseRequest(result);
}
#pragma endregion 