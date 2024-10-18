#include "stdafx.h"
#include "DBAgent.h"
#include "md5.h"
#include "MagicInstance.h"

using namespace std;
#define CYBERANTICHEAT_ACTIVE 1
#define CYBERANTICHEAT_VERSION	2
#define CYBERANTICHEAT_ALIVE_TIMEOUT 90
#define CYBERANTICHEAT_SUPPORT_CHEK 10

struct _TempsItems
{
	uint32 nItemID;
	uint32 time;
	uint8 pos;
};

uint32 s_SwiftSkills[] = { 208002 , 108010 , 207010 , 208010 , 500265 , 107725 , 108725 , 207725 , 208725 , 490230 , 490336, 101002, 102002, 105002, 106002, 107002, 108002, 201002 , 202002, 205002, 206002, 207002, 208002, 490223, 490334, 500316 };

uint32 s_HpSkills[] = { 111606, 491009, 500102, 491011 , 500103 , 500304 , 500315 , 500054 , 112606 , 211606 , 212606 , 111615 , 112615 , 211615 , 212615 , 111624 , 112624 , 211624 , 212624 , 111633 , 112633 , 211633 , 212633 , 111642 , 112642 , 211642 , 212642 , 111654 , 112654 , 211654 , 212654 , 500054 , 500354 , 111655 , 112655 , 211655 , 212655 , 111656 , 112656 , 211656 , 212656 , 111657 , 112657 , 211657 , 212657 , 112670 , 212670 , 112672 , 212672 , 112675 , 212675 };

uint32 s_AcSkills[] = { 491006, 491007, 500029, 500030, 500055, 500056, 111603, 112603, 211603, 212603, 111612, 112612, 211612, 212612, 111621, 112621, 211621, 212621, 111630, 112630, 211630, 212630, 111639, 112639, 211639, 212639,	111651 , 112651 , 211651 , 212651 , 111660 , 112660 , 211660 , 212660 , 112673 , 212673 , 112674 , 212674 };

uint32 s_ResisSkills[] = { 111609, 112609, 211609, 212609, 111627, 112627, 211627, 212627, 111636, 112636, 211636, 212636, 111645, 112645, 211645, 212645 };

uint32 s_ResSkills[] = { 111503, 112503, 211503, 212503, 111512, 112512, 211512, 212512, 111521, 112521, 211521, 212521, 111530, 112530, 211530, 212530, 111539, 112539, 211539 , 212539 , 111548 , 112548 , 211548 , 212548 , 112570 , 212570 , 112575 , 212575 , 112580 , 212580 };

