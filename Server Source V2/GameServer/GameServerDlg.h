#pragma once

#include "resource.h"
#include "LuaEngine.h"

#include "Define.h"
#include "ChatHandler.h"

class C3DMap;
class CUser;
class CBot;
#include "../shared/HardwareInformation.h"
#include "LoadServerData.h"
#include "BotHandler.h"
#include "User.h"
#include "NpcThread.h"
#include "../shared/ClientSocketMgr.h"
#include "MagicProcess.h"

typedef std::map<std::string, CUser*>  NameMap;
typedef std::map<std::string, CBot*>	BotNameMap;
typedef std::map<uint16, uint16>          ForgettenTempleMonsterList;
typedef	std::map<uint16, uint16>		   UnderTheCastleMonsterList;

class CGameServerDlg
{
public:
	bool startLoginServer;
	bool startGameServer;
	CGameServerDlg();
	bool Startup();
	uint8 SelectChar;
	bool m_bisGuardActive;
	// NPC Related Load Methods
	bool LoadMonsterItemTable();
	bool LoadNpcItemTable();
	bool LoadMakeItemGroupTable();
	bool LoadMakeWeaponItemTableData();
	bool LoadMakeDefensiveItemTableData();
	bool LoadMakeGradeItemTableData();
	bool LoadMakeLareItemTableData();
	bool LoadNpcTableData(bool bNpcData = true);
	bool LoadNpcPosTable();
	bool LoadDrakiTowerTables();
	bool LoadUserHackToolTables();
	bool LoadKnightSiegeWarRankTable();
	bool LoadKnightSiegeCastellanRankTable();
	bool LoadKnightRoyaleChestListTable();
	bool LoadServerSettingsData();
	bool CreateNpcThread();
	bool HandleTournamentStart(std::string& clan1, std::string& clan2, uint8 zone);
	std::atomic_int32_t m_iItemUniqueID;

	// NPC Related Variables
	Atomic<uint16>	m_TotalNPC;				// the atomic variable that holds total NPC number
	std::vector<uint32> m_FreeNpcList;
	std::recursive_mutex m_FreeNpcListLock;
	Atomic<uint16>	m_CurrentNPC;			// the atomic variable that holds total NPC number alive
	uint16			m_sTotalMap;			// Total number of Zone
	uint16			m_sMapEventNpc;			// the total Event NPC number in a Map
	bool			g_bNpcExit;

	void Initialize();
	bool StartUserSocketSystem();
	bool StartTheardSocketSystem();
	bool StartLisansSystem();
	bool LoadTables();
	void CreateDirectories();

	void GetTimeFromIni();
	void GetEventAwardsIni();
	void SetMiningDrops();

	bool ChaosStoneLoad();
	void ChaosStoneRespawnTimer();
	void ChaosStoneSummon(uint16 ChaosGetID, uint8 RankID, uint16 ZoneID);
	uint8 ChaosStoneSummonSelectStage(uint16 ChaosGetID, uint8 RankID, uint16 ZoneID);
	uint8 ChaosStoneSummonSelectFamilyStage(uint8 ChaosIndex, uint8 FamilyID, uint16 ZoneID);


	void AutoOnlineCount(); //yenicount
	bool LoadDungeonDefenceMonsterTable();
	bool LoadDungeonDefenceStageTable();
	bool LoadKnightRoyaleStatsTable();
	bool LoadKnightReturnLetterGiftItemTable();
	bool LoadKnightRoyaleBeginnerItemTable();
	bool LoadAchieveTitleTable();
	bool LoadAchieveWarTable();
	bool LoadAchieveMainTable();
	bool LoadAchieveMonsterTable();
	bool LoadAchieveNormalTable();
	bool LoadAchieveComTable();
	bool LoadItemTable();
	bool LoadItemSellTable();
	bool LoadItemPremiumGiftTable();
	bool LoadSetItemTable();
	bool LoadItemExchangeTable();
	bool LoadItemExchangeExpTable();
	bool LoadItemSpecialExchangeTable();
	bool LoadItemExchangeCrashTable();
	bool LoadItemUpgradeTable();
	bool LoadItemOpTable();
	bool LoadServerResourceTable();
	bool LoadDeathMatchTable();
	bool LoadDeathMatchTable1();
	bool LoadDeathMatchTable2();
	bool LoadDeathMatchTable3();
	bool LoadQuestHelperTable();
	bool LoadQuestMonsterTable();
	bool LoadMagicTable();
	bool LoadMagicType1();
	bool LoadMagicType2();
	bool LoadMagicType3();
	bool LoadMagicType4();
	bool LoadMagicType5();
	bool LoadMagicType6();
	bool LoadMagicType7();
	bool LoadMagicType8();
	bool LoadMagicType9();
	bool LoadRentalList();
	bool LoadCoefficientTable();
	bool LoadLevelUpTable();
	bool LoadAllKnights();
	bool LoadKnightsAllianceTable(bool bIsslient = false);
	bool LoadKnightsSiegeWarsTable(bool bIsslient = false);
	bool LoadUserRankings();
	bool LoadCharacterSealItemTable();
	bool LoadCharacterSealItemTableAll();
	void CleanupUserRankings();
	bool LoadKnightsCapeTable();
	bool LoadKnightsRankTable(bool bWarTime = false, bool bIsslient = false);
	bool LoadStartPositionTable();
	bool LoadStartPositionRandomTable();
	bool LoadKnightRoyaleStartPositionRandomTable();
	bool LoadBattleTable();
	bool LoadKingSystem();
	bool LoadSheriffTable();
	bool LoadMonsterResourceTable();
	bool LoadMonsterSummonListTable();
	bool LoadChaosStoneMonsterListTable();
	bool LoadChaosStoneCoordinateTable();
	bool LoadChaosStoneStage();
	bool LoadMonsterRespawnVeriableListTable();
	bool LoadMonsterRespawnStableListTable();
	bool LoadMonsterUnderTheCastleTable();
	bool LoadMonsterStoneListInformationTable();
	bool LoadJuraidMountionListInformationTable();
	bool LoadMiningFishingItemTable();
	bool LoadPremiumItemTable();
	bool LoadPremiumItemExpTable();
	bool LoadUserDailyOpTable();
	bool LoadEventTriggerTable();
	bool LoadMonsterChallengeTable();
	bool LoadMonsterChallengeSummonListTable();
	bool LoadMiningExchangeListTable();
	bool LoadUserItemTable();
	bool LoadObjectPosTable();
	bool LoadBanishWinnerTable();
	bool LoadKnightsCastellanCapeTable();
	bool LoadGiveItemExchangeTable();
	bool LoadDarkKnightMaterialMonsterDeadTable();
	bool LoadEventScheduleStatusTable();
	bool LoadTournamentScheduleStatusTable();
	bool LoadRoomEventPlayTimerTable();
	bool LoadRoyaleEventPlayTimerTable();
	bool LoadBeefEventPlayTimerTable();
	bool LoadCswEventPlayTimerTable();
	bool LoadPetStatsInfoTable();
	bool LoadPetImageChangeTable();
	bool LoadQuestSkillSetUpTable();

	bool LoadMonsterDropMainTable();
	bool LoadMonsterItemGroupItemTable();

	bool LoadBotTable();

	bool MapFileLoad();
	bool LoadNoticeData();
	bool LoadNoticeUpData();
	bool GameStartClearRemainUser();
	bool LoadPusItemsTable();
	bool LoadMonsterBowlTable(); //Bowl Event MeLanor










	/*CYBERACS System Hook System*/
	bool LoadCyberACSSettings();
	bool LoadCyberACSPusLoad();
	bool MannerStoreLoad();
	bool LoadCyberACSCollectionRaceSettings();
	bool LoadCyberACSCollectionRaceHuntList();
	bool LoadLotterySettings();
	bool LoadDailySettings();
	bool LoadDailyHuntList();
	bool GAME_SERVER_SETTINGS();


	static uint32 THREADCALL Timer_DeathMatch(void* lpParam);
	static uint32 THREADCALL Timer_CheckGameEvents(void* lpParam);
	static uint32 THREADCALL Timer_UpdateGameTime(void* lpParam);
	static uint32 THREADCALL Timer_UpdateSessions(void* lpParam);
	static uint32 THREADCALL Timer_UpdateConcurrent(void* lpParam);
	static uint32 THREADCALL Timer_NpcThreadHandleRequests(void* lpParam);

