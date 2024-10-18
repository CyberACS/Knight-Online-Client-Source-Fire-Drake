#pragma once

#include "LuaEngine.h"
#include "../shared/KOSocket.h"


#include "Unit.h"
#include "User.h"
#include "ChatHandler.h"


#include "GameDefine.h"

class CGameServerDlg;

class CBot : public Unit 
{
public:


	virtual uint16 GetID() { return m_sSid; }

	std::string& GetAccountName() { return m_strAccountID; }
	virtual std::string& GetName() { return m_strUserID; }

	std::string	m_strAccountID, m_strUserID, m_strMemo;
	GameState m_state;
	float	m_curx, m_curz, m_cury;
	uint8	m_bNation, m_bLevel, m_bZone;
	std::string MerchantChat;
	KOMap* m_pMap;
	CRegion* m_pRegion;
	uint8	m_bResHpType;
	uint32  m_pktcount;
	uint8	m_bRace;
	uint16	m_sClass;
	time_t	LastWarpTime;
	uint32	m_nHair;
	time_t  m_tLastKillTime;
	uint16	m_sChallangeAchieveID;
	uint16	m_sSid;
	uint8	m_bRank;
	uint8	m_bTitle;
	int64	m_iExp;
	uint32	m_iLoyalty, m_iLoyaltyMonthly;
	uint32	m_iMannerPoint;
	uint8	m_bFace;
	uint8	m_bCity;
	int16	m_bKnights;
	uint8	m_bFame;
	int16	m_sHp, m_sMp, m_sSp;
	uint8	m_bStats[STAT_COUNT];
	uint8	m_bRebStats[STAT_COUNT];
	uint8	m_reblvl;
	uint8	m_bAuthority;
	int16	m_sPoints; // this is just to shut the compiler up
	uint32	m_iGold, m_iBank;
	int16	m_sBind;
	//
	int16	m_sItemMaxHp;
	int16	m_sItemMaxMp;
	short	m_sItemAc;
	uint32	m_sItemWeight;
	uint16	m_sMaxWeightBonus;
	short	m_sItemHitrate;
	short	m_sItemEvasionrate;
	int16	m_sStatItemBonuses[STAT_COUNT];
	uint8	m_byAPBonusAmount;
	uint8	m_byAPClassBonusAmount[4]; // one for each of the 4 class types
	uint8	m_byAcClassBonusAmount[4]; // one for each of the 4 class types
	uint8	m_bItemExpGainAmount, m_bItemNoahGainAmount;
	uint8	m_bNPGainAmount, m_bItemNPBonus, m_bSkillNPBonus;
	uint8	m_sExpGainAmount;
	uint8	m_bNoahGainAmount;
	uint16	m_sAddArmourAc;
	uint8	m_bPctArmourAc;
	uint8	m_bPlayerAttackAmount;
	int16	m_sStatAchieveBonuses[ACHIEVE_STAT_COUNT];
	int8	m_bStatBuffs[STAT_COUNT];
	uint32	m_sMaxWeight;
	uint8	m_bMaxWeightAmount;
	uint8   m_sWantedTotalHit;
	uint8	m_sWantedKillerCount;
	uint32  m_WantedRunTime;
	uint8   m_sWantedTotalAc;
	uint8   m_sWantedCount;
	int16   m_sSpeed;
	uint32  m_nKnightCash;
	uint32  m_nKnightCashBonus;
	uint8	m_bPremiumInUse;
	uint8	m_bClanPremiumInUse;



	_ITEM_DATA m_sItemArray[INVENTORY_TOTAL];
	_ITEM_DATA m_sItemKnightRoyalArray[INVENTORY_TOTAL];


	int8	m_bMerchantState;
	bool	m_bSellingMerchantPreparing;
	bool	m_bBuyingMerchantPreparing;
	bool	m_bMerchantOpen;
	bool    m_bInEnterParty;
	bool	m_bWeaponsDisabled;
	int16	m_bMerchantViewer;
	int16	m_sMerchantsSocketID;
	_MERCH_DATA	m_arMerchantItems[MAX_MERCH_ITEMS]; //What is this person selling? Stored in "_MERCH_DATA" structure.
	bool	m_bPremiumMerchant;

	uint8    m_bstrSkill[10];

	uint16	m_sPartyIndex;



