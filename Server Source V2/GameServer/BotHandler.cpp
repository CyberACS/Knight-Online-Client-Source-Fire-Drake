#include "stdafx.h"
#include "Map.h"
#include "../shared/DateTime.h"
#include "DBAgent.h"
#include "MagicInstance.h"

using namespace std;

CBot::CBot() : Unit(UnitPlayer)
{
	Initialize();
}

void CBot::Initialize()
{
	Unit::Initialize();
	MerchantChat.clear();
	LastWarpTime = 0;
	m_tLastKillTime = 0;
	m_sChallangeAchieveID = 0;
	m_pktcount = 0;
	m_strUserID.clear();
	m_strMemo.clear();
	m_strAccountID.clear();
	m_sItemMaxHp = m_sItemMaxMp = 0;
	m_sItemAc = 0;
	m_sItemWeight = 0;
	m_sMaxWeightBonus = 0;
	m_sItemHitrate = 100;
	m_sItemEvasionrate = 100;
	m_byAPBonusAmount = 0;
	memset(&m_sStatItemBonuses, 0, sizeof(m_sStatItemBonuses));
	memset(&m_byAPClassBonusAmount, 0, sizeof(m_byAPClassBonusAmount));
	memset(&m_byAcClassBonusAmount, 0, sizeof(m_byAcClassBonusAmount));
	m_bItemExpGainAmount = m_bItemNoahGainAmount = 0;
	m_bItemNPBonus = m_bSkillNPBonus = 0;
	m_sExpGainAmount = m_bNPGainAmount = m_bNoahGainAmount = 100;
	m_sAddArmourAc = 0;
	m_bPctArmourAc = 100;
	m_bPlayerAttackAmount = 100;
	memset(&m_sStatAchieveBonuses, 0, sizeof(m_sStatAchieveBonuses));
	memset(&m_bStatBuffs, 0, sizeof(m_bStatBuffs));
	m_WantedRunTime = m_sWantedKillerCount = m_sWantedTotalHit = m_sWantedTotalAc = m_sWantedCount = 0;
	m_sSpeed = 0;
	m_bPremiumInUse = m_bClanPremiumInUse = 0;




	for (int i = 0; i < STAT_COUNT; i++)
		m_bRebStats[i] = 0;

	m_sAchieveCoverTitle = 0;

	m_bMerchantState = MERCHANT_STATE_NONE;
	m_sBind = -1;
	m_state = GAME_STATE_CONNECTED;
	m_bPartyLeader = false;
	m_bIsChicken = false;
	m_bIsHidingHelmet = false;
	m_bIsHidingCospre = false;

	m_bInParty = false;

	m_bInvisibilityType = INVIS_NONE;

	m_sDirection = ReturnSymbolisOK = 0;

	memset(&m_bStats, 0, sizeof(m_bStats));

	m_bAuthority = AUTHORITY_PLAYER;
	m_bLevel = 1;
	m_iExp = 0;
	m_iBank = m_iGold = 0;
	m_iLoyalty = 100;
	m_iLoyaltyMonthly = 0;
	m_iMannerPoint = 0;
	m_sHp = m_sMp = m_sSp = 0;

	memset(&m_arMerchantItems, 0, sizeof(m_arMerchantItems));
	m_bSellingMerchantPreparing = false;
	m_bBuyingMerchantPreparing = false;
	m_bMerchantOpen = false;
	m_bPremiumMerchant = false;
	m_bMerchantViewer = m_sMerchantsSocketID = -1;

	m_MaxHP = 0;
	m_MaxMp = 1;
	m_MaxSp = 120;

	m_bResHpType = USER_STANDING;
	m_bBlockPrivateChat = false;
	m_sPrivateChatUser = -1;
	m_bNeedParty = 0x01;
	m_bAbnormalType = ABNORMAL_NORMAL;	// User starts out in normal size.
	m_nOldAbnormalType = m_bAbnormalType;
	m_teamColour = TeamColourNone;
	m_bGenieStatus = m_bIsDevil = m_bIsHidingWings = false;
	m_bRank = 0;

	m_bSlaveMerchant = false;
	m_bSlaveUserID = -1;

	m_bInEnterParty = false;
	m_sPartyIndex = -1;

	m_bAddWeaponDamage = 0;

	m_bWeaponsDisabled = false;

	//priest bot
	m_bUserPriestBotID = -1;
	memset(m_GenieOptions, 0, sizeof(m_GenieOptions));
	m_GenieTime = 0;
	m_sFirstUsingGenie = 0;

	m_bPersonalRank = m_bKnightsRank = -1;
}

bool CBot::RegisterRegion()
{
	uint16
		new_region_x = GetNewRegionX(), new_region_z = GetNewRegionZ(),
		old_region_x = GetRegionX(), old_region_z = GetRegionZ();

	if (GetRegion() == nullptr
		|| (old_region_x == new_region_x
			&& old_region_z == new_region_z))
		return false;

	AddToRegion(new_region_x, new_region_z);

	RemoveRegion(old_region_x - new_region_x, old_region_z - new_region_z);
	InsertRegion(new_region_x - old_region_x, new_region_z - old_region_z);

	return true;
}