	uint32 m_checktime;

	void NpcThreadHandleRequests();
	void ReqUpdateConcurrent();
	void GameEventMainTimer();
	void SendFlyingSantaOrAngel();
	void BattleZoneCurrentUsers();
	void GetCaptainUserPtr();
	void Send_CommandChat(Packet* pkt, int nation, CUser* pExceptUser = nullptr);
	void SendItemZoneUsers(uint8 ZoneID, uint32 nItemID, uint16 sCount = 1, uint32 Time = 0);
	void SendItemUnderTheCastleRoomUsers(uint8 ZoneID, uint16 Room, float GetX, float GetZ);
	void KickOutZoneUsers(uint8 ZoneID, uint8 TargetZoneID = 0, uint8 bNation = 0);
	void SendItemEventRoom(uint16 nEventRoom, uint32 nItemID, uint16 sCount = 1);
	uint64 GenerateItemSerial();
	uint32 GenerateItemUniqueID();
	int KickOutAllUsers();
	void CheckAliveUser();
	int GetKnightsGrade(uint32 nPoints);
	//uint16 GetKnightsAllMembers(uint16 sClanID, Packet & result, uint16 & pktSize, bool bClanLeader);
	void GetUserRank(CUser* pUser);
	void Announcement(uint16 type, int nation = 0, int chat_type = 8, CUser* pExceptUser = nullptr, CNpc* pExpectNpc = nullptr);
	void ResetBattleZone(uint8 bType);
	void BanishLosers();
	void BanishSiegeWarFareLosers();
	void BattleZoneVictoryCheck();
	void BattleZoneOpenTimer();
	void BattleZoneOpen(int nType, uint8 bZone = 0);
	void BattleEventGiveItem(int nType);
	void BattleZoneRemnantSpawn();
	void BattleZoneClose();
	void BattleZoneResult(uint8 nation);
	void BattleWinnerResult(BattleWinnerTypes winnertype);
	void BattleZoneSelectCommanders();
	void BattleZoneResetCommanders();

	void EventMainTimer();
	void tournamentEventTimer();
	void VirtualEventTimer();
	void VirtualEventLocalTimer();
	void SingleLunarEventTimer();
	void SingleOtherEventTimer();
	void SingleOtherEventLocalTimer();

	void DeathMatchManuelOpening();
	void ChaosExpansionManuelOpening();
	void BorderDefenceWarManuelOpening();
	void JuraidMountainManuelOpening();
	void BeefEventManuelOpening();
	void KnightRoyaleManuelOpening();

	void AliveUserCheck();
	void Send_PartyMember(int party, Packet* result);
	void Send_KnightsMember(int index, Packet* pkt);
	void Send_KnightsAlliance(uint16 sAllianceID, Packet* pkt);
	void SetGameTime();
	void ResetPlayerRankings();
	void UpdateWeather();
	void UpdateGameTime();
	void ResetLoyaltyMonthly();
	CNpc* FindNpcInZone(uint16 sPid, uint8 byZone);
	uint8	MaxLevel62Control;

	CNpc* GetPetPtr(int16 m_sPetID, uint8 byZone);

	void ForgettenTempleEventTimer();
	void DrakiTowerLimitReset();
	void UnderTheCastleTimer();
	time_t	m_lastBlessTime;
	time_t	m_lastBorderTime;
	void TempleEventTimer();

	void ResetPlayerKillingRanking();
	void TrashBorderDefenceWarRanking();
	void TrashChaosExpansionRanking();
	void TrashDeathMatchRanking();

	void DungeonDefenceTimer();
	void SummonDungeonDefenceMonsters(uint16 EventRoom);
	void SendDungeonDefenceDetail(uint16 EventRoom);
	uint8 DungeonDefenceSelectStage(uint16 EventRoom);
	void DungeonDefenceUserisOut(uint16 EventRoom);

	void DrakiTowerRoomCloseTimer();
	void DrakiTowerRoomCloseUserisOut(uint16 EventRoom);

	void KnightRoyaleTimer();
	void KnightRoyaleStart();
	void KnightRoyaleEventManage();
	void KnightRoyaleGetActiveEventTime(CUser* pUser);
	void KnightRoyaleUserTeleport();
	void KnightRoyaleGasTimer();
	void KnightRoyaleGasStepSendPacket();
	void KnightRoyalSendJoinScreen(CUser* pUser = nullptr);
	bool KnightRoyaleAddEventUser(CUser* pUser);
	void KnightRoyaleRemoveEventUser(CUser* pUser);
	void KnightRoyaleFinishCheck();
	void KnightRoyaleClose();
	void KnightRoyaleTotalReset();
	void KnightRoyaleTreasureChestSpawn();
	void KnightRoyaleTreasureChestSpawnTimer();
	void KnightRoyaleTreasureChestSpawnPacket(uint8 ID);
	void KnightRoyaleMobSpawn();
	uint16 KnightRoyaleGetRankReward();
	uint16 KnightRoyaleGetExpByLevel(int nLevel);


	void CastleSiegeWarMainTimer();
	void CastleSiegeWarUserTools(bool Notice = false, uint8 NoticeType = -1, bool Flag = false, bool KickOutUser = false, bool Town = false);
	void CswRemainingTimeNotice(uint8 NoticeType, int32 Time);
	void CastleSiegeWarDeatchmatchOpen();
	void CastleSiegeDeatchmatchClose();
	void CastleSiegeCastellanWarOpen();
	void CastleSiegeDeathmatchBarrackCreated();
	void CastleSiegeWarGenelClose();
	void CastleSiegeWarDeathMatchWinnerCheck();
	void CastleSiegeWarFinishWinnerCheck();
	void CastleSiegeWarReset();
	void CastleSiegeWarAutoKilledBarrack();
	void CastleSiegeWarFragSaved();
	void CastleSiegeWarDeathMatchWinnerReward(uint16 WinnerClanID, uint8 WinnerRankID);
	void UpdateCSWRemainBarracks(uint16 sClanID);
	void CastleSiegeWarGameStartController();


	bool ChaosStoneRespawnOkey;

	void Send_Noah_Knights(Packet* pkt);
	std::string GetBattleAndNationMonumentName(int16 TrapNumber = -1, uint8 ZoneID = 0);
	void CheckNationMonumentRewards();

	void WantedEventTesterVoid(uint8 GetZoneID, uint8 GetNation);
	void WantedEventMainTimer();
	void WantedEventManuelStart(uint8 Type);
	void WantedEventSelectingSystem(uint8 ZoneID);

	void TemplEventJuraidSendJoinScreenUpdate(CUser* pUser = nullptr);
	void TemplEventBDWSendJoinScreenUpdate(CUser* pUser = nullptr);
	void TemplEventChaosSendJoinScreenUpdate(CUser* pUser = nullptr);
	void TemplEventDeathMatchSendJoinScreenUpdate(CUser* pUser = nullptr);
	void TempleEventChaosShowCounterScreen();
	void DeathMatchPrepare();
	void TempleEventReset(uint16 bActiveEvent);
	void TempleEventKickOutUser(CUser* pUser);
	void TempleEventSendActiveEventTime(CUser* pUser);
	void TempleEventGetActiveEventTime(CUser* pUser);
	void TempleEventFinish(int16 nRoom = -1);
	uint8 TempleEventGetRoomUserCount(uint16 nEventRoom, uint16 sActiveEvent, uint8 bNation = Nation::NONE);
	void TempleEventTeleportUsers();
	void TempleEventStart();
	void TempleEventManageRoom();
	void TempleEventSendAccept();
	void TempleEventScreen(uint8 ActiveEvent, uint16 sTime);
	void TempleEventRoomClose();
	void TempleEventBridgeCheck(uint8 DoorNumber);
	void TempleEventSendWinnerScreen();
	void TempleEventCreateParties();
	bool AddEventUser(CUser* pUser);
	void RemoveEventUser(CUser* pUser);

	void ResetBeefEvent();
	void BeefEventUpdateTime();
	void BeefEventSendNotice(uint8 NoticeType);