#pragma region CUser::HandleCyberACS(Packet & pkt)
/**
* @brief	Handles the CyberACS Guard packets coming from the client and
*			responds respectively.
*
* @param	pkt	The packet to be handled.
*/
void CUser::HandleCyberACS(Packet& pkt)
{
	uint8 command = pkt.read<uint8>();
	// If we're not authed yet, forced us to before we can do anything else.
	// NOTE: We're checking the account ID store here because it's only set on successful auth,
	// at which time the other account ID will be cleared out (yes, it's messy -- need to clean it up).
	if (m_strAccountID.empty())
	{
		switch (command)
		{
		case WIZ_CYBERANTICHEAT_ALIVE:
		case WIZ_CYBERANTICHEAT_COM:
			break;
		default:
			printf("[SID=%d] Hook System packet (%X) prior to account login\n", GetSocketID(), command);
			break;
		}
	}
	else if (!m_bSelectedCharacter)
	{
		switch (command)
		{
		case WIZ_CYBERANTICHEAT_COM:
			CyberACS_General(pkt);
			break;
		case WIZ_CYBERANTICHEAT_ALIVE:
			CyberACS_StayAlive(pkt);
			break;
		default:
			printf("[SID=%d] Hook System packet (%X) prior to selecting character\n", GetSocketID(), command);
			break;
		}
	}

	if (!isInGame())
	{
		switch (command)
		{
		case WIZ_CYBERANTICHEAT_COM:
			CyberACS_General(pkt);
			break;
		case WIZ_CYBERANTICHEAT_ALIVE:
			CyberACS_StayAlive(pkt);
			break;
		case WIZ_CYBERANTICHEAT_AUTHINFO:
			CyberACS_AuthInfo(pkt);
			break;
		case WIZ_CYBERANTICHEAT_PROCINFO:
			//Wiz_CYBERANTICHEAT_ProcInfo(pkt);
			break;
		case WIZ_CYBERANTICHEAT_LOG:
			CyberACS_LogHandler(pkt);
			break;
		case WIZ_CYBERANTICHEAT_UIINFO:
			CyberACS_UIRequest(pkt);
			break;
		case WIZ_CYBERANTICHEAT_PUS:
			CyberACS_PowerUpStoreRequest(pkt);
			break;
		case WIZ_CYBERANTICHEAT_RESET:
			CyberACS_AllSkillStatRestard(pkt);
			break;
		case WIZ_CYBERANTICHEAT_DROP_REQUEST:
			CyberACS_DropRequest(pkt);
			break;
		case WIZ_CYBERANTICHEAT_CLANBANK:
			//Wiz_CYBERANTICHEAT_ClanBank(pkt);
			break;
		case WIZ_CYBERANTICHEAT_USERINFO:
			CyberACS_ReqUserInfo(pkt);
			break;
		case WIZ_CYBERANTICHEAT_LOOT_SETTINS:
			ReqSaveLootSettings(pkt);
			break;
		case WIZ_CYBERANTICHEAT_CHAOTIC_EXCHANGE:
			CyberACS_ChaoticExchange(pkt);
			break;
		case WIZ_CYBERANTICHEAT_MERCHANT:
			CyberACS_MerchantHandler(pkt);
			break;
		case WIZ_CYBERANTICHEAT_USERDATA:
			CyberACS_ReqUserData(pkt);
			break;
		case WIZ_CYBERANTICHEAT_TEMPITEMS:
			CyberACS_SendTempItems(pkt);
			break;
		case WIZ_CYBERANTICHEAT_CR:
			break;
		case WIZ_CYBERANTICHEAT_SUPPORT:
			CyberACS_Support(pkt);
			break;
		case WIZ_CYBERANTICHEAT_MERCHANTLIST:
			CyberACS_ReqMerchantList(pkt);
			break;
		case WIZ_CYBERANTICHEAT_LIFESKILL:
			CyberACS_HandleLifeSkill(pkt);
			break;
		default:
			printf("[SID=%d] Hook System Unknown packet !isInGame() %X\n", GetSocketID(), command);
			break;
		}
	}
	else if (isInGame())
	{
		switch (command)
		{
		case WIZ_CYBERANTICHEAT_COM:
			CyberACS_General(pkt);
			break;
		case WIZ_CYBERANTICHEAT_ALIVE:
			CyberACS_StayAlive(pkt);
			break;
		case WIZ_CYBERANTICHEAT_AUTHINFO:
			CyberACS_AuthInfo(pkt);
			break;
		case WIZ_CYBERANTICHEAT_PROCINFO:
			//Wiz_CYBERANTICHEAT_ProcInfo(pkt);
			break;
		case WIZ_CYBERANTICHEAT_LOG:
			CyberACS_LogHandler(pkt);
			break;
		case WIZ_CYBERANTICHEAT_UIINFO:
			CyberACS_UIRequest(pkt);
			break;
		case WIZ_CYBERANTICHEAT_PUS:
			CyberACS_PowerUpStoreRequest(pkt);
			break;
		case WIZ_CYBERANTICHEAT_RESET:
			CyberACS_AllSkillStatRestard(pkt);
			break;
		case WIZ_CYBERANTICHEAT_DROP_REQUEST:
			CyberACS_DropRequest(pkt);
			break;
		case WIZ_CYBERANTICHEAT_CLANBANK:
			//Wiz_CYBERANTICHEAT_ClanBank(pkt);
			break;
		case WIZ_CYBERANTICHEAT_USERINFO:
			CyberACS_ReqUserInfo(pkt);
			break;
		case WIZ_CYBERANTICHEAT_LOOT_SETTINS:
			ReqSaveLootSettings(pkt);
			break;
		case WIZ_CYBERANTICHEAT_CHAOTIC_EXCHANGE:
			CyberACS_ChaoticExchange(pkt);
			break;
		case WIZ_CYBERANTICHEAT_MERCHANT:
			CyberACS_MerchantHandler(pkt);
			break;
		case WIZ_CYBERANTICHEAT_USERDATA:
			CyberACS_ReqUserData(pkt);
			break;
		case WIZ_CYBERANTICHEAT_TEMPITEMS:
			CyberACS_SendTempItems(pkt);
			break;
		case WIZ_CYBERANTICHEAT_CR:
			break;
		case WIZ_CYBERANTICHEAT_SUPPORT:
			CyberACS_Support(pkt);
			break;
		case WIZ_CYBERANTICHEAT_MERCHANTLIST:
			CyberACS_ReqMerchantList(pkt);
			break;
		case WIZ_CYBERANTICHEAT_LIFESKILL:
			CyberACS_HandleLifeSkill(pkt);
			break;
		case WIZ_CYBERANTICHEAT_MONSTER_INFO:
			CyberACSMonsterInfoHandler(pkt);
			break;
		case WIZ_CYBERANTICHEAT_EVENT:
			TempleProcess(pkt);
			break;
		case WIZ_CYBERANTICHEAT_VOICE:
			CyberACS_VoiceHandler(pkt);
			break;
		default:
			printf("[SID=%d] Hook System Unknown packet isInGame() %X\n", GetSocketID(), command);
			break;
		}
	}
}
#pragma endregion

