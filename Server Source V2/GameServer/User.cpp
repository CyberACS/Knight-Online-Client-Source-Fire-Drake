#include "stdafx.h"
#include "Map.h"
#include "KnightsManager.h"
#include "KingSystem.h"
#include "MagicInstance.h"
#include "DBAgent.h"
#include <algorithm>
#include "../shared/DateTime.h"
#include <boost\foreach.hpp>	
#include <sstream>

using namespace std;

#pragma region Constructor
/**
* @brief The constructor.
*/
CUser::CUser(uint16 socketID, SocketMgr* mgr) : KOSocket(socketID, mgr, -1, 16384, 3172), Unit(UnitPlayer) {}

#pragma endregion

#pragma region CUser::OnConnect()
/**
* @brief	Executes the connect action.
*/
void CUser::OnConnect()
{
	KOSocket::OnConnect();
	Initialize();
}
#pragma endregion

#pragma region CUser::Initialize()
/**
* @brief	Initializes this object.
*/
void CUser::Initialize()
{
	m_sInDeathMatchEvent = false;
	Unit::Initialize();
	m_bCharacterDataLoaded = false;
	m_strUserID.clear();
	m_strMemo.clear();
	m_strAccountID.clear();
	m_bLogout = 0;

	//transformation attack
	attacknpc = false;

	m_lastPetSatisfaction = m_AutoMiningFishingSystem = m_OfflineSystemTime = m_tPPCardTimeNormal = UNIXTIME;
	m_PettingOn = nullptr;

	m_bHasAlterOptained = false;

	m_SkillCastList.clear();
	m_tLastSurroundingUpdate = 0;
	m_iLastExchangeTime = 0;

	m_bKnightsReq = 0;

	m_iTotalTrainingExp = 0;
	m_lastTrainingTime = 0;
	m_iTotalTrainingTime = 0;
	m_MaxSp = 100;
	m_FlashExpBonus = m_FlashDcBonus = m_FlashWarBonus = 0;
	CyberACS_LASTSUPPORT = CyberACS_LASTCHECK = m_tGenieTimeNormal = UNIXTIME;
	CyberACS_TICKCOUNT = 0;

	m_GenieTime = 0;
	m_sFirstUsingGenie = 0;
	memset(m_GenieOptions, 0, sizeof(m_GenieOptions));

	m_tLastChatUseTime = UNIXTIME2;
	m_tLastType4SkillCheck = 0;

	m_bOfflineSystemType = false;
	m_WantedSelectingUser = false;
	m_WantedRunTime = m_sWantedKillerCount = m_sWantedTotalHit = m_sWantedTotalAc = m_sWantedCount = 0;

	m_TowerOwnerID = -1;

	// Achiement Related Initialization
	m_sCoverID = 0;
	m_sSkillID = 0;
	m_sCoverTitle = 0;
	m_sSkillTitle = 0;

	m_bAuthority = 1;
	m_sBind = -1;
	m_ChatRoomIndex = -1;
	m_state = GAME_STATE_CONNECTED;

	m_bSelectedCharacter = false;
	m_bStoreOpen = false;
	m_bPartyLeader = false;
	m_bIsChicken = false;
	m_bMining = false;
	m_bFishing = false;
	m_bPremiumMerchant = false;
	m_bInParty = false;
	m_bInEnterParty = false;

	m_tLastMiningAttempt = 0;
	m_tLastFishingAttempt = 0;

	m_bMerchantState = MERCHANT_STATE_NONE;
	m_bSellingMerchantPreparing = false;
	m_bBuyingMerchantPreparing = false;
	m_bMerchantOpen = false;
	m_bInvisibilityType = INVIS_NONE;

	m_sDirection = 0;

	m_sItemMaxHp = m_sItemMaxMp = 0;
	m_sItemWeight = 0;
	m_sItemAc = 0;
	m_SpeedHackTrial = 0;
	m_sExpGainAmount = m_bNPGainAmount = m_bNoahGainAmount = 100;
	m_bItemExpGainAmount = m_bItemNoahGainAmount = 0;
	m_bItemNPBonus = m_bSkillNPBonus = 0;

	m_byAPBonusAmount = 0;
	memset(&m_byAPClassBonusAmount, 0, sizeof(m_byAPClassBonusAmount));
	memset(&m_byAcClassBonusAmount, 0, sizeof(m_byAcClassBonusAmount));
	memset(&m_bStats, 0, sizeof(m_bStats));
	memset(&m_sStatItemBonuses, 0, sizeof(m_sStatItemBonuses));
	memset(&m_sStatAchieveBonuses, 0, sizeof(m_sStatAchieveBonuses));
	memset(&m_bStatBuffs, 0, sizeof(m_bStatBuffs));
	memset(&m_bRebStats, 0, sizeof(m_bRebStats));
	memset(&m_bstrSkill, 0, sizeof(m_bstrSkill));
	memset(&m_arMerchantItems, 0, sizeof(m_arMerchantItems));
	AchieveChalengeCount = false;

	m_bAddWeaponDamage = 0;
	m_bPctArmourAc = 100;
	m_sAddArmourAc = 0;

	m_sItemHitrate = 100;
	m_sItemEvasionrate = 100;
	m_bPlayerAttackAmount = 100;
	m_sSpeed = 0;

	m_bAuthority = AUTHORITY_PLAYER;
	m_bLevel = 1;
	m_bRebirthLevel = 0;
	m_iExp = 0;
	m_iBank = m_iGold = 0;
	m_iLoyalty = m_iLoyaltyMonthly = 0;
	m_iMannerPoint = 0;
	m_sHp = m_sMp = m_sSp = 0;

	m_MaxHp = 0;
	m_MaxMp = 1;
	m_MaxSp = 0;
	m_iMaxExp = 0;
	m_sMaxWeight = 0;
	m_sMaxWeightBonus = 0;

	m_sUserUpgradeCount = 0;

	m_bResHpType = USER_STANDING;
	m_bWarp = false;

	m_sMerchantsSocketID = -1;
	m_sChallengeUser = -1;
	m_sPartyIndex = -1;
	m_bRequestingChallenge = 0;
	m_bChallengeRequested = 0;

	m_sExchangeUser = -1;
	m_isRequestSender = false;
	m_bExchangeOK = 0x00;
	m_ExchangeItemList.clear();

	m_bBlockPrivateChat = false;
	m_sPrivateChatUser.clear();
	m_bNeedParty = 0x01;

	m_tHPLastTimeNormal = 0;		// For Automatic HP recovery. 
	m_tHPStartTimeNormal = 0;

	m_tSpLastTimeNormal = 0;		// For Automatic SP recovery. 
	m_tSpStartTimeNormal = 0;
	m_bSpIntervalNormal = 3;

	m_bHPAmountNormal = 0;
	m_bHPDurationNormal = 0;
	m_bHPIntervalNormal = 5;

	m_tGameStartTimeSavedMagic = 0;

	m_fSpeedHackClientTime = 0;
	m_fSpeedHackServerTime = 0;
	m_bSpeedHackCheck = 0;

	m_tBlinkExpiryTime = 0;

	m_tLastArrowUse = 0;

	m_iTimePunishment = 0;

	m_iEventJoinOrder = 0;
	m_sJoinedEvent = -1;

	m_KnightRoyaleJoinRequest = 0;
	m_KnightRoyaleJoinEvent = -1;

	m_isRoyalPoison = false;
	m_RoyaleForUserRegeneStartedCount = 0;
	m_RoyaleForUserRegeneRemainingCount = 0;
	m_RoyaleForUserKillRewarCount = 0;
	m_RoyaleForUserRankRewarCount = 0;

	m_bAbnormalType = ABNORMAL_NORMAL;	// User starts out in normal size.
	m_nOldAbnormalType = m_bAbnormalType;

	m_sWhoKilledMe = -1;
	m_iLostExp = 0;

	m_questMap.DeleteAllData();
	m_PremiumMap.DeleteAllData();
	m_AchieveInfo.Initialize();
	m_AchieveMap.DeleteAllData();
	m_AchieveTimedMap.DeleteAllData();

	ReturnSymbolisOK = 0;
	ReturnisLogOutTime = 0;
	ReturnSymbolTime = 0;

	m_sUserPartyType = 0;
	m_bPremiumInUse = m_bClanPremiumInUse = 0;
	m_iSealedExp = 0;
	bExpSealStatus = 0;
	m_iDrakiTime = 0;
	m_bDrakiStage = 0;
	m_bDrakiEnteranceLimit = 0;

	KnightRoyaleStatiMaxMp = false;
	KnightRoyaleStatiMaxHp = false;

	m_tLastTrapAreaTime = 0;
	memset(m_iSelMsgEvent, -1, MAX_MESSAGE_EVENT);

	m_sEventNid = m_sEventSid = -1;
	m_nQuestHelperID = 0;
	m_bZoneChangeFlag = false;
	m_bRegeneType = 0;
	m_tLastRegeneTime = 0;
	m_bZoneChangeSameZone = false;
	m_bZoneChangeControl = false;
	m_bSuperiorCapeFlag = false;

	m_sInEvent = 0;
	m_sInRoom = 0;





	m_pKnightsUser = nullptr;
	m_sRivalID = -1;
	m_tRivalExpiryTime = 0;
	m_byAngerGauge = 0;
	m_bWeaponsDisabled = false;
	m_teamColour = TeamColourNone;

	m_BorderDefenceWarUserPoint = 0;
	m_ChaosExpansionKillCount = 0;
	m_ChaosExpansionDeadCount = 0;
	m_PlayerKillingLoyaltyDaily = 0;
	m_PlayerKillingLoyaltyPremiumBonus = 0;

	m_bGenieStatus = false;
	m_bAccountStatus = 0;
	m_bPremiumType = 0;
	m_sPremiumTime = 0;

	m_GameMastersMute = 0;
	m_GameMastersUnMute = 0;
	m_GameMastersUnBan = 0;
	m_GameMastersBanPermit = 0;
	m_GameMastersAllowAttack = 0;
	m_GameMastersDisabledAttack = 0;
	m_GameMastersNpAdd = 0;
	m_GameMastersExpAdd = 0;
	m_GameMastersMoneyAdd = 0;
	m_GameMastersLoyaltyChange = 0;
	m_GameMastersExpChange = 0;
	m_GameMastersMoneyChange = 0;
	m_GameMastersGiveItem = 0;
	m_GameMastersGiveItemSelf = 0;
	m_GameMastersSummonUser = 0;
	m_GameMastersTpOnUser = 0;
	m_GameMastersZoneChange = 0;
	m_GameMastersGmTools = 0;
	m_GameMastersLocationChange = 0;
	m_GameMastersMonsterSummon = 0;
	m_GameMastersNpcSummon = 0;
	m_GameMastersMonKilled = 0;
	m_GameMastersTeleportAllUser = 0;
	m_GameMastersClanSummon = 0;
	m_GameMastersResetPlayerRanking = 0;
	m_GameMastersChangeEventRoom = 0;
	m_GameMastersWarOpen1 = 0;
	m_GameMastersWarOpen2 = 0;
	m_GameMastersWarOpen3 = 0;
	m_GameMastersWarOpen4 = 0;
	m_GameMastersWarOpen5 = 0;
	m_GameMastersWarOpen6 = 0;
	m_GameMastersWarOpen9 = 0;
	m_GameMastersWarSiegeOpen = 0;
	m_GameMastersWarClose = 0;
	m_GameMastersCaptainElection = 0;
	m_GameMastersCastleSiegeWarClose = 0;
	m_GameMastersSendPrsion = 0;

	m_lootSettings.iWarrior = 1;
	m_lootSettings.iRogue = 1;
	m_lootSettings.iMage = 1;
	m_lootSettings.iPriest = 1;
	m_lootSettings.iWeapon = 1;
	m_lootSettings.iArmor = 1;
	m_lootSettings.iAccessory = 1;
	m_lootSettings.iNormal = 1;
	m_lootSettings.iUpgrade = 1;
	m_lootSettings.iCraft = 1;
	m_lootSettings.iRare = 1;
	m_lootSettings.iMagic = 1;
	m_lootSettings.iUnique = 1;
	m_lootSettings.iConsumable = 1;
	m_lootSettings.iPrice = 0;

	m_LastX = 0;
	m_LastZ = 0;
	m_bIsLoggingOut = false;
	m_bWaitingOrder = false;
	m_tOrderRemain = UNIXTIME + 5;

	Camerazoom = 0;

	//Bowl Event MeLanor

	BowlStatus = false;
	bowlX = 0;
	bowlz = 0;

	memset(&m_CyberACSCollectionRaceEventMonsterID, 0, sizeof(m_CyberACSCollectionRaceEventMonsterID));
	memset(&m_CyberACSCollectionRaceEventMonsterKillCount, 0, sizeof(m_CyberACSCollectionRaceEventMonsterKillCount));
	memset(&m_CyberACSCollectionRaceEventCount, 0, sizeof(m_CyberACSCollectionRaceEventCount));
	memset(&m_CyberACSCollectionRaceEventZone, 0, sizeof(m_CyberACSCollectionRaceEventZone));

	for (int i = 0; i < 4; i++)
		m_CyberACSCollectionRaceEventMonsterName[i].clear();

	//



	memset(&m_DailyEventMonsterKillCount, 0, sizeof(m_DailyEventMonsterKillCount));
	//memset(&m_DailyEventCount, 0, sizeof(m_DailyEventCount));



	//vs event
	VsKillMiktari = 0;
	YourKill = 0;
	//vs event

	m_bSlaveMerchant = false;
	m_bSlaveUserID = -1;
	SlaveTotalTime = 0;

	m_bUserPriestBotID = -1;
	hasPriestBot = false;

	//skill sıfırlama için kontrol
	m_bSkillStatResetHook = 0;
}
#pragma endregion

#pragma region CUser::OnDisconnect()
/**
* @brief	Executes the disconnect action.
*/
void CUser::OnDisconnect()
{
	KOSocket::OnDisconnect();

	g_pMain->RemoveSessionNames(this);

	if (isInGame())
	{
		HandleSurroundingUserRegionUpdate();
		PetOnDeath();
		UserInOut(INOUT_OUT);

		if (m_VsDurumu == false && GetZoneID() == ZONE_RONARK_LAND && GetEventRoom() > 1)
		{

			CUser* VsEventUser = g_pMain->GetUserPtr(VsYapilacakKullanici, TYPE_CHARACTER);

			if (VsEventUser == nullptr)
				Disconnect();
			else if (VsEventUser->GetEventRoom() != GetEventRoom())
			{
				Disconnect();
				VsEventUser->Disconnect();
			}
			else
			{
				g_DBAgent.UpdateVSEventLog(VsEventUser->GetName(), GetName(), VsMiktari);

				short Kaybeden = 0;
				Kaybeden -= VsMiktari;

				VsEventUser->m_bEventRoom = 0;
				VsEventUser->SendLoyaltyChange(VsMiktari, false);
				VsEventUser->VsMiktari = 0;
				VsEventUser->m_VsDurumu = true;
				VsEventUser->ZoneChange(21, 0.0f, 0.0f);
				VsEventUser->VsYapilacakKullanici.clear();

				m_bEventRoom = 0;
				SendLoyaltyChange(Kaybeden, false, false, false);
				VsMiktari = 0;
				m_VsDurumu = true;
				ZoneChange(21, 0.0f, 0.0f);
				VsYapilacakKullanici.clear();

				g_pMain->LogosYolla("[1vs1 Manager]", string_format("%s vs %s Karsilasmasinin Galibi : %s Tebrikler!", GetName().c_str(), VsEventUser->GetName().c_str(), VsEventUser->GetName().c_str()), 255, 156, 3);

			}
		}

		if (isInParty())
		{
			if (isPartyLeader())
			{
				_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());
				if (pParty == nullptr)
					return;

				PartyLeaderPromote(pParty->uid[1]);
			}
		}

		PartyNemberRemove(GetSocketID());

		if (isInClan())
		{
			CKnights* pKnights = g_pMain->GetClanPtr(GetClanID());
			if (pKnights != nullptr)
			{
				if (isInClan())
					pKnights->OnLogout(this);

				if (pKnights->isInAlliance())
					pKnights->OnLogoutAlliance(this);
			}
		}

		if (isSlaveMerchant())
		{
			CBot* pSlaveUser = g_pMain->m_BotArray.GetData(m_bSlaveUserID);
			if (pSlaveUser != nullptr) {
				GiveSlaveMerchantItems();
				pSlaveUser->UserInOut(INOUT_OUT);
			}

			m_bSlaveMerchant = false;
			m_bSlaveUserID = -1;
		}
		if (isMeChatRoom(m_ChatRoomIndex))
			ChatRoomKickOut(GetSocketID());

		ResetWindows();

		if (hasRival())
			RemoveRival();
	}
	LogOut();
}
#pragma endregion

#pragma region CUser::HandlePacket(Packet & pkt)