	void GameInfoNoticeTimer();
	void GameInfo1Packet();
	void GameInfo2Packet();

	//Soccer Event System
	void TempleSoccerEventTimer();
	void TempleSoccerEventGool(uint8 nType = 2);
	void TempleSoccerEventEnd();

	void ReloadKnightAndUserRanks();
	void SetPlayerRankingRewards(uint16 ZoneID);

	void UpdateSiege(int16 m_sCastleIndex, int16 m_sMasterKnights, int16 m_bySiegeType, int16 m_byWarDay, int16 m_byWarTime, int16 m_byWarMinute);
	void UpdateSiegeTax(uint8 Zone, int16 ZoneTarrif);
	void HandleSiegeDatabaseRequest(CUser* pUser, Packet& pkt);

	//Ardream Event MeLanor

	void	ArdreamEventOpen();
	bool	OpenArdream;
	void	ArdreamEventZoneClose();
	////
	void	CZEventOpen();
	bool	OpenCZ;
	void	CZEventZoneClose();
	////
	void	BaseLandEventOpen();
	bool	OpenBaseLand;
	void	BaseLandEventClose();

	uint16 ArdreamEventFinishTime;
	bool ArdreamEventGameServerSatus;
	uint8 ArdreamEventTimer;

	bool IsDuplicateItem(uint32 nItemID, uint64 nItemSerial);
	void AddUserItem(uint32 nItemID, uint64 nItemSerial);
	void DeleteUserItem(uint32 nItemID, uint64 nItemSerial);

	void AddDatabaseRequest(Packet& pkt, CUser* pUser = nullptr);
	void AddLogRequest(std::string& LogMsg, LogTypes logtype);

	//DeathMatch Event
	void DeathMatchEventFinished();
	uint16 DeathMatchEventFinishTime;
	bool DeathMatchEventGameServerSatus;
	uint8 DeathMatchEventTimer;

	// The Under Castle & Krowas Dominion
	void TheEventUserKickOut(uint8 ZoneID = 0);

	// Get info for NPCs in regions around user (WIZ_REQ_NPCIN)
	void NpcInOutForMe(CUser* pSendUser);

	// Get info for NPCs in region
	void GetRegionNpcIn(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom, CUser* pSendUser);

	// Get list of NPC IDs in region
	void GetRegionNpcList(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom = 0);

	// Get list of NPCs for regions around user (WIZ_NPC_REGION)
	void RegionNpcInfoForMe(CUser* pSendUser);

	// Get raw list of all units in regions surrounding pOwner.
	void GetUnitListFromSurroundingRegions(Unit* pOwner, std::vector<uint16>* pList);

	// Get info for users in regions around user (WIZ_REQ_USERIN)
	void UserInOutForMe(CUser* pSendUser);

	// Get info for users in region
	void GetRegionUserIn(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom = 0);

	// Get info for Bots in region
	void GetRegionBotIn(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom = 0);

	// Get list of user IDs in region
	void GetRegionUserList(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom = 0, bool SurUpdate = false);

	// Get list of Bot IDs in region
	void GetRegionBotList(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom = 0, bool SurUpdate = false);

	// Get list of users for regions around user (WIZ_REGIONCHANGE)
	void RegionUserInOutForMe(CUser* pSendUser);

	// Get info for merchants in regions around user
	void MerchantUserInOutForMe(CUser* pSendUser);

	// Get war status
	INLINE bool isWarOpen() { return m_byBattleOpen != NO_BATTLE; }

	INLINE bool isBorderDefenceWarActive() { return pTempleEvent.ActiveEvent == TEMPLE_EVENT_BORDER_DEFENCE_WAR && pTempleEvent.isActive == true; }
	INLINE bool isJuraidMountainActive() { return pTempleEvent.ActiveEvent == TEMPLE_EVENT_JURAD_MOUNTAIN && pTempleEvent.isActive == true; }
	INLINE bool isChaosExpansionActive() { return pTempleEvent.ActiveEvent == TEMPLE_EVENT_CHAOS && pTempleEvent.isActive == true; }
	INLINE bool isKnightRoyaleActive() { return pKnightRoyaleEvent.ActiveEvent == ZONE_KNIGHT_ROYALE && pKnightRoyaleEvent.isActive == true; }
	INLINE bool isDeathMatcjActive() { return pTempleEvent.ActiveEvent == DEATHMATCH && pTempleEvent.isActive == true; }

	INLINE bool isCswActive() { return pCswEvent.Started; }
	INLINE bool isBarrackCreateActive() { return pCswEvent.Status == CswOperationStatus::BarrackCreated; }
	INLINE bool isDeathMatchActive() { return pCswEvent.Status == CswOperationStatus::DeathMatch; }
	INLINE bool isPreparationActive() { return pCswEvent.Status == CswOperationStatus::Preparation; }
	INLINE bool isCastellanWarActive() { return pCswEvent.Status == CswOperationStatus::CastellanWar; }


	// Get list of merchants in region
	void GetRegionMerchantUserIn(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom = 0);

	// Get list of merchants in region
	void GetRegionMerchantBotIn(C3DMap* pMap, uint16 region_x, uint16 region_z, Packet& pkt, uint16& t_count, uint16 nEventRoom = 0);

	void SendHelpDescription(CUser* pUser, std::string sHelpMessage);

	INLINE bool isPermanentMessageSet() { return m_bPermanentChatMode; }
	void SetPermanentMessage(const char* format, ...);

	void HandleConsoleCommand(const char* msg);

	template <ChatType chatType>
	INLINE void SendChatToPlayer(CUser* pUser, const char* msg, bool bFormatNotice = false)
	{
		Packet result;
		std::string buffer;

		if (bFormatNotice)
			GetServerResource(IDP_ANNOUNCEMENT, &buffer, msg);
		else
			buffer = msg;

		ChatPacket::Construct(&result, (uint8)chatType, &buffer);
		Send_Player(pUser, &result);
	}

	template <ChatType chatType>
	INLINE void SendChat(const char* msg, uint8 byNation = Nation::ALL, bool bFormatNotice = false)
	{
		Packet result;
		std::string buffer;

		if (bFormatNotice)
			GetServerResource(IDP_ANNOUNCEMENT, &buffer, msg);
		else
			buffer = msg;

		ChatPacket::Construct(&result, (uint8)chatType, &buffer);
		Send_All(&result, nullptr, byNation);
	}

	template <ChatType chatType>
	INLINE void SendChatToZone(const char* msg, uint8 ZoneID, uint8 byNation = Nation::ALL, bool bFormatNotice = false)
	{
		Packet result;
		std::string buffer;

		if (bFormatNotice)
			GetServerResource(IDP_ANNOUNCEMENT, &buffer, msg);
		else
			buffer = msg;

		ChatPacket::Construct(&result, (uint8)chatType, &buffer);
		Send_Zone(&result, ZoneID, nullptr, byNation);
	}

	template <ChatType chatType>
	INLINE void SendFormattedChatToPlayer(CUser* pUser, const char* msg, bool bFormatNotice = false, va_list args = nullptr)
	{
		char buffer[512];
		vsnprintf(buffer, sizeof(buffer), msg, args);
		SendChatToPlayer<chatType>(pUser, buffer, bFormatNotice);
		va_end(args);
	}

	template <ChatType chatType>
	INLINE void SendFormattedChat(const char* msg, uint8 byNation = Nation::ALL, bool bFormatNotice = false, va_list args = nullptr)
	{
		char buffer[512];
		vsnprintf(buffer, sizeof(buffer), msg, args);
		SendChat<chatType>(buffer, byNation, bFormatNotice);
		va_end(args);
	}

	template <ChatType chatType>
	void SendFormattedChat(const char* msg, uint8 byNation = Nation::ALL, bool bFormatNotice = false, ...)
	{
		va_list ap;
		va_start(ap, byNation);
		SendFormattedChat<chatType>(msg, byNation, bFormatNotice, ap);
		va_end(ap);
	}

	/* The following are simply wrappers for more readable SendChat() calls */

	INLINE void SendNotice(const char* msg, uint8 byNation = Nation::ALL)
	{
		SendChat<PUBLIC_CHAT>(msg, byNation, true);
	}