void CBot::AddToRegion(int16 new_region_x, int16 new_region_z)
{
	if (GetRegion())
		GetRegion()->Remove(this);

	SetRegion(new_region_x, new_region_z);

	if (GetRegion())
		GetRegion()->Add(this);
}

void CBot::RemoveRegion(int16 del_x, int16 del_z)
{
	if (GetMap() == nullptr)
		return;

	Packet result;
	GetInOut(result, INOUT_OUT);
	g_pMain->Send_OldRegions(&result, del_x, del_z, GetMap(), GetRegionX(), GetRegionZ(), nullptr, 0);
}

void CBot::InsertRegion(int16 insert_x, int16 insert_z)
{
	if (GetMap() == nullptr)
		return;

	Packet result;
	GetInOut(result, INOUT_IN);
	g_pMain->Send_NewRegions(&result, insert_x, insert_z, GetMap(), GetRegionX(), GetRegionZ(), nullptr, 0);
}

void CBot::SetRegion(uint16 x /*= -1*/, uint16 z /*= -1*/)
{
	m_sRegionX = x; m_sRegionZ = z;
	m_pRegion = m_pMap->GetRegion(x, z); // TODO: Clean this up
}

void CBot::StateChangeServerDirect(uint8 bType, uint32 nBuff)
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


		m_bAbnormalType = nBuff;
		break;

	case 5:
		m_bAbnormalType = nBuff;
		break;

	case 6:
		nBuff = m_bPartyLeader; // we don't set this here.
		break;

	case 7:
		break;

	case 8: // beginner quest
		break;

	case 14:
		break;
	}

	Packet result(WIZ_STATE_CHANGE);
	result << GetID() << bType << nBuff;
	SendToRegion(&result);
}

void CBot::GetInOut(Packet& result, uint8 bType)
{
	result.Initialize(WIZ_USER_INOUT);
	result << uint16(bType) << GetID();
	if (bType != INOUT_OUT)
		GetUserInfo(result);
}

void CBot::UserInOut(uint8 bType)
{
	Packet result;
	GetInOut(result, bType);

	if (bType == INOUT_OUT)
		Remove();
	else
		Add();

	SendToRegion(&result);
}

void CBot::Add()
{
	Guard lock(g_pMain->m_BotcharacterNameLock);

	if (GetRegion())
		GetRegion()->Add(this);

	m_state = GAME_STATE_INGAME;
	g_DBAgent.InsertCurrentUser(GetName(), GetName());
	std::string upperName = GetName();
	STRTOUPPER(upperName);
	g_pMain->m_BotcharacterNameMap[upperName] = this;
}

void CBot::Remove()
{
	Guard lock(g_pMain->m_BotcharacterNameLock);

	if (GetRegion())
		GetRegion()->Remove(this);

	m_state = GAME_STATE_CONNECTED;
	HandleSurroundingUserRegionUpdate();
	g_DBAgent.RemoveCurrentUser(GetName());
	std::string upperName = GetName();
	STRTOUPPER(upperName);
	g_pMain->m_BotcharacterNameMap.erase(upperName);
}

void CBot::SendToRegion(Packet* pkt, CUser* pExceptUser /*= nullptr*/, uint16 nEventRoom /*-1*/)
{
	g_pMain->Send_Region(pkt, GetMap(), GetRegionX(), GetRegionZ(), pExceptUser, nEventRoom);
}

void CBot::ShowEffect(uint32 nSkillID)
{
	Packet result(WIZ_EFFECT);
	if (isInGame())
	{
		result << GetID() << nSkillID;
		SendToRegion(&result);
	}
}


void CBot::SendMoveResult(float fX, float fY, float fZ, uint8 echo /*= 0.0f*/, int16 speed, uint16 socketid)
{
	CBot* pPriest = nullptr;
	pPriest = g_pMain->m_BotArray.GetData(GetID());

	SetPosition(fX, fY, fZ);
	RegisterRegion();

	Packet result(WIZ_MOVE);
	result << socketid << GetSPosX() << GetSPosZ() << GetSPosY() << speed << echo;

	if (GetEventRoom() > 0)
		SendToRegion(&result, nullptr, GetEventRoom());
	else
		SendToRegion(&result);

	SendToRegion(&result);
}


#pragma region CUser::GenieStart()
void CBot::GenieStart()
{

	Packet result(WIZ_GENIE, uint8(GenieStatusActive));
	result << uint8(4) << uint16(1) << m_GenieTime;
	m_bGenieStatus = true;
	UserInOut(INOUT_IN);
	SendToRegion(&result);


	SendGenieStart(true);
}
#pragma endregion

#pragma region CUser::SendGenieStart(bool isToRegion /* = false */)
void CBot::SendGenieStart(bool isToRegion /* = false */)
{
	Packet result(WIZ_GENIE, uint8(GenieInfoRequest));
	result << uint8(GenieStartHandle) << uint16(1) << uint16(3600);
	SendToRegion(&result);

	Packet result2(WIZ_GENIE, uint8(GenieInfoRequest));
	result2 << uint8(GenieActivated) << uint16(GetID()) << uint8(1);

	SendToRegion(&result2);

}
#pragma endregion