	INLINE _ITEM_DATA* GetItem(uint8 pos)
	{
		return &m_sItemArray[pos];
	}
	INLINE uint8 GetNation() { return m_bNation; }
	INLINE uint8 GetRace() { return m_bRace; }
	INLINE uint8 GetLevel() { return m_bLevel; }
	INLINE uint16 GetClass() { return m_sClass; }
	INLINE bool isWeaponsDisabled() { return m_bWeaponsDisabled; }
	INLINE uint8 GetStat(StatType type)
	{
		if (type >= STAT_COUNT)
			return 0;

		return m_bStats[type];
	}

	uint16		m_sAchieveCoverTitle;

	float	m_oldx, m_oldy, m_oldz;
	int16	m_sDirection;

	uint8	m_bNeedParty;

	bool	m_bInParty;
	bool	m_bPartyLeader;

	bool	m_bSlaveMerchant;
	int16	m_bSlaveUserID;

	int16 m_bUserPriestBotID;
	uint8	m_bAddWeaponDamage;

	bool	m_bIsChicken; // Is the character taking the beginner/chicken quest?
	bool	m_bIsHidingHelmet;
	bool	m_bIsHidingCospre;

	short	m_MaxHP, m_MaxMp;
	uint8	m_MaxSp;
	uint8	m_bInvisibilityType;

	uint32	m_bAbnormalType;			// Is the player normal, a giant, or a dwarf?
	uint32	m_nOldAbnormalType, ReturnSymbolisOK;
	bool	m_bBlockPrivateChat;
	short	m_sPrivateChatUser;

	uint16	m_sRegionX, m_sRegionZ; // this is probably redundant

	TeamColour	m_teamColour;
public:
	INLINE uint8 GetUniqueLevel() { return m_bRebStats[STAT_STR]; } // Char Rebirth mi 
	INLINE uint8 GetRebLevel() { return m_reblvl; }
	INLINE uint16 GetSPosX() { return uint16(GetX() * 10); };
	INLINE uint16 GetSPosY() { return uint16(GetY() * 10); };
	INLINE uint16 GetSPosZ() { return uint16(GetZ() * 10); };
	INLINE CRegion* GetRegion() { return m_pRegion; }
	void SetRegion(uint16 x = -1, uint16 z = -1);
	void StateChangeServerDirect(uint8 bType, uint32 nBuff);
	INLINE KOMap* GetMap() { return m_pMap; }
	INLINE float GetX() { return m_curx; }
	INLINE void SetPosition(float fx, float fy, float fz)
	{
		m_curx = fx;
		m_curz = fz;
		m_cury = fy;
	}

	bool RegisterRegion();
	void AddToRegion(int16 new_region_x, int16 new_region_z);
	void RemoveRegion(int16 del_x, int16 del_z);
	void InsertRegion(int16 insert_x, int16 insert_z);
	bool	m_bGenieStatus, m_bIsDevil, m_bIsHidingWings;

	int8	m_bPersonalRank;
	int8	m_bKnightsRank;

	char	m_GenieOptions[100];
	uint16	m_GenieTime;
	uint8   m_sFirstUsingGenie;
	uint8 GetBaseClass();
	INLINE uint16 GetNewRegionX() { return (uint16)(GetX()) / VIEW_DISTANCE; }
	INLINE uint16 GetNewRegionZ() { return (uint16)(GetZ()) / VIEW_DISTANCE; }
	INLINE float GetY() { return m_cury; }
	INLINE float GetZ() { return m_curz; }
	INLINE int16 GetClanID() { return m_bKnights; }
	INLINE void SetClanID(int16 val) { m_bKnights = val; }
	INLINE uint16 GetRegionX() { return m_sRegionX; }
	INLINE uint16 GetRegionZ() { return m_sRegionZ; }
	INLINE uint32 GetCoins() { return m_iGold; }
	INLINE uint32 GetInnCoins() { return m_iBank; }
	INLINE uint32 GetLoyalty() { return m_iLoyalty; }
	INLINE uint32 GetMonthlyLoyalty() { return m_iLoyaltyMonthly; }
	INLINE uint32 GetManner() { return m_iMannerPoint; }
	INLINE bool isInGame() { return m_state == GAME_STATE_INGAME; }
	INLINE bool isMerchanting() { return (GetMerchantState() != MERCHANT_STATE_NONE); }
	INLINE bool isSellingMerchant() { return GetMerchantState() == MERCHANT_STATE_SELLING; }
	INLINE bool isBuyingMerchant() { return GetMerchantState() == MERCHANT_STATE_BUYING; }
	INLINE int8 GetMerchantState() { return m_bMerchantState; }