	INLINE void SendNoticeAlly(const char* msg, uint8 byNation = Nation::ALL)
	{
		SendChat<ALLIANCE_CHAT>(msg, byNation, true);
	}

	INLINE void SendAnnouncementWhite(const char* msg, uint8 byNation = Nation::ALL)
	{
		SendChat<ANNOUNCEMENT_WHITE_CHAT>(msg, byNation, true);
	}

	template <ChatType chatType>
	INLINE void SendNotice(const char* msg, uint8 ZoneID, uint8 byNation = Nation::ALL, bool bFormatNotice = false)
	{
		SendChatToZone<chatType>(msg, ZoneID, byNation, bFormatNotice);
	}

	void SendFormattedNoticeToPlayer(CUser* pExceptUser, const char* msg, uint8 byNation = Nation::ALL, ...)
	{
		va_list ap;
		va_start(ap, byNation);
		SendFormattedChatToPlayer<PUBLIC_CHAT>(pExceptUser, msg, true, ap);
		va_end(ap);
	}

	void SendFormattedNotice(const char* msg, uint8 byNation = Nation::ALL, ...)
	{
		va_list ap;
		va_start(ap, byNation);
		SendFormattedChat<PUBLIC_CHAT>(msg, byNation, true, ap);
		va_end(ap);
	}

	INLINE void SendAnnouncement(const char* msg, uint8 byNation = Nation::ALL)
	{
		SendChat<WAR_SYSTEM_CHAT>(msg, byNation, true);
	}

	void SendFormattedAnnouncement(const char* msg, uint8 byNation = Nation::ALL, ...)
	{
		va_list ap;
		va_start(ap, byNation);
		SendFormattedChat<WAR_SYSTEM_CHAT>(msg, byNation, true, ap);
		va_end(ap);
	}

	void SendFormattedResource(uint32 nResourceID, uint8 byNation = Nation::ALL, bool bIsNotice = true, ...);

	bool GiveItemLua(
		uint32 nGiveItemID1 = 0,
		uint32 nGiveItemCount1 = 1,
		uint32 nGiveItemTime1 = 0,
		uint32 nGiveItemID2 = 0,
		uint32 nGiveItemCount2 = 1,
		uint32 nGiveItemTime2 = 0,
		uint32 nGiveItemID3 = 0,
		uint32 nGiveItemCount3 = 1,
		uint32 nGiveItemTime3 = 0,
		uint32 nGiveItemID4 = 0,
		uint32 nGiveItemCount4 = 1,
		uint32 nGiveItemTime4 = 0,
		uint32 nGiveItemID5 = 0,
		uint32 nGiveItemCount5 = 1,
		uint32 nGiveItemTime5 = 0,
		uint32 nGiveItemID6 = 0,
		uint32 nGiveItemCount6 = 1,
		uint32 nGiveItemTime6 = 0,
		uint32 nGiveItemID7 = 0,
		uint32 nGiveItemCount7 = 1,
		uint32 nGiveItemTime7 = 0,
		uint32 nGiveItemID8 = 0,
		uint32 nGiveItemCount8 = 1,
		uint32 nGiveItemTime8 = 0,
		uint32 nRobItemID1 = 0,
		uint32 nRobItemCount1 = 0,
		uint32 nRobItemID2 = 0,
		uint32 nRobItemCount2 = 0,
		uint32 nRobItemID3 = 0,
		uint32 nRobItemCount3 = 0,
		uint32 nRobItemID4 = 0,
		uint32 nRobItemCount4 = 0,
		uint32 nRobItemID5 = 0,
		uint32 nRobItemCount5 = 0);

	void Send_Region(Packet* pkt, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr, uint16 nEventRoom = 0);
	void Send_UnitRegion(Packet* pkt, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr, uint16 nEventRoom = 0);
	void Send_OldRegions(Packet* pkt, int old_x, int old_z, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr, uint16 nEventRoom = 0);
	void Send_NewRegions(Packet* pkt, int new_x, int new_z, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr, uint16 nEventRoom = 0);

	void Send_NearRegion(Packet* pkt, C3DMap* pMap, int region_x, int region_z, float curx, float curz, CUser* pExceptUser = nullptr, uint16 nEventRoom = 0);
	void Send_FilterUnitRegion(Packet* pkt, C3DMap* pMap, int x, int z, float ref_x, float ref_z, CUser* pExceptUser = nullptr, uint16 nEventRoom = 0);

	void Send_Zone_Matched_Class(Packet* pkt, uint8 bZoneID, CUser* pExceptUser, uint8 nation, uint8 seekingPartyOptions, uint16 nEventRoom = 0);
	void Send_Zone(Packet* pkt, uint8 bZoneID, CUser* pExceptUser = nullptr, uint8 nation = 0, uint16 nEventRoom = 0, float fRange = 0.0f);
	void Send_Player(CUser* pUser, Packet* pkt);
	void Send_All(Packet* pkt, CUser* pExceptUser = nullptr, uint8 nation = 0, uint8 ZoneID = 0, bool isSendEventUsers = false, uint16 nEventRoom = 0);

	void GameServerShutDownTimer();
	void GameServerShutDown();


	/* DeathMatch */
	void StartDeathMatch();
	void DeathMatchLogOut(CUser* pUser);
	bool DeathMatchRegister(CUser* pUser);
	bool DeathMatchRemove(CUser* pUser);
	void GetServerResource(int nResourceID, std::string* result, ...);
	_START_POSITION* GetStartPosition(int nZoneID);

	int64 GetExpByLevel(int nLevel, int RebithLevel);

	C3DMap* GetZoneByID(int zoneID);

	CUser* GetUserPtr(std::string findName, NameType type);

	CUser* GetUserPtr(uint16 sUserId);

	CNpc* GetNpcPtr(uint16 sNpcId, uint16 sZoneID);

	Unit* GetUnitPtr(uint16 id, uint16 sZoneID = 0);

	CBot* GetBotPtr(std::string findName, NameType type);

	CBot* GetBotPtr(uint16 sUserId);

	// Spawns an event NPC/monster
	void SpawnEventNpc(uint16 sSid, bool bIsMonster, uint8 byZone, float fX, float fY, float fZ, uint16 sCount = 1, uint16 sRadius = 0, uint16 sDuration = 0, uint8 nation = 0, int16 socketID = -1, uint16 nEventRoom = 0, uint8 byDirection = 0, uint8 bMoveType = 0, uint8 bGateOpen = 0, uint16 nSummonSpecialType = 0, uint32 nSummonSpecialID = 0);

	// Spawns an event NPC/monster
	void SpawnEventPet(uint16 sSid, bool bIsMonster, uint8 byZone, float fX, float fY, float fZ, uint16 sCount = 1, uint16 sRadius = 0, uint16 sDuration = 0, uint8 nation = 0, int16 socketID = -1, uint16 nEventRoom = 0, uint8 nType = 0, uint32 nSkillID = 0);

	// Spawns an event UserBots
	uint16 SpawnUserBot(int Minute, uint8 byZone, float fX, float fY, float fZ, uint8 Restipi = 5, uint8 minlevel = 1);

	// Spawns an event SlaveUserBots
	uint16 SpawnSlaveUserBot(int Minute, CUser* pUser = nullptr);

	// Spawns an event SlavePriest
	uint16 SpawnPriestBot(int Minute, CUser* pUser = nullptr);

	// Removes all the NPC/monster in the specified event room
	void RemoveAllEventNpc(uint8 byZone);

	// Resets all the NPC/monster in the specified event room
	void ResetAllEventObject(uint8 byZone);

	// Kill a Npc/Monster
	void KillNpc(uint16 sNid, uint8 byZone, Unit* pKiller = nullptr);

	// Change NPC/Monster properties for Respawn
	void NpcUpdate(uint16 sSid, bool bIsMonster, uint8 byGroup = 0, uint16 sPid = 0);

	// Adds the account name & session to a hashmap (on login)
	void AddAccountName(CUser* pSession);

	// Adds the character name & session to a hashmap (when in-game)
	void AddCharacterName(CUser* pSession);

	// Removes an existing character name/session from the hashmap, replaces the character's name 
	// and reinserts the session with the new name into the hashmap.
	void ReplaceCharacterName(CUser* pSession, std::string& strNewUserID);

