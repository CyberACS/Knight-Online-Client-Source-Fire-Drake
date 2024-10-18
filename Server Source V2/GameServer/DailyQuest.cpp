#include "stdafx.h"
#include "DBAgent.h"


COMMAND_HANDLER(CUser::HandleDailyOpen)
{
	if (!isGM())
		return false;

	if (vargs.size() < 3)
	{
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : +DailyOpen <Kacinci gorev>, <ödül id> , 1 / 0 "));
		return true;
	}

	uint32 m_sSelectMonster, m_sSelectReward;
	uint8 m_sOpen;

	vargs.pop_front();
	m_sSelectMonster = atoi(vargs.front().c_str());
	vargs.pop_front();
	m_sSelectReward = atoi(vargs.front().c_str());
	vargs.pop_front();
	m_sOpen = atoi(vargs.front().c_str());

	if (m_sOpen < 0 || m_sOpen >1)
	{
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : +DailyOpen <Kacinci gorev>, <ödül id> , 1 / 0 "));
		return true;
	}

	if (g_pMain->m_DailyGameServerSatus[m_sSelectMonster])
	{
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : Quest zaten aktif : %d saat sonra bitecek. Eventi hemen bitirmek isterseniz (+DailyOpen <Kacinci gorev>, <ödül id> , 0) yazmanýz yeterli.", (g_pMain->m_DailyFinishTime[m_sSelectMonster] / 60) / 60));
		return true;
	}

	g_pMain->m_DailySettings[m_sSelectReward] = g_pMain->m_DailySettingsArray.GetData(m_sSelectReward);
	g_pMain->m_DailyHuntList = g_pMain->m_DailyHuntListArray.GetData(m_sSelectMonster);
	g_pMain->m_DailySelectedMonster[m_sSelectMonster] = m_sSelectMonster;

	if (g_pMain->m_DailySettings[m_sSelectReward] == nullptr) {
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : Ödül verisi bulunamadý."));
		return true;
	}
	if (g_pMain->m_DailyCounts < g_pMain->m_DailySelectedMonster[m_sSelectMonster])
	{
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : Görev hedefi verisi bulunamadý."));
		return true;
	}
	if (g_pMain->m_DailyHuntList == nullptr) {
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : Hunt verisi bulunamadý."));
		return true;
	}


	if(m_sOpen == 1)
	g_pMain->m_DailyGameServerSatus[m_sSelectMonster] = true;
	else if (m_sOpen == 0)
	g_pMain->m_DailyGameServerSatus[m_sSelectMonster] = false;


	g_DBAgent.UpdateDailyQuest(m_sOpen, m_sSelectMonster);
	
	return true;
}

void CGameServerDlg::DailyFinish()
{
	for (int i = 0; i < 30; i++) {

	if (!m_DailyGameServerSatus[i])
		return;

	g_pMain->m_DailyFinishTime[i]--;

	

	if (g_pMain->m_DailyFinishTime[i] == 0)
	{
		printf("[Daily Quest] Event bitti görevler yenilendi.");
		g_pMain->m_DailyGameServerSatus[i] = false;

		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser* pUser = TO_USER(itr->second);

			if (pUser == nullptr
				|| !pUser->m_DailyRegister[i])
				continue;

			pUser->DailyEndProcess(i);
		}
	}
	}
}

void CUser::DailyEndProcess(int numara)
{
	for (int i = 0; i < 4; i++)
	{
		memset(&m_DailyEventMonsterID, 0, sizeof(m_DailyEventMonsterID[i][numara]));
		memset(&m_DailyEventMonsterKillCount, 0, sizeof(m_DailyEventMonsterKillCount[i][numara]));
		memset(&m_DailyEventCount, 0, sizeof(m_DailyEventCount[i][numara]));
		memset(&m_DailyEventZone, 0, sizeof(m_DailyEventZone[i][numara]));
		m_DailyEventMonsterName[i][numara].clear();
	}

	m_DailyRegister[numara] = false;
	Packet result(PL_DAILY);
	result << uint8(2);
	Send(&result);
}