#pragma region CUser::CyberACS_VoiceHandler(Packet& pkt) 
void CUser::CyberACS_VoiceHandler(Packet& pkt)
{
	if (!isInParty())
		return;

	uint64 len;
	pkt >> len;

	Packet result(PL_VOICE);
	result << GetID() << len;
	Send(&result);

	result.clear();
	result.Initialize(PL_VOICE);
	result << GetID();
	result.append(pkt.contents(), pkt.size());

	CUser* pPartyUser;
	_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty)
	{
		for (int i = 0; i < MAX_PARTY_USERS; i++)
		{
			pPartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
			if (pPartyUser)
			{
				if (GetID() == pPartyUser->GetID())
					continue;

				pPartyUser->Send(&result);
			}
		}
	}
}
#pragma endregion

#pragma region CUser::CyberACS_SendTempItems(Packet& pkt) 
void CUser::CyberACS_SendTempItems(Packet& pkt)
{
	Packet result(PL_TEMPITEMS);
	vector<_TempsItems> items;

	for (int i = 0; i < INVENTORY_TOTAL; i++)
	{
		if (m_sItemArray[i].nNum)
		{
			if (m_sItemArray[i].nExpirationTime <= 0)
				continue;

			_TempsItems item;
			item.nItemID = m_sItemArray[i].nNum;
			item.pos = i >= 51 && i <= 51 + MBAG_TOTAL ? 1 : 0;
			item.time = m_sItemArray[i].nExpirationTime;
			items.push_back(item);
		}
	}

	for (int i = 0; i < WAREHOUSE_MAX; i++)
	{
		if (m_sWarehouseArray[i].nNum)
		{
			if (m_sWarehouseArray[i].nExpirationTime <= 0)
				continue;

			_TempsItems item;
			item.nItemID = m_sWarehouseArray[i].nNum;
			item.pos = 2;
			item.time = m_sWarehouseArray[i].nExpirationTime;
			items.push_back(item);
		}
	}

	result << uint8(items.size());
	for (auto itr : items)
		result << itr.nItemID << itr.pos << itr.time;
	Send(&result);
}
#pragma endregion

#pragma region CUser::CyberACS_HpInfo()
void CUser::CyberACS_HpInfo()
{
	Packet result(WIZ_CYBERANTICHEAT_GUARD, uint8(WIZ_CYBERANTICHEAT_HPINFO));
	result << uint16(m_MaxHP) << m_sHp << uint16(m_MaxMP) << m_sMp;
	Send(&result);
}
#pragma endregion