_ITEM_TABLE* CBot::GetItemPrototype(uint8 pos, _ITEM_DATA*& pItem)
{
	if (pos >= INVENTORY_TOTAL)
		return nullptr;

	if (GetZoneID() == ZONE_KNIGHT_ROYALE)
		pItem = GetKnightRoyaleItem(pos);
	else
		pItem = GetItem(pos);

	return pItem->nNum == 0 ? nullptr : g_pMain->GetItemPtr(pItem->nNum);
}

void CBot::SendItemMove(uint8 command, uint8 subcommand)
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
			<< m_sMaxWeight << uint8(1) << uint16(m_MaxHP) << uint16(m_MaxMp)
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
	
}


void CBot::RemoveStealth()
{
	if (this->m_bInvisibilityType != INVIS_NONE)
	{
		CMagicProcess::RemoveStealth(this, INVIS_DISPEL_ON_MOVE);
		CMagicProcess::RemoveStealth(this, INVIS_DISPEL_ON_ATTACK);
	}
}

void CBot::SetSlotItemValue()
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

	std::map<uint16, uint32> setItems;

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

void CBot::ApplySetItemBonuses(_SET_ITEM* pItem)
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





void CBot::GetUserInfo(Packet& pkt)
{
	pkt.SByte();
	pkt << GetName() << uint16(GetNation()) << uint8(0);
	pkt << GetClanID() << GetFame();

	CKnights* pKnights = g_pMain->GetClanPtr(GetClanID());
	if (pKnights == nullptr)
	{
		if (isKing())
			pkt << uint32(0) << uint16(0) << uint8(0) << uint16(KNIGHTS_KNIG_CAPE) << uint32(0) << uint8(0);
		else
			pkt << uint32(0) << uint16(0) << uint8(0) << uint16(GetNation() == KARUS ? 97 : 98) << uint32(0) << uint8(0);
	}
	else
	{
		pkt << pKnights->GetAllianceID() << pKnights->GetName() << pKnights->m_byGrade << pKnights->m_byRanking << pKnights->m_sMarkVersion; // symbol/mark version

		CKnights* pMainClan = g_pMain->GetClanPtr(pKnights->GetAllianceID());
		_KNIGHTS_ALLIANCE* pAlliance = g_pMain->GetAlliancePtr(pKnights->GetAllianceID());

		if (pKnights->isInAlliance() && pMainClan != nullptr && pAlliance != nullptr)
		{
			if (!isKing())
			{
				_KNIGHTS_CASTELLAN_CAPE* pCastellanInfo = g_pMain->m_KnightCastellanCapeArray.GetData(pMainClan->GetID());
				if (pCastellanInfo != nullptr && pCastellanInfo->sRemainingTime >= (uint64)UNIXTIME)
				{
					if (pAlliance->sMainAllianceKnights == pKnights->GetID())
						pkt << pCastellanInfo->sCape << pCastellanInfo->sR << pCastellanInfo->sG << pCastellanInfo->sB << uint8(0);
					else if (pAlliance->sSubAllianceKnights == pKnights->GetID())
						pkt << pCastellanInfo->sCape << pKnights->m_bCapeR << pKnights->m_bCapeG << pKnights->m_bCapeB << uint8(0);
					else if (pAlliance->sMercenaryClan_1 == pKnights->GetID() || pAlliance->sMercenaryClan_2 == pKnights->GetID())
						pkt << pCastellanInfo->sCape << uint32(0); // only the cape will be present
					else
						pkt << pCastellanInfo->sCape << pMainClan->m_bCapeR << pMainClan->m_bCapeG << pMainClan->m_bCapeB << uint8(0); // this is stored in 4 bytes after all.
				}
				else
				{
					if (pAlliance->sMainAllianceKnights == pKnights->GetID())
						pkt << pMainClan->GetCapeID() << pMainClan->m_bCapeR << pMainClan->m_bCapeG << pMainClan->m_bCapeB << uint8(0);
					else if (pAlliance->sSubAllianceKnights == pKnights->GetID())
						pkt << pMainClan->GetCapeID() << pKnights->m_bCapeR << pKnights->m_bCapeG << pKnights->m_bCapeB << uint8(0);
					else if (pAlliance->sMercenaryClan_1 == pKnights->GetID() || pAlliance->sMercenaryClan_2 == pKnights->GetID())
						pkt << pMainClan->GetCapeID() << uint32(0); // only the cape will be present
					else
						pkt << pMainClan->GetCapeID() << pMainClan->m_bCapeR << pMainClan->m_bCapeG << pMainClan->m_bCapeB << uint8(0); // this is stored in 4 bytes after all.
				}
			}
			else
				pkt << uint16(KNIGHTS_KNIG_CAPE) << uint32(0); // cape ID

			// not sure what this is, but it (just?) enables the clan symbol on the cape 
			// value in dump was 9, but everything tested seems to behave as equally well...
			// we'll probably have to implement logic to respect requirements.
			pkt << ((pKnights->m_byFlag > 1 && pKnights->m_byGrade < 3) ? uint8(1) : uint8(0));
		}
		else
		{
			if (!isKing())
			{
				_KNIGHTS_CASTELLAN_CAPE* pCastellanInfo = g_pMain->m_KnightCastellanCapeArray.GetData(pKnights->GetID());
				if (pCastellanInfo != nullptr && pCastellanInfo->sRemainingTime >= (uint64)UNIXTIME)
					pkt << pCastellanInfo->sCape << pCastellanInfo->sR << pCastellanInfo->sG << pCastellanInfo->sB << uint8(0); // this is stored in 4 bytes after all.
				else
					pkt << pKnights->GetCapeID() << pKnights->m_bCapeR << pKnights->m_bCapeG << pKnights->m_bCapeB << uint8(0); // this is stored in 4 bytes after all.
			}
			else
				pkt << uint16(KNIGHTS_KNIG_CAPE) << uint32(0); // cape ID

			// not sure what this is, but it (just?) enables the clan symbol on the cape 
			// value in dump was 9, but everything tested seems to behave as equally well...
			// we'll probably have to implement logic to respect requirements.
			pkt << ((pKnights->m_byFlag > 1 && pKnights->m_byGrade < 3) ? uint8(1) : uint8(0));
		}
	}
	// There are two event-driven invisibility states; dispel on attack, and dispel on move.
	// These are handled primarily server-side; from memory the client only cares about value 1 (which we class as 'dispel on move').
	// As this is the only place where this flag is actually sent to the client, we'll just convert 'dispel on attack' 
	// back to 'dispel on move' as the client expects.
	uint8 bInvisibilityType = m_bInvisibilityType;
	if (bInvisibilityType != INVIS_NONE)
		bInvisibilityType = INVIS_DISPEL_ON_MOVE;

	pkt << GetLevel() << m_bRace << m_sClass
		<< GetSPosX() << GetSPosZ() << GetSPosY()
		<< m_bFace << m_nHair
		<< m_bResHpType << uint32(m_bAbnormalType)
		<< m_bNeedParty
		<< m_bAuthority
		<< m_bPartyLeader
		<< bInvisibilityType
		<< uint8(m_teamColour)
		<< m_bIsHidingHelmet
		<< m_bIsHidingCospre
		<< m_bIsDevil
		<< m_bIsHidingWings
		<< m_sDirection
		<< m_bIsChicken
		<< m_bRank
		<< (m_bKnightsRank <= m_bPersonalRank ? m_bKnightsRank : int8(-1))
		<< (m_bPersonalRank <= m_bKnightsRank ? m_bPersonalRank : int8(-1));

	uint8 equippedItems[] =
	{
		BREAST, LEG, HEAD, GLOVE, FOOT, SHOULDER, RIGHTHAND, LEFTHAND,
		CWING, CHELMET, CLEFT, CRIGHT, CTOP, CFAIRY, CTATTOO
	};

	uint8 equippedItemsKnightRoyale[] =
	{
		KNIGHT_ROYAL_LEFTHAND, KNIGHT_ROYAL_RIGHTHAND
	};

	bool isRoyaleSignEvent = (GetZoneID() == ZONE_KNIGHT_ROYALE);
	bool isWarOpen = (g_pMain->m_byBattleOpen == NATION_BATTLE && g_pMain->m_byBattleZone + ZONE_BATTLE_BASE != ZONE_BATTLE3);
	_ITEM_DATA* pItem = nullptr;

	if (isRoyaleSignEvent)
	{
		foreach_array(i, equippedItemsKnightRoyale)
		{
			pItem = GetItem(equippedItemsKnightRoyale[i]);
			if (pItem == nullptr)
				continue;

			pkt << pItem->nNum << pItem->sDuration << pItem->bFlag;
		}
	}
	else
	{
		foreach_array(i, equippedItems)
		{
			pItem = GetItem(equippedItems[i]);
			if (pItem == nullptr)
				continue;

			if (isWarOpen)
			{
				if (isWarrior())
				{
					if (i == RIGHTEAR)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_PAULDRON << pItem->sDuration << pItem->bFlag;
					else if (i == HEAD)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_PAD << pItem->sDuration << pItem->bFlag;
					else if (i == LEFTEAR)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_HELMET << pItem->sDuration << pItem->bFlag;
					else if (i == NECK)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_GAUNTLET << pItem->sDuration << pItem->bFlag;
					else if (i == BREAST)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_BOOTS << pItem->sDuration << pItem->bFlag;
					else
						pkt << pItem->nNum << pItem->sDuration << pItem->bFlag;
				}
				else if (isRogue())
				{
					if (i == RIGHTEAR)
						pkt << (uint32)ROGUE_DRAGON_ARMOR_PAULDRON << pItem->sDuration << pItem->bFlag;
					else if (i == HEAD)
						pkt << (uint32)ROGUE_DRAGON_ARMOR_PAD << pItem->sDuration << pItem->bFlag;
					else if (i == LEFTEAR)
						pkt << (uint32)ROGUE_DRAGON_ARMOR_HELMET << pItem->sDuration << pItem->bFlag;
					else if (i == NECK)
						pkt << (uint32)ROGUE_DRAGON_ARMOR_GAUNTLET << pItem->sDuration << pItem->bFlag;
					else if (i == BREAST)
						pkt << (uint32)ROGUE_DRAGON_ARMOR_BOOTS << pItem->sDuration << pItem->bFlag;
					else
						pkt << pItem->nNum << pItem->sDuration << pItem->bFlag;
				}
				else if (isMage())
				{
					if (i == RIGHTEAR)
						pkt << (uint32)MAGE_DRAGON_ARMOR_PAULDRON << pItem->sDuration << pItem->bFlag;
					else if (i == HEAD)
						pkt << (uint32)MAGE_DRAGON_ARMOR_PAD << pItem->sDuration << pItem->bFlag;
					else if (i == LEFTEAR)
						pkt << (uint32)MAGE_DRAGON_ARMOR_HELMET << pItem->sDuration << pItem->bFlag;
					else if (i == NECK)
						pkt << (uint32)MAGE_DRAGON_ARMOR_GAUNTLET << pItem->sDuration << pItem->bFlag;
					else if (i == BREAST)
						pkt << (uint32)MAGE_DRAGON_ARMOR_BOOTS << pItem->sDuration << pItem->bFlag;
					else
						pkt << pItem->nNum << pItem->sDuration << pItem->bFlag;
				}
				else if (isPriest())
				{
					if (i == RIGHTEAR)
						pkt << (uint32)PRIEST_DRAGON_ARMOR_PAULDRON << pItem->sDuration << pItem->bFlag;
					else if (i == HEAD)
						pkt << (uint32)PRIEST_DRAGON_ARMOR_PAD << pItem->sDuration << pItem->bFlag;
					else if (i == LEFTEAR)
						pkt << (uint32)PRIEST_DRAGON_ARMOR_HELMET << pItem->sDuration << pItem->bFlag;
					else if (i == NECK)
						pkt << (uint32)PRIEST_DRAGON_ARMOR_GAUNTLET << pItem->sDuration << pItem->bFlag;
					else if (i == BREAST)
						pkt << (uint32)PRIEST_DRAGON_ARMOR_BOOTS << pItem->sDuration << pItem->bFlag;
					else
						pkt << pItem->nNum << pItem->sDuration << pItem->bFlag;
				}
				else if (isPortuKurian())
				{
					if (i == RIGHTEAR)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_PAULDRON << pItem->sDuration << pItem->bFlag;
					else if (i == HEAD)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_PAD << pItem->sDuration << pItem->bFlag;
					else if (i == LEFTEAR)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_HELMET << pItem->sDuration << pItem->bFlag;
					else if (i == NECK)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_GAUNTLET << pItem->sDuration << pItem->bFlag;
					else if (i == BREAST)
						pkt << (uint32)WARRIOR_DRAGON_ARMOR_BOOTS << pItem->sDuration << pItem->bFlag;
					else
						pkt << pItem->nNum << pItem->sDuration << pItem->bFlag;
				}
			}
			else
			{
				pItem = GetItem(equippedItems[i]);
				if (pItem == nullptr)
					continue;

				pkt << pItem->nNum << pItem->sDuration << pItem->bFlag;
			}
		}
	}
	pkt << GetZoneID()
		<< uint8(-1)
		<< uint8(-1)
		<< uint32(0)
		<< m_bIsHidingWings
		<< m_bIsHidingHelmet
		<< m_bIsHidingCospre
		<< isGenieActive()
		<< GetRebirthLevel() /*// is reb exp 83+ thing << uint8(m_bLevel == 83) // is reb exp 83+ thing*/
		<< uint16(m_sAchieveCoverTitle)
		<< ReturnSymbolisOK // R symbol after name returned?
		<< uint32(0); // face time system
}