void CGameServerDlg::UpdateDailyInfoTime()
{
	for (int i = 0; i < 120; i++)
	{
		g_pMain->m_DailyMonsterKillCount1[i] = 0;
		g_pMain->m_DailyMonsterNum1[i] = 0;
		g_pMain->m_DailyQuestZone[i] = 0;

	}


	uint8 Count = 0;
	printf("\n[Daily Quest] = Loading Quests \n");
	foreach_stlmap(itr, g_pMain->m_DailyHuntListArray)
	{
		_CYBERACS_DAILY_HUNT_LIST* m_DailyHuntList = itr->second;
		

		m_DailyQuestID[Count] = itr->second->QuestID;

		m_DailyQuestName[Count] = itr->second->QuestName;
		m_DailyGameServerSatus[Count] = itr->second->isOpen;

		m_DailySettingsID[Count] = itr->second->SettingsID;

		
		m_DailyMonsterNum1[Count] = itr->second->MonsterNum1;
		m_DailyMonsterNum2[Count] = itr->second->MonsterNum2;
		m_DailyMonsterNum3[Count] = itr->second->MonsterNum3;
		m_DailyMonsterNum4[Count] = itr->second->MonsterNum4;

		m_DailyMonsterName1[Count] = itr->second->MonsterName1;
		m_DailyMonsterName2[Count] = itr->second->MonsterName2;
		m_DailyMonsterName3[Count] = itr->second->MonsterName3;
		m_DailyMonsterName4[Count] = itr->second->MonsterName4;

		m_DailyMonsterKillCount1[Count] = itr->second->MonsterKillCount1;
		m_DailyMonsterKillCount2[Count] = itr->second->MonsterKillCount2;
		m_DailyMonsterKillCount3[Count] = itr->second->MonsterKillCount3;
		m_DailyMonsterKillCount4[Count] = itr->second->MonsterKillCount4;

		m_DailyQuestZone[Count] = itr->second->QuestZone;

		g_pMain->m_DailySettings[Count] = g_pMain->m_DailySettingsArray.GetData(m_DailySettingsID[Count]);

		Count++;
	}
	
	m_DailyCounts = Count;
	printf("[Daily Quest] = %d Loaded Quests \n", m_DailyCounts);
}