	// Removes the account name & character names from the hashmaps (on logout)
	void RemoveSessionNames(CUser* pSession);

	// Send to Zone NPC Effect
	void ShowNpcEffect(uint16 sNpcID, uint32 nEffectID, uint8 ZoneID, uint16 nEventRoom = 0);

	//Bdw Monument
	void BDWMonumentAltarTimer();
	void BDWMonumentAltarRespawn();

	_PARTY_GROUP* GetPartyPtr(uint16 sPartyID);
	CKnights* GetClanPtr(uint16 sClanID);
	_KNIGHTS_ALLIANCE* GetAlliancePtr(uint16 sAllianceID);
	_ITEM_TABLE* GetItemPtr(uint32 nItemID);
	_KNIGHTS_SIEGE_WARFARE* GetSiegeMasterKnightsPtr(uint16 sMasterKnights);

	_PARTY_GROUP* CreateParty(CUser* pLeader);
	void DeleteParty(uint16 sIndex);

	_EVENT_STATUS						pTempleEvent;
	_EVENT_BRIDGE						pEventBridge;
	_BEEF_EVENT_STATUS					pBeefEvent;
	_CSW_STATUS							pCswEvent;


	std::atomic<uint32>											m_TempleEventLastUserOrder;
	_KNIGHT_ROYALE_EVENT_STATUS									pKnightRoyaleEvent;
	std::atomic<uint32>											m_KnightRoyaleLastUserRequestID;
	_SOCCER_STATUS												pSoccerEvent;
	_CYBERACS_SETTINGS* m_CyberACSSettings;
	_CYBERACS_COLLECTIONRACE_SETTINGS* m_CyberACSCollectionRaceSetting;
	_CYBERACS_COLLECTIONRACE_HUNT_LIST* m_CyberACSCollectionRaceHuntList;
	_CYBERACS_LOTTERY_SETTINGS* m_LotterySettings;
	_SETTINGS_INFO* m_MaintainceSettings;
	_MONSTER_SEARCH_ITEM* m_SearchItemArray;
	uint32 GetFreeID();
	void GiveIDBack(uint32 sNpcID);
	~CGameServerDlg();

	char	m_ppNotice[20][128];
	char	m_peNotice[20][128];

	// Monster Stone Process
	void TempleMonsterStoneTimer();
	uint16	m_MonsterStoneEventRoom;
	uint16	m_MonsterStoneSquadEventRoom;

	/*CollectionRace Event Vs Time System*/
	void UpdateCyberACSCollectionRaceInfoTime();
	void CyberACSCollectionRaceFinish();
	uint8 m_CyberACSCollectionRaceCounts;
	uint8 m_CyberACSCollectionRaceQuestID[MAX_CR_EVENT_COUNT], m_CyberACSCollectionRaceQuestZone[MAX_CR_EVENT_COUNT];
	uint16 m_CyberACSCollectionRaceMonsterNum1[MAX_CR_EVENT_COUNT], m_CyberACSCollectionRaceMonsterNum2[MAX_CR_EVENT_COUNT], m_CyberACSCollectionRaceMonsterNum3[MAX_CR_EVENT_COUNT];
	std::string m_CyberACSCollectionRaceMonsterName1[MAX_CR_EVENT_COUNT], m_CyberACSCollectionRaceMonsterName2[MAX_CR_EVENT_COUNT], m_CyberACSCollectionRaceMonsterName3[MAX_CR_EVENT_COUNT];
	uint16 m_CyberACSCollectionRaceMonsterKillCount1[MAX_CR_EVENT_COUNT], m_CyberACSCollectionRaceMonsterKillCount2[MAX_CR_EVENT_COUNT], m_CyberACSCollectionRaceMonsterKillCount3[MAX_CR_EVENT_COUNT];
	uint16 m_CyberACSCollectionRaceFinishTime;
	bool m_CyberACSCollectionRaceGameServerSatus;
	uint32 m_CyberACSCollectionRaceSelectedMonster;



	/*Daily Quest Event Vs Time System*/
	void UpdateDailyInfoTime();
	void DailyFinish();
	uint8 m_DailyCounts;

	uint8 m_DailyQuestID[120], m_DailyQuestZone[120], m_DailySettingsID[120];
	uint16 m_DailyMonsterNum1[120], m_DailyMonsterNum2[120], m_DailyMonsterNum3[120], m_DailyMonsterNum4[120];
	std::string m_DailyQuestName[120], m_DailyMonsterName1[120], m_DailyMonsterName2[120], m_DailyMonsterName3[120], m_DailyMonsterName4[120];
	uint16 m_DailyMonsterKillCount1[120], m_DailyMonsterKillCount2[120], m_DailyMonsterKillCount3[120], m_DailyMonsterKillCount4[120];
	uint32 m_DailyFinishTime[120];
	bool m_DailyIsRegister[120];
	bool m_DailyGameServerSatus[120];
	uint32 m_DailySelectedMonster[120];


	//_CYBERACS_SETTINGS* m_CyberACSSettings;
	_CYBERACS_DAILY_SETTINGS* m_DailySettings[MAX_CR_EVENT_COUNT];
	_CYBERACS_DAILY_HUNT_LIST* m_DailyHuntList;


	DailyHuntListArray		m_DailyHuntListArray;
	DailySettingsArray		m_DailySettingsArray;
	DailyUserListArray		m_DailyUserArray;

	// Clan Vs Time System
	void ClanTournamentTimer();
	void UpdateClanTournamentScoreBoard(CUser* pUser);

	// NPC Related Arrays
	NpcArray							m_arNpcArray;
	NpcThreadArray						m_arNpcThread;
	NpcPosArray							m_NpcPosArray;
	NpcItemArray						m_NpcItemArray;
	MonsterItemArray					m_MonsterItemArray;
	MakeItemGroupArray					m_MakeItemGroupArray;
	MakeWeaponItemTableArray			m_MakeWeaponItemArray;
	MakeWeaponItemTableArray			m_MakeDefensiveItemArray;
	MakeGradeItemTableArray				m_MakeGradeItemArray;
	MakeLareItemTableArray				m_MakeLareItemArray;
	NpcTableArray						m_arMonTable;
	NpcTableArray						m_arNpcTable;

	BotArray							m_BotArray;