	INLINE bool isSlaveMerchant() { return m_bSlaveMerchant; }
	INLINE int16 GetSlaveGetID() { return m_bSlaveUserID; }

	virtual int32 GetHealth() { return m_sHp; }
	virtual int32 GetMaxHealth() { return m_MaxHP; }
	virtual int32 GetMana() { return m_sMp; }
	virtual int32 GetMaxMana() { return m_MaxMp; }
	INLINE bool  isGenieActive() { return m_bGenieStatus; }
	INLINE uint8 GetFame() { return m_bFame; }
	INLINE bool isKing() { return m_bRank == 1; }
	INLINE uint8 GetZoneID() { return m_bZone; }
	INLINE uint8 GetPremium() { return m_bPremiumInUse; }


	INLINE bool isBeginnerWarrior() { return GetClassType() == ClassWarrior; }
	INLINE bool isBeginnerRogue() { return GetClassType() == ClassRogue; }
	INLINE bool isBeginnerMage() { return GetClassType() == ClassMage; }
	INLINE bool isBeginnerPriest() { return GetClassType() == ClassPriest; }
	INLINE bool isBeginnerKurianPortu() { return GetClassType() == ClassPortuKurian; }
	INLINE bool isMasteredWarrior() { return GetClassType() == ClassWarriorMaster; }
	INLINE bool isMasteredRogue() { return GetClassType() == ClassRogueMaster; }
	INLINE bool isMasteredMage() { return GetClassType() == ClassMageMaster; }
	INLINE bool isMasteredPriest() { return GetClassType() == ClassPriestMaster; }
	INLINE bool isMasteredKurianPortu() { return GetClassType() == ClassPortuKurianMaster; }
	INLINE bool isNoviceWarrior() { return GetClassType() == ClassWarriorNovice; }
	INLINE bool isNoviceRogue() { return GetClassType() == ClassRogueNovice; }
	INLINE bool isNoviceMage() { return GetClassType() == ClassMageNovice; }
	INLINE bool isNovicePriest() { return GetClassType() == ClassPriestNovice; }
	INLINE bool isNoviceKurianPortu() { return GetClassType() == ClassPortuKurianNovice; }
	INLINE bool isWarrior() { return JobGroupCheck(ClassWarrior); }
	INLINE bool isRogue() { return JobGroupCheck(ClassRogue); }
	INLINE bool isMage() { return JobGroupCheck(ClassMage); }
	INLINE bool isPriest() { return JobGroupCheck(ClassPriest); }
#if __VERSION > 2024
	INLINE bool isPortuKurian() { return JobGroupCheck(ClassPortuKurian); }
#endif

	INLINE ClassType GetBaseClassType()
	{
		static const ClassType classTypes[] =
		{
			ClassWarrior, ClassRogue, ClassMage, ClassPriest,
			ClassWarriorNovice, ClassWarriorMaster,		// job changed / mastered
			ClassRogueNovice, ClassRogueMaster,			// job changed / mastered
			ClassMageNovice, ClassMageMaster,			// job changed / mastered
			ClassPriestNovice, ClassPriestMaster,		// job changed / mastered
			ClassPortuKurian,ClassPortuKurianNovice,	// job changed / mastered 
			ClassPortuKurianMaster,						// job changed / mastered
		};

		uint8 classType = GetClassType();

		if (classType < 1 && classType > 19)
		{
			return ClassType(0);
		}

		return classTypes[classType - 1];
	}

	INLINE uint8 GetClassType()
	{
		return GetClass() % 100;
	}

	INLINE uint8 GetRebirthLevel() { return m_reblvl; }

	bool JobGroupCheck(short jobgroupid);
	void Home();
	void SetMaxMp();
	void SetMaxSp();
	void SetMaxHp(int iFlag = 0);
	void LogOut();
	void SendItemMove(uint8 command, uint8 subcommand);
	void Add();
	void Remove();