void CBot::SetMaxHp(int iFlag)
{
	_CLASS_COEFFICIENT* p_TableCoefficient = nullptr;
	p_TableCoefficient = g_pMain->m_CoefficientArray.GetData(m_sClass);
	if (!p_TableCoefficient) return;

	int temp_sta = GetStat(STAT_STA);

	if (GetZoneID() == ZONE_SNOW_BATTLE && iFlag == 0)
	{
		if (GetFame() == COMMAND_CAPTAIN || isKing())
			m_MaxHP = 300;
		else
			m_MaxHP = 100;
	}
	else if (m_bZone == ZONE_CHAOS_DUNGEON && iFlag == 0)
		m_MaxHP = 10000 / 10;
	else
	{
		m_MaxHP = (short)(((p_TableCoefficient->HP * GetLevel() * GetLevel() * temp_sta)
			+ 0.1 * (GetLevel() * temp_sta) + (temp_sta / 5)) + 20);

		// A player's max HP should be capped at (currently) 14,000 HP.
		if (m_MaxHP > MAX_PLAYER_HP)
			m_MaxHP = MAX_PLAYER_HP;

		if (iFlag == 1)
			m_sHp = m_MaxHP;
		else if (iFlag == 2)
			m_MaxHP = 100;
	}

	if (m_MaxHP < m_sHp)
		m_sHp = m_MaxHP;
}

