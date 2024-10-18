#include "stdafx.h"
#include "DBAgent.h"

#pragma region GenderChangeV2(Packet & pkt)
void CUser::GenderChangeV2(Packet& pkt)
{
	Packet result(WIZ_GENDER_CHANGE);

	if (isDead()
		|| isTrading()
		|| isMerchanting()
		|| isStoreOpen()
		|| isSellingMerchant()
		|| isBuyingMerchant()
		|| isMining())
		return;

	if (GetRace() == 0
		|| GetRace() == KARUS_BIG
		|| (GetRace() == KARUS_MIDDLE && GetClass() % 100 < 10))
		goto fail_return;

	uint8 newRace, newFace, resultCode = 0, opCode;
	uint32 newHair;

	pkt >> opCode >> newRace >> newFace >> newHair;

	if (!CheckExistItem(ITEM_GENDER_CHANGE))
		goto fail_return;

	if (newRace == 0 || newFace == 0 || newHair == 0)
		goto fail_return;

	if (newRace < 10 && GetNation() != 1)
		goto fail_return;

	if (newRace > 10 && GetNation() != 2)
		goto fail_return;

	if (newRace > 13 || newRace == GetRace())
		goto fail_return;

	if (newRace > 5 && GetNation() == 1)
		goto fail_return;


	m_bRace = newRace;
	m_nHair = newHair;
	m_bFace = newFace;

	resultCode = g_DBAgent.UpdateUser(GetName(), UPDATE_PACKET_SAVE, this);

	if (resultCode < 1)
		goto fail_return;

	result << uint8(1) << uint16(GetID()) << newRace << newFace << newHair;
	SendToRegion(&result, nullptr, GetEventRoom());

	RobItem(ITEM_GENDER_CHANGE);

	if (GetHealth() < (GetMaxHealth() / 2))
		HpChange(GetMaxHealth());

	return;
fail_return:
	result << resultCode;
	Send(&result);

	/*SendMyInfo();

	UserInOut(INOUT_OUT);
	SetRegion(GetNewRegionX(), GetNewRegionZ());
	UserInOut(INOUT_WARP);
	RemoveRegion(GetNewRegionX(), GetNewRegionZ());

	g_pMain->UserInOutForMe(this);
	g_pMain->RegionUserInOutForMe(this);
	g_pMain->RegionNpcInfoForMe(this);
	g_pMain->MerchantUserInOutForMe(this);

	ResetWindows();

	InitType4();
	RecastSavedMagic();*/

}
#pragma endregion


#pragma region Gender Change
bool CUser::GenderChange(uint8 Race /*= 0*/)
{
	if (Race == 0 || Race > 13)
		return false;

	if (!CheckExistItem(ITEM_GENDER_CHANGE))
		return false;

	RobItem(ITEM_GENDER_CHANGE);

	m_bRace = Race;

	g_DBAgent.UpdateUser(GetName(), UPDATE_PACKET_SAVE, this);

	if (GetHealth() < (GetMaxHealth() / 2))
		HpChange(GetMaxHealth());

	SendMyInfo();

	UserInOut(INOUT_OUT);
	SetRegion(GetNewRegionX(), GetNewRegionZ());
	UserInOut(INOUT_WARP);

	g_pMain->UserInOutForMe(this);
	g_pMain->NpcInOutForMe(this);
	g_pMain->MerchantUserInOutForMe(this);

	ResetWindows();

	InitType4();
	RecastSavedMagic();

	return true;
}
#pragma endregion

#pragma region Job Change

