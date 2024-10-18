#include "stdafx.h"

#pragma region CGameServerDlg::UpdateCyberACSCollectionRaceInfoTime()
void CGameServerDlg::UpdateCyberACSCollectionRaceInfoTime()
{
	uint8 Count = 0;
	printf("[CollectionRace Event] = Loading Quests \n");
	foreach_stlmap(itr, g_pMain->m_CyberACSCollectionRaceHuntListArray)
	{
		_CYBERACS_COLLECTIONRACE_HUNT_LIST* _CYBERACS_COLLECTIONRACE_HUNT_LIST = itr->second;
		Count++;

		m_CyberACSCollectionRaceQuestID[Count] = itr->second->QuestID;

		m_CyberACSCollectionRaceQuestZone[Count] = itr->second->QuestZone;

		m_CyberACSCollectionRaceMonsterNum1[Count] = itr->second->MonsterNum1;
		m_CyberACSCollectionRaceMonsterNum2[Count] = itr->second->MonsterNum2;
		m_CyberACSCollectionRaceMonsterNum3[Count] = itr->second->MonsterNum3;

		m_CyberACSCollectionRaceMonsterName1[Count] = itr->second->MonsterName1;
		m_CyberACSCollectionRaceMonsterName2[Count] = itr->second->MonsterName2;
		m_CyberACSCollectionRaceMonsterName3[Count] = itr->second->MonsterName3;

		m_CyberACSCollectionRaceMonsterKillCount1[Count] = itr->second->MonsterKillCount1;
		m_CyberACSCollectionRaceMonsterKillCount2[Count] = itr->second->MonsterKillCount2;
		m_CyberACSCollectionRaceMonsterKillCount3[Count] = itr->second->MonsterKillCount3;

	}
	m_CyberACSCollectionRaceCounts = Count;
	printf("[CollectionRace Event] = %d Loaded Quests \n", m_CyberACSCollectionRaceCounts);
}
#pragma endregion

#pragma region CUser::CREventProcess(uint16 MonsterNum)

void CUser::CyberACSCollectionRaceProcess(uint16 MonsterNum)
{
	if (!m_CyberACSCollectionRaceRegister)
		return;

	if (MonsterNum != m_CyberACSCollectionRaceEventMonsterID[0]
		&& MonsterNum != m_CyberACSCollectionRaceEventMonsterID[1]
		&& MonsterNum != m_CyberACSCollectionRaceEventMonsterID[2]
		&& MonsterNum != m_CyberACSCollectionRaceEventMonsterID[3])
		return;

	if (MonsterNum == m_CyberACSCollectionRaceEventMonsterID[0] && m_CyberACSCollectionRaceEventMonsterKillCount[0] != m_CyberACSCollectionRaceEventCount[0] && GetZoneID() == m_CyberACSCollectionRaceEventZone[0])
		m_CyberACSCollectionRaceEventMonsterKillCount[0]++;
	if (MonsterNum == m_CyberACSCollectionRaceEventMonsterID[1] && m_CyberACSCollectionRaceEventMonsterKillCount[1] != m_CyberACSCollectionRaceEventCount[1] && GetZoneID() == m_CyberACSCollectionRaceEventZone[1])
		m_CyberACSCollectionRaceEventMonsterKillCount[1]++;
	if (MonsterNum == m_CyberACSCollectionRaceEventMonsterID[2] && m_CyberACSCollectionRaceEventMonsterKillCount[2] != m_CyberACSCollectionRaceEventCount[2] && GetZoneID() == m_CyberACSCollectionRaceEventZone[2])
		m_CyberACSCollectionRaceEventMonsterKillCount[2]++;
	if (MonsterNum == m_CyberACSCollectionRaceEventMonsterID[3] && m_CyberACSCollectionRaceEventMonsterKillCount[3] != m_CyberACSCollectionRaceEventCount[3] && GetZoneID() == m_CyberACSCollectionRaceEventZone[3])
		m_CyberACSCollectionRaceEventMonsterKillCount[3]++;

	if (m_CyberACSCollectionRaceEventMonsterKillCount[0] == m_CyberACSCollectionRaceEventCount[0]
		&& m_CyberACSCollectionRaceEventMonsterKillCount[1] == m_CyberACSCollectionRaceEventCount[1]
		&& m_CyberACSCollectionRaceEventMonsterKillCount[2] == m_CyberACSCollectionRaceEventCount[2]
		&& m_CyberACSCollectionRaceEventMonsterKillCount[3] == m_CyberACSCollectionRaceEventCount[3])
		CyberACSCollectionRaceReward();

	else if (m_CyberACSCollectionRaceEventMonsterKillCount[0] == m_CyberACSCollectionRaceEventCount[0] && MonsterNum == m_CyberACSCollectionRaceEventMonsterID[0])
		g_pMain->SendHelpDescription(this, string_format("[Cr Event] : %s Quest Completed", m_CyberACSCollectionRaceEventMonsterName[0].c_str()));
	else if (m_CyberACSCollectionRaceEventMonsterKillCount[1] == m_CyberACSCollectionRaceEventCount[1] && MonsterNum == m_CyberACSCollectionRaceEventMonsterID[1])
		g_pMain->SendHelpDescription(this, string_format("[Cr Event] : %s Quest Completed", m_CyberACSCollectionRaceEventMonsterName[1].c_str()));
	else if (m_CyberACSCollectionRaceEventMonsterKillCount[2] == m_CyberACSCollectionRaceEventCount[2] && MonsterNum == m_CyberACSCollectionRaceEventMonsterID[2])
		g_pMain->SendHelpDescription(this, string_format("[Cr Event] : %s Quest Completed", m_CyberACSCollectionRaceEventMonsterName[2].c_str()));
	else if (m_CyberACSCollectionRaceEventMonsterKillCount[3] == m_CyberACSCollectionRaceEventCount[3] && MonsterNum == m_CyberACSCollectionRaceEventMonsterID[3])
		g_pMain->SendHelpDescription(this, string_format("[Cr Event] : %s Quest Completed", m_CyberACSCollectionRaceEventMonsterName[3].c_str()));

	Packet result(WIZ_DEAD);
	result << uint16(2) << uint16(MonsterNum);
	Send(&result);

	Packet hook(PL_CR);
	hook << uint8(2) << uint16(MonsterNum);
	Send(&hook);
}