	ZoneArray							m_ZoneArray;
	ItemtableArray						m_ItemtableArray;
	ItemSellTableArray					m_ItemSellTableArray;
	ItemPremiumGiftArray				m_ItemPremiumGiftArray;
	SetItemArray						m_SetItemArray;
	MagictableArray						m_MagictableArray;
	Magictype1Array						m_Magictype1Array;
	Magictype2Array						m_Magictype2Array;
	Magictype3Array						m_Magictype3Array;
	Magictype4Array						m_Magictype4Array;
	Magictype5Array						m_Magictype5Array;
	Magictype6Array						m_Magictype6Array;
	Magictype7Array						m_Magictype7Array;
	Magictype8Array						m_Magictype8Array;
	Magictype9Array						m_Magictype9Array;
	CoefficientArray					m_CoefficientArray;
	LevelUpArray						m_LevelUpArray;
	PartyArray							m_PartyArray;
	KnightsArray						m_KnightsArray;
	KnightsRatingArray					m_KnightsRatingArray[2]; // one for both nations
	KnightsAllianceArray				m_KnightsAllianceArray;
	KnightsSiegeWarfareArray			m_KnightsSiegeWarfareArray;
	KnightsCapeArray					m_KnightsCapeArray;
	UserNameRankMap						m_UserKarusPersonalRankMap;
	UserNameRankMap						m_UserElmoPersonalRankMap;
	UserNameRankMap						m_UserKarusKnightsRankMap;
	UserNameRankMap						m_UserElmoKnightsRankMap;
	UserRankMap							m_playerRankings[2]; // one for both nations
	std::recursive_mutex				m_userRankingsLock;
	StartPositionArray					m_StartPositionArray;
	ServerResourceArray					m_ServerResourceArray;
	QuestHelperArray					m_QuestHelperArray;
	QuestHelperNationArray				m_QuestHelperNationArray;
	QuestNpcList						m_QuestNpcList;
	MonsterResourceArray				m_MonsterResourceArray;
	QuestMonsterArray					m_QuestMonsterArray;
	RentalItemArray						m_RentalItemArray;
	ItemExchangeArray					m_ItemExchangeArray;
	ItemExchangeExpArray				m_ItemExchangeExpArray;
	ItemSpecialExchangeArray			m_ItemSpecialExchangeArray;
	ItemExchangeCrashArray				m_ItemExchangeCrashArray;
	ItemUpgradeArray					m_ItemUpgradeArray;
	ItemOpArray							m_ItemOpArray;
	KingSystemArray						m_KingSystemArray;
	CommanderArray						m_CommanderArray;
	SheriffArray						m_SheriffArray;
	EventTriggerArray					m_EventTriggerArray;
	DeathMatchItemsArray				m_DeathMatchItemsArray;
	DeathMatchItemsArray1				m_DeathMatchItemsArray1;
	DeathMatchItemsArray2				m_DeathMatchItemsArray2;
	DeathMatchItemsArray3				m_DeathMatchItemsArray3;
	MonsterChallengeArray				m_MonsterChallengeArray;
	MonsterChallengeSummonListArray		m_MonsterChallengeSummonListArray;
	MonsterSummonListArray				m_MonsterSummonList;
	MonsterUnderTheCastleArray			m_MonsterUnderTheCastleArray;
	MonsterUnderTheCastleList			m_MonsterUnderTheCastleList;
	MonsterStoneListInformationArray	m_MonsterStoneListInformationArray;
	JuraidMountionListInformationArray	m_JuraidMountionListInformationArray;
	MiningFishingItemArray				m_MiningFishingItemArray;
	PremiumItemArray					m_PremiumItemArray;
	PremiumItemExpArray					m_PremiumItemExpArray;
	UserChaosExpansionRankingArray		m_UserChaosExpansionRankingArray;
	UserDeathMatchRankingArray			m_UserDeathMatchRankingArray;
	UserBorderDefenceWarRankingArray	m_UserBorderDefenceWarRankingArray[2];
	UserPlayerKillingZoneRankingArray	m_UserPlayerKillingZoneRankingArray[2];
	UserDailyOpMap						m_UserDailyOpMap;
	TempleEventUserMap					m_TempleEventUserMap;
	TempleEventJuraidRoomList			m_TempleEventJuraidRoomList;
	TempleEventBDWRoomList				m_TempleEventBDWRoomList;
	TempleEventChaosRoomList			m_TempleEventChaosRoomList;
	TempleEventDeathMatchRoomList		m_TempleEventDeathMatchRoomList;
	TempleSoccerEventUserArray			m_TempleSoccerEventUserArray;
	TempleMonsterStoneTeleportArray		m_MonsterStoneTeleportListArray;
	NationMonumentInformationArray		m_NationMonumentWinnerNationArray;
	NationMonumentInformationArray		m_NationMonumentDefeatedNationArray;
	StartPositionRandomArray			m_StartPositionRandomArray;
	MonsterBowlArray					m_MonsterBowlArray; // Bowl Event MeLanor
	KnStartPositionRandomArray			m_KnStartPositionRandomArray;
	UserItemArray						m_UserItemArray;
	ObjectEventArray					m_ObjectEventArray;
	HardwareInformation					g_HardwareInformation;
	std::recursive_mutex				m_SeekingPartyArrayLock;
	SeekingPartyArray					m_SeekingPartyArray;
	ChatRoomArray						m_ChatRoomArray;
	CharacterSealItemArray				m_CharacterSealItemArray;
	CharacterSealItemMapping			m_CharacterSealItemMapping;
	ServerSettingsArray					m_ServerSettingsArray;
	MiningExchangeArray					m_MiningExchangeArray;
	DkmMonsterDeadGiftArray				m_DkmMonsterDeadGiftArray;

	ChaosStoneStageArray				m_ChaosStoneStageArray;
	ChaosStoneSummonListArray			m_ChaosStoneSummonListArray;
	ChaosStoneRespawnCoordinateArray	m_ChaosStoneRespawnCoordinateArray;
	ChaosStoneInfoArray					m_ChaosStoneInfoArray;
	LuaGiveItemExchangeArray			m_LuaGiveItemExchangeArray;
	EventScheduleArray					m_EventScheduleArray;
	EventTournamentScheduleArray		m_EventTournamentScheduleArray;

	RoomEventPlayTimerArray				m_RoomEventPlayTimerArray;
	RoyaleEventPlayTimerArray			m_RoyaleEventPlayTimerArray;
	BeefEventPlayTimerArray				m_BeefEventPlayTimerArray;
	CswEventPlayTimerArray				m_CswEventPlayTimerArray;

	MonsterVeriableRespawnListArray		m_MonsterVeriableRespawnListArray;
	MonsterStableRespawnListArray		m_MonsterStableRespawnListArray;
	MonsterStableSýgnRespawnListArray	m_MonsterStableSignRespawnListArray;

	// Achiement System
	AchieveTitleArray					m_AchieveTitleArray;
	AchieveMainArray					m_AchieveMainArray;
	AchieveMonsterArray					m_AchieveMonsterArray;
	AchieveNormalArray					m_AchieveNormalArray;
	AchieveWarArray						m_AchieveWarArray;
	AchieveComArray						m_AchieveComArray;
	HackToolList						m_HackToolListArray;
	DrakiRoomList						m_DrakiRoomListArray;
	DrakiMonsterList					m_DrakiMonsterListArray;
	DrakiRankList						m_DrakiRankListArray;
	MonsterDrakiTowerList				m_MonsterDrakiTowerList;
	ClanVsDataList						m_ClanVsDataList;
	KnightSiegeWarClanList				m_KnightSiegeWarClanList;
	UserRankALL							m_UserRankALL;
	UserRankEL							m_UserRankEL;
	UserRankKA							m_UserRankKA;
	KingList							m_KingList;
	LoadTickets							m_Tickets;
	KnightTournamentClanList			m_TournamentClanList;
	KnightSiegeWarBarrackClanList		m_KnightSiegeWarBarrackList;
	KnightSiegeWarCastellanClanList     m_KnightSiegeWarCastellanClanList;
	KnightSiegeWarCastellanOldWinner	m_KnightSiegeWarCastellanOldWinner;
	DungeonDefenceMonsterListArray		m_DungeonDefenceMonsterListArray;
	DungeonDefenceRoomListArray			m_DungeonDefenceRoomListArray;
	DungeonDefenceStageListArray		m_DungeonDefenceStageListArray;
	KnightRoyaleEventUserMap			m_KnightRoyaleEventUserMap;
	KnightRoyaleRoomInfo				m_KnightRoyaleRoomInfo;
	KnightRoyaleStatsListArray			m_KnightRoayleStatsListArray;
	KnightRoyaleBeginnerItemListArray   m_KnightRoyaleBeginnerItemListArray;
	KnightRoyaleTreasureChestListArray  m_KnightRoyaleTreasureChestListArray;
	KnightReturnLetterGiftListArray		m_KnightReturnLetterGiftListArray;
	KnightCastellanCapeArray			m_KnightCastellanCapeArray;
	WarBanishOfWinnerArray				m_WarBanishOfWinnerArray;
	LoadOpenQuestArray					m_LoadOpenQuestArray;
	MonsterDropMainArray				m_MonsterDropMainArray;
	MonsterItemGroupItemListArray		m_MonsterItemGroupItemListArray;

	//Pet System
	PetDataSystemArray					m_PetDataSystemArray;
	PetInfoSystemArray					m_PetInfoSystemArray;
	PetTransformSystemArray				m_PetTransformSystemArray;

	//Wanted System
	WantedEventMainControlArray			m_WantedEventMainControlArray;
	WantedEventSelectedUserArray		m_WantedEventSelectedUserArray;

	//Mining & Fishing Sistem
	uint32								m_ItemMiningDropListArrayNormal[10000];
	uint32								m_ItemFishingDropListArrayNormal[10000];

	Atomic<uint16>						m_sPartyIndex;
	Condition* s_hEvent;