void CBot::SetMaxMp()
{
	_CLASS_COEFFICIENT* p_TableCoefficient = nullptr;
	p_TableCoefficient = g_pMain->m_CoefficientArray.GetData(m_sClass);
	if (!p_TableCoefficient)
		return;

	int temp_intel = 0, temp_sta = 0;
	temp_intel = GetStat(STAT_INT) + 30;

	if (temp_intel > 255)
		temp_intel = 255;

	temp_sta = GetStat(STAT_STA);
	if (temp_sta > 255)
		temp_sta = 255;

	if (p_TableCoefficient->MP != 0)
	{
		m_MaxMp = (short)((p_TableCoefficient->MP * GetLevel() * GetLevel() * temp_intel)
			+ (0.1f * GetLevel() * 2 * temp_intel) + (temp_intel / 5) + 20);
	}
	else if (p_TableCoefficient->SP != 0)
	{
		m_MaxMp = (short)((p_TableCoefficient->SP * GetLevel() * GetLevel() * temp_sta)
			+ (0.1f * GetLevel() * temp_sta) + (temp_sta / 5));
	}

	if (m_MaxMp < m_sMp)
		m_sMp = m_MaxMp;
}

bool CBot::JobGroupCheck(short jobgroupid)
{
	if (jobgroupid > 100)
		return GetClass() == jobgroupid;

	ClassType subClass = GetBaseClassType();
	switch (jobgroupid)
	{
	case GROUP_WARRIOR:
		return (subClass == ClassWarrior || subClass == ClassWarriorNovice || subClass == ClassWarriorMaster);

	case GROUP_ROGUE:
		return (subClass == ClassRogue || subClass == ClassRogueNovice || subClass == ClassRogueMaster);

	case GROUP_MAGE:
		return (subClass == ClassMage || subClass == ClassMageNovice || subClass == ClassMageMaster);

	case GROUP_CLERIC:
		return (subClass == ClassPriest || subClass == ClassPriestNovice || subClass == ClassPriestMaster);

	case GROUP_PORTU_KURIAN:
		return (subClass == ClassPortuKurian || subClass == ClassPortuKurianNovice || subClass == ClassPortuKurianMaster);
	}

	return (subClass == jobgroupid);
}




