/*******************************************************************\
* DeathMatch.cpp - DeathMatch Event									*
* T�m DeathMatch Event Start Bilgileri Buradan Y�netilir.			*
* i�lenir.															*
\*******************************************************************/
#include "stdafx.h"

uint32 CGameServerDlg::Timer_DeathMatch(void* lpParam)
{
	while (g_bRunning)
	{
		if (g_pMain->m_bDMRegisterStarted)
		{
			uint32 time = uint32(UNIXTIME - g_pMain->m_tDMRegisterStartTime);
			if (time > 60) // 60 * 5 = 300 Saniye (5 Dk)
			{
				Guard lock(g_pMain->m_DMRegisterLock);
				foreach(itr, g_pMain->m_DMRegisterlist)
				{
					CUser* pUser = g_pMain->GetUserPtr(*itr);
					if (!pUser || !pUser->isInGame())
						continue;

					memcpy(pUser->m_sItemArrayBackup, pUser->m_sItemArray, sizeof(pUser->m_sItemArray));
					pUser->m_bRaceBackup = pUser->m_bRace;
					pUser->m_sClassBackup = pUser->m_sClass;
					memcpy(pUser->m_bStatsBackup, pUser->m_bStats, sizeof(uint8) * STAT_COUNT);
					memcpy(pUser->m_bstrSkillBackup, pUser->m_bstrSkill, sizeof(uint8) * 10);

					if (pUser->GetNation() == KARUS)
					{
						// Race ve Class'lar� belirledik
						pUser->m_bRace = 1;
						pUser->m_sClass = 106;
						// Karakterlerin STR ve HP'sini ayarlad�k
						pUser->SetStat(STAT_STR, 255);
						pUser->SetStat(STAT_STA, 177);
						pUser->SetStat(STAT_DEX, 0);
						pUser->SetStat(STAT_INT, 0);
						pUser->SetStat(STAT_CHA, 0);
						// Skill Pointlerini Free yapt�k
						pUser->GetTotalSkill(SkillPointFree, 83);
						pUser->GetTotalSkill(SkillPointCat1, 83);
						pUser->GetTotalSkill(SkillPointCat2, 83);
						pUser->GetTotalSkill(SkillPointCat3, 83);
						pUser->GetTotalSkill(SkillPointMaster, 23);
					}

					else
					{
						// Race ve Class'lar� belirledik
						pUser->m_bRace = 11;
						pUser->m_sClass = 206;
						// Karakterlerin STR ve HP'sini ayarlad�k
						pUser->SetStat(STAT_STR, 255);
						pUser->SetStat(STAT_STA, 177);
						pUser->SetStat(STAT_DEX, 0);
						pUser->SetStat(STAT_INT, 0);
						pUser->SetStat(STAT_CHA, 0);
						// Skill Pointlerini Free yapt�k
						pUser->GetTotalSkill(SkillPointFree, 83);
						pUser->GetTotalSkill(SkillPointCat1, 83);
						pUser->GetTotalSkill(SkillPointCat2, 83);
						pUser->GetTotalSkill(SkillPointCat3, 83);
						pUser->GetTotalSkill(SkillPointMaster, 23);
					}

					pUser->SendChat(ChatType::FORCE_CHAT, string_format("Ho�geldin �ovalye ! - Welcome Knight !"));
					pUser->SendChat(ChatType::FORCE_CHAT, string_format("Map Commands -> T�m karakterler E�it ve Dengelidir."));
					pUser->SendChat(ChatType::FORCE_CHAT, string_format("Map Commands -> T�m karakterler birbirlerine Atak yapabilir ve NP Kasabilir."));
					pUser->SendChat(ChatType::FORCE_CHAT, string_format("Map Commands -> Town atarsan�z eventten ��km�� olursunuz. L�tfen Event s�resinin bitmesini bekleyiniz."));

					_ITEM_DATA ItemEmpty;

					for (int i = 0; i < INVENTORY_TOTAL; i++)
					{
						_ITEM_DATA* pItem = g_pMain->m_DeathMatchItemsArray.GetData(i);
						if (!pItem)
						{
							//printf("sSlot : %d\n", i);
							memcpy(&pUser->m_sItemArray[i], &ItemEmpty, sizeof(_ITEM_DATA));
						}
						else
							memcpy(&pUser->m_sItemArray[i], pItem, sizeof(_ITEM_DATA));
					}

					pUser->m_sInEvent = 94;
					pUser->m_sInRoom = 1;
					pUser->m_sInDeathMatchEvent = true;
					pUser->ZoneChange(ZONE_FORGOTTEN_TEMPLE, float(128 + myrand(-20, +20)), float(128 + myrand(-20, +20)));
				}
				g_pMain->m_bDMStarted = true;
				g_pMain->m_bDMRegisterStarted = false;

			}
		}
		else if (g_pMain->m_bDMStarted)
		{

		}
		else
		{
			//Check auto start
		}

		Sleep(1000);
	}
	return 1;
}