uint8 CUser::JobChange(uint8 NewJob /*= 0*/)
{
	uint8 bNewClass = 0, bNewRace = 0;
	uint8 bResult = 0;

	if (NewJob < 1 || NewJob > 4)
		return 5; // Unknown job is selected...

	if (!CheckExistItem(ITEM_JOB_CHANGE))
		return 6; // Job Change scroll is not found...

	for (int i = 0; i < SLOT_MAX; i++)
	{
		_ITEM_DATA* pItem = GetItem(i);
		if (pItem == nullptr)
			continue;

		if (pItem->nNum != 0)
		{
			bResult = 7;
			break;
		}
	}

	if (bResult == 7)
	{
		Packet result(WIZ_CLASS_CHANGE, uint8(ALL_POINT_CHANGE));
		result << uint8(4) << int(0);
		Send(&result);
		return bResult; // While there are items equipped on you.
	}

	// If bResult between 1 and 4 character already selected job...

	// If selected a new job Warrior
	if (NewJob == 1)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Rogue, Magician, Priest
			if (isBeginnerRogue() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = KARUWARRIOR;
				bNewRace = KARUS_BIG;
			}
			// Skilled Karus Rogue, Magician, Priest
			else if (isNoviceRogue() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = BERSERKER;
				bNewRace = KARUS_BIG;
			}
			// Mastered Karus Rogue, Magician, Priest
			else if (isMasteredRogue() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = GUARDIAN;
				bNewRace = KARUS_BIG;
			}
		}
		else
		{
			// Beginner El Morad Rogue, Magician, Priest
			if (isBeginnerRogue() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = ELMORWARRRIOR;
				bNewRace = GetRace();
			}
			// Skilled El Morad Rogue, Magician, Priest
			else if (isNoviceRogue() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = BLADE;
				bNewRace = GetRace();
			}
			// Mastered El Morad Rogue, Magician, Priest
			else if (isMasteredRogue() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = PROTECTOR;
				bNewRace = GetRace();
			}
		}

		// Character already Warrior.
		if (bNewClass == 0 || bNewRace == 0)
			bResult = NewJob;
	}

	// If selected a new job Rogue
	if (NewJob == 2)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Warrior, Magician, Priest
			if (isBeginnerWarrior() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = KARUROGUE;
				bNewRace = KARUS_MIDDLE;
			}
			// Skilled Karus Warrior, Magician, Priest
			else if (isNoviceWarrior() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = HUNTER;
				bNewRace = KARUS_MIDDLE;
			}
			// Mastered Karus Warrior, Magician, Priest
			else if (isMasteredWarrior() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = PENETRATOR;
				bNewRace = KARUS_MIDDLE;
			}
		}
		else
		{
			// Beginner El Morad Warrior, Magician, Priest
			if (isBeginnerWarrior() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = ELMOROGUE;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Skilled El Morad Warrior, Magician, Priest
			else if (isNoviceWarrior() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = RANGER;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Mastered El Morad Warrior, Magician, Priest
			else if (isMasteredWarrior() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = ASSASSIN;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
		}

		// Character already Rogue.
		if (bNewClass == 0 || bNewRace == 0)
			bResult = NewJob;
	}

	// If selected a new job Magician
	if (NewJob == 3)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Warrior, Rogue, Priest
			if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerPriest())
			{
				bNewClass = KARUWIZARD;

				if (GetRace() == KARUS_BIG
					|| GetRace() == KARUS_MIDDLE)
					bNewRace = KARUS_SMALL;
				else
					bNewRace = GetRace();
			}
			// Skilled Karus Warrior, Rogue, Priest
			else if (isNoviceWarrior() || isNoviceRogue() || isNovicePriest())
			{
				bNewClass = SORSERER;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE)
					bNewRace = KARUS_SMALL;
				else
					bNewRace = GetRace();
			}
			// Mastered Karus Warrior, Rogue, Priest
			else if (isMasteredWarrior() || isMasteredRogue() || isMasteredPriest())
			{
				bNewClass = NECROMANCER;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE)
					bNewRace = KARUS_SMALL;
				else
					bNewRace = GetRace();
			}
		}
		else
		{
			// Beginner El Morad Warrior, Rogue, Priest
			if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerPriest())
			{
				bNewClass = ELMOWIZARD;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Skilled El Morad Warrior, Rogue, Priest
			else if (isNoviceWarrior() || isNoviceRogue() || isNovicePriest())
			{
				bNewClass = MAGE;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Mastered El Morad Warrior, Rogue, Priest
			else if (isMasteredWarrior() || isMasteredRogue() || isMasteredPriest())
			{
				bNewClass = ENCHANTER;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
		}

		// Character already Magician.
		if (bNewClass == 0 || bNewRace == 0)
			bResult = NewJob;
	}

	// If selected a new job Priest
	if (NewJob == 4)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Warrior, Rogue, Magician
			if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerMage())
			{
				bNewClass = KARUPRIEST;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
					bNewRace = KARUS_MIDDLE;
				else
					bNewRace = GetRace();
			}
			// Skilled Karus Warrior, Rogue, Magician
			else if (isNoviceWarrior() || isNoviceRogue() || isNoviceMage())
			{
				bNewClass = SHAMAN;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
					bNewRace = KARUS_MIDDLE;
				else
					bNewRace = GetRace();
			}
			// Mastered Karus Warrior, Rogue, Magician
			else if (isMasteredWarrior() || isMasteredRogue() || isMasteredMage())
			{
				bNewClass = DARKPRIEST;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
					bNewRace = KARUS_MIDDLE;
				else
					bNewRace = GetRace();
			}
		}
		else
		{
			// Beginner El Morad Warrior, Rogue, Magician
			if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerMage())
			{
				bNewClass = ELMOPRIEST;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Skilled El Morad Warrior, Rogue, Magician
			else if (isNoviceWarrior() || isNoviceRogue() || isNoviceMage())
			{
				bNewClass = CLERIC;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Mastered El Morad Warrior, Rogue, Magician
			else if (isMasteredWarrior() || isMasteredRogue() || isMasteredMage())
			{
				bNewClass = DRUID;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
		}

		// Character already Priest.
		if (bNewClass == 0 || bNewRace == 0)
			bResult = NewJob;
	}

	if (bResult == 0)
	{
		RobItem(ITEM_JOB_CHANGE);

		m_sClass = bNewClass;
		m_bRace = bNewRace;

		// Reset stat and skill points...
		AllPointChange(true);
		AllSkillPointChange(true);

		g_DBAgent.UpdateUser(GetName(), UPDATE_PACKET_SAVE, this);

		if (GetHealth() < (GetMaxHealth() / 2))
			HpChange(GetMaxHealth());

		SendMyInfo();

		UserInOut(INOUT_OUT);
		SetRegion(GetNewRegionX(), GetNewRegionZ());
		UserInOut(INOUT_WARP);

		g_pMain->UserInOutForMe(this);
		g_pMain->NpcInOutForMe(this);
		g_pMain->MerchantUserInOutForMe(this);

		ResetWindows();

		InitType4();
		RecastSavedMagic();

		foreach_stlmap(itr, g_pMain->m_LoadOpenQuestArray)
		{
			if (itr->second == nullptr)
				continue;

			if (itr->second->nEventDataIndex != 0)
				SaveEvent(itr->second->nEventDataIndex, 4);
		}
	}

	return bResult;
}
#pragma endregion