uint16 CGameServerDlg::SpawnPriestBot(int Minute, CUser* pUser)
{
	foreach_stlmap(itr, m_BotArray)
	{
		CBot* pPriest = itr->second;

		if (pPriest->m_state == GAME_STATE_INGAME)
			continue;

		if (pPriest->GetNation() != pUser->GetNation())
			continue;

		std::string userid = "priest";




		pPriest->Initialize();
		pPriest->LastWarpTime = UNIXTIME + (Minute * 60);
		pPriest->m_pMap = GetZoneByID(pUser->GetZoneID());
		pPriest->m_bZone = pUser->GetZoneID();
		pPriest->m_sClass = 212;//212
		pPriest->m_bRace = 13;
		pPriest->m_curx = pUser->m_curx;
		pPriest->m_curz = pUser->m_curz;
		pPriest->m_cury = pUser->m_cury;
		pPriest->m_oldx = pUser->m_oldx;
		pPriest->m_oldy = pUser->m_oldy;
		pPriest->m_oldz = pUser->m_oldz;
		pPriest->m_nHair = 53890920;
		pPriest->m_bFace = 4;
		pPriest->m_bFame = 0;
		pPriest->m_bLevel = 83;

		memset(pPriest->m_sItemArray, 0x00, sizeof(pPriest->m_sItemArray));
		memset(pPriest->m_arMerchantItems, 0x00, sizeof(pPriest->m_arMerchantItems));




		pPriest->m_sItemArray[0].nNum = 910156114;
		pPriest->m_sItemArray[1].nNum = 926007598;
		pPriest->m_sItemArray[2].nNum = 910156114;
		pPriest->m_sItemArray[3].nNum = 910156114;
		pPriest->m_sItemArray[4].nNum = 926005598;
		pPriest->m_sItemArray[5].nNum = 610001000;
		pPriest->m_sItemArray[6].nNum = 140210168;
		pPriest->m_sItemArray[7].nNum = 910160118;
		pPriest->m_sItemArray[8].nNum = 171510008;
		pPriest->m_sItemArray[9].nNum = 910162272;
		pPriest->m_sItemArray[10].nNum = 926006598;
		pPriest->m_sItemArray[11].nNum = 910162272;
		pPriest->m_sItemArray[12].nNum = 926008598;
		pPriest->m_sItemArray[13].nNum = 926009598;

		for (int i = 0; i < 14; i++)
			pPriest->m_sItemArray[i].sCount = 1;

		for (int i = 0; i < 14; i++)
			pPriest->m_sItemArray[i].nSerialNum = GenerateItemSerial();

		for (int i = 0; i < 14; i++)
			pPriest->m_sItemArray[i].sDuration = 1;



		pPriest->m_reblvl = pUser->m_bRebirthLevel;


		pPriest->m_bstrSkill[0] = 0;
		pPriest->m_bstrSkill[1] = 0;
		pPriest->m_bstrSkill[2] = 0;
		pPriest->m_bstrSkill[3] = 0;
		pPriest->m_bstrSkill[4] = 0;
		pPriest->m_bstrSkill[5] = 68;
		pPriest->m_bstrSkill[6] = 80;
		pPriest->m_bstrSkill[7] = 0;
		pPriest->m_bstrSkill[8] = 0;
		pPriest->m_bstrSkill[9] = 0;

		pPriest->m_iGold = pUser->m_iGold;
		pPriest->m_sPoints = pUser->m_sPoints;

		pPriest->m_bStats[0] = 167;
		pPriest->m_bStats[1] = 60;
		pPriest->m_bStats[2] = 60;
		pPriest->m_bStats[3] = 255;
		pPriest->m_bStats[4] = 50;

		pPriest->m_strUserID = "[" + pUser->GetName() + "'s Priest]";
		pPriest->m_bKnightsRank = pUser->m_bKnightsRank;
		pPriest->m_bPersonalRank = pUser->m_bPersonalRank;
		pPriest->m_sAchieveCoverTitle = pUser->m_sCoverTitle;
		pPriest->ReturnSymbolisOK = pUser->ReturnSymbolisOK;
		pPriest->m_bResHpType = USER_STANDING;

		pPriest->SetPosition(pUser->GetX(), pUser->GetY(), pUser->GetZ());
		pPriest->SetRegion(pUser->GetNewRegionX(), pUser->GetNewRegionZ());

		pPriest->m_bUserPriestBotID = pUser->GetSocketID();

		pUser->m_bUserPriestBotID = pPriest->GetID();

		pUser->hasPriestBot = true;

		if (pUser->GetNation() == KARUS)
			pPriest->m_teamColour = TeamColourBlue;
		else
			pPriest->m_teamColour = TeamColourRed;

		g_DBAgent.LoadPriestBotGenieData(userid, pUser);

		pPriest->SetMaxHp(1);
		pPriest->SetMaxMp();
		pPriest->UserInOut(INOUT_IN);

		//pPriest->GenieStart();

		return pPriest->GetID();
	}//
	return 0;
}