void CGameServerDlg::StartDeathMatch()
{
	g_pMain->LogosYolla("", string_format("Death Match event ba�lad�, sohbete +dm yazarak kay�t olabilirsiniz."), 252, 207, 25);
	SendAnnouncementWhite("Death Match event ba�lad�, sohbete +dm yazarak kay�t olabilirsiniz."); // Yukardan Notice
	SendNoticeAlly("Death Match event ba�lad�, sohbete +dm yazarak kay�t olabilirsiniz."); // Ally Notice
	//SendNoticeClan("Death Match event ba�lad�, sohbete +dm yazarak kay�t olabilirsiniz."); // Clan Notice
	//SendNotice("Death Match event ba�lad�, sohbete +dm yazarak kay�t olabilirsiniz."); // D�z Notice
	m_bDMRegisterStarted = true;
	m_tDMRegisterStartTime = UNIXTIME;
	Guard lock(m_DMRegisterLock);
	m_DMRegisterlist.clear();
}


bool CGameServerDlg::DeathMatchRegister(CUser* pUser)
{
	if (!m_bDMRegisterStarted)
		return false;
	uint16 sSecond = uint16(UNIXTIME - m_tDMRegisterStartTime);

	if (sSecond <= 60)// 60 * 5 = 300 Saniye (5 Dk)
	{
		sSecond = 60 - sSecond;// 60 * 5 = 300 Saniye (5 Dk)

		Guard lock(m_DMRegisterLock);
		if (m_DMRegisterlist.find(pUser->GetID()) == m_DMRegisterlist.end())
		{
			m_DMRegisterlist.insert(pUser->GetID());

			SendHelpDescription(pUser, string_format("Kayd�n�z al�nd�, %d saniye sonra Death Match event ba�layacakt�r, kayd�n�z� silmek i�in +dm yaz�n.", sSecond));
		}
		else
		{
			m_DMRegisterlist.erase(pUser->GetID());
			SendHelpDescription(pUser, string_format("Kayd�n�z ba�ar�yla silinmi�tir.", sSecond));
		}
	}
	return true;
}

bool CGameServerDlg::DeathMatchRemove(CUser* pUser)
{
	if (!m_bDMRegisterStarted)
		return false;
	uint16 sSecond = uint16(UNIXTIME - m_tDMRegisterStartTime);

	if (sSecond <= 60)// 60 * 5 = 300 Saniye (5 Dk)
	{
		sSecond = 60 - sSecond;// 60 * 5 = 300 Saniye (5 Dk)

		Guard lock(m_DMRegisterLock);
		if (m_DMRegisterlist.find(pUser->GetID()) != m_DMRegisterlist.end())
		{
			m_DMRegisterlist.erase(pUser->GetID());

			SendHelpDescription(pUser, string_format("Kayd�n�z al�nd�, %d saniye sonra Death Match event ba�layacakt�r, kayd�n�z� silmek i�in +dm yaz�n.", sSecond));
		}
	}
	return true;
}

void CGameServerDlg::DeathMatchLogOut(CUser* pUser)
{
	memcpy(pUser->m_sItemArray, pUser->m_sItemArrayBackup, sizeof(pUser->m_sItemArray));
	pUser->m_bRace = pUser->m_bRaceBackup;
	pUser->m_sClass = pUser->m_sClassBackup;
	pUser->m_sInDeathMatchEvent = false;

	memcpy(pUser->m_bstrSkill, pUser->m_bstrSkillBackup, sizeof(uint8) * 10);

	memcpy(pUser->m_bStats, pUser->m_bStatsBackup, sizeof(uint8) * STAT_COUNT);

}

void CGameServerDlg::DeathMatchEventFinished()
{
	uint32 nHour = g_localTime.tm_hour;
	uint32 nMinute = g_localTime.tm_min;
	uint32 nSeconds = g_localTime.tm_sec;

	KickOutZoneUsers(ZONE_DEATHMATCH, ZONE_MORADON);

	DeathMatchEventGameServerSatus = false;
	DeathMatchEventFinishTime = 0;

	g_pMain->LogosYolla("", string_format("DeathMatch Event bitmistir, kat�lan oyuncular�m�za �d�lleri g�nderilmistir."), 132, 112, 255);
	SendNoticeAlly("[DeathMatch Event]: Event bitmistir, kat�lan oyuncular�m�za �d�lleri g�nderilmistir."); // Ally Notice
	printf("[DeathMatch Event]: Event Close %02d:%02d:%02d \n", nHour, nMinute, nSeconds);
}