/**
* @brief	Handles an incoming user packet.
*
* @param	pkt	The packet.
*
* @return	true if it succeeds, false if it fails.
*/
bool CUser::HandlePacket(Packet& pkt)
{
	/*uint8 command = pkt.GetOpcode();
	//TRACE("[SID=%d] Packet: %X (len=%d)\n", GetSocketID(), command, pkt.size());
	// If we're not authed yet, forced us to before we can do anything else.
	// NOTE: We're checking the account ID store here because it's only set on successful auth,
	// at which time the other account ID will be cleared out (yes, it's messy -- need to clean it up).
	if (m_strAccountID.empty())
	{
		if (command == WIZ_VERSION_CHECK)
			VersionCheck(pkt);

		if (command == WIZ_LOGIN)
			LoginProcess(pkt);

		if (g_pMain->m_HackToolListArray.GetSize() > 0)
		{
			Packet result(Wiz_CyberAntiCheat_GUARD, uint8(AchieveSummary));
			result << (uint8)g_pMain->m_HackToolListArray.GetSize();

			foreach_stlmap(itr, g_pMain->m_HackToolListArray)
				result << itr->second->nChecking;

			Send(&result);
		}
		return true;
	}*/

	uint8 command = pkt.GetOpcode();

	TRACE("[SID=%d] Packet: %X (len=%d)\n", GetSocketID(), command, pkt.size());

	// If crypto's not been enabled yet, force the version packet to be sent.
	if (command == WIZ_VERSION_CHECK)
	{
		if (command == WIZ_VERSION_CHECK)
		{
			VersionCheck(pkt);
			return true;
		}
		else
		{
			printf("[SID=%d] Unhandled packet (%X) prior to version check\n", GetSocketID(), command);
		}

		return true;
	}
	// If we're not authed yet, forced us to before we can do anything else.
	// NOTE: We're checking the account ID store here because it's only set on successful auth,
	// at which time the other account ID will be cleared out (yes, it's messy -- need to clean it up).
	else if (m_strAccountID.empty())
	{
		if (command == WIZ_LOGIN)
		{
			LoginProcess(pkt);
			return true;
		}
		else if (command == PL_COM)
		{
			return true;
		}
		else if (command == PL_ALIVE)
		{
			CyberACS_StayAlive(pkt);
		}
		else
		{
			printf("[SID=%d] Unhandled packet (%X) prior to account login\n", GetSocketID(), command);
		}

		return true;
	}

	// If we haven't logged in yet, don't let us hit in-game packets.
	// TODO: Make sure we support all packets in the loading stage (and rewrite this logic considerably better).
	else if (!m_bSelectedCharacter)
	{
		switch (command)
		{
		case PL_ALIVE:
			CyberACS_StayAlive(pkt);
			break;
		case PL_AUTHINFO:
			CyberACS_AuthInfo(pkt);
			break;
		case PL_COM:
			CyberACS_General(pkt);
			break;
		case WIZ_SEL_NATION:
			SelNationToAgent(pkt);
			break;
		case WIZ_LOADING_LOGIN:
			LoadingLogin(pkt);
			break;
		case WIZ_ALLCHAR_INFO_REQ:
			AllCharInfo(pkt);
			break;
		case WIZ_CHANGE_HAIR:
			ChangeHair(pkt);
			break;
		case WIZ_NEW_CHAR:
			NewCharToAgent(pkt);
			break;
		case WIZ_DEL_CHAR:
			DelCharToAgent(pkt);
			break;
		case WIZ_SEL_CHAR:
			SelCharToAgent(pkt);
			break;
		case WIZ_SPEEDHACK_CHECK:
			SpeedHackTime(pkt);
			break;
		case WIZ_NAME_CHANGE:
			HandleNameChange(pkt);
			break;
		case WIZ_MASTER_SECURITY:
			SehzadeGuard(pkt);
			break;
			/*case WIZ_CYBERANTICHEAT_GUARD:
				HandleCyberACS(pkt);
				break;*/
		default:
			printf("[SID=%d] Unhandled packet (%X) prior to selecting character\n", GetSocketID(), command);
			break;
		}
		return true;
	}
	if (!isInGame())
	{
		switch (command)
		{
		case PL_ALIVE:
			CyberACS_StayAlive(pkt);
			break;
		case PL_USERDATA:
			CyberACS_ReqUserData(pkt);
			break;
		case PL_SUPPORT:
			CyberACS_Support(pkt);
			break;
		case PL_MERCHANTLIST:
			CyberACS_ReqMerchantList(pkt);
			break;
		case PL_CHAOTIC_EXCHANGE:
			CyberACS_ChaoticExchange(pkt);
			break;
		case PL_USERINFO:
			CyberACS_ReqUserInfo(pkt);
			break;
		case PL_CR:
			break;
		case PL_AUTHINFO:
			CyberACS_AuthInfo(pkt);
			break;
		case PL_PROCINFO:
			//Wiz_CYBERANTICHEAT_ProcInfo(pkt);
			break;
		case PL_LOG:
			CyberACS_LogHandler(pkt);
			break;
		case PL_COM:
			CyberACS_General(pkt);
			break;
		case PL_GMTOOLS:
			GmTools(pkt);
			break;
		case PL_LIFESKILL:
			MannerStore(pkt);
			break;
		case PL_LOOT_SETTINS:
			ReqSaveLootSettings(pkt);
			break;
		case PL_CLANBANK:
			CYBACSClanWarehouseProcess(pkt);
			break;
		case PL_DROP_REQUEST:
			CyberACS_DropRequest(pkt);
			break;
		case PL_PUS:
			CyberACS_PowerUpStoreRequest(pkt);
			break;
		case PL_UIINFO:
			CyberACS_UIRequest(pkt);
			break;
		case WIZ_GAMESTART:
			GameStart(pkt);
			break;
		case WIZ_ALLCHAR_INFO_REQ:
			AllCharInfo(pkt);
			break;
		case WIZ_SERVER_INDEX:
			SendServerIndex();
			break;
		case WIZ_RENTAL:
			RentalSystem(pkt);
			break;
		case WIZ_SKILLDATA:
			SkillDataProcess(pkt);
			break;
		case WIZ_REGENE:
			Regene(pkt.read<uint8>()); // respawn type
			break;
		case WIZ_REQ_USERIN:
			RequestUserIn(pkt);
			break;
		case WIZ_REQ_NPCIN:
			RequestNpcIn(pkt);
			break;
		case WIZ_ZONE_CHANGE:
			RecvZoneChange(pkt);
			break;
		case WIZ_STATE_CHANGE:
			StateChange(pkt);
			break;
		case WIZ_QUEST:
			QuestV2PacketProcess(pkt);
			break;
		case WIZ_MAP_EVENT:
			RecvMapEvent(pkt);
			break;
		case WIZ_MAGIC_PROCESS:
			CMagicProcess::MagicPacket(pkt, this);
			break;
		case WIZ_OBJECT_EVENT:
			ObjectEvent(pkt);
			break;
		case WIZ_WEATHER:
		case WIZ_TIME:
			UpdateGameWeather(pkt);
			break;
		case WIZ_CONCURRENTUSER:
			CountConcurrentUser();
			break;
		case WIZ_DATASAVE:
			UserDataSaveToAgent();
			break;
		case WIZ_KNIGHTS_PROCESS:
			CKnightsManager::PacketProcess(this, pkt);
			break;
		case WIZ_SPEEDHACK_CHECK:
			SpeedHackTime(pkt);
			break;
		case WIZ_FRIEND_PROCESS:
			FriendProcess(pkt);
			break;
		case WIZ_WARP_LIST:
			SelectWarpList(pkt);
			break;
		case WIZ_VIRTUAL_SERVER:
			ServerChangeOk(pkt);
			break;
		case WIZ_CLIENT_EVENT:
			ClientEvent(pkt.read<uint16>());
			break;
		case WIZ_SELECT_MSG:
			RecvSelectMsg(pkt);
			break;
		case WIZ_EVENT:
			TempleProcess(pkt);
			break;
		case WIZ_SHOPPING_MALL: // letter system's used in here too
			ShoppingMall(pkt);
			break;
		case WIZ_HELMET:
			HandleHelmet(pkt);
			break;
		case WIZ_USER_INFORMATIN:
			UserRegionChat(pkt);
			break;
		case WIZ_GENIE:
			HandleGenie(pkt);
			break;
		case WIZ_LOGOSSHOUT:
			LogosShout(pkt);
			break;
		case WIZ_CHANGE_HAIR:
			ChangeHair(pkt);
			break;
		case WIZ_USER_ACHIEVE:
			HandleUserAchieve(pkt);
			break;
		case WIZ_EXP_SEAL:
			ExpSealHandler(pkt);
			break;
		case WIZ_HACKTOOL:
			HandleHackTool(pkt);
			break;
		case WIZ_PREMIUM:
			HandlePremium(pkt);
			break;
		case WIZ_MASTER_SECURITY:
			SehzadeGuard(pkt);
			break;

			/*case WIZ_CYBERANTICHEAT_GUARD:
				HandleCyberACS(pkt);
				break;*/
		default:
			printf("!isInGame [SID=%d] Unknown packet %X\n", GetSocketID(), command);
			break;
		}
	}
	else if (isInGame())
	{
		switch (command)
		{
		case PL_VS_EVENT:
			VsEvent(pkt);
			break;
		case PL_DEATH_NOTICE:
			DeathNotice(pkt);
			break;
		case PL_USER_TOOLS:
			UserTools(pkt);
			break;
		case PL_GENERALRANK:
			Ranks(pkt);
			break;
		case PL_2NDPASSWORD:
			Password(pkt);
			break;
		case PL_LOTTERY:
			LotteryEvent(pkt);
			break;
		case PL_DAILY:
			DailyQuest(pkt);
			break;
		case PL_SLAVE_MERC:
			SlaveMerc(pkt);
			break;
		case PL_JOBCHANGEDM:
			JobChangev2(pkt);
			break;
		case PL_JOBCHANGE:
			JobChangev2(pkt);
			break;
		case 0xB3:
			break;
		case PL_AUTHINFO:
			CyberACS_AuthInfo(pkt);
			break;
		case PL_ALIVE:
			CyberACS_StayAlive(pkt);
			break;
		case PL_VOICE:
			CyberACS_VoiceHandler(pkt);
			break;
		case PL_EVENT:
			TempleProcess(pkt);
			break;
		case PL_MONSTER_INFO:
			CyberACSMonsterInfoHandler(pkt);
			break;
		case PL_USERDATA:
			CyberACS_ReqUserData(pkt);
			break;
		case PL_SUPPORT:
			CyberACS_Support(pkt);
			break;
		case PL_MERCHANTLIST:
			CyberACS_ReqMerchantList(pkt);
			break;
		case PL_CHAOTIC_EXCHANGE:
			CyberACS_ChaoticExchange(pkt);
			break;
		case PL_USERINFO:
			CyberACS_ReqUserInfo(pkt);
			break;
		case PL_CR:
			break;
		case PL_PROCINFO:
			//Wiz_CyberAntiCheat_ProcInfo(pkt);
			break;
		case PL_LOG:
			CyberACS_LogHandler(pkt);
			break;
		case PL_COM:
			CyberACS_General(pkt);
			break;
		case PL_GMTOOLS:
			GmTools(pkt);
			break;
		case PL_LIFESKILL:
			MannerStore(pkt);
			break;
		case PL_LOOT_SETTINS:
			ReqSaveLootSettings(pkt);
			break;
		case PL_CLANBANK:
			CYBACSClanWarehouseProcess(pkt);
			break;
		case PL_DROP_REQUEST:
			CyberACS_DropRequest(pkt);
			break;
		case PL_MERCHANT:
			CyberACS_MerchantHandler(pkt);
			break;
		case PL_RESET:
			CyberACS_AllSkillStatRestard(pkt);
			break;
		case PL_PUS:
			CyberACS_PowerUpStoreRequest(pkt);
			break;
		case PL_UIINFO:
			CyberACS_UIRequest(pkt);
			break;
		case WIZ_ALLCHAR_INFO_REQ:
			AllCharInfo(pkt);
			break;
		case WIZ_GAMESTART:
			GameStart(pkt);
			break;
		case WIZ_SERVER_INDEX:
			SendServerIndex();
			break;
		case WIZ_RENTAL:
			RentalSystem(pkt);
			break;
		case WIZ_SKILLDATA:
			SkillDataProcess(pkt);
			break;
		case WIZ_MOVE:
			MoveProcess(pkt);
			break;
		case WIZ_ROTATE:
			Rotate(pkt);
			break;
		case WIZ_ATTACK:
			Attack(pkt);
			break;
		case WIZ_CHAT:
			Chat(pkt);
			break;
		case WIZ_CHAT_TARGET:
			ChatTargetSelect(pkt);
			break;
		case WIZ_REGENE:
			Regene(pkt.read<uint8>()); // respawn type
			break;
		case WIZ_REQ_USERIN:
			RequestUserIn(pkt);
			break;
		case WIZ_REQ_NPCIN:
			RequestNpcIn(pkt);
			break;
		case WIZ_WARP:
			RecvWarp(pkt);
			break;
		case WIZ_ITEM_MOVE:
			ItemMove(pkt);
			break;
		case WIZ_NATION_CHAT:
			ChatRoomHandle(pkt);
			break;
		case WIZ_NPC_EVENT:
			NpcEvent(pkt);
			break;
		case WIZ_ITEM_TRADE:
			ItemTrade(pkt);
			break;
		case WIZ_TARGET_HP:
			HandleTargetHP(pkt);
			break;
		case WIZ_BUNDLE_OPEN_REQ:
			BundleOpenReq(pkt);
			break;
		case WIZ_ITEM_GET:
			ItemGet(pkt);
			break;
		case WIZ_ZONE_CHANGE:
			RecvZoneChange(pkt);
			break;
		case WIZ_POINT_CHANGE:
			PointChange(pkt);
			break;
		case WIZ_STATE_CHANGE:
			StateChange(pkt);
			break;
		case WIZ_PARTY:
			PartySystemProcess(pkt);
			break;
		case WIZ_EXCHANGE:
			ExchangeProcess(pkt);
			break;
		case WIZ_QUEST:
			QuestV2PacketProcess(pkt);
			break;
		case WIZ_MAP_EVENT:
			RecvMapEvent(pkt);
			break;
		case WIZ_MERCHANT:
			MerchantProcess(pkt);
			break;
		case WIZ_MAGIC_PROCESS:
			CMagicProcess::MagicPacket(pkt, this);
			break;
		case WIZ_SKILLPT_CHANGE:
			SkillPointChange(pkt);
			break;
		case WIZ_OBJECT_EVENT:
			ObjectEvent(pkt);
			break;
		case WIZ_WEATHER:
		case WIZ_TIME:
			UpdateGameWeather(pkt);
			break;
		case WIZ_CLASS_CHANGE:
			ClassChange(pkt);
			break;
		case WIZ_CONCURRENTUSER:
			CountConcurrentUser();
			break;
		case WIZ_DATASAVE:
			UserDataSaveToAgent();
			break;
		case WIZ_ITEM_REPAIR:
			ItemRepair(pkt);
			break;
		case WIZ_KNIGHTS_PROCESS:
			CKnightsManager::PacketProcess(this, pkt);
			break;
		case WIZ_ITEM_REMOVE:
			ItemRemove(pkt);
			break;
		case WIZ_OPERATOR:
			OperatorCommand(pkt);
			break;
		case WIZ_SPEEDHACK_CHECK:
			SpeedHackTime(pkt);
			break;
		case WIZ_WAREHOUSE:
			WarehouseSystem(pkt);
			break;
		case WIZ_CLANWAREHOUSE:
			CYBACSClanWarehouseProcess(pkt);
			break;
		case WIZ_HOME:
			Home();
			break;
		case WIZ_FRIEND_PROCESS:
			FriendProcess(pkt);
			break;
		case WIZ_WARP_LIST:
			SelectWarpList(pkt);
			break;
		case WIZ_VIRTUAL_SERVER:
			ServerChangeOk(pkt);
			break;
		case WIZ_PARTY_BBS:
			PartyBBS(pkt);
			break;
		case WIZ_CLIENT_EVENT:
			ClientEvent(pkt.read<uint16>());
			break;
		case WIZ_SELECT_MSG:
			RecvSelectMsg(pkt);
			break;
		case WIZ_ITEM_UPGRADE:
			ExchangeSystemProcess(pkt);
			break;
		case WIZ_EVENT:
			TempleProcess(pkt);
			break;
		case WIZ_SHOPPING_MALL: // letter system's used in here too
			ShoppingMall(pkt);
			break;
		case WIZ_NAME_CHANGE:
			HandleNameChange(pkt);
			break;
		case WIZ_KING:
			CKingSystem::PacketProcess(this, pkt);
			break;
		case WIZ_HELMET:
			HandleHelmet(pkt);
			break;
		case WIZ_CAPE:
			HandleCapeChange(pkt);
			break;
		case WIZ_CHALLENGE:
			HandleChallenge(pkt);
			break;
		case WIZ_RANK:
			HandlePlayerRankings(pkt);
			break;
		case WIZ_MINING:
			HandleMiningSystem(pkt);
			break;
		case WIZ_USER_INFORMATIN:
			UserRegionChat(pkt);
			break;
		case WIZ_GENIE:
			HandleGenie(pkt);
			break;
		case WIZ_LOGOSSHOUT:
			LogosShout(pkt);
			break;
		case WIZ_SIEGE:
			SiegeWarFareProcess(pkt);
			break;
		case WIZ_NATION_TRANSFER:
			HandleNationChange(pkt);
			break;
		case WIZ_GENDER_CHANGE:
			GenderChangeV2(pkt);
			break;
		case WIZ_CHANGE_HAIR:
			ChangeHair(pkt);
			break;
		case WIZ_VIPWAREHOUSE:
			VIPhouseProcess(pkt);
			break;
		case WIZ_REPORT:
			SheriffVote(pkt);
			break;
			/*case WIZ_CYBERANTICHEAT_GUARD:
				HandleCyberACS(pkt);
				break;*/
		case WIZ_MOVING_TOWER:
			HandleTowerPackets(pkt);
			break;
#if 0
		case WIZ_CAPTURE:
			HandleBDWCapture(pkt);
			break;
#endif
		case WIZ_USER_ACHIEVE:
			HandleUserAchieve(pkt);
			break;
		case WIZ_EXP_SEAL:
			ExpSealHandler(pkt);
			break;
		case WIZ_PREMIUM:
			HandlePremium(pkt);
			break;
		case WIZ_HACKTOOL:
			HandleHackTool(pkt);
			break;
		case WIZ_KNIGHT_ROYALE:
			KnightRoyaleProcess(pkt);
			break;
		case WIZ_PET:
			MainPetProcess(pkt);
			break;
		case WIZ_VANGUARD:
			VanGuardProcess(pkt);
			break;
		case WIZ_EDIT_BOX:
			PPCardProcess(pkt);
			break;
		case WIZ_MASTER_SECURITY:
			SehzadeGuard(pkt);
			break;
		case WIZ_PROGRAMCHECK:
			ProgramCheckProcess(pkt);
			break;
		default:
			printf("isInGame [SID=%d] Unknown packet %X\n", GetSocketID(), command);
			break;
		}
		Update();
		//CyberACS_Main();
		CyberACS_SendPartyProcess();
	}
	return true;
}
#pragma endregion
#pragma region CUser::Update()
/**
* @brief	Updates timed player data, e.g. skills & save requests.
*/
void CUser::Update()
{

	DateTime nSeconds(&g_localTime);


	if (!isInGame())
		return;

	EventBowlStatus();


	if (isMonsterTransformation() && attacknpc && m_npcattack <= UNIXTIME)
		attacknpc = false;

	if (m_tGameStartTimeSavedMagic != 0 && (UNIXTIME - m_tGameStartTimeSavedMagic) >= 2)
	{
		m_tGameStartTimeSavedMagic = 0;
		// Restore scrolls...
		InitType4();
		RecastSavedMagic();
	}

	if (!isBlinking())
	{
		if (m_tHPLastTimeNormal != 0
			&& (UNIXTIME - m_tHPLastTimeNormal) > m_bHPIntervalNormal)
			HPTimeChange();	// For Sitdown/Standup HP restoration.
	}

	if (isPortuKurian())
	{
		if (saniye != nSeconds.GetSecond())
			SpTimeChange();
	}

	saniye = nSeconds.GetSecond();

	if (m_bResHpType == USER_SITDOWN)
		TrainingProcess();

	if (m_tGenieTimeNormal + PLAYER_GENIE_INTERVAL < UNIXTIME && isGenieActive())
	{
		m_tGenieTimeNormal = UNIXTIME;
		UpdateGenieTime(--m_GenieTime);
	}

	// Handles DOT/HOT skills (not COLD skills though.)
	if (m_bType3Flag)
		HPTimeChangeType3();

	if (UNIXTIME2 - m_tLastType4SkillCheck > 100)
	{
		// Check for expired type 4 buffs
		Type4Duration();

		// Expire any timed out saved skills.
		CheckSavedMagic();
		m_tLastType4SkillCheck = UNIXTIME2;

		if (isTransformed())
			CMagicProcess::CheckExpiredType6Skills(this);

		// Check for expired type 9/visibility skills
		CMagicProcess::CheckExpiredType9Skills(this);
	}

	if (m_lastPetSatisfaction + (PLAYER_TRAINING_INTERVAL * 4) < UNIXTIME)
	{
		PetSatisFactionUpdate(-100);
		m_lastPetSatisfaction = UNIXTIME;
	}

	if (isBlinking())		// Should you stop blinking?
		BlinkTimeCheck();

	if (!isBlinking() && isTransformed() && m_bCanUseSkills == false)
		m_bCanUseSkills = true;

	if (hasRival() && hasRivalryExpired())
		RemoveRival();

	if ((UNIXTIME - m_lastSaveTime) >= PLAYER_SAVE_INTERVAL)
	{
		m_lastSaveTime = UNIXTIME; // this is set by UpdateUser(), however it may result in multiple requests unless it's set first.
		UserDataSaveToAgent();
	}

	for (int i = 0; i < WAREHOUSE_MAX; i++)
	{
		_ITEM_DATA* pItem = &m_sWarehouseArray[i];

		if (pItem == nullptr)
			continue;

		if (pItem->nExpirationTime < (uint32)UNIXTIME && pItem->nExpirationTime != 0)
			memset(pItem, 0, sizeof(_ITEM_DATA));
	}

	for (int i = 0; i < VIPWAREHOUSE_MAX; i++)
	{
		_ITEM_DATA* pItem = &m_sVIPWarehouseArray[i];

		if (pItem == nullptr)
			continue;

		if (pItem->nExpirationTime < (uint32)UNIXTIME && pItem->nExpirationTime != 0)
			memset(pItem, 0, sizeof(_ITEM_DATA));
	}

	for (int i = 0; i < INVENTORY_TOTAL; i++)
	{
		_ITEM_DATA* pItem = GetItem(i);

		if (pItem == nullptr)
			continue;

		if (pItem->nExpirationTime != 0 && pItem->nExpirationTime < (uint32)UNIXTIME)
		{
			if (i < SLOT_MAX)
				SendStackChange(0, 0, 0, i, true, 0, ITEM_SECTION_SLOT);
			else if (INVENTORY_INVENT <= i && i < INVENTORY_COSP)
				SendStackChange(0, 0, 0, i - INVENTORY_INVENT, true, 0, ITEM_SECTION_INVEN);
			else if (INVENTORY_COSP <= i && i < INVENTORY_MBAG)
			{
				if (i == CWING)
					SendStackChange(0, 0, 0, COSP_WINGS, true, 0, ITEM_SECTION_COSPRE);
				else if (i == CHELMET)
					SendStackChange(0, 0, 0, COSP_HELMET, true, 0, ITEM_SECTION_COSPRE);
				else if (i == CLEFT)
					SendStackChange(0, 0, 0, COSP_GLOVE, true, 0, ITEM_SECTION_COSPRE);
				else if (i == CRIGHT)
					SendStackChange(0, 0, 0, COSP_GLOVE2, true, 0, ITEM_SECTION_COSPRE);
				else if (i == CTOP)
					SendStackChange(0, 0, 0, COSP_BREAST, true, 0, ITEM_SECTION_COSPRE);
				else if (i == CFAIRY)
					SendStackChange(0, 0, 0, COSP_FAIRY, true, 0, ITEM_SECTION_COSPRE);
				else if (i == BAG1)
					SendStackChange(0, 0, 0, COSP_BAG1, true, 0, ITEM_SECTION_COSPRE);
				else if (i == BAG2)
					SendStackChange(0, 0, 0, COSP_BAG2, true, 0, ITEM_SECTION_COSPRE);
				else if (i == CTATTOO)
					SendStackChange(0, 0, 0, COSP_TATTO, true, 0, ITEM_SECTION_COSPRE);
			}
			else
				SendStackChange(0, 0, 0, i - INVENTORY_MBAG, true, 0, ITEM_SECTION_MBAG);

			memset(pItem, 0, sizeof(_ITEM_DATA));
		}
	}

	if (m_bVIPStorageVaultExpiration != 0 && UNIXTIME >= m_bVIPStorageVaultExpiration)
	{
		m_bVIPStorageVaultExpiration = 0;

		/*Packet result(WIZ_VIPWAREHOUSE);
		result << uint8(7)
			<< uint8(1);
		Send(&result);*/
	}

	// Start the 10 Level Skill
	if (GetLevel() == 10
		&& CheckExistEvent(71, 0))
		SaveEvent(71, 1);

	bool anyExpiration = false;
	std::set<uint8> deleted;
	deleted.clear();
	foreach_stlmap(itr, m_PremiumMap)
	{
		_PREMIUM_DATA* pPremium = itr->second;
		if (pPremium == nullptr
			|| pPremium->iPremiumTime == 0)
			continue;

		if (pPremium->iPremiumTime < UNIXTIME)
		{
			anyExpiration = true;
			pPremium->iPremiumTime = 0;
			deleted.insert(pPremium->bPremiumType);

			if (pPremium->bPremiumType == m_bPremiumInUse)
				m_bPremiumInUse = NO_PREMIUM;
		}
	}

	if (anyExpiration)
	{
		foreach(itr, deleted)
		{
			m_PremiumMap.DeleteData(*itr);
		}

		SendPremiumInfo();
	}

	if (ReturnSymbolisOK > 0)
	{
		if (ReturnSymbolTime < int64(UNIXTIME))
		{
			ReturnSymbolTime = 0;
			ReturnSymbolisOK = 0;
			//KnightReturnLetterItemDeleted();
		}
	}

	if (GetZoneID() == ZONE_DRAKI_TOWER)
		DrakiTowerKickTimer();

	if (GetZoneID() == ZONE_KNIGHT_ROYALE)
		KnightRoyaleGasinDamage();

	if (isInPKZone() && isWantedUser())
		WantedEventUserisMove();

	//Automatic Mining Fishing System





	/*if (m_AutoMiningFishingSystem + (PLAYER_TRAINING_INTERVAL - 13) < UNIXTIME)
	{
		uint32 nSeconds = g_localTime.tm_sec;
		uint32 sayi = myrand(0, 60);

		if (GetItem(SHOULDER)->nNum == AUTOMATIC_MINING
			|| GetItem(SHOULDER)->nNum == AUTOMATIC_DROP_MINING)
		{
			uint32 bRandSlot = myrand(0, 9999);
			uint32 nItemID = 0;
			nItemID = g_pMain->m_ItemMiningDropListArrayNormal[bRandSlot];

			if (nItemID == ITEM_EXP)
				ExpChange(GetLevel() * myrand(0, 40) * 2);
			else if (nItemID > 0)
				GiveWerehouseItem(nItemID, 1, true, false);
		}

		if (GetItem(SHOULDER)->nNum == AUTOMATIC_FISHING
			|| GetItem(SHOULDER)->nNum == AUTOMATIC_DROP_FISHING)
		{
			uint32 bRandSlot = myrand(0, 9999);
			uint32 nItemID = 0;
			nItemID = g_pMain->m_ItemFishingDropListArrayNormal[bRandSlot];

			if (nItemID == ITEM_EXP && nSeconds == sayi)
				ExpChange(GetLevel() * myrand(0, 40) * 2);
			else if (nItemID > 0 && nSeconds == sayi)
				GiveWerehouseItem(nItemID, 1, false, true);
		}
		m_AutoMiningFishingSystem = UNIXTIME;
	}*/

	//Automatic Mining Fishing System
	if (m_AutoMiningFishingSystem + (30 - 10) < UNIXTIME)
	{
		std::vector<uint32> pFishingList;
		std::vector<uint32> pMiningList;
		_MINING_FISHING_ITEM* pMining = nullptr;
		_MINING_FISHING_ITEM* pFishing = nullptr;

		if (GetItem(SHOULDER)->nNum == AUTOMATIC_MINING
			|| GetItem(SHOULDER)->nNum == AUTOMATIC_DROP_MINING)
		{
			//RLOCK(g_pMain->m_MiningFishingItemArray);
			foreach_stlmap_nolock(itr, g_pMain->m_MiningFishingItemArray)
			{
				pMining = itr->second;

				if (pMining == nullptr)
					continue;

				if (pMining->nTableType != 0)
					continue;

				if (pMining->UseItemType == 1)
				{
					if (pMining->nWarStatus == MiningResultError)
						pMiningList.push_back(pMining->nIndex);
					else
						continue;
				}
			}
			//RULOCK(g_pMain->m_MiningFishingItemArray);
		}

		if (GetItem(SHOULDER)->nNum == AUTOMATIC_FISHING
			|| GetItem(SHOULDER)->nNum == AUTOMATIC_DROP_FISHING)
		{
			//RLOCK(g_pMain->m_MiningFishingItemArray);
			foreach_stlmap_nolock(itr, g_pMain->m_MiningFishingItemArray)
			{
				pFishing = itr->second;

				if (pFishing == nullptr)
					continue;

				if (pFishing->nTableType != 1)
					continue;

				if (pFishing->UseItemType == 1)
				{
					if (pFishing->nWarStatus == MiningResultError)
						pFishingList.push_back(pFishing->nIndex);
					else
						continue;
				}
			}
			//RULOCK(g_pMain->m_MiningFishingItemArray);
		}

		if (pFishingList.size() > 0)
		{
			int offset = 0;
			uint32 bRandArray[10000];
			uint8 sItemSlot = 0;
			foreach(itr, pFishingList)
			{
				pFishing = g_pMain->m_MiningFishingItemArray.GetData(*itr);

				if (pFishing == nullptr)
					return;

				if (offset >= 10000)
					break;

				for (int i = 0; i < int(pFishing->SuccessRate / 5); i++)
				{
					if (i + offset >= 10000)
						break;

					bRandArray[offset + i] = pFishing->nGiveItemID;
				}

				offset += int(pFishing->SuccessRate / 5);
			}

			uint32 bRandSlot = myrand(0, offset);
			uint32 nItemID = bRandArray[bRandSlot];
			_ITEM_TABLE* pItem = g_pMain->m_ItemtableArray.GetData(nItemID);

			if (pItem == nullptr)
				return;

			uint8 bFreeSlots = 0;
			for (int i = 0; i < WAREHOUSE_MAX; i++)
			{
				if (GetWerehouseItem(i)->nNum == 0 && ++bFreeSlots >= ITEMS_IN_EXCHANGE_GROUP)
					break;
			}

			if (bFreeSlots < 1)
				return;

			int SlotForItem = FindWerehouseSlotForItem(pItem->m_iNum, 1);
			if (SlotForItem == -1)
				return;

			if (nItemID == ITEM_EXP)
			{
				if (GetLevel() >= 1 && GetLevel() <= 34)
					ExpChange(50);
				if (GetLevel() >= 35 && GetLevel() <= 59)
					ExpChange(100);
				if (GetLevel() >= 60 && GetLevel() <= 69)
					ExpChange(200);
				if (GetLevel() >= 70 && GetLevel() <= 83)
					ExpChange(300);
			}
			else
			{
				GiveWerehouseItem(nItemID, 1, false, true);
			}
		}

		if (pMiningList.size() > 0)
		{
			int offset = 0;
			uint32 bRandArray[10000];
			uint8 sItemSlot = 0;
			foreach(itr, pMiningList)
			{
				pMining = g_pMain->m_MiningFishingItemArray.GetData(*itr);

				if (pMining == nullptr)
					continue;

				if (offset >= 10000)
					break;

				for (int i = 0; i < int(pMining->SuccessRate / 5); i++)
				{
					if (i + offset >= 10000)
						break;

					bRandArray[offset + i] = pMining->nGiveItemID;
				}

				offset += int(pMining->SuccessRate / 5);
			}

			uint32 bRandSlot = myrand(0, offset);
			uint32 nItemID = bRandArray[bRandSlot];
			_ITEM_TABLE* pItem = g_pMain->m_ItemtableArray.GetData(nItemID);

			if (pItem == nullptr)
				return;

			uint8 bFreeSlots = 0;
			for (int i = 0; i < WAREHOUSE_MAX; i++)
			{
				if (GetWerehouseItem(i)->nNum == 0 && ++bFreeSlots >= ITEMS_IN_EXCHANGE_GROUP)
					break;
			}

			if (bFreeSlots < 1)
				return;

			int SlotForItem = FindWerehouseSlotForItem(pItem->m_iNum, 1);
			if (SlotForItem == -1)
				return;

			if (nItemID == ITEM_EXP)
			{
				if (GetLevel() >= 1 && GetLevel() <= 34)
					ExpChange(50);
				if (GetLevel() >= 35 && GetLevel() <= 59)
					ExpChange(100);
				if (GetLevel() >= 60 && GetLevel() <= 69)
					ExpChange(200);
				if (GetLevel() >= 70 && GetLevel() <= 83)
					ExpChange(300);
			}
			else
			{
				GiveWerehouseItem(nItemID, 1, true, false);
			}
		}
		m_AutoMiningFishingSystem = UNIXTIME;
	}



	if (m_OfflineSystemTime + (PLAYER_TRAINING_INTERVAL - 13) < UNIXTIME)
	{
		if (isOfflineSystemSocket()
			&& isOfflineSystemTimeSocket())
		{
			_ITEM_TABLE* pItem = GetItemPrototype(CFAIRY);
			if (pItem != nullptr)
			{
				if (pItem->GetNum() == OFFLINE_MINING_ITEM)
					HandleMiningAttempt((Packet)(WIZ_MINING, MiningAttempt));
				else if (pItem->GetNum() == OFFLINE_FISHING_ITEM)
					HandleFishingAttempt((Packet)(WIZ_MINING, FishingAttempt));
			}
		}
		m_OfflineSystemTime = UNIXTIME;
	}

	if (isOfflineSystemSocket()
		&& isOfflineSystemTimeSocket())
	{
		if (UNIXTIME >= isOfflineSystemTime())
		{
			m_bOfflineSystemSocketType = false;
			m_bOfflineSystemTimeType = false;
			DateTime time;
			g_pMain->WriteOfflineSystemLog(string_format("[Offline System Disconnect - %d:%d:%d ] = %s\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str()));
			UserDataSaveToAgent();
			Disconnect();
			return;
		}
	}
	//CheckSexyGuardAlive();
	/*if (!g_pMain->g_HardwareInformation.IsValidHardwareID(g_pMain->m_HardwareIDArray))
	{
		UserDataSaveToAgent();
		Disconnect();
		return;
	}*/
}
#pragma endregion

/**
* @brief	Checks the SexyGuard ALIVE packets are being received to understand
*			whether our Guard is alive at the client side.
*			This also starts the SexyGuard if enabled.
*/
void CUser::CheckSexyGuardAlive()
{
	if (!g_pMain->m_bisGuardActive)
		return;

	if (!isInGame())
		return;

	if (isSexyGuardAlive == false)
	{
		m_tGuardAliveExpiryTime = uint32(UNIXTIME);
		isSexyGuardAlive = true;
	}
	else if (isSexyGuardAlive == true)
	{
		if (m_tGuardAliveExpiryTime != 0 && (uint32(UNIXTIME) - m_tGuardAliveExpiryTime) >= CYBERANTICHEAT_GUARD_TIME_SECOND)
		{
			DateTime time;
			g_pMain->WriteCheatLogFile(string_format("[SexyGuard Alive - TIMEOUT | %d:%d:%d ] Disconnected = %s | SocketID = %d\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID()));
			TRACE("%s was disconnected since no SexyGuard Alive packet was received in %d seconds", GetName().c_str(), CYBERANTICHEAT_GUARD_TIME_SECOND);
			Packet result(WIZ_MASTER_SECURITY, uint8(GuardOpCodes::DISCONNECT));
			Send(&result);
			Disconnect();
		}
	}

}

void CUser::DeathNotice(Packet& pkt)
{
	uint8 GetNation, noticeType, m_sDeathNoticeStatus;
	uint16 killerGetID, DeathGetID;
	string killerName, DeathName;
	float xrota, zrota;


	pkt >> GetNation
		>> noticeType
		>> killerGetID
		>> killerName
		>> DeathGetID
		>> DeathName
		>> xrota >> zrota
		>> m_sDeathNoticeStatus;

	CUser* pUserKiller = g_pMain->GetUserPtr(killerName, TYPE_CHARACTER);

	if (m_sDeathNoticeStatus == 0)
	{
		Packet result(WIZ_CHAT, uint8(DEATH_NOTICE));
		result.SByte();
		result << GetNation
			<< noticeType
			<< killerGetID
			<< killerName
			<< DeathGetID
			<< DeathName
			<< xrota << zrota;
		Send(&result);
	}
	if (m_sDeathNoticeStatus == 1)
	{



		if (killerName == GetName() || DeathName == GetName())
		{
			Packet result(WIZ_CHAT, uint8(DEATH_NOTICE));
			result.SByte();
			result << GetNation
				<< noticeType
				<< killerGetID
				<< killerName
				<< DeathGetID
				<< DeathName
				<< xrota << zrota;
			Send(&result);
		}
		else
		{
			if (isInParty())
			{
				_PARTY_GROUP* pParty;
				pParty = g_pMain->GetPartyPtr(GetPartyID());
				for (int i = 0; i < MAX_PARTY_USERS; i++)
				{
					CUser* pUser = g_pMain->GetUserPtr(pParty->uid[i]);
					if (pUser == nullptr)
						continue;
					if (killerName == pUser->GetName() || DeathName == pUser->GetName())
					{
						Packet result(WIZ_CHAT, uint8(DEATH_NOTICE));
						result.SByte();
						result << GetNation
							<< noticeType
							<< killerGetID
							<< killerName
							<< DeathGetID
							<< DeathName
							<< xrota << zrota;
						Send(&result);

					}
				}
			}
		}
	}
	if (m_sDeathNoticeStatus == 2)
	{
		Packet result(WIZ_CHAT, uint8(DEATH_NOTICE));
		result.SByte();
		result << GetNation
			<< noticeType
			<< killerGetID
			<< killerName
			<< DeathGetID
			<< DeathName
			<< xrota << zrota;
		Send(&result);
	}
	if (m_sDeathNoticeStatus == 3)
		return;
}




void CUser::UserTools(Packet& pkt)
{
	uint8 sOpCode;
	pkt >> sOpCode;
	Packet result(PL_USER_TOOLS);
	if (sOpCode == 1)
	{
		if (m_iGold > 1000000)
		{
			GoldGain(-1000000);
			g_pMain->SpawnEventNpc(505, false, GetZoneID(), GetX(), GetY(), GetZ()), 1, 2, 60 * 3;
		}
		else
			return;
	}
	else if (sOpCode == 2)
	{
		uint8 sOpCode;
		pkt >> sOpCode;
		if (sOpCode == 1)
		{
			string baslik[20], icerik[20], cevap[20];
			uint16 status[20];
			for (int i = 0; i < 20; i++)
			{
				status[i] = 0;
				cevap[i] = " ";
				baslik[i] = " ";
				icerik[i] = " ";
			}

			if (g_pMain->m_Tickets.GetSize())
				g_pMain->m_Tickets.DeleteAllData();

			g_DBAgent.LoadTickets(GetName());
			Packet result(PL_USER_TOOLS);
			result << uint8(2);

			for (int i = 0; i < g_pMain->m_Tickets.GetSize(); i++)
			{
				_LOAD_CYBERACS_TICKETS* ticket = g_pMain->m_Tickets.GetData(i + 1);

				baslik[i] = ticket->baslik;
				icerik[i] = ticket->icerik;
				cevap[i] = ticket->cevap;
				status[i] = ticket->status;
			}

			for (int i = 0; i < g_pMain->m_Tickets.GetSize(); i++)
			{
				result << baslik[i] << icerik[i] << cevap[i] << status[i];
			}
			Send(&result);
		}
		else if (sOpCode == 2)
		{
			string baslik1, icerik1;
			pkt >> baslik1 >> icerik1;

			if (g_pMain->m_Tickets.GetSize())
				g_pMain->m_Tickets.DeleteAllData();

			g_DBAgent.LoadTickets(GetName());




			if (g_pMain->m_Tickets.GetSize() == 0)
				g_DBAgent.InsertTickets(GetName(), baslik1, icerik1);
			else
			{
				for (int i = 0; i < g_pMain->m_Tickets.GetSize(); i++)
				{
					_LOAD_CYBERACS_TICKETS* ticket = g_pMain->m_Tickets.GetData(i + 1);

					if (ticket == nullptr)
						g_DBAgent.InsertTickets(GetName(), baslik1, icerik1);


					else if (baslik1 != ticket->baslik)
					{
						g_DBAgent.InsertTickets(GetName(), baslik1, icerik1);
					}

					else if (baslik1 == ticket->baslik)
					{

						g_DBAgent.UpdateTickets(GetName(), baslik1, icerik1);

					}

				}
			}
		}
	}
	else if (sOpCode == 3)
	{

		uint32 pass;
		pkt >> pass;

		g_DBAgent.UpdatePassword(GetAccountName(), pass);

	}
}



void CUser::Ranks(Packet& pkt)
{
	uint8 sOpCode;
	pkt >> sOpCode;
	Packet result(PL_GENERALRANK);
	if (sOpCode == 1)
	{

		if (g_pMain->m_UserRankALL.GetSize())
			g_pMain->m_UserRankALL.DeleteAllData();

		g_DBAgent.UserRankingALL();
		string strName[20], m_strName[20];
		uint8 Nation[20];
		uint32 loyalty[20];
		for (int i = 0; i < 20; i++) {
			loyalty[i] = 0;
			Nation[i] = 0;
		}



		CUser* pUserLooker = g_pMain->GetUserPtr(GetName(), TYPE_CHARACTER);

		for (int i = 0; i < 20; i++)
		{
			_USER_RANK_ALL* pUserALL = g_pMain->m_UserRankALL.GetData(i + 1);
			if (pUserALL == nullptr)
				continue;

			Nation[i] = pUserALL->Nation;
			strName[i] = pUserALL->strUserId;
			loyalty[i] = pUserALL->LoyaltyMonthly;
			m_strName[i] = pUserLooker->GetName();
		}
		result << uint8(1);
		for (int i = 0; i < 20; i++)
			result << Nation[i] << strName[i] << loyalty[i] << m_strName[i];
		Send(&result);

	}
	if (sOpCode == 2)
	{
		string empty = "", strName[20], strChief[20], m_strName[20];
		uint8 Nation[20];
		for (int i = 0; i < 20; i++)
			Nation[i] = 0;

		CKnights* pKnightsUser = g_pMain->GetClanPtr(GetClanID());

		int i = 0;
		foreach_stlmap_nolock(itr, g_pMain->m_KnightsRatingArray[1])
		{
			if (pKnightsUser != NULL) {
				CKnights* pKnights = g_pMain->GetClanPtr(itr->second->sClanID);

				Nation[i] = pKnights->m_byNation;
				strName[i] = pKnights->m_strName;
				strChief[i] = pKnights->m_strChief;
				m_strName[i] = pKnightsUser->m_strName;

				i++;
			}
			else if (pKnightsUser == NULL) {
				CKnights* pKnights = g_pMain->GetClanPtr(itr->second->sClanID);
				Nation[i] = pKnights->m_byNation;
				strName[i] = pKnights->m_strName;
				strChief[i] = pKnights->m_strChief;
				m_strName[i] = empty;
				i++;
			}
		}
		result << uint8(2);
		for (int i = 0; i < 20; i++)
			result << Nation[i] << strName[i] << strChief[i] << m_strName[i];

		Send(&result);

	}
	if (sOpCode == 3)
	{
		if (g_pMain->m_UserRankKA.GetSize())
			g_pMain->m_UserRankKA.DeleteAllData();

		g_DBAgent.UserRankingKA();
		string strName[20], m_strName[20];
		uint8 Nation[20];
		uint32 loyalty[20];
		for (int i = 0; i < 20; i++) {
			loyalty[i] = 0;
			Nation[i] = 0;
		}

		CUser* pUserLooker = g_pMain->GetUserPtr(GetName(), TYPE_CHARACTER);

		for (int i = 0; i < 20; i++)
		{
			_USER_RANK_KA* pUserKA = g_pMain->m_UserRankKA.GetData(i + 1);
			if (pUserKA == nullptr)
				continue;
			Nation[i] = 1;
			strName[i] = pUserKA->strUserId;
			loyalty[i] = pUserKA->LoyaltyMonthly;
			m_strName[i] = pUserLooker->GetName();
		}
		result << uint8(3);
		for (int i = 0; i < 20; i++)
			result << Nation[i] << strName[i] << loyalty[i] << m_strName[i];
		Send(&result);
	}
	if (sOpCode == 4)
	{
		if (g_pMain->m_UserRankEL.GetSize())
			g_pMain->m_UserRankEL.DeleteAllData();

		g_DBAgent.UserRankingEL();
		string strName[20], m_strName[20];
		uint8 Nation[20];
		uint32 loyalty[20];
		for (int i = 0; i < 20; i++) {
			loyalty[i] = 0;
			Nation[i] = 0;
		}

		CUser* pUserLooker = g_pMain->GetUserPtr(GetName(), TYPE_CHARACTER);

		for (int i = 0; i < 20; i++)
		{
			_USER_RANK_EL* pUserEL = g_pMain->m_UserRankEL.GetData(i + 1);
			if (pUserEL == nullptr)
				continue;
			Nation[i] = 2;
			strName[i] = pUserEL->strUserId;
			loyalty[i] = pUserEL->LoyaltyMonthly;
			m_strName[i] = pUserLooker->GetName();
		}
		result << uint8(4);
		for (int i = 0; i < 20; i++)
			result << Nation[i] << strName[i] << loyalty[i] << m_strName[i];
		Send(&result);

	}
	if (sOpCode == 5)
	{
		if (g_pMain->m_KingList.GetSize())
			g_pMain->m_KingList.DeleteAllData();


		g_DBAgent.LoadKings();

		_KINGS_LIST* pKingList = g_pMain->m_KingList.GetData(1);
		result << uint8(5);

		result << pKingList->strElMoradKingName << pKingList->strElMoradNotice << pKingList->strKarusKingName << pKingList->strKarusNotice;
		Send(&result);

	}
}

void CUser::Password(Packet& pkt)
{
	uint8 sOpCode;
	pkt >> sOpCode;
	if (sOpCode == 1) //Check password
	{
		uint32 password;
		pkt >> password;


		g_DBAgent.NDPassword(GetName(), GetAccountName());

		if (ndpasswrd == password)
		{
			Packet result(PL_2NDPASSWORD);
			result << uint8(2);
			Send(&result);
		}
		if (ndpasswrd != password)
		{
			Packet result(PL_2NDPASSWORD);
			result << uint8(3);
			Send(&result);
		}

	}
	if (sOpCode == 2) //Check password
	{
		Disconnect();
	}
}

#pragma region CUser::CyberACS_UIRequest(Packet& pkt)
void CUser::CyberACS_ReqUserData(Packet& pkt)
{

	Packet result(PL_USERDATA);
	result << GetSocketID()
		<< GetName()
		<< GetClass()
		<< GetRace()
		<< GetLevel()
		<< GetRebirthLevel()
		<< GetNation()
		<< m_iMaxExp
		<< m_iExp
		<< m_iMannerPoint
		<< uint16(m_bStats[0])
		<< uint16(m_bStats[1])
		<< uint16(m_bStats[2])
		<< uint16(m_bStats[3])
		<< uint16(m_bStats[4])
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
#pragma endregion

#pragma region CUser::CyberACS_Support(Packet& pkt)
void CUser::CyberACS_ReqMerchantList(Packet& pkt)
{
	bool bIsNeutralZone = (isInMoradon() || isInElmoradEslant() || isInKarusEslant() || isInElmoradCastle() || isInLufersonCastle());
	Packet result(WIZ_MERCHANT, uint8(MERCHANT_MENISIA_LIST));

	if (!bIsNeutralZone)
	{
		result << uint8(1) << uint8(0) << uint8(3);
		Send(&result);
		return;
	}

	uint8 subCode = pkt.read<uint8>();

	switch (subCode)
	{
	case 1:
	{
		Packet result(PL_MERCHANTLIST);
		uint32 merchantCount = 0;
		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		std::vector<uint32> MerchantOfficalListen;
		foreach(itr, sessMap)
		{
			CUser* pUser = TO_USER(itr->second);
			if (pUser == nullptr)
				continue;

			if (!pUser->isInGame()
				|| !pUser->isMerchanting())
				continue;

			for (int j = 0; j < MAX_MERCH_ITEMS; j++) {
				_MERCH_DATA* pMerch = &pUser->m_arMerchantItems[j];

				if (pMerch->nNum == 0
					|| pMerch->nPrice == 0)
					continue;

				merchantCount++;
			}
			MerchantOfficalListen.push_back(pUser->GetSocketID());
		}
		result << merchantCount;

		foreach(itr, MerchantOfficalListen)
		{
			CUser* pUser = g_pMain->GetUserPtr(*itr);
			if (pUser == nullptr)
				continue;

			if (!pUser->isInGame()
				|| !pUser->isMerchanting())
				continue;

			for (int j = 0; j < MAX_MERCH_ITEMS; j++) {
				_MERCH_DATA* pMerch = &pUser->m_arMerchantItems[j];

				if (pMerch->nNum == 0 || pMerch->nPrice == 0)
					continue;

				result << pUser->GetSocketID()
					<< uint32(pUser->GetID())
					<< pUser->GetName()
					<< uint8((pUser->isSellingMerchant() ? uint8(0) : uint8(1)))
					<< pMerch->nNum
					<< pMerch->sCount
					<< pMerch->nPrice
					<< pMerch->isKC
					<< pUser->GetX()
					<< pUser->GetY()
					<< pUser->GetZ();
			}
		}
		Send(&result);
	}break;
	case 2:
	{
		string s_sTragetID;
		pkt >> s_sTragetID;

		CUser* pUser = g_pMain->GetUserPtr(s_sTragetID, NameType::TYPE_CHARACTER);
		if (pUser == nullptr)
			return;

		if (!pUser->isMerchanting())
			return;

		ZoneChange(pUser->GetZoneID(), pUser->GetX(), pUser->GetZ());
	}break;
	case 3:
	{
		string s_sTragetID;
		pkt >> s_sTragetID;

		CUser* pUser = g_pMain->GetUserPtr(s_sTragetID, NameType::TYPE_CHARACTER);
		if (pUser == nullptr)
			return;

		Packet result;
		string msg = "   ";
		ChatPacket::Construct(&result, PRIVATE_CHAT, &msg, &pUser->GetName());
		Send(&result);

	}break;
	default:
		break;
	}
}
#pragma endregion

#pragma region CUser::CyberACS_ChaoticExchange(Packet& pkt)
void CUser::CyberACS_ChaoticExchange(Packet& pkt)
{
	enum ResultOpCodes
	{
		Failed = 0,
		Success = 1
	};

	enum ResultMessages
	{
		EffectNone = 0, // No effect
		EffectRed = 1, // There will be better days.
		EffectGreen = 2, // Don't be too disappointed. You're luck isn't that bad.
		EffectWhite = 3 // It must be your lucky day.
	};

	uint16 nObjectID = 0;
	uint32 nExchangeItemID = 0;
	ResultOpCodes resultOpCode = Failed;
	ResultMessages resultMessage = EffectNone;
	uint32 nTotalPercent = 0;
	uint32 bRandArray[10000];
	memset(&bRandArray, 0, sizeof(bRandArray));
	int offset = 0;
	_ITEM_EXCHANGE* pExchange;
	std::vector<uint32> ExchangeIndexList;
	uint8 sItemSlot = 0;
	uint8 sExchangeItemSlot = 0;
	uint8 bStockInn = 0;
	uint8 bSell = 0;
	uint32 transactionPrice = 0; uint32 nItemID = 0; uint32 bRandSlot = 0;
	pkt >> nObjectID >> nExchangeItemID >> bStockInn >> bSell;

	if (bStockInn < 0
		|| bStockInn > 1
		|| bSell < 0
		|| bSell > 1)
		goto fail_return;

	CNpc* pNpc = g_pMain->GetNpcPtr(nObjectID, GetZoneID());
	if (pNpc == nullptr
		|| (pNpc->GetType() != NPC_CHAOTIC_GENERATOR && pNpc->GetType() != NPC_CHAOTIC_GENERATOR2)
		|| isTrading()
		|| isMerchanting()
		|| isMining()
		|| (UNIXTIME2 - m_iLastExchangeTime < 400))
		goto fail_return;

	m_iLastExchangeTime = UNIXTIME2;
	sExchangeItemSlot = FindSlotForItem(nExchangeItemID, 1) - SLOT_MAX;
	if (g_pMain->m_ItemExchangeArray.GetSize() > 0)
	{
		foreach_stlmap(itr, g_pMain->m_ItemExchangeArray)
		{
			if (itr->second->nOriginItemNum[0] == nExchangeItemID)
				ExchangeIndexList.push_back(itr->second->nIndex);
			else
				continue;
		}
	}

	foreach(itr, ExchangeIndexList)
	{
		pExchange = g_pMain->m_ItemExchangeArray.GetData(*itr);

		if (pExchange == nullptr
			|| !CheckExchange(*itr)
			|| pExchange->bRandomFlag >= 101
			|| !CheckExistItemAnd(pExchange->nOriginItemNum[0], pExchange->sOriginItemCount[0], 0, 0, 0, 0, 0, 0, 0, 0))
			continue;

		if (offset >= 10000)
			break;

		for (int i = 0; i < int(pExchange->sExchangeItemCount[0] / 5); i++)
		{
			if (i + offset >= 10000)
				break;

			bRandArray[offset + i] = pExchange->nExchangeItemNum[0];
		}

		offset += int(pExchange->sExchangeItemCount[0] / 5);
	}

	if (offset >= 10000)
		offset = 10000;

	bRandSlot = myrand(0, offset);
	nItemID = bRandArray[bRandSlot];

	_ITEM_TABLE* pItem = g_pMain->m_ItemtableArray.GetData(nItemID);
	if (pItem == nullptr)
		goto fail_return;

	uint8 bFreeSlots = 0;
	for (int i = SLOT_MAX; i < SLOT_MAX + HAVE_MAX; i++)
	{
		if (GetItem(i)->nNum == 0 && ++bFreeSlots >= ITEMS_IN_EXCHANGE_GROUP)
			break;
	}

	if (bFreeSlots <= 1)
		goto fail_return;

	int SlotForItem = FindSlotForItem(pItem->m_iNum, 1);
	if (SlotForItem == -1)
		goto fail_return;

	sItemSlot = GetEmptySlot() - SLOT_MAX;
	RobItem(pExchange->nOriginItemNum[0], 1);

	if (bStockInn == 1 && !bSell)
		GiveWerehouseItem(nItemID, 1, false, false);
	else if (bStockInn == 0 && !bSell)
		GiveItem(nItemID, 1);
	else
	{
		_ITEM_TABLE* pItemTables = g_pMain->GetItemPtr(nItemID);
		if (pItemTables == nullptr)
			goto fail_return;

		if (pItemTables->m_ItemType == 4)
		{
			if (bStockInn == 1)
				GiveWerehouseItem(nItemID, 1, false, false);
			else
				GiveItem(nItemID, 1);
		}
		else
		{
			if (pItemTables->m_iSellPrice != SellTypeFullPrice)
				transactionPrice += ((pItemTables->m_iBuyPrice / (GetPremiumProperty(PremiumItemSellPercent) > 0 ? 4 : 6))); // /6 is normal, /4 for prem/discount
			else
				transactionPrice += pItemTables->m_iBuyPrice;

			if (GetCoins() + transactionPrice > COIN_MAX)
				goto fail_return;
		}
	}

	if (pItem->m_ItemType == 4)
		resultMessage = EffectWhite;
	else if (pItem->m_ItemType == 5
		|| pItem->m_ItemType == 11
		|| pItem->m_ItemType == 12)
		resultMessage = EffectGreen;
	else
		resultMessage = EffectRed;

	resultOpCode = Success;
fail_return:
	Packet result(WIZ_ITEM_UPGRADE);
	result << (uint8)ITEM_BIFROST_EXCHANGE
		<< (uint8)resultOpCode
		<< nItemID
		<< sItemSlot
		<< nExchangeItemID
		<< sExchangeItemSlot
		<< (uint8)resultMessage;

	Send(&result);

	result.clear();
	result.SetOpcode(WIZ_OBJECT_EVENT);
	result << (uint8)OBJECT_ARTIFACT << (uint8)resultMessage << nObjectID;

	if (resultOpCode != Failed)
	{
		if (GetEventRoom() > 0)
			SendToRegion(&result, nullptr, GetEventRoom());
		else
			SendToRegion(&result);
	}

	_ITEM_TABLE* pItemTables = g_pMain->GetItemPtr(nItemID);

	if (resultOpCode != Failed && pItemTables != nullptr)
	{
		if (bStockInn == 1)
		{
			if ((bSell && pItemTables->m_ItemType == 4) || !bSell)
				g_pMain->SendHelpDescription(this, string_format("%s has been stored.", pItemTables->m_sName.c_str()));
		}

		if (bSell
			&& pItemTables->m_ItemType != 4
			&& transactionPrice > 0)
			GoldGain(transactionPrice);
	}

	if (resultOpCode == Failed)
	{
		result.clear();
		result.Initialize(PL_CHAOTIC_RESULT);
		result << uint8(PL_CHAOTIC_RESULT);
		Send(&result);
	}
}
#pragma endregion

#pragma region CUser::CyberACS_ReqUserInfo(Packet& pkt)
void CUser::CyberACS_ReqUserInfo(Packet& pkt)
{
	uint32 moneyReq = 0;
	if (GetPremium() == WAR_Premium)
		moneyReq = 0;
	else
	{
		moneyReq = (int)pow((GetLevel() * 2.0f), 3.4f);
		if (GetLevel() < 30)
			moneyReq = (int)(moneyReq * 0.4f);
		else if (GetLevel() >= 60)
			moneyReq = (int)(moneyReq * 1.5f);

		if ((g_pMain->m_sDiscount == 1
			&& g_pMain->m_byOldVictory == GetNation())
			|| g_pMain->m_sDiscount == 2)
			moneyReq /= 2;
	}
	Packet result(PL_USERINFO);
	result << uint32(m_nKnightCash) << uint32(m_nKnightCashBonus)
		<< uint16(m_sDaggerR) << uint16(m_sAxeR) << uint16(m_sSwordR)
		<< uint16(m_sMaceR) << uint16(m_sSpearR) << uint16(m_sBowR)
		<< uint16(m_sJamadarR) << uint32(moneyReq);
	Send(&result);
}
#pragma endregion

#pragma region CUser::CyberACS_CashLose(uint32 cash)
bool CUser::CyberACS_CashLose(uint32 cash)
{
	if (m_nKnightCash < cash)
		return false;

	m_nKnightCash -= cash;

	Packet result(PL_CASHCHANGE);
	result << m_nKnightCash << m_nKnightCashBonus;
	Send(&result);

	return true;
}
#pragma endregion

#pragma region CUser::CyberACS_CashGain(uint32 cash)
void CUser::CyberACS_CashGain(uint32 cash)
{
	m_nKnightCash += cash;

	Packet result(PL_CASHCHANGE);
	result << m_nKnightCash << m_nKnightCashBonus;
	Send(&result);
}
#pragma endregion
void CUser::DailyQuest(Packet& pkt)
{
	uint8 sOpCode;
	pkt >> sOpCode;
	if (sOpCode == 1) //GÖREV TABLOSU
	{
		uint8 status[120], statusUser[120];
		for (int Count = 0; Count < 120; Count++) {
			statusUser[Count] = 0;
			status[Count] = 0;
			g_pMain->m_DailyFinishTime[Count] = 0;
		}

		g_DBAgent.LoadUserQuests(GetName());

		for (int Count = 0; Count < 120; Count++) {
			if (m_DailyRegister[Count] == true)
				statusUser[Count] = 1;
			else if (m_DailyRegister[Count] == false)
				statusUser[Count] = 0;
		}

		for (int Count = 0; Count < 120; Count++) {
			if (g_pMain->m_DailyGameServerSatus[Count] == true)
				status[Count] = 1;
			else if (g_pMain->m_DailyGameServerSatus[Count] == false)
				status[Count] = 0;
		}

		Packet result(PL_DAILY);
		result << uint8(0);

		for (int i = 0; i < 120; i++)
		{
			result << g_pMain->m_DailyQuestName[i] << status[i] << statusUser[i] << g_pMain->m_DailyFinishTime[i];
		}

		Send(&result);
	}
	if (sOpCode == 2) //GÖREV KENDİSİ 
	{

		std::string Name;
		pkt >> Name;
		DailyLoadProcessPacket();
		for (int Count = 0; Count < 120; Count++)
		{
			if (g_pMain->m_DailyQuestName[Count] == Name)
			{
				Packet result(PL_DAILY);
				result << uint8(1) << g_pMain->m_DailyQuestName[Count];

				for (int i = 0; i < 4; i++)
					result << m_DailyEventMonsterID[i][Count]
					<< m_DailyEventCount[i][Count]
					<< m_DailyEventMonsterKillCount[0][Count];

				for (int i = 0; i < 2; i++)
					result << g_pMain->m_DailySettings[Count]->CREventItemID[i]
					<< g_pMain->m_DailySettings[Count]->CREventItemCount[i];

				result << g_pMain->m_DailySettings[Count]->CREventWinCoin
					<< g_pMain->m_DailySettings[Count]->CREventWinEXP
					<< g_pMain->m_DailySettings[Count]->CREventWinNP
					<< g_pMain->m_DailySettings[Count]->CREventWinKC
					<< uint32(g_pMain->m_DailyFinishTime[Count]);

				Send(&result);

				DailyLoadProcess(Count);

			}
		}
	}
}

void CUser::DailyLoadProcessPacket() {

	/*for (int i = 0; i < 4; i++)
	{
		for (int b = 0; b < 120; i++)
		{
			//m_DailyEventMonsterID[i][b] = 0;
			m_DailyEventMonsterKillCount[i][b] = 0;
			m_DailyEventCount[i][b] = 0;
			//m_DailyEventZone[i][b] = 0;
		}
	}*/

	for (int i = 0; i < 120; i++)
	{

		m_DailyEventCount[0][i] = g_pMain->m_DailyMonsterKillCount1[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterName[0][i] = g_pMain->m_DailyMonsterName1[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterID[0][i] = g_pMain->m_DailyMonsterNum1[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventZone[0][i] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[i]];

		m_DailyEventCount[1][i] = g_pMain->m_DailyMonsterKillCount2[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterName[1][i] = g_pMain->m_DailyMonsterName2[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterID[1][i] = g_pMain->m_DailyMonsterNum2[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventZone[1][i] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[i]];

		m_DailyEventCount[2][i] = g_pMain->m_DailyMonsterKillCount3[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterName[2][i] = g_pMain->m_DailyMonsterName3[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterID[2][i] = g_pMain->m_DailyMonsterNum3[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventZone[2][i] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[i]];

		m_DailyEventCount[3][i] = g_pMain->m_DailyMonsterKillCount4[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterName[3][i] = g_pMain->m_DailyMonsterName4[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventMonsterID[3][i] = g_pMain->m_DailyMonsterNum4[g_pMain->m_DailySelectedMonster[i]];
		m_DailyEventZone[3][i] = g_pMain->m_DailyQuestZone[g_pMain->m_DailySelectedMonster[i]];
	}
}


void CUser::SlaveMerc(Packet& pkt) {

	uint8 sOpCode;
	pkt >> sOpCode;
	if (sOpCode == 1) //UPDATE
	{
		g_DBAgent.LoadSlave(GetName());

		uint32 sellinItem[12], sellinItemCount[12];

		for (int i = 0; i < 12; i++)
			SlaveItemID[i];
		for (int i = 0; i < 12; i++)
			SlaveItemCount[i];



		Packet Update(PL_SLAVE_MERC);
		Update << uint8(3) << SlaveTotalKC << SlaveTotalCoins;
		for (int i = 0; i < 12; i++)
			Update << SlaveItemID[i] << SlaveItemCount[i];
		for (int i = 14; i < 42; i++)
			Update << m_sItemArray[i].nNum << m_sItemArray[i].sCount;

		Send(&Update);
	}
	else if (sOpCode == 2)// give coins&kc back
	{
		uint32 kc, coin; // çekilen miktar
		uint32 total_kc, total_coin; // geriye kalan
		pkt >> total_kc >> total_coin;

		if (total_kc > 0 && total_coin <= 0) {
			kc = SlaveTotalKC - total_kc;
			coin = 0;
		}
		else if (total_kc <= 0 && total_coin > 0) {
			kc = 0;
			coin = SlaveTotalCoins - total_coin;
		}

		if (kc > 0 && coin <= 0) {
			SendKnightCash(kc);
			g_DBAgent.RemoveSlaveKC(GetName(), total_kc);
		}
		else if (kc <= 0 && coin > 0) {
			GoldGain(coin);
			g_DBAgent.RemoveSlaveCoins(GetName(), total_coin);
		}
	}
	else if (sOpCode == 4) // send item to inventory
	{
		for (int i = 0; i < 14; i++)
		{
			if (!GiveItem(SlaveItemID[i], SlaveItemCount[i]))
				g_pMain->SendHelpDescription(this, "Error : Item could not be added");
		}
	}
	else if (sOpCode == 5)
	{
		uint8 isCashOrCoin, result;
		uint32 coins2, COINS;

		pkt >> COINS >> isCashOrCoin;

		coins2 = COINS;
		result = isCashOrCoin;

		Packet Update(PL_SLAVE_MERC);
		Update << uint8(5) << coins2 << result;
		Send(&Update);
	}
	else if (sOpCode == 6)
	{
		uint8 CanIopenMerchant = 0;
		pkt >> CanIopenMerchant;

		if (CanIopenMerchant == 1)
		{
			isSlave = CanIopenMerchant; // 1 oluyor
		}
		else if (CanIopenMerchant == 0)
		{
			isSlave = CanIopenMerchant; // 0 oluyor
		}

		if (isSlave != 0) {
			Packet result(PL_SLAVE_MERC);
			result << uint8(1);
			Send(&result);
		}
		else {

			CBot* pSlaveUser = g_pMain->m_BotArray.GetData(m_bSlaveUserID);
			if (pSlaveUser != nullptr) {
				GiveSlaveMerchantItems();
				pSlaveUser->UserInOut(INOUT_OUT);
			}

			for (int i = 0; i < 12; i++)
				SlaveItemID[i] = 0;
			for (int i = 0; i < 12; i++)
				SlaveItemCount[i] = 0;

		}

	}
}

#pragma region CUser::CyberACS_AuthInfo(Packet& pkt)
void CUser::CyberACS_AuthInfo(Packet& pkt) {
	uint8 u_auth;
	pkt >> u_auth;
	if (&u_auth != nullptr) {
		if (u_auth == AUTHORITY_GAME_MASTER && !isGM()) {
			string graphicCards, processor, ip;
			pkt >> graphicCards >> processor;
			ip = GetRemoteIP();
			DateTime time;
			g_pMain->WriteCheatLogFile(string_format("[CYBERACS Log Detection - %d:%d:%d] %s, %s, %s, %s, %s, %s\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetAccountName().c_str(), GetName().c_str(), "Wallhack", graphicCards.c_str(), processor.c_str(), ip.c_str()));
			g_pMain->SendFormattedNotice("%s is currently disconnect due to editing memory.", Nation::ALL, GetName().c_str());
			UserDataSaveToAgent();
			Disconnect();
			return;
		}
	}
	else {
		UserDataSaveToAgent();
		Disconnect();
		return;
	}
}
#pragma endregion

#pragma region CUser::CyberACS_LogHandler(Packet & pkt)
void CUser::CyberACS_LogHandler(Packet& pkt)
{
	string log, graphicCards, processor, ip;
	pkt >> log >> graphicCards >> processor;
	ip = GetRemoteIP();

	if (&log == nullptr)
		return;

	DateTime time;
	g_pMain->WriteCheatLogFile(string_format("[CYBERACS Log Support - %d:%d:%d] %s, %s, %s, %s, %s, %s\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetAccountName().c_str(), GetName().c_str(), ip.c_str(), log.c_str(), graphicCards.c_str(), processor.c_str()));
}
#pragma endregion

#pragma region CUser::CyberACS_AuthInfo(Packet& pkt)
void CUser::CyberACS_General(Packet& pkt)
{
	Packet result;
	uint8 sOpCode = pkt.read<uint8>();
	DateTime time;
	switch (sOpCode)
	{
	case 22:
	{
		uint32 mackey;
		string hardwareid;
		std::string s_strAccountID;
		pkt >> s_strAccountID >> mackey >> hardwareid;
		if (m_strAccountID != s_strAccountID)
		{
			UserDataSaveToAgent();
			Disconnect();
			return;
		}

		uint8 HardwareIDCheck = g_DBAgent.UserHWIDCheck(s_strAccountID, hardwareid, mackey);
		if (HardwareIDCheck == 9
			|| HardwareIDCheck == 4)
		{
			UserDataSaveToAgent();
			Disconnect();
			return;
		}
		else if (HardwareIDCheck == 3
			|| HardwareIDCheck == 6)
		{

			result << uint8(PL_MESSAGE) << "Error" << "This account is banned.";
			Send(&result);
			UserDataSaveToAgent();
			Disconnect();
			return;
		}
		else if (HardwareIDCheck == 2)
		{
			//printf("HWID Success: %s - %s\n", GetRemoteIP().c_str(), GetAccountName().c_str());
			//m_mackey = mackey;
		}
		else
		{

			result << uint8(PL_MESSAGE) << "Error" << "An unknown error has been occured. Please contact with administrators.";
			Send(&result);
			UserDataSaveToAgent();
			Disconnect();
			return;
		}
	}break;
	default:
		break;
	}
}
#pragma endregion

void CUser::CyberACS_GMTOOLS(Packet& pkt)
{
	Packet result(PL_GMTOOLS);
	result << ((uint8)1);
	Send(&result);
}

#pragma region CUser::CyberACS_HandleLifeSkill(Packet& pkt)
void CUser::CyberACS_HandleLifeSkill(Packet& pkt)
{
	CyberACS_SendLifeSkills();
}

void CUser::CyberACS_SendLifeSkills()
{
	/*LF_VEC2 m_War = GetLifeSkillLevel(LIFE_SKILL::WAR);
	LF_VEC2 m_Hunting = GetLifeSkillLevel(LIFE_SKILL::HUNTING);
	LF_VEC2 m_Smithery = GetLifeSkillLevel(LIFE_SKILL::SMITHERY);
	LF_VEC2 m_Karma = GetLifeSkillLevel(LIFE_SKILL::KARMA);

	Packet result(PL_LIFESKILL);
	result  << m_War.LVL		<< m_War.PER
			<< m_Hunting.LVL	<< m_Hunting.PER
			<< m_Smithery.LVL	<< m_Smithery.PER
			<< m_Karma.LVL		<< m_Karma.PER;
	Send(&result);*/
}
#pragma endregion

#pragma region CUser::CyberACS_HandleLifeSkill(Packet& pkt)
void CUser::MannerStore(Packet& pkt)
{
	uint8 sOpCode;
	pkt >> sOpCode;

	if (sOpCode == 1)
	{
		uint32 sItemCountSpecial = 0, sItemCountScrolls = 0;



		foreach_stlmap_nolock(itr, g_pMain->m_MannerStoreArray)
			if (itr->second->sType == 1)
				sItemCountSpecial++;
			else if (itr->second->sType == 2)
				sItemCountScrolls++;


		Packet result(PL_LIFESKILL);
		result << uint32(sItemCountSpecial) << uint32(sItemCountScrolls) << GetManner();

		foreach_stlmap_nolock(itr, g_pMain->m_MannerStoreArray)
		{
			if (itr->second->sType == 1)
			{
				result << uint32(itr->second->sItemID) << uint32(itr->second->sPrice) << uint32(itr->second->sBuyCount);
			}

		}

		foreach_stlmap_nolock(itr, g_pMain->m_MannerStoreArray)
		{
			if (itr->second->sType == 2)
			{
				result << uint32(itr->second->sItemID) << uint32(itr->second->sPrice) << uint32(itr->second->sBuyCount);
			}

		}

		Send(&result);
	}

	if (sOpCode == 2)
	{
		uint32 ItemIDManner, itemPrice;
		uint16 ItemCount;

		pkt >> ItemIDManner >> itemPrice >> ItemCount;

		SendMannerChange(-itemPrice);

		GiveItem(ItemIDManner, ItemCount);
	}

}

#pragma endregion


#pragma region CUser::CyberACS_PowerUpStoreRequest(Packet& pkt)
void CUser::CyberACS_PowerUpStoreRequest(Packet& pkt)
{
	uint8 sCode;
	pkt >> sCode;
	switch (sCode)
	{
	case 0:
		CyberACS_PowerUpStore();
		break;
	case 1:
		CyberACS_PowerUpStorePurchase(pkt);
		break;
		/*case 2:
			CyberACS_PowerUpStorePurchase(pkt);
			break;*/ //scode 1 yönlendiriyor.
	default:
		break;
	}
}
#pragma endregion

#pragma region CUser::CyberACS_PowerUpStorePurchase(Packet& pkt)
void CUser::CyberACS_PowerUpStorePurchase(Packet& pkt)
{
	if (isMerchanting()
		|| isTrading()
		|| isDead()
		|| isBanned())
	{
		if (isMerchanting())
			CyberACS_SendMessageBox("Failed", "You can't use power-up-store while merchanting.");
		if (isTrading())
			CyberACS_SendMessageBox("Failed", "You can't use power-up-store while trading.");
		return;
	}
	uint32 sItemID;
	pkt >> sItemID;

	_CYBERACS_PUS_ITEM* pPowerUpStore = g_pMain->m_CyberACSPusItemArray.GetData(sItemID);
	if (pPowerUpStore == nullptr)
		return;

	if (m_nKnightCash >= pPowerUpStore->sPrice)
	{
		_ITEM_TABLE* pItemTables = g_pMain->m_ItemtableArray.GetData(pPowerUpStore->sItemID);
		if (pItemTables == nullptr)
			return;

		int SlotForItem = FindSlotForItem(pPowerUpStore->sItemID, pPowerUpStore->sBuyCount);
		if (SlotForItem == -1)
		{
			CyberACS_SendMessageBox("Congratulations", "No Inventory Space");
			return;
		}

		m_nKnightCash -= pPowerUpStore->sPrice;
		g_DBAgent.UpdateKnightCash(this);
		GiveItem(pPowerUpStore->sItemID, pPowerUpStore->sBuyCount);
		CyberACS_SendMessageBox("Congratulations", string_format("You have purchased %s for %d Knight Cash.", pItemTables->m_sName.c_str(), pPowerUpStore->sPrice));
		Packet result(PL_CASHCHANGE);
		result << uint32(m_nKnightCash) << uint32(m_nKnightCashBonus);
		Send(&result);
	}
	else if (m_nKnightCashBonus >= pPowerUpStore->sPrice)
	{
		_ITEM_TABLE* pItemTables = g_pMain->m_ItemtableArray.GetData(pPowerUpStore->sPrice);
		if (pItemTables == nullptr)
			return;

		int SlotForItem = FindSlotForItem(pPowerUpStore->sItemID, pPowerUpStore->sBuyCount);
		if (SlotForItem == -1)
		{
			CyberACS_SendMessageBox("Congratulations", "No Inventory Space");
			return;
		}

		m_nKnightCashBonus -= pPowerUpStore->sPrice;
		GiveItem(pPowerUpStore->sItemID, pPowerUpStore->sBuyCount);
		CyberACS_SendMessageBox("Congratulations", string_format("You have purchased %s for %d Knight Cash.", pItemTables->m_sName.c_str(), pPowerUpStore->sPrice));
		Packet result(PL_CASHCHANGE);
		result << uint32(m_nKnightCash) << uint32(m_nKnightCashBonus);
		Send(&result);
	}
}
#pragma endregion

#pragma region CUser::CyberACSMonsterInfoHandler()
void CUser::CyberACS_PowerUpStore()
{
	uint32 sItemCount = g_pMain->m_CyberACSPusItemArray.GetSize();
	if (sItemCount == 0)
		return;

	Packet result(uint8(PL_PUS));
	result << uint32(sItemCount);
	foreach_stlmap(itr, g_pMain->m_CyberACSPusItemArray)
		result << uint32(itr->second->sItemID) << uint32(itr->second->sPrice) << uint8(itr->second->sType) << uint32(itr->second->sBuyCount);
	Send(&result);
}
#pragma endregion

#pragma region CUser::CyberACS_UIRequest(Packet& pkt)
void CUser::CyberACS_UIRequest(Packet& pkt)
{
	int moneyReq = 0;
	if (GetPremium() == WAR_Premium)
		moneyReq = 0;
	else
	{
		moneyReq = (int)pow((GetLevel() * 2.0f), 3.4f);
		if (GetLevel() < 30)
			moneyReq = (int)(moneyReq * 0.4f);
		else if (GetLevel() >= 60)
			moneyReq = (int)(moneyReq * 1.5f);

		if ((g_pMain->m_sDiscount == 1
			&& g_pMain->m_byOldVictory == GetNation())
			|| g_pMain->m_sDiscount == 2)
			moneyReq /= 2;
	}
	Packet result(PL_UIINFO);
	result << uint32(m_nKnightCash)
		<< uint32(m_nKnightCashBonus)
		<< uint16(m_sDaggerR)
		<< uint16(m_sAxeR)
		<< uint16(m_sSwordR)
		<< uint16(m_sMaceR)
		<< uint16(m_sSpearR)
		<< uint16(m_sBowR)
		<< uint16(m_sJamadarR)
		<< uint32(moneyReq)
		<< GetSocketID()
		<< GetName()
		<< GetClass()
		<< GetRace()
		<< GetLevel()
		<< GetRebirthLevel()
		<< GetNation()
		<< m_bStats[0]
		<< m_bStats[1]
		<< m_bStats[2]
		<< m_bStats[3]
		<< m_bStats[4];
	Send(&result);


	result << uint8(PL_USERDATA)
		<< GetSocketID()
		<< GetName()
		<< GetClass()
		<< GetRace()
		<< GetLevel()
		<< GetRebirthLevel()
		<< GetNation()
		<< m_iMaxExp
		<< m_iExp
		<< m_iMannerPoint
		<< uint16(m_bStats[0])
		<< uint16(m_bStats[1])
		<< uint16(m_bStats[2])
		<< uint16(m_bStats[3])
		<< uint16(m_bStats[4])
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

	CyberACS_PowerUpStoreRequest((Packet)(NULL));


	result << uint8(PL_TEMPITEMS);
	CyberACS_SendTempItems(result);


	uint8 otp = g_DBAgent.NDPasswordCheck(GetAccountName());
	if (otp == 1)
	{
		Packet passwrd(PL_2NDPASSWORD);
		passwrd << uint8(1);
		Send(&passwrd);
	}


}
#pragma region

#pragma region CUser::CyberACS_AllSkillStatRestard(Packet & pkt)
void CUser::CyberACS_AllSkillStatRestard(Packet& pkt)
{
	uint8 sSubCode;
	pkt >> sSubCode;
	if (isMerchanting()
		|| isTrading()
		|| isStoreOpen()
		|| isFishing()
		|| isMining()
		|| isDead()
		|| !isInGame())
		return;

	if (GetZoneID() == ZONE_KNIGHT_ROYALE
		|| GetZoneID() == ZONE_PRISON
		|| GetZoneID() == ZONE_CHAOS_DUNGEON
		|| GetZoneID() == ZONE_DUNGEON_DEFENCE
		|| GetZoneID() == ZONE_DEATHMATCH)
		return;

	switch (sSubCode)
	{
	case 1:
	{
		uint8 point[4];
		for (int i = 0; i < 4; i++)
			pkt >> point[i];

		uint8 ceilPoint[4];

		if (isWarrior()
			|| isPortuKurian())
		{
			ceilPoint[0] = 80;
			ceilPoint[1] = 80;
			ceilPoint[2] = 83;
		}
		else if (isRogue())
		{
			ceilPoint[0] = 80;
			ceilPoint[1] = 80;
			ceilPoint[2] = 80;
		}
		else if (isMage())
		{
			ceilPoint[0] = 80;
			ceilPoint[1] = 80;
			ceilPoint[2] = 80;
		}
		else if (isPriest())
		{
			ceilPoint[0] = 80;
			ceilPoint[1] = 80;
			ceilPoint[2] = 80;
		}

		if (isMastered())
			ceilPoint[3] = 23;
		else
			ceilPoint[3] = 0;

		for (int i = 0; i < 4; i++) {
			if (point[i] > ceilPoint[i])
				return;
		}

		uint16 usedPoints = 0;
		uint16 maxPoints = (GetLevel() - 9) * 2;

		for (int i = 0; i < 4; i++)
			usedPoints += point[i];

		if (usedPoints > maxPoints)
			return;

		Packet result(WIZ_CLASS_CHANGE, uint8(ALL_SKILLPT_CHANGE));
		int index = 0, skill_point = 0, money = 0, temp_value = 0, old_money = 0;
		uint8 type = 0;

		temp_value = (int)pow((GetLevel() * 2.0f), 3.4f);
		if (GetLevel() < 30)
			temp_value = (int)(temp_value * 0.4f);
		else if (GetLevel() >= 60)
			temp_value = (int)(temp_value * 1.5f);

		// If global discounts are enabled 
		if (g_pMain->m_sDiscount == 2 // or war discounts are enabled
			|| (g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == m_bNation))
			temp_value /= 2;

		// Level too low.
		if (GetLevel() < 10)
			goto fail_return_skill;

		if (GetPremium() == WAR_Premium
			|| GetPremium() == Royal_Premium)
			temp_value = 0;

		// Get total skill points
		for (int i = 0; i < 9; i++)
			skill_point += m_bstrSkill[i];

		// If we don't have any skill points, there's no point resetting now is there.
		if (skill_point <= 0)
		{
			type = 2;
			goto fail_return_skill;
		}

		// Not enough money.
		if (!GoldLose(temp_value, false))
			goto fail_return_skill;

		// Reset skill points.
		m_bstrSkill[0] = maxPoints - usedPoints;
		for (int i = 1; i < 9; i++)
			m_bstrSkill[i] = 0;

		for (int i = 0; i < 4; i++)
			m_bstrSkill[i + 5] = point[i];

		result << uint8(1) << GetCoins() << m_bstrSkill[0];
		Send(&result);

		ResetWindows();
		InitType4();
		RecastSavedMagic();
		SendMyInfo();
		SetUserAbility();
		g_pMain->RegionUserInOutForMe(this);
		g_pMain->RegionNpcInfoForMe(this);

		return;
	fail_return_skill:
		result << type << temp_value;
		Send(&result);
	}break;
	case 2: {
		uint8 point[5];
		for (int i = 0; i < 5; i++)
			pkt >> point[i];

		uint8 basePoint[5];
		basePoint[0] = 50;
		basePoint[1] = 60;
		basePoint[2] = 60;
		basePoint[3] = 50;
		basePoint[4] = 50;

		if (isPriest())
			basePoint[4] += 20;
		else if (isWarrior()
			|| isPortuKurian())
		{
			basePoint[0] += 15;
			basePoint[1] += 5;
		}
		else if (isMage())
		{
			basePoint[2] += 10;
			basePoint[4] += 20;
			basePoint[1] -= 10;
		}
		else if (isRogue())
		{
			basePoint[0] += 10;
			basePoint[2] += 10;
		}

		uint16 totalMinus = 0;

		for (int i = 0; i < 5; i++)
		{
			if (point[i] < basePoint[i])
				point[i] = basePoint[i];
			totalMinus += point[i] - basePoint[i];
		}

		uint16 freePoint = 10 + (GetLevel() - 1) * 3;

		if (GetLevel() > 60)
			freePoint += 2 * (GetLevel() - 60);

		if (totalMinus > freePoint)
			return;

		freePoint -= totalMinus;

		Packet result(WIZ_CLASS_CHANGE, uint8(ALL_POINT_CHANGE));
		int temp_money;
		uint16 statTotal;

		uint16 byStr, bySta, byDex, byInt, byCha;
		uint8 bResult = 0;

		bool bIsFree = false;

		if (GetLevel() > g_pMain->m_byMaxLevel)
			goto fail_return;

		if (GetPremium() == WAR_Premium
			|| GetPremium() == Royal_Premium)
			bIsFree = true;

		temp_money = (int)pow((GetLevel() * 2.0f), 3.4f);
		if (GetLevel() < 30)
			temp_money = (int)(temp_money * 0.4f);
		else if (GetLevel() >= 60)
			temp_money = (int)(temp_money * 1.5f);

		if ((g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == GetNation())
			|| g_pMain->m_sDiscount == 2)
			temp_money /= 2;

		for (int i = 0; i < SLOT_MAX; i++)
		{
			if (m_sItemArray[i].nNum) {
				bResult = 4;
				goto fail_return;
			}
		}

		bool allDefault = true;

		for (int j = 0; j < 5; j++)
			if (point[j] != basePoint[j])
				allDefault = false;

		if (allDefault
			&& GetStatTotal() == 290)
		{
			bResult = 2;
			goto fail_return;
		}

		if (GetPremium() == WAR_Premium
			|| GetPremium() == Royal_Premium)
			temp_money = 0;

		if (!bIsFree && !GoldLose(temp_money, false))
			goto fail_return;

		SetStat(STAT_STR, point[0]);
		SetStat(STAT_STA, point[1]);
		SetStat(STAT_DEX, point[2]);
		SetStat(STAT_CHA, point[3]);
		SetStat(STAT_INT, point[4]);

		statTotal = GetStatTotal();
		SetUserAbility();

		byStr = GetStat(STAT_STR);
		bySta = GetStat(STAT_STA),
			byDex = GetStat(STAT_DEX);
		byInt = GetStat(STAT_INT),
			byCha = GetStat(STAT_CHA);

		m_sPoints = freePoint;

		result << uint8(1)
			<< GetCoins()
			<< byStr << bySta << byDex << byInt << byCha
			<< m_MaxHp << m_MaxMp << m_sTotalHit << m_sMaxWeight << m_sPoints;
		Send(&result);
		return;

	fail_return:
		result << bResult << temp_money;
		Send(&result);

	}break;
	default:
		break;
	}
}
#pragma endregion

#pragma region CUser::CyberACS_ItemNotice(uint8 type, uint32 nItemID)
void CUser::CyberACS_ItemNotice(uint8 type, uint32 nItemID)
{
	Packet result(PL_AUTODROP);
	result << type << nItemID;
	Send(&result);
}
#pragma endregion

#pragma region CUser::CyberACS_MerchantHandler(Packet& pkt)
void CUser::CyberACS_MerchantHandler(Packet& pkt)
{
	uint8 subCode;
	pkt >> subCode;

	if (!isSellingMerchantingPreparing()
		|| isBuyingMerchantingPreparing())
		return;

	switch (subCode)
	{
	case MERCHANT_ITEM_ADD:
		CyberACS_MerchantItemAdd(pkt);
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region CUser::CyberACS_MerchantItemAdd(Packet& pkt)
void CUser::CyberACS_MerchantItemAdd(Packet& pkt)
{
	if (!isSellingMerchantingPreparing()
		|| isBuyingMerchantingPreparing())
		return;

	Packet result(WIZ_MERCHANT, uint8(MERCHANT_ITEM_ADD));
	uint32 nGold, nItemID;
	uint16 sCount;
	uint8 bSrcPos, // It sends the "actual" inventory slot (SLOT_MAX -> INVENTORY_MAX-SLOT_MAX), so need to allow for it. 
		bDstPos,
		bMode, // Might be a flag for normal / "premium" merchant mode, once skills are implemented take another look at this.
		isKC;

	pkt >> nItemID >> sCount >> nGold >> bSrcPos >> bDstPos >> bMode >> isKC;
	// TODO: Implement the possible error codes for these various error cases.
	if (bSrcPos >= HAVE_MAX
		|| bDstPos >= MAX_MERCH_ITEMS)
		return;

	_ITEM_TABLE* pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable == nullptr
		|| nItemID >= ITEM_NO_TRADE_MIN && nItemID <= ITEM_NO_TRADE_MAX // Cannot be traded, sold or stored.
		|| pTable->m_bRace == RACE_UNTRADEABLE // Cannot be traded or sold.
		|| (pTable->m_bKind == 255 && sCount != 1)) // Cannot be traded or sold.
		goto fail_return;

	bSrcPos += SLOT_MAX;
	_ITEM_DATA* pSrcItem = GetItem(bSrcPos);
	if (pSrcItem == nullptr
		|| pSrcItem->nNum != nItemID
		|| pSrcItem->sCount == 0
		|| sCount == 0
		|| pSrcItem->sCount < sCount
		|| pSrcItem->isRented()
		|| pSrcItem->isSealed()
		|| pSrcItem->isBound()
		|| pSrcItem->isDuplicate()
		|| pSrcItem->isExpirationTime())
		goto fail_return;

	_MERCH_DATA* pMerch = &m_arMerchantItems[bDstPos];

	if (pMerch == nullptr)
		return;

	pMerch->nNum = nItemID;
	pMerch->nPrice = nGold;
	pMerch->sCount = sCount; // Selling Count
	if (pTable->m_bKind == 255)
		pMerch->bCount = sCount; // Original Count ( INVENTORY )
	else
		pMerch->bCount = pSrcItem->sCount; // Original Count ( INVENTORY )

	pMerch->sDuration = pSrcItem->sDuration;
	pMerch->nSerialNum = pSrcItem->nSerialNum; // NOTE: Stackable items will have an issue with this.
	pMerch->bOriginalSlot = bSrcPos;
	pMerch->isKC = isKC;
	pMerch->IsSoldOut = false;

	// Take the user's item.
	//memset(pSrcItem, 0, sizeof(_ITEM_DATA));  // merchant reloads bug deneme

	result << uint16(1)
		<< nItemID << sCount << pMerch->sDuration << nGold
		<< bSrcPos << bDstPos << isKC;
	Send(&result);

fail_return:
	result << uint16(0)
		<< nItemID << sCount << (uint16)bSrcPos + bDstPos << nGold
		<< bSrcPos << bDstPos << isKC;
	Send(&result);
}
#pragma endregion

#pragma region CUser::CyberACSMonsterInfoHandler(Packet & pkt)
void CUser::CyberACS_DropRequest(Packet& pkt)
{
	uint8 command;
	Packet result;
	pkt >> command;
	switch (command)
	{
	case 1:
	{
		uint32 sTraget;
		pkt >> sTraget;
		CNpc* pNpc = g_pMain->GetNpcPtr(GetTargetID(), GetZoneID());
		if (pNpc == nullptr)
			return;

		_K_MONSTER_ITEM* pMonsterItem = g_pMain->m_MonsterItemArray.GetData(pNpc->GetProtoID());
		if (pMonsterItem == nullptr)
			return;

		/*Packet result2(PL_DROP_REQUEST);
		result2 << command;
		*/

		Packet result1(PL_DROP_REQUEST);
		result1 << command
			<< pNpc->GetProtoID()
			<< pMonsterItem->iItem[0]
			<< pMonsterItem->sPercent[0]
			<< pMonsterItem->iItem[1]
			<< pMonsterItem->sPercent[1]
			<< pMonsterItem->iItem[2]
			<< pMonsterItem->sPercent[2]
			<< pMonsterItem->iItem[3]
			<< pMonsterItem->sPercent[3]
			<< pMonsterItem->iItem[4]
			<< pMonsterItem->sPercent[4]
			<< pMonsterItem->iItem[5]
			<< pMonsterItem->sPercent[5];
		Send(&result1);



	}break;
	case 2:
	{

		uint32 sGropID;
		uint8 sCount = 0;
		pkt >> sGropID;
		_MAKE_ITEM_GROUP* pItemGrops = g_pMain->m_MakeItemGroupArray.GetData(sGropID);
		if (pItemGrops == nullptr)
			return;


		Packet result2(PL_DROP_REQUEST);
		result2 << command
			<< uint8(30);

		for (uint32 itemID : pItemGrops->iItems)
		{
			if (sCount > 29)
				continue;

			result2 << itemID;
			sCount++;
		}
		Send(&result2);
	}break;
	case 3:
	{
		uint16 sProtoID;
		pkt >> sProtoID;

		_K_MONSTER_ITEM* pMonsterItems = g_pMain->m_MonsterItemArray.GetData(sProtoID);
		if (pMonsterItems == nullptr)
			return;

		Packet result3(PL_DROP_REQUEST);
		result3 << uint8(1) << sProtoID;


		result3 << pMonsterItems->iItem[0]
			<< pMonsterItems->sPercent[0]
			<< pMonsterItems->iItem[1]
			<< pMonsterItems->sPercent[1]
			<< pMonsterItems->iItem[2]
			<< pMonsterItems->sPercent[2]
			<< pMonsterItems->iItem[3]
			<< pMonsterItems->sPercent[3]
			<< pMonsterItems->iItem[4]
			<< pMonsterItems->sPercent[4]
			<< pMonsterItems->iItem[5]
			<< pMonsterItems->sPercent[5];

		Send(&result3);

	}break;
	case 4:
	{
		string ItemName;
		pkt >> ItemName;


		if (g_pMain->m_SearchItemArrays.GetSize() > 0)
			g_pMain->m_SearchItemArrays.DeleteAllData();



		g_DBAgent.LoadSearchItem(ItemName);


		Packet result(PL_DROP_REQUEST);
		result << uint8(3) << g_pMain->m_SearchItemArrays.GetSize();

		foreach_stlmap(itr, g_pMain->m_SearchItemArrays) {
			result << itr->second->ID << itr->second->Name << itr->second->ZoneName << itr->second->ZoneNo << itr->second->LeftX << itr->second->RightX;
			Send(&result);
		}
	}
	break;


	default:
		break;
	}
}
#pragma endregion

/**
* @brief	Sends the server index.
*/
void CUser::SendServerIndex()
{
	Packet result(WIZ_SERVER_INDEX);
	result << uint16(1) << uint16(g_pMain->m_nServerNo);
	Send(&result);
}

/**
* @brief	Initiates a database request to save the character's information.
*/
void CUser::UserDataSaveToAgent()
{
	if (!isInGame())
		return;

	Packet result(WIZ_DATASAVE);
	result << GetAccountName() << GetName();
	g_pMain->AddDatabaseRequest(result, this);
}

/**
* @brief	Logs a player out.
*/
void CUser::LogOut()
{
	if (m_strUserID.empty())
		return;
	if (m_bAuthority == AUTHORITY_GM_USER)
		m_bAuthority = AUTHORITY_GAME_MASTER;

	/* DeathmatchLogOut */
	if (m_sInDeathMatchEvent)
		g_pMain->DeathMatchLogOut(this);
	/* DeathmatchLogOut */

	if (hasPriestBot == true)
	{
		CBot* pPriest = nullptr;
		pPriest = g_pMain->m_BotArray.GetData(m_bUserPriestBotID);
		pPriest->UserInOut(INOUT_OUT);
		m_bUserPriestBotID = -1;
		hasPriestBot = false;
	}

	if (m_bCharacterDataLoaded)
	{
		Packet result;
		UserInOut(INOUT_OUT);
		result.Initialize(WIZ_LOGOUT);
		m_deleted = true; // make this session unusable until the logout is complete
		m_bIsLoggingOut = true;
		g_pMain->AddDatabaseRequest(result, this);
	}
}

/**
* @brief	Sends the server time.
*/
void CUser::SendTime()
{
	Packet result(WIZ_TIME);
	result << uint16(g_pMain->m_sYear) << uint16(g_pMain->m_sMonth) << uint16(g_pMain->m_sDate)
		<< uint16(g_pMain->m_sHour) << uint16(g_pMain->m_sMin);
	Send(&result);
}

/**
* @brief	Sends the weather status.
*/
void CUser::SendWeather()
{
	Packet result(WIZ_WEATHER);
	result << g_pMain->m_byWeather << g_pMain->m_sWeatherAmount;
	Send(&result);
}

/**
* @brief	Sets various zone flags to control how
* 			the client handles other players/NPCs.
* 			Also sends the zone's current tax rate.
*/
void CUser::SetZoneAbilityChange(uint16 sNewZone)
{
	C3DMap* pMap = g_pMain->GetZoneByID(sNewZone);
	_KNIGHTS_SIEGE_WARFARE* pSiegeWar = g_pMain->GetSiegeMasterKnightsPtr(1);
	CKingSystem* pKingSystem = g_pMain->m_KingSystemArray.GetData(GetNation());

	if (pMap == nullptr)
		return;

	switch (sNewZone)
	{
	case ZONE_KARUS:
	case ZONE_KARUS2:
	case ZONE_KARUS3:
	case ZONE_ELMORAD:
	case ZONE_ELMORAD2:
	case ZONE_ELMORAD3:
	case ZONE_KARUS_ESLANT:
	case ZONE_KARUS_ESLANT2:
	case ZONE_KARUS_ESLANT3:
	case ZONE_ELMORAD_ESLANT:
	case ZONE_ELMORAD_ESLANT2:
	case ZONE_ELMORAD_ESLANT3:
	case ZONE_BIFROST:
	case ZONE_BATTLE:
	case ZONE_BATTLE2:
	case ZONE_BATTLE3:
	case ZONE_BATTLE4:
	case ZONE_BATTLE5:
	case ZONE_BATTLE6:
	case ZONE_SNOW_BATTLE:
	case ZONE_RONARK_LAND:
	case ZONE_ARDREAM:
	case ZONE_RONARK_LAND_BASE:
	case ZONE_KROWAZ_DOMINION:
	case ZONE_STONE1:
	case ZONE_STONE2:
	case ZONE_STONE3:
	case ZONE_BORDER_DEFENSE_WAR:
	case ZONE_UNDER_CASTLE:
	case ZONE_JURAID_MOUNTAIN:
	case ZONE_PARTY_VS_1:
	case ZONE_PARTY_VS_2:
	case ZONE_PARTY_VS_3:
	case ZONE_PARTY_VS_4:
	case ZONE_CLAN_WAR_ARDREAM:
	case ZONE_CLAN_WAR_RONARK:
	case ZONE_KNIGHT_ROYALE:
	case ZONE_CHAOS_DUNGEON:
	case ZONE_DEATHMATCH:
		if (pKingSystem != nullptr)
			pMap->SetTariff(10 + pKingSystem->m_nTerritoryTariff);
		else
			pMap->SetTariff(10);
		break;
	case ZONE_MORADON:
	case ZONE_MORADON2:
	case ZONE_MORADON3:
	case ZONE_MORADON4:
	case ZONE_MORADON5:
	case ZONE_OLDMORADON:
	case ZONE_ARENA:
		if (pSiegeWar != nullptr)
			pMap->SetTariff((uint8)pSiegeWar->sMoradonTariff);
		else
			pMap->SetTariff(10);
		break;
	case ZONE_DELOS:
	case ZONE_DESPERATION_ABYSS:
	case ZONE_HELL_ABYSS:
	case ZONE_DELOS_CASTELLAN:
		if (pSiegeWar != nullptr)
			pMap->SetTariff((uint8)pSiegeWar->sDellosTariff);
		else
			pMap->SetTariff(10);
		break;
	default:
		printf("King and Deos Tariff unhandled zone %d \n", sNewZone);
		TRACE("King and Deos Tariff unhandled zone %d \n", sNewZone);
		break;
	}

	Packet result(WIZ_ZONEABILITY, uint8(1));

	result << pMap->canTradeWithOtherNation()
		<< pMap->GetZoneType()
		<< pMap->canTalkToOtherNation()
		<< uint16(pMap->GetTariff());

	Send(&result);

	if (sNewZone == ZONE_ARDREAM
		|| sNewZone == ZONE_RONARK_LAND
		|| sNewZone == ZONE_RONARK_LAND_BASE
		|| sNewZone == ZONE_CLAN_WAR_ARDREAM
		|| sNewZone == ZONE_CLAN_WAR_RONARK
		|| GetZoneID() == ZONE_ARDREAM
		|| GetZoneID() == ZONE_RONARK_LAND
		|| GetZoneID() == ZONE_RONARK_LAND_BASE
		|| GetZoneID() == ZONE_CLAN_WAR_ARDREAM
		|| GetZoneID() == ZONE_CLAN_WAR_RONARK)
		PlayerKillingAddPlayerRank();
	else
		PlayerKillingRemovePlayerRank();

	if (g_pMain->m_byBattleOpen == NATION_BATTLE
		&& g_pMain->m_byBattleZone + ZONE_BATTLE_BASE != ZONE_BATTLE3)
	{
		result.Initialize(WIZ_MAP_EVENT);
		result << uint8(GOLDSHELL) << uint8(true) << GetSocketID();
		Send(&result);
	}

	g_pMain->KillNpc(GetSocketID(), GetZoneID(), this);

	if (isSiegeTransformation())
	{
		_MAGIC_TYPE6* pType = g_pMain->m_Magictype6Array.GetData(m_bAbnormalType);

		m_sMaxHPAmount = 0;
		m_sMaxMPAmount = 0;

		if (pType != nullptr)
		{
			if (pType->sTotalAc == 0 && pType->sTotalAc > 0)
				m_sACPercent -= (pType->sTotalAc - 100);
			else
				m_sACAmount -= pType->sTotalAc;
		}
		result.Initialize(WIZ_MAGIC_PROCESS);
		result << uint8(MAGIC_CANCEL_TRANSFORMATION);

		m_transformationType = TransformationNone;
		m_sTransformHpchange = false;
		m_sTransformMpchange = false;
		Send(&result);

		SetUserAbility();
		RemoveSavedMagic(m_bAbnormalType);
		StateChangeServerDirect(3, ABNORMAL_NORMAL);
	}

	if (sNewZone == ZONE_BATTLE4
		|| sNewZone == ZONE_BATTLE5)
		g_pMain->SendEventRemainingTime(false, this, (uint8)sNewZone);

	if (sNewZone == ZONE_BIFROST
		|| GetZoneID() == ZONE_BIFROST
		|| sNewZone == ZONE_RONARK_LAND
		|| GetZoneID() == ZONE_RONARK_LAND)
		BeefEventGetTime();

	// Clear skill cooldowns...
	m_RHitRepeatList.clear();
	m_CoolDownList.clear();
	m_SkillCastList.clear();
	m_MagicTypeCooldownList.clear();
}


void CUser::PlayerEffect(uint16 sEffect)
{
	Packet result(WIZ_MINING, uint8(MiningAttempt));
	uint16 resultCode = MiningResultSuccess;
	result << resultCode << GetID() << sEffect;
	SendToRegion(&result);
}

/**
* @brief	Requests user info for the specified session IDs.
*
* @param	pkt	The packet.
*/
void CUser::RequestUserIn(Packet& pkt)
{
	Packet result(WIZ_REQ_USERIN);
	short user_count = pkt.read<uint16>(), online_count = 0;
	if (user_count > 1000)
		user_count = 1000;

	result << uint16(0); // placeholder for user count

	for (int i = 0; i < user_count; i++)
	{
		uint16 nid = pkt.read<uint16>();

		if (nid < MAX_USER)
		{
			CUser* pUser = g_pMain->GetUserPtr(nid);
			if (pUser == nullptr
				|| !pUser->isInGame())
				continue;

			if (GetEventRoom() >= 0
				&& pUser->GetEventRoom() != GetEventRoom())
				continue;

			if (pUser->m_bAbnormalType == ABNORMAL_INVISIBLE)
				continue;

			result << uint8(0) << pUser->GetSocketID();
			pUser->GetUserInfo(result);
		}
		else
		{
			CBot* pUser = g_pMain->m_BotArray.GetData(nid);
			if (pUser == nullptr
				|| !pUser->isInGame())
				continue;

			result << uint8(0) << pUser->GetID();
			pUser->GetUserInfo(result);
		}
		online_count++;
	}

	result.put(0, online_count); // substitute count in
	SendCompressed(&result);
}

/**
* @brief	Request NPC info for the specified NPC IDs.
*
* @param	pkt	The packet.
*/
void CUser::RequestNpcIn(Packet& pkt)
{
	Packet result(WIZ_REQ_NPCIN);
	uint16 npc_count = pkt.read<uint16>();
	if (npc_count > 1000)
		npc_count = 1000;

	result << uint16(0); // NPC count placeholder

	CKnights* pKnights = g_pMain->GetClanPtr(GetClanID());
	_KNIGHTS_SIEGE_WARFARE* pSiegeWars = g_pMain->GetSiegeMasterKnightsPtr(Aktive);

	for (int i = 0; i < npc_count; i++)
	{
		uint16 NpcID = pkt.read<uint16>();

		if (NpcID < NPC_BAND || NpcID > INVALID_BAND)
			continue;

		CNpc* pNpc = g_pMain->GetNpcPtr(NpcID, GetZoneID());
		if (pNpc == nullptr
			|| pNpc->isDead())
			continue;

		if (GetEventRoom() >= 0
			&& pNpc->GetEventRoom() != GetEventRoom())
			continue;

		if ((pNpc->GetProtoID() == 541
			&& pNpc->GetType() == NPC_DESTROYED_ARTIFACT
			&& pNpc->GetZoneID() == ZONE_DELOS)
			|| (pNpc->GetType() == NPC_GATE
				&& pNpc->GetZoneID() == ZONE_DELOS
				&& (pNpc->GetProtoID() == 561
					|| pNpc->GetProtoID() == 562
					|| pNpc->GetProtoID() == 563)))
		{
			if (pKnights != nullptr && pSiegeWars != nullptr)
			{
				if (g_pMain->m_byBattleOpen == SIEGE_BATTLE)
				{
					if (pSiegeWars->sMasterKnights == 0)
						pNpc->m_bNation = 0;
					else if (pSiegeWars->sMasterKnights == pKnights->GetAllianceID()
						|| pSiegeWars->sMasterKnights == pKnights->GetID())
						pNpc->m_bNation = 3;
					else
						pNpc->m_bNation = 0;
				}
				else
					pNpc->m_bNation = 3;
			}
			else
				pNpc->m_bNation = 3;
		}

		result << pNpc->GetID();
		pNpc->GetNpcInfo(result);
	}

	result.put(0, npc_count);
	SendCompressed(&result);
}

/**
* @brief	Calculates & resets item stats/bonuses.
*/
void CUser::SetSlotItemValue()
{
	_ITEM_TABLE* pTable = nullptr;
	int item_hit = 0, item_ac = 0;

	m_sItemMaxHp = m_sItemMaxMp = 0;
	m_sItemAc = 0;
	m_sItemWeight = m_sMaxWeightBonus = 0;
	m_sItemHitrate = m_sItemEvasionrate = 100;

	memset(m_sStatItemBonuses, 0, sizeof(uint16) * STAT_COUNT);
	m_sFireR = m_sColdR = m_sLightningR = m_sMagicR = m_sDiseaseR = m_sPoisonR = 0;
	m_sDaggerR = m_sSwordR = m_sJamadarR = m_sAxeR = m_sMaceR = m_sSpearR = m_sBowR = 0;

	m_byAPBonusAmount = 0;
	memset(&m_byAPClassBonusAmount, 0, sizeof(m_byAPClassBonusAmount));
	memset(&m_byAcClassBonusAmount, 0, sizeof(m_byAcClassBonusAmount));

	m_bItemExpGainAmount = m_bItemNPBonus = m_bItemNoahGainAmount = 0;

	Guard lock(m_equippedItemBonusLock);
	m_equippedItemBonuses.clear();

	map<uint16, uint32> setItems;

	// Apply stat bonuses from all equipped & cospre items.
	// Total up the weight of all items.
	for (int i = 0; i < INVENTORY_TOTAL; i++)
	{
		_ITEM_DATA* pItem = nullptr;
		pTable = GetItemPrototype(i, pItem);

		if (pTable == nullptr)
			continue;

		// Bags increase max weight, they do not weigh anything.
		if (i == INVENTORY_COSP + COSP_BAG1 + 1
			|| i == INVENTORY_COSP + COSP_BAG2 + 1)
		{
			m_sMaxWeightBonus += pTable->m_sDuration;
		}
		// All other items are attributed to the total weight of items in our inventory.
		else
		{
			if (!pTable->m_bCountable)
				m_sItemWeight += pTable->m_sWeight;
			else
				// Non-stackable items should have a count of 1. If not, something's broken.
				m_sItemWeight += pTable->m_sWeight * pItem->sCount;
		}

		// Do not apply stats to unequipped items
		if ((i >= SLOT_MAX && i < INVENTORY_COSP)
			// or disabled weapons.
			|| (isWeaponsDisabled()
				&& (i == (GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND)
					|| i == (GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND))
				&& !pTable->isShield())
			// or items in magic bags.
			|| i >= INVENTORY_MBAG
			|| pItem->isDuplicate())
			continue;

		item_ac = pTable->m_sAc;
		if (pItem->sDuration == 0)
			item_ac /= 10;

		m_sItemMaxHp += pTable->m_MaxHpB;
		m_sItemMaxMp += pTable->m_MaxMpB;
		m_sItemAc += item_ac;
		m_sStatItemBonuses[STAT_STR] += pTable->m_sStrB;
		m_sStatItemBonuses[STAT_STA] += pTable->m_sStaB;
		m_sStatItemBonuses[STAT_DEX] += pTable->m_sDexB;
		m_sStatItemBonuses[STAT_INT] += pTable->m_sIntelB;
		m_sStatItemBonuses[STAT_CHA] += pTable->m_sChaB;
		m_sItemHitrate += pTable->m_sHitrate;
		m_sItemEvasionrate += pTable->m_sEvarate;

		m_sFireR += pTable->m_bFireR;
		m_sColdR += pTable->m_bColdR;
		m_sLightningR += pTable->m_bLightningR;
		m_sMagicR += pTable->m_bMagicR;
		m_sDiseaseR += pTable->m_bCurseR;
		m_sPoisonR += pTable->m_bPoisonR;

		m_sDaggerR += pTable->m_sDaggerAc;
		m_sJamadarR += pTable->m_JamadarAc;
		m_sSwordR += pTable->m_sSwordAc;
		m_sAxeR += pTable->m_sAxeAc;
		m_sMaceR += pTable->m_sMaceAc;
		m_sSpearR += pTable->m_sSpearAc;
		m_sBowR += pTable->m_sBowAc;

		ItemBonusMap bonusMap;
		if (pTable->m_bFireDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_FIRE, pTable->m_bFireDamage));

		if (pTable->m_bIceDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_COLD, pTable->m_bIceDamage));

		if (pTable->m_bLightningDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_LIGHTNING, pTable->m_bLightningDamage));

		if (pTable->m_bPoisonDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_POISON, pTable->m_bPoisonDamage));

		if (pTable->m_bHPDrain)
			bonusMap.insert(std::make_pair(ITEM_TYPE_HP_DRAIN, pTable->m_bHPDrain));

		if (pTable->m_bMPDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_MP_DAMAGE, pTable->m_bMPDamage));

		if (pTable->m_bMPDrain)
			bonusMap.insert(std::make_pair(ITEM_TYPE_MP_DRAIN, pTable->m_bMPDrain));

		if (pTable->m_bMirrorDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_MIRROR_DAMAGE, pTable->m_bMirrorDamage));

		// If we have bonuses to apply, store them.
		if (!bonusMap.empty())
			m_equippedItemBonuses[i] = bonusMap;

		// Apply cospre item stats
		if (pTable->GetKind() == ITEM_KIND_COSPRE)
		{
			// If this item exists in the set table, it has bonuses to be applied.
			_SET_ITEM* pSetItem = g_pMain->m_SetItemArray.GetData(pTable->m_iNum);

			if (pSetItem != nullptr)
				ApplySetItemBonuses(pSetItem);
		}

		// All set items start with race over 100
		if (pTable->m_bRace < 100)
			continue;

		// Each set is uniquely identified by item's race
		auto itr = setItems.find(pTable->m_bRace);

		// If the item doesn't exist in our map yet...
		if (itr == setItems.end())
		{
			// Generate the base set ID and insert it into our map
			setItems.insert(make_pair(pTable->m_bRace, pTable->m_bRace * 10000));
			itr = setItems.find(pTable->m_bRace);
		}

		// Update the final set ID depending on the equipped set item 
		switch (pTable->m_bSlot)
		{
		case ItemSlotHelmet:
			itr->second += 2;
			break;
		case ItemSlotPauldron:
			itr->second += 16;
			break;
		case ItemSlotPads:
			itr->second += 512;
			break;
		case ItemSlotGloves:
			itr->second += 2048;
			break;
		case ItemSlotBoots:
			itr->second += 4096;
			break;
		}
	}

	// Now we can add up all the set bonuses, if any.
	foreach(itr, setItems)
	{
		// Test if this set item exists (if we're not using at least 2 items from the set, this will fail)
		_SET_ITEM* pItem = g_pMain->m_SetItemArray.GetData(itr->second);

		if (pItem == nullptr)
			continue;

		ApplySetItemBonuses(pItem);
	}

	if (m_sAddArmourAc > 0)
		m_sItemAc += m_sAddArmourAc;
	else
		m_sItemAc = m_sItemAc * m_bPctArmourAc / 100;
}


void CUser::ApplySetItemBonuses(_SET_ITEM* pItem)
{
	m_sItemAc += pItem->ACBonus;
	m_sItemMaxHp += pItem->HPBonus;
	m_sItemMaxMp += pItem->MPBonus;

	m_sStatItemBonuses[STAT_STR] += pItem->StrengthBonus;
	m_sStatItemBonuses[STAT_STA] += pItem->StaminaBonus;
	m_sStatItemBonuses[STAT_DEX] += pItem->DexterityBonus;
	m_sStatItemBonuses[STAT_INT] += pItem->IntelBonus;
	m_sStatItemBonuses[STAT_CHA] += pItem->CharismaBonus;

	m_sFireR += pItem->FlameResistance;
	m_sColdR += pItem->GlacierResistance;
	m_sLightningR += pItem->LightningResistance;
	m_sMagicR += pItem->MagicResistance;
	m_sDiseaseR += pItem->CurseResistance;
	m_sPoisonR += pItem->PoisonResistance;

	m_bItemExpGainAmount += pItem->XPBonusPercent;
	m_bItemNoahGainAmount += pItem->CoinBonusPercent;
	m_bItemNPBonus += pItem->NPBonus;

	m_sMaxWeightBonus += pItem->MaxWeightBonus;

	// NOTE: The following percentages use values such as 3 to indicate +3% (not the typical 103%).
	// Also note that at this time, there are no negative values used, so we can assume it's always a bonus.
	m_byAPBonusAmount += pItem->APBonusPercent;
	if (pItem->APBonusClassType >= 1 && pItem->APBonusClassType <= 4)
		m_byAPClassBonusAmount[pItem->APBonusClassType - 1] += pItem->APBonusClassPercent;

	if (pItem->ACBonusClassType >= 1 && pItem->ACBonusClassType <= 4)
		m_byAcClassBonusAmount[pItem->ACBonusClassType - 1] += pItem->ACBonusClassPercent;
}

void CUser::RecvUserExp(CNpc* pNpc, uint32 iDamage, uint32 iTotalDamage)
{
	if (pNpc == nullptr
		|| !isInRange(pNpc, RANGE_50M))
		return;

	switch (pNpc->GetProtoID())
	{
	case 4301:
	case 4351:
		if (!VaccuniBoxExp(1))
			return;
		break;
	case 616:
	case 666:
		if (!VaccuniBoxExp(2))
			return;
		break;
	case 605:
	case 611:
	case 655:
		if (!VaccuniBoxExp(3))
			return;
		break;
	}

	int32 iNpcExp = pNpc->GetProto()->m_iExp;
	int32 iNpcLoyalty = pNpc->GetProto()->m_iLoyalty;
	if (iNpcExp <= 0 && iNpcLoyalty <= 0)
		return;

	_PARTY_GROUP* pParty = nullptr;
	uint32 nFinalExp, nFinalLoyalty;
	double TempValue = 0;

	// Calculate base XP earned for the damage dealt.
	if (iNpcExp > 0)
	{
		TempValue = iNpcExp * ((double)iDamage / (double)iTotalDamage);
		nFinalExp = (int)TempValue;
		if (TempValue > nFinalExp)
			nFinalExp++;
	}

	// Calculate base NP earned for the damage dealt.
	if (iNpcLoyalty > 0)
	{
		TempValue = iNpcLoyalty * ((double)iDamage / (double)iTotalDamage);
		nFinalLoyalty = (int)TempValue;
		if (TempValue > nFinalLoyalty)
			nFinalLoyalty++;
	}

	// Handle solo XP/NP gain
	if (!isInParty()
		|| (pParty = g_pMain->GetPartyPtr(GetPartyID())) == nullptr)
	{
		if (isDead())
			return;

		// Calculate the amount to adjust the XP/NP based on level difference.
		float fModifier = pNpc->GetRewardModifier(GetLevel());

		// Give solo XP
		if (iNpcExp > 0)
		{
			TempValue = nFinalExp * fModifier;
			nFinalExp = (int)TempValue;
			if (TempValue > nFinalExp)
				nFinalExp++;

			if (GetZoneID() == ZONE_KNIGHT_ROYALE)
			{
				if (g_pMain->isKnightRoyaleActive())
					KnightRoyaleExpChange(nFinalExp);
			}
			else
				ExpChange(nFinalExp);
		}

		// Give solo NP
		if (iNpcLoyalty > 0)
		{
			bool UseModifier = false;

			if (UseModifier)
			{
				TempValue = nFinalLoyalty * fModifier;
				nFinalLoyalty = (int)TempValue;
				if (TempValue > nFinalLoyalty)
					nFinalLoyalty++;
			}
			SendLoyaltyChange(nFinalLoyalty);
		}
		return;
	}

	// Handle party XP/NP gain
	std::vector<CUser*> partyUsers;
	uint32 nTotalLevel = 0;
	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser* pUser = g_pMain->GetUserPtr(pParty->uid[i]);
		if (pUser == nullptr
			|| pUser->isDead()
			|| !pUser->isInRange(pNpc, RANGE_50M))
			continue;

		partyUsers.push_back(pUser);
		nTotalLevel += pUser->GetLevel();
	}

	const float fPartyModifierXP = 0.3f;
	const float fPartyModifierNP = 0.2f;

	uint32 nPartyMembers = (uint32)partyUsers.size();

	foreach(itr, partyUsers)
	{
		CUser* pUser = (*itr);
		if (pUser->isDead()
			|| !pUser->isInRange(pNpc, RANGE_50M))
			nPartyMembers--;
	}
	// Calculate the amount to adjust the XP/NP based on level difference.
	float fModifier = pNpc->GetPartyRewardModifier(nTotalLevel, nPartyMembers);
	//float fModifier = 1.0f;

	if (iNpcExp > 0)
	{
		TempValue = nFinalExp * fModifier;
		nFinalExp = (int)TempValue;
		if (TempValue > nFinalExp)
			nFinalExp++;
	}

	if (iNpcLoyalty > 0)
	{
		TempValue = nFinalLoyalty * fModifier;
		nFinalLoyalty = (int)TempValue;
		if (TempValue > nFinalLoyalty)
			nFinalLoyalty++;
	}

	// Hand out kill rewards to all users in the party and still in range.
	foreach(itr, partyUsers)
	{
		CUser* pUser = (*itr);
		if (pUser->isDead()
			|| !pUser->isInRange(pNpc, RANGE_50M))
			continue;

		if (iNpcExp > 0)
		{
			TempValue = (nFinalExp * (1 + fPartyModifierXP * (nPartyMembers - 1))) * (double)pUser->GetLevel() / (double)nTotalLevel;
			int iExp = (int)TempValue;
			if (TempValue > iExp)
				iExp++;

			if (pUser->GetZoneID() == ZONE_KNIGHT_ROYALE)
			{
				if (g_pMain->isKnightRoyaleActive())
					pUser->KnightRoyaleExpChange(iExp);
			}
			else
				pUser->ExpChange(iExp);
		}

		if (iNpcLoyalty > 0)
		{
			TempValue = (nFinalLoyalty * (1 + fPartyModifierNP * (nPartyMembers - 1))) * (double)pUser->GetLevel() / (double)nTotalLevel;
			int iLoyalty = (int)TempValue;
			if (TempValue > iLoyalty)
				iLoyalty++;

			pUser->SendLoyaltyChange(iLoyalty);
		}
	}
}

/**
* @brief	Sends a HP update to the user's party.
*/
void CUser::SendPartyHPUpdate()
{
	Packet result(WIZ_PARTY);
	result << uint8(PARTY_HPCHANGE)
		<< GetSocketID()
		<< m_MaxHp << m_sHp
		<< m_MaxMp << m_sMp;
	g_pMain->Send_PartyMember(GetPartyID(), &result);
}

/**
* @brief	Shows the specified skill's effect
* 			to the surrounding regions.
*
* @param	nSkillID	Skill identifier.
*/
void CUser::ShowEffect(uint32 nSkillID)
{
	Packet result(WIZ_EFFECT);
	if (isInGame())
	{
		result << GetID() << nSkillID;
		SendToRegion(&result);
	}
}

/**
* @brief	Shows an effect on the NPC currently
* 			being interacted with.
*
* @param	nEffectID	Identifier for the effect.
*/
void CUser::ShowNpcEffect(uint32 nEffectID, bool bSendToRegion)
{
	Packet result(WIZ_OBJECT_EVENT, uint8(OBJECT_NPC));
	result << uint8(3) << m_sEventNid << nEffectID;
	if (bSendToRegion)
		SendToRegion(&result);
	else
		Send(&result);
}

/**
* @brief	Sends the target's HP to the player.
*
* @param	echo  	Client-based flag that we must echo back to the client.
* 					Set to 0 if not responding to the client.
* @param	tid   	The target's ID.
* @param	damage	The amount of damage taken on this request, 0 if it does not apply.
*/
void CUser::SendTargetHP(uint8 echo, int tid, int damage)
{
	int hp = 0, maxhp = 0;

	Unit* pTarget = nullptr;

	if (tid >= NPC_BAND)
	{
		CNpc* pNpc = g_pMain->GetNpcPtr(tid, GetZoneID());
		if (pNpc == nullptr)
			return;

		hp = pNpc->m_iHP;
		maxhp = pNpc->m_MaxHP;
		pTarget = pNpc;


	}
	else
	{
		if (tid < MAX_USER) {
			CUser* pUser = g_pMain->GetUserPtr(tid);
			if (pUser == nullptr || pUser->isDead())
				return;

			hp = pUser->m_sHp;
			maxhp = pUser->m_MaxHp;
			pTarget = pUser;
		}
		else {
			CBot* pUser = g_pMain->m_BotArray.GetData(tid);
			if (pUser == nullptr
				|| pUser->m_sHp <= 0)
				return;

			hp = pUser->m_sHp;
			maxhp = pUser->m_MaxHP;
		}
	}

	Packet result(WIZ_TARGET_HP);
	result << uint16(tid) << echo << maxhp << hp << uint16(damage);
	Send(&result);
}

#pragma region CUser::HandleTargetHP(Packet & pkt)
void CUser::HandleTargetHP(Packet& pkt)
{
	uint16 uid = pkt.read<uint16>();
	uint8 echo = pkt.read<uint8>();
	m_targetID = uid;
	SendTargetHP(echo, uid);
	if (isGM() && echo == 1)
	{
		CNpc* pNpc = g_pMain->GetNpcPtr(uid, GetZoneID());
		if (pNpc == nullptr)
			return;

		std::string strNpcName = pNpc->GetName();
		if (strNpcName.length() == 0)
			strNpcName = "<NoName>";
		g_pMain->SendHelpDescription(this, string_format("NpcID : %d - testID : %d NpcName : %s - NpcDefance : %d - NpcAttack : %u - NpcExp : %u", pNpc != nullptr ? pNpc->GetProtoID() : 0, pNpc != nullptr ? pNpc->GetID() : 0, pNpc != nullptr ? pNpc->GetName().c_str() : "NULL", pNpc != nullptr ? pNpc->m_sTotalAc : 0, pNpc != nullptr ? pNpc->m_sTotalHit : 0, pNpc != nullptr ? pNpc->GetProto()->m_iExp : 0));

	}
}
#pragma endregion

/**
* @brief	Packet handler for various player state changes.
*
* @param	pkt	The packet.
*/
void CUser::StateChange(Packet& pkt)
{
	if (isDead())
		return;

	uint8 bType = pkt.read<uint8>(), buff;
	uint32 nBuff = pkt.read<uint32>();
	buff = *(uint8*)&nBuff; // don't ask
	m_iTotalTrainingExp = 0;
	m_iTotalTrainingTime = 0;
	m_lastTrainingTime = 0;

	switch (bType)
	{
	case 1:
		if (buff != USER_STANDING && buff != USER_SITDOWN && buff != USER_MONUMENT)
			return;
		break;

	case 3:
		// /unview | /view
		if ((buff == 1 || buff == 5)
			&& !isGM())
			return;
		break;

	case 4: // emotions
		switch (buff)
		{
		case 1: // Greeting 1-3
		case 2:
		case 3:
		case 11: // Provoke 1-3
		case 12:
		case 13:
			if (GetFame() == COMMAND_CAPTAIN)
				return;
			break;
		case 14: // additional animations randomly used when hitting spacebar
		case 15:
			break; // don't do anything with them (this can be handled neater, but just for testing purposes), just make sure they're allowed

		default:
			TRACE("[SID=%d] StateChange: %s tripped (bType=%d, buff=%d, nBuff=%d) somehow, HOW!?\n", GetSocketID(), GetName().c_str(), bType, buff, nBuff);
			printf("[SID=%d] StateChange: %s tripped (bType=%d, buff=%d, nBuff=%d) somehow, HOW!?\n", GetSocketID(), GetName().c_str(), bType, buff, nBuff);
			break;
		}
		break;

	case 5:
		if ((buff == 0 && m_bAbnormalType == 0)
			|| (buff == 1 && m_bAbnormalType == 1)
			|| (!isGM()))
			return;
		break;

	case 7: // invisibility flag, we don't want users overriding server behaviour.
		return;

	default:
		TRACE("[SID=%d] StateChange: %s tripped (bType=%d, buff=%d, nBuff=%d) somehow, HOW!?\n", GetSocketID(), GetName().c_str(), bType, buff, nBuff);
		printf("[SID=%d] StateChange: %s tripped (bType=%d, buff=%d, nBuff=%d) somehow, HOW!?\n", GetSocketID(), GetName().c_str(), bType, buff, nBuff);
		return;
	}

	StateChangeServerDirect(bType, nBuff);
}

/**
* @brief	Changes a player's state directly from the server
* 			without any checks.
*
* @param	bType	State type.
* @param	nBuff	The buff/flag (depending on the state type).
*/
void CUser::StateChangeServerDirect(uint8 bType, uint32 nBuff)
{
	uint8 buff = *(uint8*)&nBuff; // don't ask
	switch (bType)
	{
	case 1:
		m_bResHpType = buff;
		break;

	case 2:
		m_bNeedParty = buff;
		break;

	case 3:
		m_nOldAbnormalType = m_bAbnormalType;

		// If we're a GM, we need to show ourselves before transforming.
		// Otherwise the visibility state is completely desynced.
		if (isGM())
			StateChangeServerDirect(5, 1);

		m_bAbnormalType = nBuff;
		break;
	case 4:
		break;
	case 5:
		if (!isGM())
			return;

		m_bAbnormalType = nBuff;
		nBuff == 0 ? GmInOut(INOUT_OUT) : GmInOut(INOUT_IN);
		break;

	case 6:
		nBuff = m_bPartyLeader; // we don't set this here.
		break;

	case 7:
		UpdateVisibility((InvisibilityType)buff);
		break;

	case 8: // beginner quest
		break;

	case 11:
		if (buff == TeamColourRed)
			m_teamColour = TeamColourRed;
		else if (buff == TeamColourBlue)
			m_teamColour = TeamColourBlue;
		else
			m_teamColour = TeamColourNone;
		break;
	case 12:
		break;
	default:
		printf("StateChangeServerDirect unhandled buff %d \n", bType);
		break;
	}

	Packet result(WIZ_STATE_CHANGE);
	result << GetSocketID() << bType << uint64(nBuff);

	if (GetEventRoom() > 0)
		SendToRegion(&result, nullptr, GetEventRoom());
	else
		SendToRegion(&result);
}

#pragma region CUser::LoyaltyChange(int16 tid, uint16 bonusNP /*= 0*/)
/**
* @brief	Takes a target's loyalty points (NP)
* 			and rewards some/all to the killer (current user).
*
* @param	tid		The target's ID.
* @param	bonusNP Bonus NP to be awarded to the killer as-is.
*/
void CUser::LoyaltyChange(int16 tid, uint16 bonusNP /*= 0*/)
{
	short loyalty_source = 0, loyalty_target = 0;

	CUser* pTUser = g_pMain->GetUserPtr(tid);
	if (pTUser == nullptr || !pTUser->isInGame())
		return;

	if (GetZoneID() == ZONE_RONARK_LAND && GetEventRoom() > 2000)
		return;

	if (GetZoneID() == ZONE_DELOS)
	{
		if (pTUser->GetNation() == GetNation())
		{
			if (!GetMap()->canAttackSameNation())
				return;
		}
		else
		{
			if (!GetMap()->canAttackOtherNation())
				return;
		}
	}
	else
	{
		// TODO: Rewrite this out, it shouldn't handle all cases so generally like this
		if ((!GetMap()->isNationPVPZone())
			|| GetZoneID() == ZONE_DESPERATION_ABYSS
			|| GetZoneID() == ZONE_HELL_ABYSS
			|| GetZoneID() == ZONE_DRAGON_CAVE
			|| GetZoneID() == ZONE_CAITHAROS_ARENA)
			return;
	}

	if (pTUser->GetZoneID() == ZONE_DELOS && GetZoneID() == ZONE_DELOS)
	{
		if (g_pMain->pCswEvent.Started || g_pMain->m_byBattleOpen == SIEGE_BATTLE)
		{
			if (g_pMain->pCswEvent.Status == CswOperationStatus::DeathMatch)
			{
				if (pTUser->GetLoyalty() == 0)
				{
					loyalty_source = 0;
					loyalty_target = 0;
				}

				loyalty_source = g_pMain->m_Loyalty_Other_Zone_Source;
				loyalty_target = g_pMain->m_Loyalty_Other_Zone_Target;
			}
			else if (g_pMain->pCswEvent.Status == CswOperationStatus::CastellanWar)
			{
				if (pTUser->GetLoyalty() == 0)
				{
					loyalty_source = 0;
					loyalty_target = 0;
				}

				loyalty_source = g_pMain->m_Loyalty_Other_Zone_Source;
				loyalty_target = g_pMain->m_Loyalty_Other_Zone_Target;
			}
		}
	}
	else if (pTUser->GetZoneID() == ZONE_DEATHMATCH && GetZoneID() == ZONE_DEATHMATCH)
	{
		loyalty_source = g_pMain->m_Loyalty_Ronark_Land_Base_Source;
		loyalty_target = g_pMain->m_Loyalty_Ronark_Land_Base_Target;
	}
	else if (pTUser->GetNation() != GetNation())
	{
		if (pTUser->GetLoyalty() == 0)
		{
			int64 nExpLost = 0;
			loyalty_source = 0;
			loyalty_target = 0;

			if (GetMap()->m_bExpLost != 0)
			{
				bool isNationZoneExpLost = ((GetNation() == KARUS && pTUser->GetZoneID() == ZONE_ELMORAD) || (GetNation() == ELMORAD && pTUser->GetZoneID() == ZONE_KARUS));

				if (isNationZoneExpLost)
					nExpLost = pTUser->m_iMaxExp / 100;
				else
					nExpLost = pTUser->m_iMaxExp / 20;

				if (pTUser->GetPremiumProperty(PremiumExpRestorePercent) > 0)
					nExpLost = nExpLost * (pTUser->GetPremiumProperty(PremiumExpRestorePercent)) / 100;

				if (pTUser->GetClanPremiumProperty(PremiumExpRestorePercent) > 0)
					nExpLost = nExpLost * (pTUser->GetClanPremiumProperty(PremiumExpRestorePercent)) / 100;

				pTUser->ExpChange(-nExpLost);
				goto fail_return;
			}
		}
		// Ardream
		else if (pTUser->GetZoneID() == ZONE_ARDREAM)
		{
			loyalty_source = g_pMain->m_Loyalty_Ardream_Source;
			loyalty_target = g_pMain->m_Loyalty_Ardream_Target;
		}
		// Ronark Land Base
		else if (pTUser->GetZoneID() == ZONE_RONARK_LAND_BASE)
		{
			loyalty_source = g_pMain->m_Loyalty_Ronark_Land_Base_Source;
			loyalty_target = g_pMain->m_Loyalty_Ronark_Land_Base_Target;
		}
		else if (pTUser->GetZoneID() == ZONE_RONARK_LAND)
		{
			loyalty_source = g_pMain->m_Loyalty_Ronark_Land_Source;
			loyalty_target = g_pMain->m_Loyalty_Ronark_Land_Target;
		}
		else if (pTUser->GetZoneID() == ZONE_KARUS
			|| pTUser->GetZoneID() == ZONE_ELMORAD
			|| (pTUser->GetZoneID() >= ZONE_BATTLE
				&& pTUser->GetZoneID() <= ZONE_BATTLE6))
		{
			loyalty_source = g_pMain->m_Loyalty_Ronark_Land_Source;
			loyalty_target = g_pMain->m_Loyalty_Other_Zone_Target;
		}
		// Other zones
		else
		{
			loyalty_source = g_pMain->m_Loyalty_Other_Zone_Source;
			loyalty_target = g_pMain->m_Loyalty_Other_Zone_Target;
		}
	}

	// Include any bonus NP (e.g. rival NP bonus)
	loyalty_source += bonusNP;
	loyalty_target -= bonusNP;

	SendLoyaltyChange(loyalty_source, true, false, pTUser->GetMonthlyLoyalty() > 0 ? true : false);
	pTUser->SendLoyaltyChange(loyalty_target, true, false, pTUser->GetMonthlyLoyalty() > 0 ? true : false);

fail_return:
	// TODO: Move this to a better place (death handler, preferrably)
	// If a war's running, and we died/killed in a war zone... (this method should NOT be so tied up in specifics( 
	if (g_pMain->isWarOpen()
		&& GetMap()->isWarZone())
	{
		// Update the casualty count
		if (pTUser->GetNation() == KARUS)
			g_pMain->m_sKarusDead++;
		else
			g_pMain->m_sElmoradDead++;

		Packet result(PL_RANK);
		result << uint8(3) << g_pMain->m_sKarusDead << g_pMain->m_sElmoradDead;
		Send(&result);
	}
}

#pragma endregion

void CUser::SpeedHackUser()
{
	if (!isInGame()
		|| isGM())
		return;

	DateTime time;
	int16 nMaxSpeed = 45;

	if (GetFame() == COMMAND_CAPTAIN
		|| isRogue())
		nMaxSpeed = 90;
	else if (isWarrior()
		|| isMage()
		|| isPriest())
		nMaxSpeed = 67;
	else if (isPortuKurian())
		nMaxSpeed = 67;

	if (m_sSpeed > nMaxSpeed || m_sSpeed < -nMaxSpeed)
		g_pMain->WriteCheatLogFile(string_format("[ SpeedHack - %d:%d:%d ] %s is Disconnected.\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str()));
}

void CUser::UserLookChange(int pos, int itemid, int durability)
{
	if (pos >= SLOT_MAX) // let's leave it at this for the moment, the updated check needs considerable reworking
		return;

	Packet result(WIZ_USERLOOK_CHANGE);
	result << GetSocketID() << uint8(pos) << itemid << uint16(durability);

	if (GetEventRoom() > 0)
		SendToRegion(&result, this, GetEventRoom());
	else
		SendToRegion(&result);
}

void CUser::SendNotice()
{
	Packet result(WIZ_NOTICE);
	uint8 count = 0;

	result << uint8(2); // new-style notices (top-right of screen)
	result << count; // placeholder the count

	// Use first line for header, 2nd line for data, 3rd line for header... etc.
	// It's most likely what they do officially (as usual, | is their line separator)
	for (int i = 0; i < 10; i += 2)
		AppendNoticeEntry(result, count, g_pMain->m_ppNotice[i + 1], g_pMain->m_ppNotice[i]);

	AppendExtraNoticeData(result, count);
	result.put(1, count); // replace the placeholdered line count

	Send(&result);
}
void CUser::TopSendNotice()
{
	Packet result(WIZ_NOTICE);
	uint8 count = 0;//uint8

	result << uint8(1); // Old-style notices (top-right of screen)
	result << count; // placeholder the 
	result.SByte();
	// Use first line for header, 2nd line for data, 3rd line for header... etc.
	// It's most likely what they do officially (as usual, | is their line separator)
	for (int i = 0; i < 20; i++)
		AppendNoticeEntryOld(result, count, g_pMain->m_peNotice[i]);

	AppendExtraNoticeData(result, count);
	result.put(1, count); // replace the placeholdered line count
	Send(&result);
}
void CUser::AppendNoticeEntryOld(Packet& pkt, uint8& elementCount, const char* message)
{
	if (message == nullptr || *message == '\0')
		return;

	pkt << message;
	elementCount++;
}
void CUser::AppendNoticeEntry(Packet& pkt, uint8& elementCount, const char* message, const char* title)
{
	if (message == nullptr || *message == '\0'
		|| title == nullptr || *title == '\0')
		return;

	pkt << title << message;
	elementCount++;
}

void CUser::AppendExtraNoticeData(Packet& pkt, uint8& elementCount)
{
	string message;

	if (g_pMain->m_byExpEventAmount > 0)
	{
		g_pMain->GetServerResource(IDS_EXP_REPAY_EVENT, &message, g_pMain->m_byExpEventAmount);
		AppendNoticeEntry(pkt, elementCount, message.c_str(), "Exp Event");
	}

	if (g_pMain->m_byCoinEventAmount > 0)
	{
		g_pMain->GetServerResource(IDS_MONEY_REPAY_EVENT, &message, g_pMain->m_byCoinEventAmount);
		AppendNoticeEntry(pkt, elementCount, message.c_str(), "Noah Event");
	}

	if (g_pMain->m_byNPEventAmount > 0)
	{
		g_pMain->GetServerResource(IDS_NP_REPAY_EVENT, &message, g_pMain->m_byNPEventAmount);
		AppendNoticeEntry(pkt, elementCount, message.c_str(), "Np Event");
	}
}

void CUser::UpdateGameWeather(Packet& pkt)
{
	if (!isGM())	// is this user a GM?
		return;

	if (pkt.GetOpcode() == WIZ_WEATHER)
	{
		pkt >> g_pMain->m_byWeather >> g_pMain->m_sWeatherAmount;
	}
	else
	{
		uint16 y, m, d;
		pkt >> y >> m >> d >> g_pMain->m_sHour >> g_pMain->m_sMin;
	}
	Send(&pkt); // pass the packet straight on
}

void CUser::GetUserInfoForAI(Packet& result)
{
	Guard lock(m_equippedItemBonusLock);

	result.SByte();
	result << GetSocketID()
		<< GetName() << GetZoneID() << GetNation() << GetLevel()
		<< m_sHp << m_sMp
		<< m_sTotalHit << m_bAttackAmount
		<< m_sTotalAc << m_sACAmount
		<< m_fTotalHitrate << m_fTotalEvasionrate
		<< m_sItemAc
		<< GetPartyID() << GetAuthority()
		<< m_bInvisibilityType
		<< uint32(m_equippedItemBonuses.size());

	foreach(itr, m_equippedItemBonuses)
	{
		result << itr->first << uint32(itr->second.size()); // slot ID & number of bonuses
		foreach(bonusItr, itr->second)
			result << bonusItr->first << bonusItr->second; // bonus type, bonus amount
	}
}

void CUser::CountConcurrentUser()
{
	if (!isGM())
		return;

	uint16 count = 0;
	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	BOOST_FOREACH(auto itr, sessMap)
	{
		if (TO_USER(itr.second)->isInGame())
			count++;
	}

	Guard lock(g_pMain->m_BotcharacterNameLock);
	count += g_pMain->m_BotcharacterNameMap.size();

	Packet result(WIZ_CONCURRENTUSER);
	result << count;
	Send(&result);
}

void CUser::ItemWoreOut(int type, int damage)
{
	static uint8 weaponsTypes[] = { GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND };
	static uint8 armourTypes[] = { GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_HEAD : HEAD, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_PAULDRON : BREAST, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_PANTS : LEG, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_GLOVE : GLOVE, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_BOOTS : FOOT };
	static uint8 repairTypes[] = { GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_HEAD : HEAD, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_PAULDRON : BREAST, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_PANTS : LEG, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_GLOVE : GLOVE, GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_BOOTS : FOOT };
	uint8 totalSlots;
	int worerate;

	if (type == ACID_ALL
		|| type == UTC_ATTACK
		|| type == UTC_DEFENCE)
		worerate = damage;
	else
		worerate = (int)sqrt(damage / 10.0f);

	if (worerate == 0) return;

	ASSERT(type == ATTACK
		|| type == DEFENCE
		|| type == REPAIR_ALL
		|| type == ACID_ALL
		|| type == UTC_ATTACK
		|| type == UTC_DEFENCE);

	if (type == ATTACK)
		totalSlots = sizeof(weaponsTypes) / sizeof(*weaponsTypes);
	else if (type == DEFENCE)
		totalSlots = sizeof(armourTypes) / sizeof(*armourTypes);
	else if (type == REPAIR_ALL)
		totalSlots = sizeof(repairTypes) / sizeof(*repairTypes);
	else if (type == ACID_ALL)
		totalSlots = sizeof(armourTypes) / sizeof(*armourTypes);
	else if (type == UTC_ATTACK)
		totalSlots = sizeof(weaponsTypes) / sizeof(*weaponsTypes);
	else if (type == UTC_DEFENCE)
		totalSlots = sizeof(armourTypes) / sizeof(*armourTypes);

	for (uint8 i = 0; i < totalSlots; i++)
	{
		uint8 slot;
		if (type == ATTACK)
			slot = weaponsTypes[i];
		else if (type == DEFENCE)
			slot = armourTypes[i];
		else if (type == REPAIR_ALL)
			slot = repairTypes[i];
		else if (type == UTC_ATTACK)
			slot = weaponsTypes[i];
		else if (type == UTC_DEFENCE)
			slot = armourTypes[i];
		else
			slot = armourTypes[i];
		_ITEM_DATA* pItem = nullptr;

		if (GetZoneID() == ZONE_KNIGHT_ROYALE)
			pItem = GetKnightRoyaleItem(slot);
		else
			pItem = GetItem(slot);

		_ITEM_TABLE* pTable = nullptr;

		// Is a non-broken item equipped?
		if (pItem == nullptr
			|| (pItem->sDuration <= 0 && type != REPAIR_ALL)
			// Does the item exist?
			|| (pTable = g_pMain->GetItemPtr(pItem->nNum)) == nullptr
			// If it's in the left or righthand slot, is it a shield? (this doesn't apply to weapons)
			|| ((type == ATTACK
				|| type == UTC_ATTACK)
				&& ((slot == (GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND)
					|| slot == (GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND))
					&& pTable->m_bSlot == ItemSlot1HLeftHand)))
			continue;

		if (type == REPAIR_ALL)
		{
			pItem->sDuration = pTable->m_sDuration;
			SendDurability(slot, pItem->sDuration);
			UserLookChange(slot, pItem->nNum, pItem->sDuration);
			SetUserAbility(true);
			continue;
		}

		int beforepercent = (int)((pItem->sDuration / (double)pTable->m_sDuration) * 100);
		int curpercent;

		if (worerate > pItem->sDuration)
			pItem->sDuration = 0;
		else
			pItem->sDuration -= worerate;

		if (m_sItemArray[slot].sDuration <= 0)
			m_sItemArray[slot].sDuration = 0;

		if (m_sItemArray[slot].sDuration == 0)
		{
			SendDurability(slot, 0);
			SetUserAbility(false);
			SendItemMove(1, 1);
			continue;
		}

		SendDurability(slot, pItem->sDuration);

		curpercent = (int)((pItem->sDuration / (double)pTable->m_sDuration) * 100);

		if ((curpercent / 5) != (beforepercent / 5))
		{
			if (curpercent >= 65 && curpercent < 70
				|| curpercent >= 25 && curpercent < 30)
				UserLookChange(slot, pItem->nNum, pItem->sDuration);
		}
	}
}

void CUser::SendDurability(uint8 slot, uint16 durability)
{
	Packet result(WIZ_DURATION, slot);
	result << durability;
	Send(&result);
}

void CUser::SendItemMove(uint8 command, uint8 subcommand)
{
	Packet result(WIZ_ITEM_MOVE, command);

	if (m_bAttackAmount == 0)
		m_bAttackAmount = 100;

	// If the subcommand is not error, send the stats.
	if (subcommand != 0)
	{
		uint32 moneyReq = 0;
		if (GetPremium() == WAR_Premium)
			moneyReq = 0;
		else {
			moneyReq = (int)pow((GetLevel() * 2.0f), 3.4f);
			if (GetLevel() < 30)
				moneyReq = (int)(moneyReq * 0.4f);
			else if (GetLevel() >= 60)
				moneyReq = (int)(moneyReq * 1.5f);

			if ((g_pMain->m_sDiscount == 1
				&& g_pMain->m_byOldVictory == GetNation())
				|| g_pMain->m_sDiscount == 2)
				moneyReq /= 2;
		}

		result << subcommand << uint16(m_sTotalHit * m_bAttackAmount / 100)
			<< uint16(m_sTotalAc + m_sACAmount)
			<< m_sMaxWeight << uint8(1) << uint16(m_MaxHp) << uint16(m_MaxMp)
			<< GetStatBonusTotal(STAT_STR) << GetStatBonusTotal(STAT_STA)
			<< GetStatBonusTotal(STAT_DEX) << GetStatBonusTotal(STAT_INT) << GetStatBonusTotal(STAT_CHA)
			<< uint16(((m_sFireR + m_bAddFireR + m_bResistanceBonus) * m_bPctFireR / 100))
			<< uint16(((m_sColdR + m_bAddColdR + m_bResistanceBonus) * m_bPctColdR / 100))
			<< uint16(((m_sLightningR + m_bAddLightningR + m_bResistanceBonus) * m_bPctLightningR / 100))
			<< uint16(((m_sMagicR + m_bAddMagicR + m_bResistanceBonus) * m_bPctMagicR / 100))
			<< uint16(((m_sDiseaseR + m_bAddDiseaseR + m_bResistanceBonus) * m_bPctDiseaseR / 100))
			<< uint16(((m_sPoisonR + m_bAddPoisonR + m_bResistanceBonus) * m_bPctPoisonR / 100))
			<< uint32(m_nKnightCash) << uint32(m_nKnightCashBonus)
			<< uint16(m_sDaggerR) << uint16(m_sAxeR) << uint16(m_sSwordR)
			<< uint16(m_sMaceR) << uint16(m_sSpearR) << uint16(m_sBowR)
			<< uint16(m_sJamadarR) << uint32(moneyReq) << m_sHp << m_sMp;
	}
	Send(&result);
}

void CUser::SendAllKnightsID()
{
	Packet result(WIZ_KNIGHTS_LIST, uint8(1));
	uint16 count = 0;

	foreach_stlmap(itr, g_pMain->m_KnightsArray)
	{
		CKnights* pKnights = itr->second;
		if (pKnights == nullptr)
			continue;
		result << pKnights->m_sIndex << pKnights->m_strName;
		count++;
	}

	result.put(0, count);
	SendCompressed(&result);
}

int CUser::FindSlotForItem(uint32 nItemID, uint16 sCount /*= 1*/)
{
	int result = -1;
	_ITEM_TABLE* pTable = g_pMain->GetItemPtr(nItemID);
	_ITEM_DATA* pItem = nullptr;

	if (pTable == nullptr)
		return result;

	// If the item's stackable, try to find it a home.
	// We could do this in the same logic, but I'd prefer one initial check
	// over the additional logic hit each loop iteration.
	if (pTable->m_bCountable)
	{
		for (int i = SLOT_MAX; i < SLOT_MAX + HAVE_MAX; i++)
		{
			if (GetZoneID() == ZONE_KNIGHT_ROYALE)
				pItem = GetKnightRoyaleItem(i);
			else
				pItem = GetItem(i);

			if (pItem == nullptr)
				continue;

			// If it's the item we're after, and there will be room to store it...
			if (pItem
				&& pItem->nNum == nItemID
				&& pItem->sCount + sCount <= ITEMCOUNT_MAX)
				return i;

			// Found a free slot, we'd prefer to stack it though
			// so store the first free slot, and ignore it.
			if (pItem->nNum == 0
				&& result < 0)
				result = i;
		}

		// If we didn't find a slot countaining our stackable item, it's possible we found
		// an empty slot. So return that (or -1 if it none was found; no point searching again).
		return result;
	}

	// If it's not stackable, don't need any additional logic.
	// Just find the first free slot.
	return GetEmptySlot();
}

int CUser::GetEmptySlot()
{
	_ITEM_DATA* pItem;

	for (int i = SLOT_MAX; i < SLOT_MAX + HAVE_MAX; i++)
	{
		if (GetZoneID() == ZONE_KNIGHT_ROYALE)
			pItem = GetKnightRoyaleItem(i);
		else
			pItem = GetItem(i);

		if (!pItem)
			continue;

		if (pItem->nNum == 0)
			return i;
	}

	return -1;
}
void CUser::Home()
{
	if (isDead()
		// When transformed into a "Kaul", you are unable to /town or attack.
		|| isKaul()
		/* No cheats allowed */
		|| GetHealth() < (GetMaxHealth() / 2)
		|| isInEventNoTownZone()
		|| hasBuff(BUFF_TYPE_FREEZE))
		return;

	CUser* VsEventUser = g_pMain->GetUserPtr(VsYapilacakKullanici, TYPE_CHARACTER);

	if (VsEventUser != nullptr)
	{
		if (VsEventUser->m_bEventRoom > 2000 && VsEventUser->m_VsDurumu == false && VsEventUser->GetZoneID() == ZONE_RONARK_LAND)
			return;
	}

	if (m_bEventRoom > 2000 && m_VsDurumu == false && GetZoneID() == ZONE_ARENA)
		return;

	// The point where you will be warped to.
	short x = 0, z = 0;
	_OBJECT_EVENT* pEvent = nullptr;
	pEvent = GetMap()->GetObjectEvent(m_sBind);
	// Resurrect at a bind/respawn point
	if (pEvent && pEvent->byLife == 1 && GetZoneID() != ZONE_DELOS && pEvent->sIndex != 101 && pEvent->sIndex != 201)
	{
		SetPosition(pEvent->fPosX + x, 0.0f, pEvent->fPosZ + z);
		x = short(pEvent->fPosX);
		z = short(pEvent->fPosZ);
		Warp(x * 10, z * 10);
		return;
	}

	if (isInSoccerEvent())
	{
		isEventSoccerEnd();

		if (g_pMain->m_TempleSoccerEventUserArray.GetData(GetSocketID()) != nullptr)
			g_pMain->m_TempleSoccerEventUserArray.DeleteData(GetSocketID());
	}

	// Forgotten Temple
	if (GetZoneID() == ZONE_FORGOTTEN_TEMPLE)
	{
		KickOutZoneUser(true);
		return;
	}

	if (GetZoneID() == ZONE_DEATHMATCH)
	{
		Packet result1(PL_EVENT);
		result1 << uint8(18);
		Send(&result1);

		KickOutZoneUser(true);
		return;
	}
	// Prevent /town'ing in quest arenas
	else if ((GetZoneID() / 10) == 5
		|| !GetStartPosition(x, z))
		return;

	if (GetZoneID() == ZONE_DELOS)
	{
		if (g_pMain->pCswEvent.Status == CswOperationStatus::DeathMatch && GetZoneID() == ZONE_DELOS)
		{
			_DEATHMATCH_BARRACK_INFO* pData4 = g_pMain->m_KnightSiegeWarBarrackList.GetData(GetClanID());
			if (pData4 == nullptr)
				return;

			_DEATHMATCH_WAR_INFO* pKnightList = g_pMain->m_KnightSiegeWarClanList.GetData(GetClanID());
			if (pKnightList == nullptr)
				return;

			uint8 xrand = myrand(1, 5);
			uint8 zrand = myrand(1, 5);

			Warp((pData4->m_tBaseX + xrand) * 10, (pData4->m_tBaseZ + zrand) * 10);
			return;
		}
	}

	Warp(x * 10, z * 10);
	PetHome(x * 10, 0, z * 10);
}

#pragma region CUser::GiveWerehouseItem(uint32 itemid, uint16 count, bool mining /*= true*/, bool fishing /*= true*/, uint32 Time)

bool CUser::GiveWerehouseItem(uint32 itemid, uint16 count, bool mining /*= true*/, bool fishing /*= true*/, uint32 Time)
{
	int8 pos;
	bool bNewItem = true;
	_ITEM_TABLE* pTable = g_pMain->GetItemPtr(itemid);
	_ITEM_DATA* pItem = nullptr;

	if (pTable == nullptr)
		return false;

	pos = FindWerehouseSlotForItem(itemid, count);
	if (pos < 0)
		return false;

	pItem = GetWerehouseItem(pos);

	if (pItem->nNum != 0 || pItem == nullptr)
		bNewItem = false;

	if (bNewItem)
		pItem->nSerialNum = g_pMain->GenerateItemSerial();

	pItem->nNum = itemid;
	pItem->sCount += count;
	pItem->sDuration = pTable->m_sDuration;

	if (pItem->sCount > MAX_ITEM_COUNT)
		pItem->sCount = MAX_ITEM_COUNT;

	if (Time != 0)
		pItem->nExpirationTime = int32(UNIXTIME) + ((60 * 60 * 24) * Time);
	else
		pItem->nExpirationTime = 0;

	if (mining)
		CyberACS_ItemNotice(0, pItem->nNum);

	if (fishing)
		CyberACS_ItemNotice(2, pItem->nNum);

	DateTime time;
	g_pMain->WriteItemTransactionLogFile(string_format("%d:%d:%d || ZoneID = %d(%d,%d),UserID=%s,Task=WerehoseItem,ItemName=%s,ItemID=%d,Count=%d\n",
		time.GetHour(), time.GetMinute(), time.GetSecond(), GetZoneID(), uint16(GetX()), uint16(GetZ()), GetName().c_str(), pTable->m_sName.c_str(), pTable->m_iNum, count));

	return true;
}

#pragma endregion

int CUser::FindWerehouseSlotForItem(uint32 nItemID, uint16 sCount /*= 1*/)
{
	int result = -1;
	_ITEM_TABLE* pTable = g_pMain->GetItemPtr(nItemID);
	_ITEM_DATA* pItem = nullptr;

	if (pTable == nullptr)
		return result;

	// If the item's stackable, try to find it a home.
	// We could do this in the same logic, but I'd prefer one initial check
	// over the additional logic hit each loop iteration.
	if (pTable->m_bCountable)
	{
		for (int i = 0; i < WAREHOUSE_MAX; i++)
		{
			pItem = GetWerehouseItem(i);

			if (pItem == nullptr)
				continue;

			// If it's the item we're after, and there will be room to store it...
			if (pItem
				&& pItem->nNum == nItemID
				&& pItem->sCount + sCount <= ITEMCOUNT_MAX)
				return i;

			// Found a free slot, we'd prefer to stack it though
			// so store the first free slot, and ignore it.
			if (pItem->nNum == 0
				&& result < 0)
				result = i;
		}

		// If we didn't find a slot countaining our stackable item, it's possible we found
		// an empty slot. So return that (or -1 if it none was found; no point searching again).
		return result;
	}

	// If it's not stackable, don't need any additional logic.
	// Just find the first free slot.
	return GetWerehouseEmptySlot();
}

int CUser::GetWerehouseEmptySlot()
{
	_ITEM_DATA* pItem;

	for (int i = 0; i < WAREHOUSE_MAX; i++)
	{
		pItem = GetWerehouseItem(i);

		if (!pItem)
			continue;

		if (pItem->nNum == 0)
			return i;
	}

	return -1;
}
bool CUser::GetStartPosition(short& x, short& z, uint8 bZone /*= 0 */)
{
	// Get start position data for current zone (unless we specified a zone).
	int nZoneID = (bZone == 0 ? GetZoneID() : bZone);
	_START_POSITION* pData = g_pMain->GetStartPosition(nZoneID);
	if (pData == nullptr)
		return false;

	_KNIGHTS_SIEGE_WARFARE* pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(Aktive);
	if (pKnightSiege == nullptr)
		return false;

	// TODO: Allow for Delos/CSW.

	// NOTE: This is how mgame does it.
	// This only allows for positive randomisation; we should really allow for the full range...
	if (pKnightSiege->sMasterKnights == GetClanID() && GetZoneID() == ZONE_DELOS && GetClanID() != 0)
	{
		x = 505 + myrand(0, pData->bRangeX);
		z = 840 + myrand(0, pData->bRangeZ);
	}
	else
	{
		// NOTE: This is how mgame does it.
		// This only allows for positive randomisation; we should really allow for the full range...
		if (GetNation() == KARUS)
		{
			x = pData->sKarusX + myrand(0, pData->bRangeX);
			z = pData->sKarusZ + myrand(0, pData->bRangeZ);
		}
		else
		{
			x = pData->sElmoradX + myrand(0, pData->bRangeX);
			z = pData->sElmoradZ + myrand(0, pData->bRangeZ);
		}
	}

	return true;
}

bool CUser::GetStartPositionRandom(short& x, short& z, uint8 bZone)
{
	int nRandom = myrand(0, g_pMain->m_StartPositionRandomArray.GetSize() - 1);
	goto GetPosition;

GetPosition:
	{
		if (g_pMain->m_StartPositionRandomArray.GetData(nRandom)->ZoneID == (bZone == 0 ? GetZoneID() : bZone))
		{
			x = g_pMain->m_StartPositionRandomArray.GetData(nRandom)->PosX + myrand(0, g_pMain->m_StartPositionRandomArray.GetData(nRandom)->Radius);
			z = g_pMain->m_StartPositionRandomArray.GetData(nRandom)->PosZ + myrand(0, g_pMain->m_StartPositionRandomArray.GetData(nRandom)->Radius);
			return true;
		}

		nRandom = myrand(0, g_pMain->m_StartPositionRandomArray.GetSize() - 1);
		goto GetPosition;
	}

	return GetStartPosition(x, z);
}

bool CUser::GetStartKnightRoyalePositionRandom(short& x, short& z)
{
	_KNIGHT_ROYALE_INFO* pRoomInfo = g_pMain->m_KnightRoyaleRoomInfo.GetData(1);
	if (pRoomInfo == nullptr)
		return 0;

	int nRandom = myrand(0, g_pMain->m_KnStartPositionRandomArray.GetSize() - 1);
	goto GetPosition;

GetPosition:
	{
		if (g_pMain->m_KnStartPositionRandomArray.GetData(nRandom)->GasStep == (pRoomInfo->m_KnightRoyaleGasStepCount == 0 ? 0 : 2))
		{
			x = g_pMain->m_KnStartPositionRandomArray.GetData(nRandom)->PosX + myrand(0, 10);
			z = g_pMain->m_KnStartPositionRandomArray.GetData(nRandom)->PosZ + myrand(0, 10);
			return true;
		}

		nRandom = myrand(0, g_pMain->m_KnStartPositionRandomArray.GetSize() - 1);
		goto GetPosition;
	}

	return GetStartPosition(x, z);
}

void CUser::ResetWindows()
{
	if (isTrading())
		ExchangeCancel();

	if (m_bRequestingChallenge)
		HandleChallengeCancelled(m_bRequestingChallenge);

	if (m_bChallengeRequested)
		HandleChallengeRejected(m_bChallengeRequested);

	// If we're a vendor, close the stall
	if (isMerchanting())
		MerchantClose();

	// If we're just browsing, free up our spot so others can browse the vendor.
	if (m_sMerchantsSocketID >= 0)
		CancelMerchant();

	if (isMining())
		HandleMiningStop((Packet)(WIZ_MINING, MiningStop));

	if (isFishing())
		HandleFishingStop((Packet)(WIZ_MINING, FishingStop));
}

CUser* CUser::GetItemRoutingUser(uint32 nItemID, uint16 sCount)
{
	if (!isInParty())
		return this;

	_ITEM_TABLE* pTable;
	_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr
		|| (pTable = g_pMain->GetItemPtr(nItemID)) == nullptr
		|| pParty->bItemRouting >= MAX_PARTY_USERS)
		return nullptr;

	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser* pUser = g_pMain->GetUserPtr(pParty->uid[pParty->bItemRouting]);

		if (pParty->bItemRouting > 6)
			pParty->bItemRouting = 0;
		else
			pParty->bItemRouting++;

		if (pUser == nullptr
			|| !pUser->isInRange(GetX(), GetZ(), RANGE_50M)
			|| pUser->isDead()
			|| !pUser->CheckWeight(pTable, nItemID, sCount))
			continue;

		return pUser;
	}

	return nullptr;
}

CUser* CUser::GetItemRoutingUserLooting(uint32 nItemID, uint16 sCount)
{
	if (!isInParty())
		return this;

	_ITEM_TABLE* pTable;
	_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr
		|| (pTable = g_pMain->GetItemPtr(nItemID)) == nullptr
		|| pParty->bItemRouting >= MAX_PARTY_USERS)
		return nullptr;

	float PartyLootRange = RANGE_50M + RANGE_50M;

	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser* pUser = g_pMain->GetUserPtr(pParty->uid[pParty->bItemRouting]);

		if (pParty->bItemRouting > 6)
			pParty->bItemRouting = 0;
		else
			pParty->bItemRouting++;

		if (pUser == nullptr
			|| !pUser->isInRange(GetX(), GetZ(), PartyLootRange)
			|| pUser->isDead()
			|| !pUser->CheckWeight(pTable, nItemID, sCount))
			continue;

		return pUser;
	}

	return nullptr;
}

void CUser::SendAnvilRequest(uint16 sNpcID, uint8 bType /*= ITEM_UPGRADE_REQ*/)
{
	Packet result(WIZ_ITEM_UPGRADE, uint8(bType));
	result << sNpcID;
	Send(&result);
}

void CUser::UpdateVisibility(InvisibilityType bNewType)
{
	m_bInvisibilityType = (uint8)(bNewType);
}

/**
* @brief	Forces a reset of the GM's persistent visibility
* 			state.
*/
void CUser::ResetGMVisibility()
{
	if (!isGM()
		|| isTransformed())
		return;

	// Force the client to reset
	if (m_bAbnormalType != ABNORMAL_NORMAL)
	{
		Packet result(WIZ_STATE_CHANGE);
		result << GetID() << uint8(5) << uint8(0);
		Send(&result);
		m_bAbnormalType = ABNORMAL_INVISIBLE;
	}
}

void CUser::BlinkStart(int exBlinkTime)
{
	if (isGM()
		|| GetMap()->isWarZone()
		|| GetMap()->m_bBlinkZone != 1
		|| isSiegeTransformation()
		|| isMonsterTransformation()
		|| isNPCTransformation())
	{
		if (isGM()
			|| isSiegeTransformation()
			|| isMonsterTransformation()
			|| isNPCTransformation())
			return;

		if (m_bAbnormalType == ABNORMAL_BLINKING)
		{
			m_bRegeneType = REGENE_NORMAL;
			m_bCanUseSkills = true;

			if (GetZoneID() == ZONE_CHAOS_DUNGEON
				|| GetZoneID() == ZONE_DUNGEON_DEFENCE
				|| GetZoneID() == ZONE_KNIGHT_ROYALE
				|| GetZoneID() == ZONE_DEATHMATCH)
				StateChangeServerDirect(3, ABNORMAL_CHAOS_NORMAL);
			else
				StateChangeServerDirect(3, ABNORMAL_NORMAL);

			UpdateVisibility(INVIS_NONE);
		}
		return;
	}

	m_bAbnormalType = ABNORMAL_BLINKING;
	m_tBlinkExpiryTime = UNIXTIME + BLINK_TIME + exBlinkTime;
	m_bRegeneType = REGENE_ZONECHANGE;
	m_bCanUseSkills = false;
	UpdateVisibility(INVIS_DISPEL_ON_ATTACK); // AI shouldn't see us
	m_bInvisibilityType = INVIS_NONE; // but players should. 
	StateChangeServerDirect(3, ABNORMAL_BLINKING);
}

void CUser::BlinkTimeCheck()
{
	if (UNIXTIME < m_tBlinkExpiryTime)
		return;

	m_bRegeneType = REGENE_NORMAL;
	m_bCanUseSkills = true;

	if (GetZoneID() == ZONE_CHAOS_DUNGEON
		|| GetZoneID() == ZONE_DUNGEON_DEFENCE
		|| GetZoneID() == ZONE_KNIGHT_ROYALE
		|| GetZoneID() == ZONE_DEATHMATCH)
		StateChangeServerDirect(3, ABNORMAL_CHAOS_NORMAL);
	else
		StateChangeServerDirect(3, ABNORMAL_NORMAL);

	UpdateVisibility(INVIS_NONE);
}



void CUser::SendChat(uint8 chattype, string msg, string Sender) {
	Packet chtpkt;
	ChatPacket::Construct(&chtpkt, chattype, msg.c_str(), Sender.c_str(), GetNation());
	Send(&chtpkt);
}

void CUser::TrapProcess()
{
	// If the time interval has passed
	if ((UNIXTIME - m_tLastTrapAreaTime) >= ZONE_TRAP_INTERVAL)
	{
		if (GetZoneID() == ZONE_BIFROST)
			SendUserStatusUpdate(USER_STATUS_BLIND, USER_STATUS_INFLICT);

		HpChange(-ZONE_TRAP_DAMAGE, this);
		m_tLastTrapAreaTime = UNIXTIME;
	}
}

void CUser::KickOutZoneUser(bool home, uint8 nZoneID)
{
	if (home)
	{
		C3DMap* pMap = g_pMain->GetZoneByID(nZoneID);
		if (pMap == nullptr)
			return;

		int eventID = 0;
		int random = myrand(0, 9000);
		if (random >= 0 && random < 3000)			eventID = 0;
		else if (random >= 3000 && random < 6000)	eventID = 1;
		else if (random >= 6000 && random < 9001)	eventID = 2;

		_REGENE_EVENT* pRegene = pMap->GetRegeneEvent(eventID);
		if (pRegene == nullptr)
		{
			KickOutZoneUser();
			return;
		}

		ZoneChange(pMap->m_nZoneNumber,
			pRegene->fRegenePosX + (float)myrand(0, (int)pRegene->fRegeneAreaX),
			pRegene->fRegenePosZ + (float)myrand(0, (int)pRegene->fRegeneAreaZ));
		return;
	}

	// Teleport the player to their native zone.
	short x, z;

	if (GetStartPosition(x, z, GetNation()))
		ZoneChange(GetNation(), x, z);
	else
	{
		TRACE("### KickOutZoneUser - StartPosition not found : Nation=%d", GetNation());
	}
}

void CUser::NativeZoneReturn()
{
	if (GetLevel() < MIN_LEVEL_ARDREAM)
	{
		_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(ZONE_MORADON);
		if (pStartPosition == nullptr)
			return;

		m_bZone = ZONE_MORADON;

		m_curx = (float)((m_bNation == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX));
		m_curz = (float)((m_bNation == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ));
	}
	else
	{
		_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(m_bNation);
		if (pStartPosition == nullptr)
			return;

		m_bZone = m_bNation;

		m_curx = (float)((m_bNation == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX));
		m_curz = (float)((m_bNation == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ));
	}
}

/**
* @brief	Sends a packet to all players within the
* 			user's current region and surrounding regions
* 			(i.e. visible area)
*
* @param	pkt		   	The packet.
* @param	pExceptUser	User to except. If specified, will ignore this user.
*/
void CUser::SendToRegion(Packet* pkt, CUser* pExceptUser /*= nullptr*/, uint16 nEventRoom /*-1*/) {
	g_pMain->Send_Region(pkt, GetMap(), GetRegionX(), GetRegionZ(), pExceptUser, nEventRoom);
}

/**
* @brief	Sends a packet to all players within the
* 			user's current zone.
*
* @param	pkt		   	The packet.
* @param	pExceptUser	User to except. If specified, will ignore this user.
*/
void CUser::SendToZone(Packet* pkt, CUser* pExceptUser /*= nullptr*/, uint16 nEventRoom /*-1*/, float fRange)
{
	g_pMain->Send_Zone(pkt, GetZoneID(), pExceptUser, 0, nEventRoom, fRange);
}

// We have no clan handler, we probably won't need to implement it (but we'll see).
void CUser::SendClanUserStatusUpdate(bool bToRegion /*= true*/)
{
	Packet result(WIZ_KNIGHTS_PROCESS, uint8(KNIGHTS_MODIFY_FAME));
	result << uint8(1) << GetSocketID()
		<< GetClanID() << GetFame();

	// TODO: Make this region code user-specific to perform faster.
	if (bToRegion)
		SendToRegion(&result);
	else
		Send(&result);
}

void CUser::HandleHelmet(Packet& pkt)
{
	if (isDead())
		return;

	Packet result(WIZ_HELMET);
	m_bIsHidingHelmet = pkt.read<bool>();
	m_bIsHidingCospre = pkt.read<bool>();
	result << m_bIsHidingHelmet << m_bIsHidingCospre << uint32(GetSocketID());

	if (GetEventRoom() > 0)
		SendToRegion(&result, nullptr, GetEventRoom());
	else
		SendToRegion(&result);

	if (m_bIsHidingCospre)
	{
		uint8 equippedItems[] = { BREAST, LEG, GLOVE, FOOT };
		foreach_array(i, equippedItems)
		{
			_ITEM_DATA* pItem = GetItem(equippedItems[i]);

			if (pItem == nullptr)
			{
				UserLookChange(equippedItems[i], 0, 0);
				continue;
			}
			UserLookChange(equippedItems[i], pItem->nNum, pItem->sDuration);
		}
	}
}

bool Unit::isInAttackRange(Unit* pTarget, _MAGIC_TABLE* pSkill /*= nullptr*/)
{
	if (pTarget == nullptr)
		return false;

	if (pTarget == this
		|| !isPlayer())
		return true;

	const float fBaseMeleeRange = 15.0f; // far too generous
	const float fBaseRangedRange = 65.0f;

	float fRange = fBaseMeleeRange, fWeaponRange = 0.0f;

	_ITEM_DATA* pItem = nullptr;
	_ITEM_TABLE* pTable = TO_USER(this)->GetItemPrototype(TO_USER(this)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND, pItem);

	if (pTable != nullptr
		&& pItem->sDuration > 0)
	{
		fWeaponRange = pTable->m_sRange;
	}
	else
	{
		pTable = TO_USER(this)->GetItemPrototype(TO_USER(this)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND, pItem);
		if (pTable != nullptr
			&& pItem->sDuration != 0)
			fWeaponRange = pTable->m_sRange;
	}

	if (pSkill != nullptr)
	{
		// Not an attack skill, don't need to enforce these restrictions.
		if (pSkill->bMoral != MORAL_ENEMY && pSkill->bMoral > MORAL_PARTY)
			return true;

		// For physical attack skills (type 1 - melee, type 2 - ranged), we'll need take into account 
		// the weapon's range.
		if (pSkill->bType[0] != 3)
			fRange = fWeaponRange;

		// For physical melee & magic skills, try to use the skill's range if it's set.
		// Need to allow more for lag, and poorly thought out skill ranges.
		// If not, resort to using the weapon range -- or predefined 15m range in the case of type 3 skills.
		if (pSkill->bType[0] != 2)
		{
			return isInRangeSlow(pTarget, fBaseMeleeRange + (pSkill->sRange == 0 ? fRange : pSkill->sRange) + (pSkill->bType[0] == 1 ? fWeaponRange : 0));
		}
		// Ranged skills (type 2) don't typically have the main skill range set to anything useful, so
		// we need to allow for the: bow's range, flying skill-specific range, and an extra 50m for the
		// also extremely poorly thought out ranges.
		else
		{
			_MAGIC_TYPE2* pType2 = g_pMain->m_Magictype2Array.GetData(pSkill->iNum);
			return pType2 != nullptr && isInRangeSlow(pTarget, fRange + pType2->sAddRange + fBaseRangedRange);
		}
	}

	// Regular attack range.
	if (fWeaponRange != 0.0f)
		fRange = fBaseMeleeRange + fWeaponRange;

	return isInRangeSlow(pTarget, fRange);
}

/**
* @brief	Determine if we can use the specified item
* 			via the magic/skill system.
*
* @param	itemid	The ID of the item.
* @param	count 	Stack (can probably disregard, as it's always 1).
*
* @return	true if we can use item, false if not.
*/
bool CUser::CanUseItem(uint32 nItemID, uint16 sCount /*= 1*/)
{
	_ITEM_TABLE* pItem = pItem = g_pMain->GetItemPtr(nItemID);
	if (pItem == nullptr)
		return false;

	// Disable scroll usage while transformed.
	if (isTransformed())
	{
		// Various NPC transformations ("Transform Scrolls") are exempt from this rule -- it's just monsters.
		// Also, siege transformations can use their own buff scrolls.
		if (isNPCTransformation()
			&& isSiegeTransformation())
			return false;
	}

	// If the item's class specific, ensure it can be used by this user.
	if ((pItem->m_bClass != 0 && !JobGroupCheck(pItem->m_bClass))
		// Check the item's level requirement
		|| (GetLevel() < pItem->m_bReqLevel || GetLevel() > pItem->m_bReqLevelMax)
		// Ensure the item exists.
		|| !CheckExistItem(nItemID, sCount))
		return false;

	return true;
}

void CUser::SendUserStatusUpdate(UserStatus type, UserStatusBehaviour status)
{
	Packet result(WIZ_ZONEABILITY, uint8(2));
	result << uint8(type) << uint8(status);
	/*
	1				, 0 = Cure damage over time
	1				, 1 = Damage over time
	2				, 0 = Cure poison
	2				, 1 = poison (purple)
	3				, 0 = Cure disease
	3				, 1 = disease (green)
	4				, 1 = blind
	5				, 0 = Cure grey HP
	5				, 1 = HP is grey (not sure what this is)
	*/
	Send(&result);

	if (isInParty())
		SendPartyStatusUpdate(type, status);
}

/**
* @brief	Gets an item's prototype from a slot in a player's inventory.
*
* @param	pos	The position of the item in the player's inventory.
* @returns	nullptr if an invalid position is specified, or if there's no item at that position.
* 			The item's prototype (_ITEM_TABLE *) otherwise.
*/
_ITEM_TABLE* CUser::GetItemPrototype(uint8 pos, _ITEM_DATA*& pItem)
{
	if (pos >= INVENTORY_TOTAL)
		return nullptr;

	if (GetZoneID() == ZONE_KNIGHT_ROYALE)
		pItem = GetKnightRoyaleItem(pos);
	else
		pItem = GetItem(pos);

	return pItem->nNum == 0 ? nullptr : g_pMain->GetItemPtr(pItem->nNum);
}

/**
* @brief	Checks & removes any expired skills from
* 			the saved magic list.
*/
void CUser::CheckSavedMagic()
{
	Guard lock(m_savedMagicLock);
	if (m_savedMagicMap.empty())
		return;

	set<uint32> deleteSet;
	foreach(itr, m_savedMagicMap)
	{
		if (itr->second <= UNIXTIME2)
			deleteSet.insert(itr->first);
	}

	foreach(itr, deleteSet)
		m_savedMagicMap.erase(*itr);
}

/**
* @brief	Inserts a skill to the saved magic list
* 			to persist across zone changes/logouts.
*
* @param	nSkillID 	Identifier for the skill.
* @param	sDuration	The duration.
*/
void CUser::InsertSavedMagic(uint32 nSkillID, uint16 sDuration)
{
	Guard lock(m_savedMagicLock);
	UserSavedMagicMap::iterator itr = m_savedMagicMap.find(nSkillID);

	// If the buff is already in the savedBuffMap there's no need to add it again!
	if (itr != m_savedMagicMap.end())
		return;

	m_savedMagicMap.insert(make_pair(nSkillID, UNIXTIME2 + sDuration * 1000));
}

/**
* @brief	Removes the specified skill from the saved magic list.
*
* @param	nSkillID	Identifier for the skill.
*/
void CUser::RemoveSavedMagic(uint32 nSkillID)
{
	Guard lock(m_savedMagicLock);
	m_savedMagicMap.erase(nSkillID);
}

/**
* @brief	Checks if the given skill ID is already in our
* 			saved magic list.
*
* @param	nSkillID	Identifier for the skill.
*
* @return	true if the skill exists in the saved magic list, false if not.
*/
bool CUser::HasSavedMagic(uint32 nSkillID)
{
	Guard lock(m_savedMagicLock);
	return m_savedMagicMap.find(nSkillID) != m_savedMagicMap.end();
}

/**
* @brief	Gets the duration for a saved skill,
* 			if applicable.
*
* @param	nSkillID	Identifier for the skill.
*
* @return	The saved magic duration.
*/
int16 CUser::GetSavedMagicDuration(uint32 nSkillID)
{
	Guard lock(m_savedMagicLock);
	auto itr = m_savedMagicMap.find(nSkillID);
	if (itr == m_savedMagicMap.end())
		return 0;

	return int16((itr->second - UNIXTIME2) / 1000);
}

/**
* @brief	Recasts any saved skills on login/zone change.
*/
void CUser::RecastSavedMagic(uint8 buffType /* = 0*/)
{
	Guard lock(m_savedMagicLock);
	UserSavedMagicMap castSet;
	foreach(itr, m_savedMagicMap)
	{
		if (itr->first != 0
			|| itr->second != 0)
			castSet.insert(make_pair(itr->first, itr->second));
	}

	if (castSet.empty())
		return;

	foreach(itr, castSet)
	{
		if (buffType > 0)
		{
			_MAGIC_TYPE4* pType = g_pMain->m_Magictype4Array.GetData(itr->first);

			if (pType == nullptr)
				continue;

			if (pType->bBuffType != buffType)
				continue;
		}

		if (isSiegeTransformation())
			continue;

		MagicInstance instance;
		instance.sCasterID = GetID();
		instance.sTargetID = GetID();
		instance.nSkillID = itr->first;
		instance.sSkillCasterZoneID = GetZoneID();
		instance.bIsRecastingSavedMagic = true;

		instance.Run();
	}
}

/**
* @brief	Recasts any lockable scrolls on debuff.
*/
void CUser::RecastLockableScrolls(uint8 buffType)
{
	InitType4(false, buffType);
	RecastSavedMagic(buffType);
}

//Stop
void CUser::InitializeStealth()
{
	Packet pkt(WIZ_STEALTH);
	pkt << uint8(0) << uint16(0);
	Send(&pkt);
}

uint32 CUser::GetEventTrigger()
{
	CNpc* pNpc = g_pMain->GetNpcPtr(GetTargetID(), GetZoneID());
	if (pNpc == nullptr)
		return 0;

	foreach_stlmap(itr, g_pMain->m_EventTriggerArray)
	{
		_EVENT_TRIGGER* pEventTrigger = itr->second;
		if (pEventTrigger == nullptr
			|| pNpc->GetType() != pEventTrigger->bNpcType)
			continue;

		if (pNpc->m_byTrapNumber == pEventTrigger->sNpcID)
			return pEventTrigger->nTriggerNum;
	}

	return 0;
}

void CUser::RemoveStealth()
{
	if (this->m_bInvisibilityType != INVIS_NONE)
	{
		CMagicProcess::RemoveStealth(this, INVIS_DISPEL_ON_MOVE);
		CMagicProcess::RemoveStealth(this, INVIS_DISPEL_ON_ATTACK);
	}
}

void CUser::RobChaosSkillItems()
{
	if (GetItemCount(ITEM_LIGHT_PIT) > 0)
		RobItem(ITEM_LIGHT_PIT, GetItemCount(ITEM_LIGHT_PIT));
	if (GetItemCount(ITEM_DRAIN_RESTORE) > 0)
		RobItem(ITEM_DRAIN_RESTORE, GetItemCount(ITEM_DRAIN_RESTORE));
	if (GetItemCount(ITEM_KILLING_BLADE) > 0)
		RobItem(ITEM_KILLING_BLADE, GetItemCount(ITEM_KILLING_BLADE));
}

void CUser::LogosShout(Packet& pkt)
{
	uint8 sOpCode, R, G, B, C;
	string sMessage;
	DateTime time;

	pkt.SByte();
	pkt >> sOpCode >> R >> G >> B >> C >> sMessage;

	Packet result(WIZ_LOGOSSHOUT);

	if (!CheckExistItem(LOGOSSHOUT1, 1))
		sOpCode = 2;
	else if (sMessage.empty()
		|| sMessage.size() > 128)
		sOpCode = 2;
	else
		sOpCode = 1;

	string bMessage = GetName() + ": " + sMessage;
	result.SByte();

	if (sOpCode == 1)
		result << uint8(2) << sOpCode << R << G << B << C << bMessage;
	else
		result << uint8(1) << sOpCode;

	if (CheckExistItem(LOGOSSHOUT1, 1))
		RobItem(LOGOSSHOUT1, 1);

	if (sOpCode == 1)
		g_pMain->WriteChatLogFile(string_format("[ LOGOS' SHOULT - %d:%d:%d ] %s : %s ( Zone=%d, X=%d, Z=%d )\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), sMessage.c_str(), GetZoneID(), uint16(GetX()), uint16(GetZ())));

	if (sOpCode == 1)
		g_pMain->Send_All(&result);
	else
		Send(&result);
}

bool CUser::VaccuniBoxExp(uint16 MonsterType)
{
	if (!isInGame())
		return false;

	if (MonsterType <= 0
		&& MonsterType >= 4)
		return true;

	switch (MonsterType)
	{
	case 1:
		if ((CheckExistEvent(793, 1)
			|| CheckExistEvent(794, 1)))
		{
			_ITEM_DATA* pItem;
			_ITEM_TABLE* pTable = GetItemPrototype(RIGHTHAND, pItem);
			if (pItem != nullptr
				&& pTable != nullptr
				&& pItem->sDuration > 0
				&& pTable->isTimingFlowTyon())
				return false;
		}
		break;
	case 2:
		if (CheckExistEvent(798, 1)
			|| CheckExistEvent(799, 1))
		{
			_ITEM_DATA* pItem;
			_ITEM_TABLE* pTable = GetItemPrototype(RIGHTHAND, pItem);
			if (pItem != nullptr
				&& pTable != nullptr
				&& pItem->sDuration > 0
				&& pTable->isTimingFlowHellHound())
				return false;
		}
		break;
	case 3:
		if ((CheckExistEvent(795, 1)
			|| CheckExistEvent(796, 1)))
		{
			_ITEM_DATA* pItem;
			_ITEM_TABLE* pTable = GetItemPrototype(RIGHTHAND, pItem);
			if (pItem != nullptr
				&& pTable != nullptr
				&& pItem->sDuration > 0
				&& pTable->isTimingFlowMeganthereon())
				return false;
		}
		break;
	default:
		return true;
		break;
	}
	return true;
}

bool CUser::VaccuniAttack()
{
	CNpc* pNpc = g_pMain->GetNpcPtr(GetTargetID(), GetZoneID());

	if (pNpc == nullptr
		|| !isInGame())
		return false;

	switch (pNpc->GetProtoID())
	{
	case 4301:
	case 4351:
		if (CheckExistEvent(793, 1)
			|| CheckExistEvent(794, 1))
		{
			_ITEM_DATA* pItem;
			_ITEM_TABLE* pTable = GetItemPrototype(RIGHTHAND, pItem);
			if (pItem != nullptr
				&& pTable != nullptr
				&& pItem->sDuration > 0
				&& pTable->isTimingFlowTyon())
				return false;
		}
		break;
	case 605:
	case 611:
	case 655:
		if (CheckExistEvent(795, 1)
			|| CheckExistEvent(796, 1))
		{
			_ITEM_DATA* pItem;
			_ITEM_TABLE* pTable = GetItemPrototype(RIGHTHAND, pItem);
			if (pItem != nullptr
				&& pTable != nullptr
				&& pItem->sDuration > 0
				&& pTable->isTimingFlowMeganthereon())
				return false;
		}
		break;
	case 616:
	case 666:
		if (CheckExistEvent(798, 1)
			|| CheckExistEvent(799, 1))
		{
			_ITEM_DATA* pItem;
			_ITEM_TABLE* pTable = GetItemPrototype(RIGHTHAND, pItem);
			if (pItem != nullptr
				&& pTable != nullptr
				&& pItem->sDuration > 0
				&& pTable->isTimingFlowHellHound())
				return false;
		}
		break;
	default:
		return true;
		break;
	}

	return true;
}
int CUser::GetExpPercent()
{
	foreach(itr, g_pMain->m_LevelUpArray)
	{
		if (itr->second->Level == GetLevel()
			&& itr->second->RebithLevel == GetRebirthLevel())
			return itr->second->Exp == m_iExp ? 100 : 0;
	}

	return 0;
}

void CUser::HackToolList(bool bSender)
{
	Packet result(WIZ_HACKTOOL);

	if (g_pMain->m_HackToolListArray.GetSize() <= 0)
		return;

	if (!bSender)
	{
		result << uint8(6) << uint16(-1);
		Send(&result);
		return;
	}

	result << uint8(1) << (uint16)g_pMain->m_HackToolListArray.GetSize();
	result.SByte();

	foreach_stlmap(itr, g_pMain->m_HackToolListArray)
		result << itr->second->nChecking;

	Send(&result);
}

void CUser::HandleHackTool(Packet& pkt)
{
	uint16 mSize;
	std::string mTools;
	pkt.SByte();
	pkt >> mSize >> mTools;

	if (g_pMain->m_HackToolListArray.GetSize() <= 0)
		return;

	foreach_stlmap(itr, g_pMain->m_HackToolListArray)
	{
		if (STRCASECMP(itr->second->nChecking.c_str(), mTools.c_str()) == 0)
		{
			Disconnect();
			break;
		}
	}
}

#pragma region CUser::isInPetZone()

bool CUser::isInPetZone()
{

	if (GetZoneID() == ZONE_RONARK_LAND
		|| GetZoneID() == ZONE_RONARK_LAND_BASE
		|| GetZoneID() == ZONE_ARDREAM
		|| GetZoneID() == ZONE_SNOW_BATTLE
		|| GetZoneID() == ZONE_BATTLE
		|| GetZoneID() == ZONE_BATTLE2
		|| GetZoneID() == ZONE_BATTLE3
		|| GetZoneID() == ZONE_BATTLE4
		|| GetZoneID() == ZONE_BATTLE5
		|| GetZoneID() == ZONE_BATTLE6
		|| GetZoneID() == ZONE_JURAID_MOUNTAIN
		|| GetZoneID() == ZONE_BORDER_DEFENSE_WAR
		|| GetZoneID() == ZONE_CLAN_WAR_ARDREAM
		|| GetZoneID() == ZONE_CLAN_WAR_RONARK
		|| GetZoneID() == ZONE_HELL_ABYSS
		|| GetZoneID() == ZONE_DELOS
		|| GetZoneID() == ZONE_DESPERATION_ABYSS
		|| GetZoneID() == ZONE_BIFROST)
		return true;
	else
		return false;
}

void CUser::ReqSaveLootSettings(Packet& pkt)
{


	uint8 iWarrior, iRogue, iMage, iPriest, iWeapon, iArmor, iAccessory, iNormal, iUpgrade, iCraft, iRare, iMagic, iUnique, iConsumable, camera;
	uint32 iPrice;

	Camerazoom = camera;

	pkt >> iWarrior >> iRogue >> iMage >> iPriest >> iWeapon >> iArmor >> iAccessory >> iNormal >> iUpgrade >> iCraft >> iRare >> iMagic >> iUnique >> iConsumable >> iPrice >> camera;

	/*if (iWarrior > 1 || iWarrior < 0 || iRogue > 1 || iRogue < 0 || iMage > 1 || iMage < 0 || iPriest > 1 || iPriest < 0)
		return;

	if (iWeapon > 1 || iWeapon < 0 || iArmor > 1 || iArmor < 0 || iAccessory > 1 || iAccessory)
		return;

	if (iNormal > 1 || iNormal < 0 || iUpgrade > 1 || iUpgrade < 0 || iCraft > 1 || iCraft < 0 || iRare > 1 || iRare < 0 || iMagic > 1 || iMagic < 0 || iUnique > 1 || iUnique < 0)
		return;

	if (iConsumable > 1 || iConsumable < 0)
		return;*/

	if (iPrice > 999999999 || iPrice < 0)
		return;


	g_DBAgent.UpdateLootSettings(this, GetName(), iWarrior, iRogue, iMage, iPriest, iWeapon, iArmor, iAccessory, iNormal, iUpgrade, iCraft, iRare, iMagic, iUnique, iConsumable, iPrice);

	g_DBAgent.LoadLootSettings(GetAccountName(), this);
}