#pragma region CUser::CyberACS_Support(Packet& pkt)
void CUser::CyberACS_Support(Packet& pkt)
{
	if (CyberACS_LASTSUPPORT == 0)
		CyberACS_LASTSUPPORT = UNIXTIME;

	if (UNIXTIME - CyberACS_LASTSUPPORT < CYBERANTICHEAT_SUPPORT_CHEK)
		return;

	uint8 subCode;
	pkt >> subCode;
	string subject, message;
	pkt >> subject >> message;
	if (subject.size() > 25 || message.size() > 40)
		return;

	DateTime time;
	g_DBAgent.Support(subject, message);
	g_pMain->WriteCheatLogFile(string_format("[CyberACS Log Support - %d:%d:%d] %s, %s, %s, %s, %s, %s\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetAccountName().c_str(), GetName().c_str(), GetRemoteIP().c_str(), subject.c_str(), message.c_str(), subCode == 0 ? "Bug" : "Bot & Koxp"));
	CyberACS_LASTSUPPORT = UNIXTIME;
}
#pragma endregion


#pragma region CUser::CyberACS_StayAlive(Packet& pkt)
void CUser::CyberACS_StayAlive(Packet& pkt)
{
	CyberACS_BotPriestSystem();
	std::string public_key = md5("0x" + std::to_string(CYBERANTICHEAT_VERSION) + "52824" + m_strAccountID);
	std::string uPublic_key;
	pkt >> uPublic_key;
	if (&uPublic_key != nullptr) {
		if (public_key != uPublic_key) {
			//printf("%s Version mismatch. Please update your game or reinstall.", m_strAccountID.c_str());
			UserDataSaveToAgent();
			Disconnect();
			return;
		}
		else {
			CyberACS_LASTCHECK = UNIXTIME;
			Packet result(PL_USERDATA);
			result << GetSocketID()
				<< GetName() << GetClass() << GetRace() << GetLevel() << GetRebirthLevel() << GetNation() << m_iMaxExp << m_iExp << m_iMannerPoint
				<< uint16(m_bStats[0]) << uint16(m_bStats[1]) << uint16(m_bStats[2]) << uint16(m_bStats[3]) << uint16(m_bStats[4])
				<< g_pMain->m_CyberACSSettings->ACS_Validation
				<< g_pMain->m_CyberACSSettings->Button_Facebook
				<< g_pMain->m_CyberACSSettings->Button_Discord
				<< g_pMain->m_CyberACSSettings->Button_Live
				<< g_pMain->m_CyberACSSettings->Button_Support
				<< g_pMain->m_CyberACSSettings->Button_Youtube
				<< g_pMain->m_CyberACSSettings->Button_Twitter
				<< g_pMain->m_CyberACSSettings->Button_Skype
				<< g_pMain->m_CyberACSSettings->URL_Facebook
				<< g_pMain->m_CyberACSSettings->URL_Discord
				<< g_pMain->m_CyberACSSettings->URL_Live
				<< g_pMain->m_CyberACSSettings->URL_Youtube
				<< g_pMain->m_CyberACSSettings->URL_Twitter
				<< g_pMain->m_CyberACSSettings->URL_Skype
				<< g_pMain->m_CyberACSSettings->URL_KCBayi
				<< g_pMain->m_CyberACSSettings->KCMerchant_MinPrice
				<< g_pMain->m_CyberACSSettings->KCMerchant_MaxPrice
				<< g_pMain->m_CyberACSSettings->State_StatReset
				<< g_pMain->m_CyberACSSettings->State_SkillReset
				<< g_pMain->m_CyberACSSettings->State_TempItemList
				<< g_pMain->m_CyberACSSettings->State_PUS;
			Send(&result);
		}
	}
	else {
		UserDataSaveToAgent();
		Disconnect();
		return;
	}
}
#pragma endregion



#pragma region CUser::CyberACS_Main()
void CUser::CyberACS_Main()
{
	if (!isInGame()
		|| !m_bSelectedCharacter)
		return;

	if (CyberACS_TICKCOUNT == 0)
		CyberACS_LASTCHECK = UNIXTIME;
	if (UNIXTIME - CyberACS_LASTCHECK > CYBERANTICHEAT_ALIVE_TIMEOUT) {
		if (CYBERANTICHEAT_ACTIVE == 1) {
			//printf("%s Couldn't connect to ACS server.\n", m_strAccountID.c_str());
			g_pMain->SendHelpDescription(this, "Couldn't connect to ACS server.");
			UserDataSaveToAgent();
			Disconnect();
			return;
		}
	}
	else {
		CyberACS_TICKCOUNT++;
	}
}
#pragma endregion

#pragma region CUser::CyberACSMonsterInfoHandler(Packet & pkt)
void CUser::CyberACSMonsterInfoHandler(Packet& pkt)
{
	uint32 MonsterID;
	pkt >> MonsterID;
	CNpcTable* pMob = g_pMain->m_arMonTable.GetData(MonsterID);
	if (pMob == nullptr)
		return;

	Packet result(PL_MONSTER_INFO);
	result << pMob->m_sLevel << pMob->m_MaxHP << pMob->m_sDamage << pMob->m_sDefense;
	Send(&result);
}
#pragma endregion


#pragma region CUser::CyberACS_SendMessageBox(string title, string message)
void CUser::CyberACS_SendMessageBox(string title, string message)
{
	Packet result(WIZ_CYBERANTICHEAT_GUARD, uint8(WIZ_CYBERANTICHEAT_MESSAGE));
	result << title << message;
	Send(&result);
}
#pragma endregion

#pragma region CUser::LifeSkillHuntProcess(uint16 MonsterNum)
void CUser::LifeSkillHuntProcess(uint16 MonsterNum)
{
	CNpc* pNpc = g_pMain->GetNpcPtr(MonsterNum, GetZoneID());
	if (pNpc == nullptr)
		return;

	if (GetLevel() > pNpc->GetLevel())
	{
		if (GetLevel() - pNpc->GetLevel() > 60)
			return;
	}

	int exp = 1;
	if (pNpc->GetLevel() >= 110)
		exp = 10;

	if (isInParty())
	{
		CUser* pPartyUser;
		_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());

		if (pParty == nullptr)
			return;

		for (int i = 0; i < MAX_PARTY_USERS; i++) {
			pPartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
			if (pPartyUser == nullptr)
				continue;

			if (!pPartyUser->isInGame()
				|| pPartyUser->isDead()
				|| pPartyUser->isFishing()
				|| pPartyUser->isMining()
				|| pPartyUser->isStoreOpen()
				|| pPartyUser->isMerchanting()
				|| pPartyUser->isTrading()
				|| !pPartyUser->isInRange(this, RANGE_50M))
				continue;

			pPartyUser->m_sLifeSkills.HuntingExp += exp;
			pPartyUser->CyberACS_SendLifeSkills();
		}
	}
	if (!isInParty())
	{
		if (!isInGame()
			|| isDead()
			|| isFishing()
			|| isMining()
			|| isStoreOpen()
			|| isMerchanting()
			|| isTrading())
			return;

		m_sLifeSkills.HuntingExp += exp;
		CyberACS_SendLifeSkills();
	}
}
#pragma endregion

#pragma region CUser::CyberACS_SendPartyProcess()
void CUser::CyberACS_SendPartyProcess()
{
	Packet result(PL_PARTY_PROCESS);
	uint8 m_Count = 8;
	result << uint8(1) << m_Count;
	_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());

	if (pParty == nullptr)
		return;

	for (int32 j = 0; j < MAX_PARTY_USERS; j++)
	{
		uint8 s_swift = 0; uint8 s_hpbuff = 0; uint8 s_acbuff = 0; uint8 s_resisbuff = 0; uint8 s_resbuff = 0;

		CUser* pPartyUser = g_pMain->GetUserPtr(pParty->uid[j]);
		if (pPartyUser == nullptr)
			continue;

		result << pPartyUser->GetID() << short(pPartyUser->GetMaxHealth()) << short(pPartyUser->GetHealth()) << pPartyUser->GetLevel();

		Guard lock(pPartyUser->m_buffLock);

		foreach(itr, pPartyUser->m_buffMap)
		{
			for (int i = 0; i < 26; i++)
				if (itr->second.m_nSkillID == s_SwiftSkills[i])
					s_swift = 1;

			for (int i = 0; i < 51; i++)
				if (itr->second.m_nSkillID == s_HpSkills[i])
					s_hpbuff = 1;

			for (int i = 0; i < 38; i++)
				if (itr->second.m_nSkillID == s_AcSkills[i])
					s_acbuff = 1;

			for (int i = 0; i < 16; i++)
				if (itr->second.m_nSkillID == s_ResisSkills[i])
					s_resisbuff = 1;

			for (int i = 0; i < 30; i++)
				if (itr->second.m_nSkillID == s_ResSkills[i])
					s_resbuff = 1;
		}

		result << s_swift << s_hpbuff << s_acbuff << s_resisbuff << s_resbuff;
		m_Count++;
	}
	if (m_Count < 2)
		result.put(0, 0);

	result.put(2, m_Count);
	Send(&result);
}
#pragma endregion