void CUser::DailyLoadProcess(uint8 Quest)
{

	m_DailyEventCount[0][Quest] = g_pMain->m_DailyMonsterKillCount1[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterName[0][Quest] = g_pMain->m_DailyMonsterName1[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterID[0][Quest] = g_pMain->m_DailyMonsterNum1[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventZone[0][Quest] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[Quest]];

	m_DailyEventCount[1][Quest] = g_pMain->m_DailyMonsterKillCount2[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterName[1][Quest] = g_pMain->m_DailyMonsterName2[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterID[1][Quest] = g_pMain->m_DailyMonsterNum2[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventZone[1][Quest] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[Quest]];

	m_DailyEventCount[2][Quest] = g_pMain->m_DailyMonsterKillCount3[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterName[2][Quest] = g_pMain->m_DailyMonsterName3[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterID[2][Quest] = g_pMain->m_DailyMonsterNum3[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventZone[2][Quest] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[Quest]];

	m_DailyEventCount[3][Quest] = g_pMain->m_DailyMonsterKillCount4[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterName[3][Quest] = g_pMain->m_DailyMonsterName4[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventMonsterID[3][Quest] = g_pMain->m_DailyMonsterNum4[g_pMain->m_DailySelectedMonster[Quest]];
	m_DailyEventZone[3][Quest] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[Quest]];

	Packet result(PL_DAILY);
	result << uint8(2) ; //uint16(1)elleme

	
	for (int b = 0; b < 4 ; b++) 
	{
	
			result << uint16(1)
				   << m_DailyEventMonsterKillCount[b][Quest]
				   << m_DailyEventCount[b][Quest];
			
	}		
	Send(&result);
	
}



void CUser::DailyProcess(uint16 MonsterNum)
{

	

	for (int b = 0; b < 120; b++) 
	{


	if (MonsterNum != m_DailyEventMonsterID[0][b]
		&& MonsterNum != m_DailyEventMonsterID[1][b]
		&& MonsterNum != m_DailyEventMonsterID[2][b]
		&& MonsterNum != m_DailyEventMonsterID[3][b])
		return;

	if (MonsterNum == m_DailyEventMonsterID[0][b] && m_DailyEventMonsterKillCount[0][b] != m_DailyEventCount[0][b] )
	{
		m_DailyEventMonsterKillCount[0][b]++;
	}
		
	if (MonsterNum == m_DailyEventMonsterID[1][b] && m_DailyEventMonsterKillCount[1][b] != m_DailyEventCount[1][b])
	{
		m_DailyEventMonsterKillCount[1][b]++;
	}
		
	if (MonsterNum == m_DailyEventMonsterID[2][b] && m_DailyEventMonsterKillCount[2][b] != m_DailyEventCount[2][b] )
	{
		m_DailyEventMonsterKillCount[2][b]++;
	}
		
	if (MonsterNum == m_DailyEventMonsterID[3][b] && m_DailyEventMonsterKillCount[3][b] != m_DailyEventCount[3][b])
	{
		m_DailyEventMonsterKillCount[3][b]++;
	}
		

	

	if (m_DailyEventMonsterKillCount[0][b] == m_DailyEventCount[0][b]
		&& m_DailyEventMonsterKillCount[1][b] == m_DailyEventCount[1][b]
		&& m_DailyEventMonsterKillCount[2][b] == m_DailyEventCount[2][b]
		&& m_DailyEventMonsterKillCount[3][b] == m_DailyEventCount[3][b])
		DailyReward(b);

	else if (m_DailyEventMonsterKillCount[0][b] == m_DailyEventCount[0][b] && MonsterNum == m_DailyEventMonsterID[0][b])
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : %s Quest Completed", m_DailyEventMonsterName[0][b].c_str()));
	else if (m_DailyEventMonsterKillCount[1][b] == m_DailyEventCount[1][b] && MonsterNum == m_DailyEventMonsterID[1][b])
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : %s Quest Completed", m_DailyEventMonsterName[1][b].c_str()));
	else if (m_DailyEventMonsterKillCount[2][b] == m_DailyEventCount[2][b] && MonsterNum == m_DailyEventMonsterID[2][b])
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : %s Quest Completed", m_DailyEventMonsterName[2][b].c_str()));
	else if (m_DailyEventMonsterKillCount[3][b] == m_DailyEventCount[3][b] && MonsterNum == m_DailyEventMonsterID[3][b])
		g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : %s Quest Completed", m_DailyEventMonsterName[3][b].c_str()));

	

	Packet result(WIZ_DEAD);
	result << uint16(2) << uint16(MonsterNum);
	Send(&result);


	

	}

	for (int Count = 0; Count < 120; Count++)
	{
		Packet result(PL_DAILY);
		result << uint8(3);
		for (int i = 0; i < 4; i++) {
			result << m_DailyEventCount[i][Count]
				<< m_DailyEventMonsterKillCount[i][Count];
		}
			

		
		Send(&result);
	}
}

void CUser::DailyReward(int numara)
{
	if (!g_pMain->m_DailyGameServerSatus[numara] || !m_DailyRegister[numara])
		return;

	uint8 rewardCount = 0;
	std::map<uint32, uint32> rewardMap;

	if (g_pMain->m_DailySettings[numara]->CREventWinEXP > 1) {
		ExpChange(g_pMain->m_DailySettings[numara]->CREventWinEXP);
		rewardMap.insert(std::make_pair(ITEM_EXP + (rewardCount++), g_pMain->m_DailySettings[numara]->CREventWinEXP));
	}

	if (g_pMain->m_DailySettings[numara]->CREventWinCoin > 1) {
		GoldGain(g_pMain->m_DailySettings[numara]->CREventWinCoin);
		rewardMap.insert(std::make_pair(ITEM_GOLD + (rewardCount++), g_pMain->m_DailySettings[numara]->CREventWinCoin));
	}

	if (g_pMain->m_DailySettings[numara]->CREventWinNP > 1) {
		SendLoyaltyChange(g_pMain->m_DailySettings[numara]->CREventWinNP, false, false, false);
		rewardMap.insert(std::make_pair(ITEM_COUNT + (rewardCount++), g_pMain->m_DailySettings[numara]->CREventWinNP));
	}

	if (g_pMain->m_DailySettings[numara]->CREventWinKC > 1) {
		SendKnightCash(g_pMain->m_DailySettings[numara]->CREventWinKC);
	}

	for (int i = 0; i < 2; i++) {
		if (g_pMain->m_DailySettings[numara]->CREventItemID[i] > 1 && g_pMain->m_DailySettings[numara]->CREventItemCount[i] > 0 && g_pMain->m_DailySettings[numara]->CREventItemTimed[i] > 0) {
			GiveItem(g_pMain->m_DailySettings[numara]->CREventItemID[i], g_pMain->m_DailySettings[numara]->CREventItemCount[i], true, g_pMain->m_DailySettings[numara]->CREventItemTimed[i]);
			rewardMap.insert(std::make_pair(g_pMain->m_DailySettings[numara]->CREventItemID[i] + (rewardCount++), g_pMain->m_DailySettings[numara]->CREventItemCount[i]));
		}
		else if (g_pMain->m_DailySettings[numara]->CREventItemID[i] > 0 && g_pMain->m_DailySettings[numara]->CREventItemCount[i] > 0) {
			GiveItem(g_pMain->m_DailySettings[numara]->CREventItemID[i], g_pMain->m_DailySettings[numara]->CREventItemCount[i]);
			rewardMap.insert(std::make_pair(g_pMain->m_DailySettings[numara]->CREventItemID[i] + (rewardCount++), g_pMain->m_DailySettings[numara]->CREventItemCount[i]));
		}
	}

	Packet result(WIZ_QUEST);
	result << uint8(10);

	foreach(itr, rewardMap)
		result << itr->first << itr->second;

	result << uint32(0);
	Send(&result);

	
	memset(&m_DailyEventMonsterKillCount, 0, sizeof(m_DailyEventMonsterKillCount));


	g_DBAgent.UpdateDailyQuestUser(numara+1, GetName());
	

	for (int Count = 0; Count < 120; Count++)
	{
		Packet result(PL_DAILY);
		result << uint8(3);
		for (int i = 0; i < 4; i++) {
			result << m_DailyEventCount[i][Count]
				<< m_DailyEventMonsterKillCount[i][Count];
		}

		result << uint8(1);
		Send(&result);
	}

	g_pMain->SendHelpDescription(this, string_format("[DAILY EVENT] : Tebrikler görev tamamlandý"));
}