#pragma region Job Change

uint8 CUser::JobChangev2(Packet& pkt)
{
	int8 NewJob;
	pkt >> NewJob;
	uint8 bNewClass = 0, bNewRace = 0;
	uint8 bResult = 0;

	if (GetZoneID() == ZONE_DEATHMATCH) {

		if (NewJob < 1 || NewJob > 4)
			return 5; // Unknown job is selected...

		// If bResult between 1 and 4 character already selected job...

		// If selected a new job Warrior
		if (NewJob == 1)
		{
			if (GetNation() == KARUS)
			{
				bNewClass = GUARDIAN;
				bNewRace = KARUS_BIG;
				SetStat(STAT_STR, 255);
				SetStat(STAT_STA, 177);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
				//
				//GetTotalSkill(SkillPointFree, 83);
				GetTotalSkill(SkillPointCat1, 83);
				GetTotalSkill(SkillPointCat2, 83);
				GetTotalSkill(SkillPointCat3, 83);
				GetTotalSkill(SkillPointMaster, 23);

			}
			else
			{
				bNewClass = PROTECTOR;
				bNewRace = GetRace();
				SetStat(STAT_STR, 255);
				SetStat(STAT_STA, 177);
				SetStat(STAT_DEX, 60);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
				//
				//GetTotalSkill(SkillPointFree, 83);
				GetTotalSkill(SkillPointCat1, 83);
				GetTotalSkill(SkillPointCat2, 83);
				GetTotalSkill(SkillPointCat3, 83);
				GetTotalSkill(SkillPointMaster, 23);

			}

			// Character already Warrior.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		// If selected a new job Rogue
		if (NewJob == 2)
		{
			if (GetNation() == KARUS)
			{
				bNewClass = PENETRATOR;
				bNewRace = KARUS_MIDDLE;
				SetStat(STAT_STR, 60);
				SetStat(STAT_STA, 177);
				SetStat(STAT_DEX, 255);
				SetStat(STAT_INT, 50);
				SetStat(STAT_CHA, 50);
				//
				//GetTotalSkill(SkillPointFree, 83);
				GetTotalSkill(SkillPointCat1, 83);
				GetTotalSkill(SkillPointCat2, 83);
				GetTotalSkill(SkillPointCat3, 83);
				GetTotalSkill(SkillPointMaster, 23);
			}
			else
			{

				bNewClass = ASSASSIN;

				if (GetRace() == BABARIAN) {
					bNewRace = ELMORAD_MAN;
					SetStat(STAT_STR, 60);
					SetStat(STAT_STA, 177);
					SetStat(STAT_DEX, 255);
					SetStat(STAT_INT, 50);
					SetStat(STAT_CHA, 50);
					//
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
				else {
					bNewRace = GetRace();
					SetStat(STAT_STR, 60);
					SetStat(STAT_STA, 177);
					SetStat(STAT_DEX, 255);
					SetStat(STAT_INT, 50);
					SetStat(STAT_CHA, 50);
					//
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
					
			}

			// Character already Rogue.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		// If selected a new job Magician
		if (NewJob == 3)
		{
			if (GetNation() == KARUS)
			{

				bNewClass = NECROMANCER;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE) {
					bNewRace = KARUS_SMALL;
					SetStat(STAT_STR, 50);
					SetStat(STAT_STA, 60);
					SetStat(STAT_DEX, 60);
					SetStat(STAT_INT, 160);
					SetStat(STAT_CHA, 255);
					//
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
					
				else {
					bNewRace = GetRace();
					SetStat(STAT_STR, 50);
					SetStat(STAT_STA, 60);
					SetStat(STAT_DEX, 60);
					SetStat(STAT_INT, 160);
					SetStat(STAT_CHA, 255);
					//
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
					
					
			}
			else
			{

				bNewClass = ENCHANTER;

				if (GetRace() == BABARIAN) {
					bNewRace = ELMORAD_MAN;
					SetStat(STAT_STR, 50);
					SetStat(STAT_STA, 60);
					SetStat(STAT_DEX, 60);
					SetStat(STAT_INT, 160);
					SetStat(STAT_CHA, 255);
					//
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
					
				else {
					bNewRace = GetRace();
					SetStat(STAT_STR, 50);
					SetStat(STAT_STA, 60);
					SetStat(STAT_DEX, 60);
					SetStat(STAT_INT, 160);
					SetStat(STAT_CHA, 255);
					//
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
					

			}

			// Character already Magician.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		// If selected a new job Priest
		if (NewJob == 4)
		{
			if (GetNation() == KARUS)
			{

				bNewClass = DARKPRIEST;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
					bNewRace = KARUS_MIDDLE;
				else
					bNewRace = GetRace();

			}
			else
			{

				bNewClass = DRUID;

				if (GetRace() == BABARIAN) {
					bNewRace = ELMORAD_MAN;
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
				
				else
				{
					bNewRace = GetRace();
					//GetTotalSkill(SkillPointFree, 83);
					GetTotalSkill(SkillPointCat1, 83);
					GetTotalSkill(SkillPointCat2, 83);
					GetTotalSkill(SkillPointCat3, 83);
					GetTotalSkill(SkillPointMaster, 23);
				}
					

			}

			// Character already Priest.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		if (bResult == 0)
		{

			m_sClass = bNewClass;
			m_bRace = bNewRace;

			//AllPointChange(true);
			//AllSkillPointChange(true);


			_ITEM_DATA ItemEmpty;

			for (int i = 0; i < INVENTORY_TOTAL; i++)
			{
				if (NewJob == 1) {
					_ITEM_DATA* pItem = g_pMain->m_DeathMatchItemsArray.GetData(i);
					if (!pItem)
					{
						//printf("sSlot : %d\n", i);
						memcpy(&m_sItemArray[i], &ItemEmpty, sizeof(_ITEM_DATA));
					}
					else
						memcpy(&m_sItemArray[i], pItem, sizeof(_ITEM_DATA));
				}
				if (NewJob == 2) {
					_ITEM_DATA* pItem = g_pMain->m_DeathMatchItemsArray1.GetData(i);
					if (!pItem)
					{
						//printf("sSlot : %d\n", i);
						memcpy(&m_sItemArray[i], &ItemEmpty, sizeof(_ITEM_DATA));
					}
					else
						memcpy(&m_sItemArray[i], pItem, sizeof(_ITEM_DATA));
				}
				if (NewJob == 3) {
					_ITEM_DATA* pItem = g_pMain->m_DeathMatchItemsArray2.GetData(i);
					if (!pItem)
					{
						//printf("sSlot : %d\n", i);
						memcpy(&m_sItemArray[i], &ItemEmpty, sizeof(_ITEM_DATA));
					}
					else
						memcpy(&m_sItemArray[i], pItem, sizeof(_ITEM_DATA));
				}
				if (NewJob == 4) {
					_ITEM_DATA* pItem = g_pMain->m_DeathMatchItemsArray3.GetData(i);
					if (!pItem)
					{
						//printf("sSlot : %d\n", i);
						memcpy(&m_sItemArray[i], &ItemEmpty, sizeof(_ITEM_DATA));
					}
					else
						memcpy(&m_sItemArray[i], pItem, sizeof(_ITEM_DATA));
				}

			}

			if (GetHealth() < GetMaxHealth())
				HpChange(GetMaxHealth());

			/*if (GetMana() < GetMaxMana())
				HpChange(GetMaxHealth());*/



			SendMyInfo();

			UserInOut(INOUT_OUT);
			SetRegion(GetNewRegionX(), GetNewRegionZ());
			UserInOut(INOUT_WARP);

			g_pMain->UserInOutForMe(this);
			g_pMain->NpcInOutForMe(this);
			g_pMain->MerchantUserInOutForMe(this);

			ResetWindows();

			InitType4();
			RecastSavedMagic();


		}

		return bResult;
	}
	else {
		if (NewJob < 1 || NewJob > 4)
			return 5; // Unknown job is selected...

		if (!CheckExistItem(ITEM_JOB_CHANGE))
			return 6; // Job Change scroll is not found...

		for (int i = 0; i < SLOT_MAX; i++)
		{
			_ITEM_DATA* pItem = GetItem(i);
			if (pItem == nullptr)
				continue;

			if (pItem->nNum != 0)
			{
				bResult = 7;
				break;
			}
		}

		if (bResult == 7)
		{
			Packet result(WIZ_CLASS_CHANGE, uint8(ALL_POINT_CHANGE));
			result << uint8(4) << int(0);
			Send(&result);
			return bResult; // While there are items equipped on you.
		}

		// If bResult between 1 and 4 character already selected job...

		// If selected a new job Warrior
		if (NewJob == 1)
		{
			if (GetNation() == KARUS)
			{
				// Beginner Karus Rogue, Magician, Priest
				if (isBeginnerRogue() || isBeginnerMage() || isBeginnerPriest())
				{
					bNewClass = KARUWARRIOR;
					bNewRace = KARUS_BIG;
				}
				// Skilled Karus Rogue, Magician, Priest
				else if (isNoviceRogue() || isNoviceMage() || isNovicePriest())
				{
					bNewClass = BERSERKER;
					bNewRace = KARUS_BIG;
				}
				// Mastered Karus Rogue, Magician, Priest
				else if (isMasteredRogue() || isMasteredMage() || isMasteredPriest())
				{
					bNewClass = GUARDIAN;
					bNewRace = KARUS_BIG;
				}
			}
			else
			{
				// Beginner El Morad Rogue, Magician, Priest
				if (isBeginnerRogue() || isBeginnerMage() || isBeginnerPriest())
				{
					bNewClass = ELMORWARRRIOR;
					bNewRace = GetRace();
				}
				// Skilled El Morad Rogue, Magician, Priest
				else if (isNoviceRogue() || isNoviceMage() || isNovicePriest())
				{
					bNewClass = BLADE;
					bNewRace = GetRace();
				}
				// Mastered El Morad Rogue, Magician, Priest
				else if (isMasteredRogue() || isMasteredMage() || isMasteredPriest())
				{
					bNewClass = PROTECTOR;
					bNewRace = GetRace();
				}
			}

			// Character already Warrior.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		// If selected a new job Rogue
		if (NewJob == 2)
		{
			if (GetNation() == KARUS)
			{
				// Beginner Karus Warrior, Magician, Priest
				if (isBeginnerWarrior() || isBeginnerMage() || isBeginnerPriest())
				{
					bNewClass = KARUROGUE;
					bNewRace = KARUS_MIDDLE;
				}
				// Skilled Karus Warrior, Magician, Priest
				else if (isNoviceWarrior() || isNoviceMage() || isNovicePriest())
				{
					bNewClass = HUNTER;
					bNewRace = KARUS_MIDDLE;
				}
				// Mastered Karus Warrior, Magician, Priest
				else if (isMasteredWarrior() || isMasteredMage() || isMasteredPriest())
				{
					bNewClass = PENETRATOR;
					bNewRace = KARUS_MIDDLE;
				}
			}
			else
			{
				// Beginner El Morad Warrior, Magician, Priest
				if (isBeginnerWarrior() || isBeginnerMage() || isBeginnerPriest())
				{
					bNewClass = ELMOROGUE;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
				// Skilled El Morad Warrior, Magician, Priest
				else if (isNoviceWarrior() || isNoviceMage() || isNovicePriest())
				{
					bNewClass = RANGER;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
				// Mastered El Morad Warrior, Magician, Priest
				else if (isMasteredWarrior() || isMasteredMage() || isMasteredPriest())
				{
					bNewClass = ASSASSIN;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
			}

			// Character already Rogue.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		// If selected a new job Magician
		if (NewJob == 3)
		{
			if (GetNation() == KARUS)
			{
				// Beginner Karus Warrior, Rogue, Priest
				if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerPriest())
				{
					bNewClass = KARUWIZARD;

					if (GetRace() == KARUS_BIG
						|| GetRace() == KARUS_MIDDLE)
						bNewRace = KARUS_SMALL;
					else
						bNewRace = GetRace();
				}
				// Skilled Karus Warrior, Rogue, Priest
				else if (isNoviceWarrior() || isNoviceRogue() || isNovicePriest())
				{
					bNewClass = SORSERER;

					if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE)
						bNewRace = KARUS_SMALL;
					else
						bNewRace = GetRace();
				}
				// Mastered Karus Warrior, Rogue, Priest
				else if (isMasteredWarrior() || isMasteredRogue() || isMasteredPriest())
				{
					bNewClass = NECROMANCER;

					if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE)
						bNewRace = KARUS_SMALL;
					else
						bNewRace = GetRace();
				}
			}
			else
			{
				// Beginner El Morad Warrior, Rogue, Priest
				if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerPriest())
				{
					bNewClass = ELMOWIZARD;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
				// Skilled El Morad Warrior, Rogue, Priest
				else if (isNoviceWarrior() || isNoviceRogue() || isNovicePriest())
				{
					bNewClass = MAGE;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
				// Mastered El Morad Warrior, Rogue, Priest
				else if (isMasteredWarrior() || isMasteredRogue() || isMasteredPriest())
				{
					bNewClass = ENCHANTER;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
			}

			// Character already Magician.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		// If selected a new job Priest
		if (NewJob == 4)
		{
			if (GetNation() == KARUS)
			{
				// Beginner Karus Warrior, Rogue, Magician
				if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerMage())
				{
					bNewClass = KARUPRIEST;

					if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
						bNewRace = KARUS_MIDDLE;
					else
						bNewRace = GetRace();
				}
				// Skilled Karus Warrior, Rogue, Magician
				else if (isNoviceWarrior() || isNoviceRogue() || isNoviceMage())
				{
					bNewClass = SHAMAN;

					if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
						bNewRace = KARUS_MIDDLE;
					else
						bNewRace = GetRace();
				}
				// Mastered Karus Warrior, Rogue, Magician
				else if (isMasteredWarrior() || isMasteredRogue() || isMasteredMage())
				{
					bNewClass = DARKPRIEST;

					if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
						bNewRace = KARUS_MIDDLE;
					else
						bNewRace = GetRace();
				}
			}
			else
			{
				// Beginner El Morad Warrior, Rogue, Magician
				if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerMage())
				{
					bNewClass = ELMOPRIEST;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
				// Skilled El Morad Warrior, Rogue, Magician
				else if (isNoviceWarrior() || isNoviceRogue() || isNoviceMage())
				{
					bNewClass = CLERIC;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
				// Mastered El Morad Warrior, Rogue, Magician
				else if (isMasteredWarrior() || isMasteredRogue() || isMasteredMage())
				{
					bNewClass = DRUID;

					if (GetRace() == BABARIAN)
						bNewRace = ELMORAD_MAN;
					else
						bNewRace = GetRace();
				}
			}

			// Character already Priest.
			if (bNewClass == 0 || bNewRace == 0)
				bResult = NewJob;
		}

		if (bResult == 0)
		{
			RobItem(ITEM_JOB_CHANGE);

			m_sClass = bNewClass;
			m_bRace = bNewRace;

			// Reset stat and skill points...
			AllPointChange(true);
			AllSkillPointChange(true);

			g_DBAgent.UpdateUser(GetName(), UPDATE_PACKET_SAVE, this);

			if (GetHealth() < (GetMaxHealth() / 2))
				HpChange(GetMaxHealth());

			SendMyInfo();

			UserInOut(INOUT_OUT);
			SetRegion(GetNewRegionX(), GetNewRegionZ());
			UserInOut(INOUT_WARP);

			g_pMain->UserInOutForMe(this);
			g_pMain->NpcInOutForMe(this);
			g_pMain->MerchantUserInOutForMe(this);

			ResetWindows();

			InitType4();
			RecastSavedMagic();

			foreach_stlmap(itr, g_pMain->m_LoadOpenQuestArray)
			{
				if (itr->second == nullptr)
					continue;

				if (itr->second->nEventDataIndex != 0)
					SaveEvent(itr->second->nEventDataIndex, 4);
			}

		}

		return bResult;
	}
}
#pragma endregion




void CUser::JobChangeCagir()
{

	if (g_pMain->pTempleEvent.ActiveEvent = DEATHMATCH) {
		Packet result(PL_JOBCHANGEDM);
		result << uint8(1);
		Send(&result);
	}
	else {
		Packet result(PL_JOBCHANGE);
		result << uint8(1);
		Send(&result);
	}



}

#pragma endregion