uint16 CGameServerDlg::SpawnSlaveUserBot(int Minute, CUser* pUser)
{
	foreach_stlmap(itr, m_BotArray)
	{
		CBot* pSlaveUser = itr->second;

		if (pSlaveUser->m_state == GAME_STATE_INGAME)
			continue;

		if (pSlaveUser->GetLevel() > pUser->GetLevel()
			|| pSlaveUser->GetNation() != pUser->GetNation())
			continue;

		pSlaveUser->Initialize();
		pSlaveUser->LastWarpTime = UNIXTIME + (Minute * 60);
		pSlaveUser->m_pMap = GetZoneByID(pUser->GetZoneID());
		pSlaveUser->m_bZone = pUser->GetZoneID();
		pSlaveUser->m_sClass = pUser->GetClass();
		pSlaveUser->m_bRace = pUser->m_bRace;
		pSlaveUser->m_curx = pUser->m_curx;
		pSlaveUser->m_curz = pUser->m_curz;
		pSlaveUser->m_cury = pUser->m_cury;
		pSlaveUser->m_oldx = pUser->m_oldx;
		pSlaveUser->m_oldy = pUser->m_oldy;
		pSlaveUser->m_oldz = pUser->m_oldz;
		pSlaveUser->m_nHair = pUser->m_nHair;
		pSlaveUser->m_bFace = pUser->m_bFace;
		pSlaveUser->m_bFame = pUser->m_bFame;
		pSlaveUser->m_bLevel = pUser->GetLevel();

		memset(pSlaveUser->m_sItemArray, 0x00, sizeof(pSlaveUser->m_sItemArray));
		memset(pSlaveUser->m_arMerchantItems, 0x00, sizeof(pSlaveUser->m_arMerchantItems));

		for (int i = 0; i < INVENTORY_TOTAL; i++)
			pSlaveUser->m_sItemArray[i] = pUser->m_sItemArray[i];

		pSlaveUser->m_reblvl = pUser->m_bRebirthLevel;

		for (int i = 0; i < 10; i++)
			pSlaveUser->m_bstrSkill[i] = pUser->m_bstrSkill[i];

		pSlaveUser->m_iGold = pUser->m_iGold;
		pSlaveUser->m_sPoints = pUser->m_sPoints;

		for (int i = 0; i < 5; i++)
			pSlaveUser->m_bStats[i] = pUser->m_bStats[i];

		pSlaveUser->m_strUserID = "[Slave]" + pUser->GetName();
		pSlaveUser->m_bKnightsRank = pUser->m_bKnightsRank;
		pSlaveUser->m_bPersonalRank = pUser->m_bPersonalRank;
		pSlaveUser->m_sAchieveCoverTitle = pUser->m_sCoverTitle;
		pSlaveUser->ReturnSymbolisOK = pUser->ReturnSymbolisOK;
		pSlaveUser->m_bResHpType = pUser->m_bResHpType;

		pSlaveUser->SetPosition(pUser->GetX(), pUser->GetY(), pUser->GetZ());
		pSlaveUser->SetRegion(pUser->GetNewRegionX(), pUser->GetNewRegionZ());

		pSlaveUser->m_bSlaveMerchant = true;
		pSlaveUser->m_bSlaveUserID = pUser->GetSocketID();

		pUser->m_bSlaveMerchant = true;
		pUser->m_bSlaveUserID = pSlaveUser->GetID();

		if (pUser->GetNation() == KARUS)
			pSlaveUser->m_teamColour = TeamColourBlue;
		else
			pSlaveUser->m_teamColour = TeamColourRed;

		pSlaveUser->SetMaxHp(1);
		pSlaveUser->SetMaxMp();
		pSlaveUser->UserInOut(INOUT_IN);
		return pSlaveUser->GetID();
	}
	return 0;
}

void CBot::HandleSurroundingUserRegionUpdate()
{
	Packet result(WIZ_USER_INFORMATIN, uint8(UserInfoShow));
	result.SByte();
	result << GetName();
	g_pMain->Send_Zone(&result, GetZoneID());
}