#pragma endregion

#pragma region CGameServerDlg::CyberACSCollectionRaceLoadProcess()
void CUser::CyberACSCollectionRaceLoadProcess()
{
	m_CyberACSCollectionRaceRegister = true;

	m_CyberACSCollectionRaceEventCount[0] = g_pMain->m_CyberACSCollectionRaceMonsterKillCount1[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventMonsterName[0] = g_pMain->m_CyberACSCollectionRaceMonsterName1[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventMonsterID[0] = g_pMain->m_CyberACSCollectionRaceMonsterNum1[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventZone[0] = g_pMain->m_CyberACSCollectionRaceQuestZone[g_pMain->m_CyberACSCollectionRaceSelectedMonster];

	m_CyberACSCollectionRaceEventCount[1] = g_pMain->m_CyberACSCollectionRaceMonsterKillCount2[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventMonsterName[1] = g_pMain->m_CyberACSCollectionRaceMonsterName2[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventMonsterID[1] = g_pMain->m_CyberACSCollectionRaceMonsterNum2[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventZone[1] = g_pMain->m_CyberACSCollectionRaceQuestZone[g_pMain->m_CyberACSCollectionRaceSelectedMonster];

	m_CyberACSCollectionRaceEventCount[2] = g_pMain->m_CyberACSCollectionRaceMonsterKillCount3[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventMonsterName[2] = g_pMain->m_CyberACSCollectionRaceMonsterName3[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventMonsterID[2] = g_pMain->m_CyberACSCollectionRaceMonsterNum3[g_pMain->m_CyberACSCollectionRaceSelectedMonster];
	m_CyberACSCollectionRaceEventZone[2] = g_pMain->m_CyberACSCollectionRaceQuestZone[g_pMain->m_CyberACSCollectionRaceSelectedMonster];


	Packet result(PL_CR);
	result << uint8(0);

	for (int i = 0; i < 3; i++)
		result << m_CyberACSCollectionRaceEventMonsterID[i]
		<< m_CyberACSCollectionRaceEventCount[i]
		<< m_CyberACSCollectionRaceEventMonsterKillCount[i];

	for (int i = 0; i < 5; i++)
		result << g_pMain->m_CyberACSCollectionRaceSetting->CREventItemID[i]
		<< g_pMain->m_CyberACSCollectionRaceSetting->CREventItemCount[i];

	result << g_pMain->m_CyberACSCollectionRaceSetting->CREventWinCoin
		<< g_pMain->m_CyberACSCollectionRaceSetting->CREventWinEXP
		<< g_pMain->m_CyberACSCollectionRaceSetting->CREventWinNP
		<< g_pMain->m_CyberACSCollectionRaceSetting->CREventWinKC
		<< uint32(g_pMain->m_CyberACSCollectionRaceFinishTime);

	Send(&result);
}
#pragma endregion

#pragma region CGameServerDlg::CyberACSCollectionRaceEndProcess()
void CUser::CyberACSCollectionRaceEndProcess()
{
	memset(&m_CyberACSCollectionRaceEventMonsterID, 0, sizeof(m_CyberACSCollectionRaceEventMonsterID));
	memset(&m_CyberACSCollectionRaceEventMonsterKillCount, 0, sizeof(m_CyberACSCollectionRaceEventMonsterKillCount));
	memset(&m_CyberACSCollectionRaceEventCount, 0, sizeof(m_CyberACSCollectionRaceEventCount));
	memset(&m_CyberACSCollectionRaceEventZone, 0, sizeof(m_CyberACSCollectionRaceEventZone));

	for (int i = 0; i < 3; i++)
		m_CyberACSCollectionRaceEventMonsterName[i].clear();

	m_CyberACSCollectionRaceRegister = false;

	Packet result(PL_CR);
	result << uint8(1);
	Send(&result);
}
#pragma endregion

#pragma region CGameServerDlg::CyberACSCollectionRaceReward()
void CUser::CyberACSCollectionRaceReward()
{
	if (!m_CyberACSCollectionRaceRegister)
		return;

	uint8 rewardCount = 0;
	std::map<uint32, uint32> rewardMap;

	if (g_pMain->m_CyberACSCollectionRaceSetting->CREventWinEXP > 1) {
		ExpChange(g_pMain->m_CyberACSCollectionRaceSetting->CREventWinEXP);
		rewardMap.insert(std::make_pair(ITEM_EXP + (rewardCount++), g_pMain->m_CyberACSCollectionRaceSetting->CREventWinEXP));
	}

	if (g_pMain->m_CyberACSCollectionRaceSetting->CREventWinCoin > 1) {
		GoldGain(g_pMain->m_CyberACSCollectionRaceSetting->CREventWinCoin);
		rewardMap.insert(std::make_pair(ITEM_GOLD + (rewardCount++), g_pMain->m_CyberACSCollectionRaceSetting->CREventWinCoin));
	}

	if (g_pMain->m_CyberACSCollectionRaceSetting->CREventWinNP > 1) {
		SendLoyaltyChange(g_pMain->m_CyberACSCollectionRaceSetting->CREventWinNP, false, false, false);
		rewardMap.insert(std::make_pair(ITEM_COUNT + (rewardCount++), g_pMain->m_CyberACSCollectionRaceSetting->CREventWinNP));
	}

	if (g_pMain->m_CyberACSCollectionRaceSetting->CREventWinKC > 1) {
		SendKnightCash(g_pMain->m_CyberACSCollectionRaceSetting->CREventWinKC);
	}

	for (int i = 0; i < 2; i++) {
		if (g_pMain->m_CyberACSCollectionRaceSetting->CREventItemID[i] > 1 && g_pMain->m_CyberACSCollectionRaceSetting->CREventItemCount[i] > 0 && g_pMain->m_CyberACSCollectionRaceSetting->CREventItemTimed[i] > 0) {
			GiveItem(g_pMain->m_CyberACSCollectionRaceSetting->CREventItemID[i], g_pMain->m_CyberACSCollectionRaceSetting->CREventItemCount[i], true, g_pMain->m_CyberACSCollectionRaceSetting->CREventItemTimed[i]);
			rewardMap.insert(std::make_pair(g_pMain->m_CyberACSCollectionRaceSetting->CREventItemID[i] + (rewardCount++), g_pMain->m_CyberACSCollectionRaceSetting->CREventItemCount[i]));
		}
		else if (g_pMain->m_CyberACSCollectionRaceSetting->CREventItemID[i] > 0 && g_pMain->m_CyberACSCollectionRaceSetting->CREventItemCount[i] > 0) {
			GiveItem(g_pMain->m_CyberACSCollectionRaceSetting->CREventItemID[i], g_pMain->m_CyberACSCollectionRaceSetting->CREventItemCount[i]);
			rewardMap.insert(std::make_pair(g_pMain->m_CyberACSCollectionRaceSetting->CREventItemID[i] + (rewardCount++), g_pMain->m_CyberACSCollectionRaceSetting->CREventItemCount[i]));
		}
	}

	Packet result(WIZ_QUEST);
	result << uint8(10);

	foreach(itr, rewardMap)
		result << itr->first << itr->second;

	result << uint32(0);
	Send(&result);

	memset(&m_CyberACSCollectionRaceEventMonsterID, 0, sizeof(m_CyberACSCollectionRaceEventMonsterID));
	memset(&m_CyberACSCollectionRaceEventMonsterKillCount, 0, sizeof(m_CyberACSCollectionRaceEventMonsterKillCount));
	memset(&m_CyberACSCollectionRaceEventCount, 0, sizeof(m_CyberACSCollectionRaceEventCount));
	memset(&m_CyberACSCollectionRaceEventZone, 0, sizeof(m_CyberACSCollectionRaceEventZone));

	for (int i = 0; i < 3; i++)
		m_CyberACSCollectionRaceEventMonsterName[i].clear();

	m_CyberACSCollectionRaceRegister = false;

	result.clear();
	result.Initialize(PL_CR);
	result << uint8(1);
	Send(&result);

	g_pMain->SendHelpDescription(this, string_format("[Cr Event] : Tebrikler görev tamamlandý"));
}
#pragma endregion

#pragma region CUser::HandleCollectionInfoMessage
COMMAND_HANDLER(CUser::HandleCollectionInfoMessage)
{
	if (!m_CyberACSCollectionRaceRegister)
	{
		g_pMain->SendHelpDescription(this, string_format("[Collection Race] : Evente kayýtlý deðilsin"));
		return true;
	}

	g_pMain->SendHelpDescription(this, string_format("[Collection Race] : Event  %d  dakika sonra bitiyor", g_pMain->m_CyberACSCollectionRaceFinishTime / 60));
	g_pMain->SendHelpDescription(this, string_format("[Collection Race] : %s : %d/%d | %s : %d/%d | %s : %d/%d | %s : %d/%d",
		m_CyberACSCollectionRaceEventMonsterName[0].c_str(), m_CyberACSCollectionRaceEventMonsterKillCount[0], m_CyberACSCollectionRaceEventCount[0],
		m_CyberACSCollectionRaceEventMonsterName[1].c_str(), m_CyberACSCollectionRaceEventMonsterKillCount[1], m_CyberACSCollectionRaceEventCount[1],
		m_CyberACSCollectionRaceEventMonsterName[2].c_str(), m_CyberACSCollectionRaceEventMonsterKillCount[2], m_CyberACSCollectionRaceEventCount[2],
		m_CyberACSCollectionRaceEventMonsterName[3].c_str(), m_CyberACSCollectionRaceEventMonsterKillCount[3], m_CyberACSCollectionRaceEventCount[3]));
	return true;
}
#pragma endregion

#pragma region CUser::HandleCollectýonOpen
COMMAND_HANDLER(CUser::HandleCollectionOpen)
{
	if (!isGM())
		return true;

	if (g_pMain->m_CyberACSCollectionRaceGameServerSatus)
	{
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Event zaten aktif : %d dakika sonra bitecek. Eventi hemen bitirmek isterseniz +endcr yazmanýz yeterli.", g_pMain->m_CyberACSCollectionRaceFinishTime / 60));
		return true;
	}

	if (vargs.size() < 3)
	{
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : +CollectionOpen <dakika> <hedef id> <ödül id>"));
		return true;
	}

	int m_sTime = atoi(vargs.front().c_str());
	if (m_sTime < 10
		|| m_sTime > 500)
	{
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Süre  10 ile 500 dakika arasýnda olmalý."));
		return true;
	}

	vargs.pop_front();
	uint32 m_sSelectMonster, m_sSelectReward;
	m_sSelectMonster = atoi(vargs.front().c_str());
	vargs.pop_front();
	m_sSelectReward = atoi(vargs.front().c_str());



	g_pMain->m_CyberACSCollectionRaceSetting = g_pMain->m_CyberACSCollectionRaceSettingsArray.GetData(m_sSelectReward);
	g_pMain->m_CyberACSCollectionRaceHuntList = g_pMain->m_CyberACSCollectionRaceHuntListArray.GetData(m_sSelectMonster);
	g_pMain->m_CyberACSCollectionRaceSelectedMonster = m_sSelectMonster;

	if (g_pMain->m_CyberACSCollectionRaceSetting == nullptr) {
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Ödül verisi bulunamadý."));
		return true;
	}
	if (g_pMain->m_CyberACSCollectionRaceCounts < g_pMain->m_CyberACSCollectionRaceSelectedMonster)
	{
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Görev hedefi verisi bulunamadý."));
		return true;
	}
	if (g_pMain->m_CyberACSCollectionRaceHuntList == nullptr) {
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Hunt verisi bulunamadý."));
		return true;
	}

	g_pMain->m_CyberACSCollectionRaceFinishTime = m_sTime * 60;

	uint16 m_sMaxCount = 0;

	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach(itr, sessMap)
	{
		CUser* pUser = TO_USER(itr->second);
		if (pUser == nullptr)
			continue;

		if (pUser->m_CyberACSCollectionRaceRegister)
			continue;

		if (pUser->GetLevel() < g_pMain->m_CyberACSCollectionRaceSetting->CREventMinLevel)
			continue;

		if (pUser->GetZoneID() != g_pMain->m_CyberACSCollectionRaceHuntList->QuestZone)
			continue;

		if (m_sMaxCount == g_pMain->m_CyberACSCollectionRaceSetting->CREventMaxUser)
			continue;

		pUser->CyberACSCollectionRaceLoadProcess();
		m_sMaxCount++;
	}
	if (m_sMaxCount > 0)
	{
		g_pMain->m_CyberACSCollectionRaceGameServerSatus = true;
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Event baþladý! | Katýlýmcý : %d | Süre : %d", m_sMaxCount, g_pMain->m_CyberACSCollectionRaceFinishTime / 60));
	}
	else
	{
		g_pMain->m_CyberACSCollectionRaceFinishTime = 0;
		g_pMain->m_CyberACSCollectionRaceGameServerSatus = false;
		g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Event Baþlatýlamadý.! | Katýlýmcý : %d | Süre : %d", m_sMaxCount, g_pMain->m_CyberACSCollectionRaceFinishTime / 60));
	}

	return true;
}
#pragma endregion


#pragma region CUser::HandleCollectýonOpen
COMMAND_HANDLER(CGameServerDlg::HandleCollectionOpen)
{


	if (g_pMain->m_CyberACSCollectionRaceGameServerSatus)
	{
		printf("[CR EVENT] : Event zaten aktif : %d dakika sonra bitecek. Eventi hemen bitirmek isterseniz +endcr yazmanýz yeterli.", g_pMain->m_CyberACSCollectionRaceFinishTime / 60);
		return true;
	}

	if (vargs.size() < 3)
	{
		printf("[CR EVENT] : +CollectionOpen <dakika> <hedef id> <ödül id>");
		return true;
	}

	int m_sTime = atoi(vargs.front().c_str());
	if (m_sTime < 10
		|| m_sTime > 500)
	{
		printf("[CR EVENT] : Süre  10 ile 500 dakika arasýnda olmalý.");
		return true;
	}

	vargs.pop_front();
	uint32 m_sSelectMonster, m_sSelectReward;
	m_sSelectMonster = atoi(vargs.front().c_str());
	vargs.pop_front();
	m_sSelectReward = atoi(vargs.front().c_str());

	g_pMain->m_CyberACSCollectionRaceSetting = g_pMain->m_CyberACSCollectionRaceSettingsArray.GetData(m_sSelectReward);
	g_pMain->m_CyberACSCollectionRaceHuntList = g_pMain->m_CyberACSCollectionRaceHuntListArray.GetData(m_sSelectMonster);
	g_pMain->m_CyberACSCollectionRaceSelectedMonster = m_sSelectMonster;

	if (g_pMain->m_CyberACSCollectionRaceSetting == nullptr) {
		printf("[CR EVENT] : Ödül verisi bulunamadý.");
		return true;
	}
	if (g_pMain->m_CyberACSCollectionRaceCounts < g_pMain->m_CyberACSCollectionRaceSelectedMonster)
	{
		printf("[CR EVENT] : Görev hedefi verisi bulunamadý.");
		return true;
	}
	if (g_pMain->m_CyberACSCollectionRaceHuntList == nullptr) {
		printf("[CR EVENT] : Hunt verisi bulunamadý.");
		return true;
	}

	g_pMain->m_CyberACSCollectionRaceFinishTime = m_sTime * 60;

	uint16 m_sMaxCount = 0;

	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach(itr, sessMap)
	{
		CUser* pUser = TO_USER(itr->second);
		if (pUser == nullptr)
			continue;

		if (pUser->m_CyberACSCollectionRaceRegister)
			continue;

		if (pUser->GetLevel() < g_pMain->m_CyberACSCollectionRaceSetting->CREventMinLevel)
			continue;

		if (pUser->GetZoneID() != g_pMain->m_CyberACSCollectionRaceHuntList->QuestZone)
			continue;

		if (m_sMaxCount == g_pMain->m_CyberACSCollectionRaceSetting->CREventMaxUser)
			continue;

		pUser->CyberACSCollectionRaceLoadProcess(); m_sMaxCount++;
	}
	if (m_sMaxCount > 0)
	{
		g_pMain->m_CyberACSCollectionRaceGameServerSatus = true;
		printf("[CR EVENT] : Event baþladý! | Katýlýmcý : %d | Süre : %d", m_sMaxCount, g_pMain->m_CyberACSCollectionRaceFinishTime / 60);
	}
	else
	{
		g_pMain->m_CyberACSCollectionRaceFinishTime = 0;
		g_pMain->m_CyberACSCollectionRaceGameServerSatus = false;
		printf("[CR EVENT] : Event Baþlatýlamadý.! | Katýlýmcý : %d | Süre : %d", m_sMaxCount, g_pMain->m_CyberACSCollectionRaceFinishTime / 60);
	}

	return true;
}
#pragma endregion

#pragma region CUser::CollectýonClose
COMMAND_HANDLER(CGameServerDlg::HandleCollectionClose)
{

	g_pMain->m_CyberACSCollectionRaceFinishTime = 0;
	g_pMain->m_CyberACSCollectionRaceGameServerSatus = false;

	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach(itr, sessMap)
	{
		CUser* pUser = TO_USER(itr->second);

		if (pUser == nullptr)
			continue;

		pUser->CyberACSCollectionRaceEndProcess();
	}
	return true;
}
#pragma endregion

#pragma region CUser::CollectýonClose
COMMAND_HANDLER(CUser::HandleCollectionClose)
{
	if (!isGM())
		return false;

	g_pMain->m_CyberACSCollectionRaceFinishTime = 0;
	g_pMain->m_CyberACSCollectionRaceGameServerSatus = false;

	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach(itr, sessMap)
	{
		CUser* pUser = TO_USER(itr->second);

		if (pUser == nullptr)
			continue;

		pUser->CyberACSCollectionRaceEndProcess();
	}
	return true;
}
#pragma endregion

#pragma region CGameServerDlg::CyberACSCollectionRaceFinish()
void CGameServerDlg::CyberACSCollectionRaceFinish()
{
	if (!m_CyberACSCollectionRaceGameServerSatus)
		return;

	g_pMain->m_CyberACSCollectionRaceFinishTime--;

	if (g_pMain->m_CyberACSCollectionRaceFinishTime == 15 * 60
		|| g_pMain->m_CyberACSCollectionRaceFinishTime == 10 * 60
		|| g_pMain->m_CyberACSCollectionRaceFinishTime == 5 * 60
		|| g_pMain->m_CyberACSCollectionRaceFinishTime == 3 * 60
		|| g_pMain->m_CyberACSCollectionRaceFinishTime == 1 * 60)
		printf("[Collection Race] Eventin bitmesine %d dakika kaldý.", g_pMain->m_CyberACSCollectionRaceFinishTime / 60);

	if (g_pMain->m_CyberACSCollectionRaceFinishTime == 0)
	{
		printf("[Collection Race] Event bitti tüm katýlýmcýlara teþekkürler.");
		g_pMain->m_CyberACSCollectionRaceGameServerSatus = false;

		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser* pUser = TO_USER(itr->second);

			if (pUser == nullptr
				|| !pUser->m_CyberACSCollectionRaceRegister)
				continue;

			pUser->CyberACSCollectionRaceEndProcess();
		}
	}
}
#pragma endregion