	short GetDamage(Unit* pTarget, _MAGIC_TABLE* pSkill = nullptr, bool bPreviewOnly = false);
	virtual void HpChange(int amount, Unit* pAttacker = nullptr, bool isDOT = false);
	bool isHostileTo(Unit* pTarget);
	bool isMoral2Checking(Unit* pTarget, _MAGIC_TABLE* pSkill);
	bool isMoral7Checking(Unit* pTarget, _MAGIC_TABLE* pSkill);
	bool isInArena();
	bool isInMeleeArena();
	bool isInClanArena();
	bool isInPVPZone();
	bool isInOwnSafetyArea();
	void RemoveStealth();
	uint8 GetPartyMemberAmount(_PARTY_GROUP* pParty = nullptr);
	bool CheckSkillPoint(uint8 skillnum, uint8 min, uint8 max);
	void SetUserAbility(bool bSendPacket = true);
	float SetCoefficient();
	void SetSlotItemValue();
	void ApplySetItemBonuses(_SET_ITEM* pItem);
	void ShowEffect(uint32 nSkillID);
	virtual void SpChange(int amount);





	virtual bool isDead() { return m_bResHpType == USER_DEAD || m_sHp <= 0; }
	virtual void MSpChange(int amount);

	void SendToRegion(Packet* pkt, CUser* pExceptUser = nullptr, uint16 nEventRoom = 0);
	void SendMoveResult(float fX, float fY, float fZ, uint8 echo /*= 0.0f*/, int16 speed, uint16 socketid);
	virtual void GetInOut(Packet& result, uint8 bType);
	void UserInOut(uint8 bType);
	void HandleSurroundingUserRegionUpdate();
	void GetUserInfo(Packet& pkt);
	void GenieStart();
	void SendGenieStart(bool isToRegion = false);
	virtual void Initialize();
	bool isInPartyArena();
	bool isInEnemySafetyArea();
	INLINE bool isInParty() { return m_bInParty && m_bInEnterParty; }
	INLINE uint16 GetPartyID() { return m_sPartyIndex; }
	INLINE bool isInClan() { return GetClanID() > 0; }
	INLINE bool isGM() { return GetAuthority() == AUTHORITY_GAME_MASTER; }
	INLINE uint8 GetAuthority() { return m_bAuthority; }

	INLINE int16 GetStatWithItemBonus(StatType type)
	{
		return GetStat(type) + GetStatItemBonus(type);
	}

	INLINE int16 GetStatBonusTotal(StatType type)
	{
		return GetStatBuff(type) + GetRebStatBuff(type) + GetStatItemBonus(type) + GetStatAchieveBonus((UserAchieveStatTypes)type);
	}

	INLINE int16 GetStatAchieveBonus(UserAchieveStatTypes type)
	{
		if (type >= ACHIEVE_STAT_COUNT)
			return 0;

		return m_sStatAchieveBonuses[type];
	}

	INLINE int16 GetStatItemBonus(StatType type)
	{
		ASSERT(type < STAT_COUNT);
		return m_sStatItemBonuses[type];
	}

	INLINE uint8 GetRebStatBuff(StatType type)
	{
		if (type >= STAT_COUNT)
		{
			return 0;
		}
		return m_bRebStats[type];
	}

	INLINE int8 GetStatBuff(StatType type)
	{
		ASSERT(type < STAT_COUNT);
		return m_bStatBuffs[type];
	}

	INLINE bool isMastered()
	{
		uint16 sClass = GetClassType();
		return (sClass == ClassWarriorMaster
			|| sClass == ClassRogueMaster
			|| sClass == ClassMageMaster
			|| sClass == ClassPriestMaster
			|| sClass == ClassPortuKurianMaster);
	}

	INLINE _ITEM_DATA* GetKnightRoyaleItem(uint8 pos)
	{
		if (pos > INVENTORY_TOTAL)
			return false;

		ASSERT(pos < INVENTORY_TOTAL);
		return &m_sItemKnightRoyalArray[pos];

	}

	INLINE _ITEM_TABLE* GetItemPrototype(uint8 pos)
	{
		_ITEM_DATA* pItem;
		ASSERT(pos < INVENTORY_TOTAL);
		return GetItemPrototype(pos, pItem);
	}

	_ITEM_TABLE* GetItemPrototype(uint8 pos, _ITEM_DATA*& pItem);

	~CBot() {}
	CBot();

};