	//CyberACS Settiring
	CyberACSSettingsArray					       m_CyberACSSettingsArray;
	CyberACSPusItemArray						    m_CyberACSPusItemArray;
	CyberACSCollectionRaceHuntListArray		        m_CyberACSCollectionRaceHuntListArray;
	CyberACSCollectionRaceSettingsArray		        m_CyberACSCollectionRaceSettingsArray;
	LotteryArray									m_LotteryArray;
	MaintainceSettingsArray							m_MaintainceSettingsArray;
	SearchItemArray									m_SearchItemArrays;
	MannerStoreArray								m_MannerStoreArray;




	std::recursive_mutex m_CommanderArrayLock;
	std::recursive_mutex m_PetSystemlock;
	uint16 m_sMonMin = 0;
	uint16 m_sYear, m_sMonth, m_sDate, m_sHour, m_sMin, m_sSec;
	uint8 m_byWeather;
	uint16 m_sWeatherAmount;
	int m_nCastleCapture;
	uint8 m_ReloadKnightAndUserRanksMinute;

	/* DeathMatch */
	time_t	m_tDMRegisterStartTime;
	bool	m_bDMRegisterStarted;
	std::recursive_mutex m_DMRegisterLock;
	//std::set<uint16>	m_DMRegisterlist;
	bool	m_bDMStarted;
	std::set<uint16>	m_DMRegisterlist;


	std::string clanlar[16];

	//

	uint8   m_byBattleOpen, m_byOldBattleOpen, m_byBattleNoticeTime;
	uint8  m_byBattleZone, m_byBattleZoneType, m_bVictory, m_byOldVictory, m_bResultDelayVictory, m_bKarusFlag, m_bElmoradFlag, m_bMiddleStatueNation;
	int32	m_byBattleOpenedTime;
	int32	m_byBattleTime;
	int32	m_byBattleRemainingTime;
	int32	m_byNereidsIslandRemainingTime;
	int32	m_sBattleTimeDelay;
	int32  m_sBattleResultDelay;

	uint8	m_sKilledKarusNpc, m_sKilledElmoNpc;
	uint8	m_sKarusMonuments, m_sElmoMonuments;
	uint16  m_sKarusMonumentPoint, m_sElmoMonumentPoint;
	bool    m_bCommanderSelected;
	bool    m_byKarusOpenFlag, m_byElmoradOpenFlag, m_byBanishFlag, m_byBattleSave, m_bResultDelay, m_bySiegeBanishFlag;
	short   m_sDiscount;
	short	m_sKarusDead, m_sElmoradDead, m_sBanishDelay, m_sKarusCount, m_sElmoradCount;

	std::string m_strKarusCaptain, m_strElmoradCaptain;


	uint8   m_nDeathMatchEventTime[DEATHMATCH_EVENT_COUNT]; //NEW

	uint8	m_nPVPMonumentNation[MAX_ZONE_ID];
	uint8	m_sNereidsIslandMonuArray[7];
	uint32  m_GameServerPort;
	uint8	m_byMaxLevel;
	int32	m_nGameMasterRHitDamage;
	int32	m_nBonusTimeInterval, m_nBonusTimeGold, m_nBonusPVPWarItem;
	uint16	m_CountofTickets;
	uint8	m_nPlayerRankingResetTime;
	//uint8	m_upgrade1 , m_upgrade2;
	std::string  m_sPlayerRankingsRewardZones;
	uint32  m_nPlayerRankingKnightCashReward;
	uint32  m_nPlayerRankingLoyaltyReward, m_Grade1, m_Grade2, m_Grade3, m_Grade4;

	uint32  m_sChaosFinishItem, m_sChaosFinishItems, m_sChaosFinishItem1, m_sChaosFinishItem2, m_sChaosFinishItem3, m_sChaosFinishItem4;
	uint32  m_sBorderFinishItem, m_sBorderFinishItem1, m_sBorderFinishItem2, m_sBorderFinishItem3, m_sBorderFinishItem4, m_sBorderFinishItem5;
	uint32  m_sJuraidFinishItem, m_sJuraidFinishItem1;
	uint32  m_sCastleSiegeWarDeathMatchWinner, m_sCastleSiegeWarDeathMatchWinner1, m_sCastleSiegeWarDeathMatchWinner2, m_sCastleSiegeWarDeathMatchWinner3;
	uint32  m_sCastleSiegeWarWinner, m_sCastleSiegeWarWinner1, m_sCastleSiegeWarWinner2, m_sCastleSiegeWarWinner3;
	uint32  m_sKnightRoyalWinner;
	uint8 toplamclan, active;
	uint16	m_nVirtualEventRoomRemainSeconds;
	uint16  m_nKnightRoyaleEventRemainSeconds;

	uint16  m_GameInfo1Time;
	uint16  m_GameInfo2Time;

	uint16 m_GameServerShutDownSeconds;
	bool   m_GameServerShutDownOK;

	uint8	m_bMaxRegenePoint;

	bool	m_bPermanentChatMode;
	std::string	m_strPermanentChat;

	uint8	m_bSantaOrAngel;
	uint8	m_sRankResetHour;

	// National Points Settings
	int m_Loyalty_Ardream_Source;
	int m_Loyalty_Ardream_Target;
	int m_Loyalty_Ronark_Land_Base_Source;
	int m_Loyalty_Ronark_Land_Base_Target;
	int m_Loyalty_Ronark_Land_Source;
	int m_Loyalty_Ronark_Land_Target;
	int m_Loyalty_Other_Zone_Source;
	int m_Loyalty_Other_Zone_Target;

	//Military Camp
	uint8 m_sKarusMilitary;
	uint8 m_sKarusEslantMilitary;
	uint8 m_sHumanMilitary;
	uint8 m_sHumanEslantMilitary;
	uint8 m_sMoradonMilitary;
	uint8 m_sCapeAC, m_sCapeAtk, m_sCapeNp, m_sCapeHp;

	uint8 m_sJackExpPots;
	uint8 m_sJackGoldPots;
	uint32 MonsterDeadCount;

	uint32 m_WantedSystemMapShowTime;
	uint32 m_WantedSystemTownControlTime;

	void SendEventRemainingTime(bool bSendAll = false, CUser* pUser = nullptr, uint8 ZoneID = 0);
	void LogosYolla(std::string LogosName, std::string LogosMessage, uint8 R, uint8 G, uint8 B);
	//van
	void LogosYolla_Zone(std::string LogosName, std::string LogosMessage, uint8 R, uint8 G, uint8 B, uint8 ZoneNumber);
	bool m_IsMagicTableInUpdateProcess;
	bool m_IsPlayerRankingUpdateProcess;




	//summon
	bool IsSummon;
	std::string NameGm;

	std::vector<int64> m_HardwareIDArray;
	std::vector<std::string> m_sHardwareIpArray;
	// Forgetten Temple
	std::vector<uint16>        m_nForgettenTempleUsers;
	bool              m_bForgettenTempleIsActive;
	int8              m_nForgettenTempleStartHour;
	int8              m_nForgettenTempleLevelMin;
	int8              m_nForgettenTempleLevelMax;
	int32              m_nForgettenTempleStartTime;
	uint8              m_nForgettenTempleChallengeTime;
	bool              m_bForgettenTempleSummonMonsters;
	uint8              m_nForgettenTempleCurrentStage;
	uint8              m_nForgettenTempleLastStage;
	ForgettenTempleMonsterList    m_ForgettenTempleMonsterList;
	uint32              m_nForgettenTempleLastSummonTime;
	bool              m_nForgettenTempleBanishFlag;
	uint8              m_nForgettenTempleBanishTime;
	// Under The Castle
	std::vector<uint16>				m_nUnderTheCastleUsers;
	UnderTheCastleMonsterList		m_UnderTheCastleMonsterList;
	bool							m_bUnderTheCastleIsActive;
	bool							m_bUnderTheCastleMonster;
	int32							m_nUnderTheCastleEventTime;
	/*int8							m_nUnderTheCastleStartHour;
	int8							m_nUnderTheCastleLevelMin;
	int8							m_nUnderTheCastleLevelMax;
	int32							m_nUnderTheCastleStartTime;
	uint8							m_nUnderTheCastleEventTime;
	bool							m_bUnderTheCastleSummonMonsters;
	uint8							m_nUnderTheCastleCurrentStage;
	uint8							m_nUnderTheCastleLastStage;
	UnderTheCastleMonsterList		m_UnderTheCastleMonsterList;
	uint32							m_nUnderTheCastleLastSummonTime;
	bool							m_nUnderTheCastleBanishFlag;
	uint8							m_nUnderTheCastleBanishTime;*/
	// zone server info
	int					m_nServerNo, m_nServerGroupNo;
	int					m_nServerGroup;	// serverÀÇ ¹øÈ£(0:¼­¹ö±ºÀÌ ¾ø´Ù, 1:¼­¹ö1±º, 2:¼­¹ö2±º)
	ServerArray			m_ServerArray;
	ServerArray			m_ServerGroupArray;