uint16 CGameServerDlg::SpawnUserBot(int Minute, uint8 byZone, float fX, float fY, float fZ, uint8 Restipi, uint8 minlevel /* = 1*/)
{
	foreach_stlmap(itr, m_BotArray)
	{
		CBot* pUser = itr->second;

		if (pUser->m_state == GAME_STATE_INGAME)
			continue;

		if (byZone <= ZONE_ELMORAD && byZone != pUser->m_bNation
			|| (byZone >= ZONE_KARUS_ESLANT
				&& byZone <= ZONE_ELMORAD_ESLANT
				&& byZone != (pUser->m_bNation + 10)))
			continue;

		if (pUser->m_bLevel > minlevel)
			continue;

		pUser->m_bMerchantState = MERCHANT_STATE_NONE;
		pUser->LastWarpTime = UNIXTIME + (Minute * 60);
		pUser->m_pMap = GetZoneByID(byZone);
		pUser->m_bZone = byZone;

		if (Restipi == 1)
		{
			_ITEM_DATA* pItem = &pUser->m_sItemArray[RIGHTHAND];
			if (pItem == nullptr)
			{
				_ITEM_TABLE* pTable = GetItemPtr(MATTOCK);
				if (pTable == nullptr)
					continue;

				_ITEM_DATA* pTItem = &pUser->m_sItemArray[LEFTHAND];
				if (pTItem != nullptr)
					memset(pTItem, 0x00, sizeof(_ITEM_DATA));

				memset(pItem, 0x00, sizeof(_ITEM_DATA));
				pItem->nNum = pTable->m_iNum;
				pItem->nSerialNum = GenerateItemSerial();
				pItem->sCount = 1;
				pItem->sDuration = pTable->m_sDuration;

				pUser->m_bResHpType = USER_MINING;
			}
			else
			{
				_ITEM_TABLE* pTable = GetItemPtr(pItem->nNum);
				if (pTable == nullptr)
					continue;

				if (pTable->isPickaxe())
					pUser->m_bResHpType = USER_MINING;
				else
				{
					pTable = GetItemPtr(MATTOCK);
					if (pTable == nullptr)
						continue;

					_ITEM_DATA* pTItem = &pUser->m_sItemArray[LEFTHAND];
					if (pTItem != nullptr)
						memset(pTItem, 0x00, sizeof(_ITEM_DATA));

					memset(pItem, 0x00, sizeof(_ITEM_DATA));
					pItem->nNum = pTable->m_iNum;
					pItem->nSerialNum = GenerateItemSerial();
					pItem->sCount = 1;
					pItem->sDuration = pTable->m_sDuration;

					pUser->m_bResHpType = USER_MINING;
				}
			}
		}
		else if (Restipi == 2)
		{
			_ITEM_DATA* pItem = &pUser->m_sItemArray[RIGHTHAND];
			if (pItem == nullptr)
			{
				_ITEM_TABLE* pTable = GetItemPtr(FISHING);
				if (pTable == nullptr)
					continue;

				_ITEM_DATA* pTItem = &pUser->m_sItemArray[LEFTHAND];
				if (pTItem != nullptr)
					memset(pTItem, 0x00, sizeof(_ITEM_DATA));

				memset(pItem, 0x00, sizeof(_ITEM_DATA));
				pItem->nNum = pTable->m_iNum;
				pItem->nSerialNum = GenerateItemSerial();
				pItem->sCount = 1;
				pItem->sDuration = pTable->m_sDuration;

				pUser->m_bResHpType = USER_FLASHING;
			}
			else
			{
				_ITEM_TABLE* pTable = GetItemPtr(pItem->nNum);
				if (pTable == nullptr)
					continue;

				if (pTable->isFishing())
					pUser->m_bResHpType = USER_FLASHING;
				else
				{
					pTable = GetItemPtr(FISHING);
					if (pTable == nullptr)
						continue;

					_ITEM_DATA* pTItem = &pUser->m_sItemArray[LEFTHAND];
					if (pTItem != nullptr)
						memset(pTItem, 0x00, sizeof(_ITEM_DATA));

					memset(pItem, 0x00, sizeof(_ITEM_DATA));
					pItem->nNum = pTable->m_iNum;
					pItem->nSerialNum = GenerateItemSerial();
					pItem->sCount = 1;
					pItem->sDuration = pTable->m_sDuration;

					pUser->m_bResHpType = USER_FLASHING;
				}
			}
		}
		else if (Restipi == 3
			|| Restipi == 4)
			pUser->m_bResHpType = Restipi == 3 ? USER_STANDING : USER_SITDOWN;
		else if (Restipi == 5)
		{
			int Random = myrand(0, 10000);

			pUser->m_bResHpType = Random > 5000 ? USER_STANDING : USER_SITDOWN;
		}
		else
			continue;

		pUser->SetPosition(fX, fY, fZ);
		pUser->SetRegion(pUser->GetNewRegionX(), pUser->GetNewRegionZ());

		pUser->SetMaxHp(1);
		pUser->SetMaxMp();
		pUser->UserInOut(INOUT_IN);
		return pUser->GetID();
	}
	return true;
}