#pragma region CUser::SpeedHackTime(Packet & pkt)
void CUser::SpeedHackTime(Packet& pkt)
{
	if (!isInGame() || isGM())
		return;

	DateTime time;
	float nSpeed = 45.0f;

	if (GetFame() == COMMAND_CAPTAIN
		|| isRogue())
		nSpeed = 90.0f;
	else if (isWarrior()
		|| isMage()
		|| isPriest())
		nSpeed = 67.0f;
	else if (isPortuKurian())
		nSpeed = 67.0f;

	nSpeed += 15.0f; // Tolerance...

	float nRange = (pow(GetX() - m_LastX, 2.0f) + pow(GetZ() - m_LastZ, 2.0f)) / 100.0f;

	if (nRange >= nSpeed)
	{
		m_SpeedHackTrial++;

		if (m_SpeedHackTrial >= 3)
			g_pMain->WriteCheatLogFile(string_format("SpeedHack - %d:%d:%d || %s is Warp to Last Position. Range = %d, Speed = %d\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), (int)nRange, (int)nSpeed));

		if (m_SpeedHackTrial >= 10)
			m_SpeedHackTrial = 0;
	}
	else
	{
		m_SpeedHackTrial = 0;
		m_LastX = GetX();
		m_LastZ = GetZ();
	}

#if 0 // temporarily disabled
	uint8 b_first;
	float servertime = 0.0f, clienttime = 0.0f, client_gap = 0.0f, server_gap = 0.0f;

	pkt >> b_first >> clienttime;

	if (b_first) {
		m_fSpeedHackClientTime = clienttime;
		m_fSpeedHackServerTime = TimeGet();
	}
	else {
		servertime = TimeGet();

		server_gap = servertime - m_fSpeedHackServerTime;
		client_gap = clienttime - m_fSpeedHackClientTime;

		if (client_gap - server_gap > 10.0f) {
			TRACE("%s SpeedHack User Checked By Server Time\n", m_id);
			Close();
		}
		else if (client_gap - server_gap < 0.0f) {
			m_fSpeedHackClientTime = clienttime;
			m_fSpeedHackServerTime = TimeGet();
		}
	}
#endif
}
#pragma endregion

#pragma region CUser::ProgramCheckProcess(Packet & pkt)
void CUser::ProgramCheckProcess(Packet& pkt)
{
	uint8 sOpCode;
	std::string sStrUserID, sProgramInfo;
	pkt >> sOpCode;

	if (sOpCode == 1)
	{
		if (!isGM())
			return;

		pkt >> sStrUserID;
		g_pMain->sGameMasterSocketID = -1;

		CUser* pUser = g_pMain->GetUserPtr(sStrUserID, TYPE_CHARACTER);

		if (pUser == nullptr
			|| !pUser->isInGame())
			return;

		g_pMain->sGameMasterSocketID = GetSocketID();
		Packet result(WIZ_PROGRAMCHECK, uint8(0x02));
		result << uint8(0x01);
		pUser->Send(&result);
	}
	else
	{
		pkt >> sProgramInfo;
		CUser* pUser = g_pMain->GetUserPtr(g_pMain->sGameMasterSocketID);

		if (pUser == nullptr
			|| !pUser->isInGame())
			return;

		g_pMain->SendHelpDescription(pUser, sProgramInfo.c_str());
	}
}
#pragma endregion