	NameMap		m_accountNameMap,
		m_characterNameMap;

	BotNameMap	m_BotcharacterNameMap;

	std::recursive_mutex	m_accountNameLock,
		m_characterNameLock,
		m_questNpcLock,
		m_BotcharacterNameLock;

	// Controlled weather events set by Kings
	uint8 m_byKingWeatherEvent;
	uint8 m_byKingWeatherEvent_Day;
	uint8 m_byKingWeatherEvent_Hour;
	uint8 m_byKingWeatherEvent_Minute;

	//VS Event
	uint32 vsEventMaxNp;
	uint32 vsEventMinNp;
	uint32 m_VsEventLastRoom;

	uint32 m_DeathMatchItem1;
	uint32 m_DeathMatchItem2;
	uint32 m_DeathMatchItem3;

	int16 sGameMasterSocketID;

	bool PetFree;

	//Bowl Event MeLanor

	bool BownEvent;

	// XP/coin/NP events
	uint8 m_byExpEventAmount, m_byCoinEventAmount, m_byNPEventAmount;

	INLINE CLuaEngine* GetLuaEngine() { return &m_luaEngine; }

	KOSocketMgr<CUser> m_socketMgr;
	

	FILE* m_fpDeathUser;
	FILE* m_fpDeathNpc;
	FILE* m_fpChat;
	FILE* m_fpCheat;
	FILE* m_fpTrade;
	FILE* m_fpGiveItem;
	FILE* m_fpMerchant;
	FILE* m_fpLetter;
	FILE* m_fpBanList;
	FILE* m_fpItemTransaction;
	FILE* m_fpItemUpgradeTransaction;
	FILE* m_fpCharacterIpTransaction;
	FILE* m_fpTournament;
	FILE* m_fpOfflineSystemLog;

	void WriteDeathUserLogFile(std::string& logMessage);
	void WriteDeathNpcLogFile(std::string& logMessage);
	void WriteChatLogFile(std::string& logMessage);
	void WriteCheatLogFile(std::string& logMessage);
	void WriteTradeLogFile(std::string& logMessage);
	void WriteMerchantLogFile(std::string& logMessage);
	void WriteLetterLogFile(std::string& logMessage);
	void WriteBanListLogFile(std::string& logMessage);
	void WriteItemTransactionLogFile(std::string& logMessage);
	void WriteUpgradeLogFile(std::string& logMessage);
	void WriteCharacterIpLogs(std::string& logMessage);
	void WriteTournamentLogs(std::string& logMessage);

	void WriteOfflineSystemLog(std::string& logMessage);
	void SendYesilNotice(CUser* pUser, std::string sHelpMessage);

	uint16 m_sLoginingPlayer;

	Atomic<uint16>	m_DrakiRiftTowerRoomIndex;
	Atomic<uint16>	m_DungeonDefenceRoomIndex;

private:
	CLuaEngine	m_luaEngine;

	std::string m_strGameDSN, m_strAccountDSN;
	std::string m_strGameUID, m_strAccountUID;
	std::string m_strGamePWD, m_strAccountPWD;
	bool m_bMarsEnabled;

	bool ProcessServerCommand(std::string& command);

public:
	void InitServerCommands();
	void CleanupServerCommands();

	static ServerCommandTable s_commandTable;

	COMMAND_HANDLER(HandleHelpCommand);
	COMMAND_HANDLER(HandleNoticeCommand);
	COMMAND_HANDLER(HandleNoticeallCommand);
	COMMAND_HANDLER(HandleKillUserCommand);
	COMMAND_HANDLER(HandleWar1OpenCommand);
	COMMAND_HANDLER(HandleWar2OpenCommand);
	COMMAND_HANDLER(HandleWar3OpenCommand);
	COMMAND_HANDLER(HandleWar4OpenCommand);
	COMMAND_HANDLER(HandleWar5OpenCommand);
	COMMAND_HANDLER(HandleWar6OpenCommand);
	COMMAND_HANDLER(HandleSnowWarOpenCommand);
	COMMAND_HANDLER(HandleSiegeWarOpenCommand);
	COMMAND_HANDLER(HandleWarCloseCommand);
	COMMAND_HANDLER(HandleShutdownCommand);
	COMMAND_HANDLER(HandleDiscountCommand);
	COMMAND_HANDLER(HandleGlobalDiscountCommand);
	COMMAND_HANDLER(HandleDiscountOffCommand);
	COMMAND_HANDLER(HandleCaptainCommand);
	COMMAND_HANDLER(HandleSantaCommand);
	COMMAND_HANDLER(HandleSantaOffCommand);
	COMMAND_HANDLER(HandleAngelCommand);
	COMMAND_HANDLER(HandlePermanentChatCommand);
	COMMAND_HANDLER(HandlePermanentChatOffCommand);
	COMMAND_HANDLER(HandleReloadNoticeCommand);
	COMMAND_HANDLER(HandleReloadTablesCommand);
	COMMAND_HANDLER(HandleReloadTables2Command);
	COMMAND_HANDLER(HandleReloadTables3Command);
	COMMAND_HANDLER(HandleReloadMagicsCommand);
	COMMAND_HANDLER(HandleReloadQuestCommand);
	COMMAND_HANDLER(HandleReloadRanksCommand);
	COMMAND_HANDLER(HandleReloadDropsCommand);
	COMMAND_HANDLER(HandleReloadKingsCommand);
	COMMAND_HANDLER(HandleReloadItemsCommand);
	COMMAND_HANDLER(HandleEventScheduleResetTable);
	COMMAND_HANDLER(HandleReloadDungeonDefenceTables);
	COMMAND_HANDLER(HandleReloadDrakiTowerTables);
	COMMAND_HANDLER(HandleNPAddCommand);
	COMMAND_HANDLER(HandleExpAddCommand);
	COMMAND_HANDLER(HandleGiveItemCommand);
	COMMAND_HANDLER(HandleMoneyAddCommand);
	COMMAND_HANDLER(HandleTeleportAllCommand);
	COMMAND_HANDLER(HandleCountCommand);
	COMMAND_HANDLER(HandlePermitConnectCommand);
	COMMAND_HANDLER(HandlePermanentBannedCommand);
	COMMAND_HANDLER(HandleWarResultCommand);
	COMMAND_HANDLER(HandleEventUnderTheCastleCommand);
	COMMAND_HANDLER(HandleCastleSiegeWarClose);
	COMMAND_HANDLER(HandleTournamentStart);
	COMMAND_HANDLER(HandleTournamentClose);
	COMMAND_HANDLER(HandleChaosExpansionOpen);
	COMMAND_HANDLER(HandleBorderDefenceWar);
	COMMAND_HANDLER(HandleJuraidMountain);
	COMMAND_HANDLER(HandleKnightRoyale);
	COMMAND_HANDLER(HandleBeefEvent);
	COMMAND_HANDLER(HandleWantedEvent);
	COMMAND_HANDLER(HandleServerGameTestCommand);
	COMMAND_HANDLER(HandleCollectionOpen);
	COMMAND_HANDLER(HandleCollectionClose);
	COMMAND_HANDLER(MaintainceSend);
	//daily quest
	COMMAND_HANDLER(HandleDailyOpen);
	COMMAND_HANDLER(HandleDailyClose);
	//daily quest

	// lottery event
	uint16 TotalLottery;
	uint16 Time;
	uint16 TotalWinner;
	uint32 RequiredMoney;
	uint8  rewardid;
	uint8  isKC;
	uint16 TotalPurchasedLottery = 0;
	bool	OpenLottery;
};

extern CGameServerDlg* g_pMain;

