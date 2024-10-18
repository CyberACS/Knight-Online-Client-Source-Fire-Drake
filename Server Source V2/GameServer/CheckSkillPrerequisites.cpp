#include "stdafx.h"
#include "MagicProcess.h"
#include "MagicInstance.h"


SkillUseResult MagicInstance::CheckSkillPrerequisites()
{
	CUser* pUserhasBot = TO_USER(pSkillCaster);

	//printf("bOpcode %d \n", bOpcode);
	if (pSkill == nullptr)
		return SkillUseResult::Failed;

	if (pSkillCaster == nullptr)
	{
		TRACE("#### CheckSkillPrerequisites : pSkillCaster == nullptr ####\n");
		return SkillUseResult::Failed;
	}

	
	CBot* pUser2 = nullptr;
	pUser2 = g_pMain->m_BotArray.GetData(sCasterID);
	if (pUser2 != nullptr)
		return SkillUseResult::Successful;
	

	/***********controla el tiempo de uso de skill para mago,priest y kurian de magic_tipe3 y magic_tipe4***********/
	if (pSkillCaster->isPlayer() && pSkill->bCastTime > 0 && nSkillID < 400000 && (TO_USER(pSkillCaster)->GetClass() == pSkill->iNum / 1000))
	{
		Guard lock(TO_USER(pSkillCaster)->m_SkillCastListLock);
		if (pSkill->bType[0] != 8)
		{
			if (bOpcode == MAGIC_CASTING)
				// we shall do nothing to the casting skills	
				TO_USER(pSkillCaster)->m_SkillCastList[nSkillID] = UNIXTIME2;

			// but if we are casting a skill, well s/he needs to wait a second!
			else if (bOpcode == MAGIC_EFFECTING)
			{
				auto itr = TO_USER(pSkillCaster)->m_SkillCastList.find(nSkillID);

				if (itr == TO_USER(pSkillCaster)->m_SkillCastList.end() || TO_USER(pSkillCaster)->m_SkillCastList.empty())
					return SkillUseResult::Successful;

				if (TO_USER(pSkillCaster)->GetFame() == COMMAND_CAPTAIN)
				{
					if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(ULONGLONG(pSkill->bCastTime / 2) * (TO_USER(pSkillCaster)->isMage() ? 60 : pSkill->bCastTime)))
						return SkillUseResult::Successful;
					else
						TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
				}
				else if (TO_USER(pSkillCaster)->isTransformed() && TO_USER(pSkillCaster)->m_transformationType == Unit::TransformationNPC) // esta transformado
				{
					if (TO_USER(pSkillCaster)->isWarrior())
					{
						if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(ULONGLONG(pSkill->bCastTime) * 30))
							return SkillUseResult::Successful;
						else
							TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
					}
					else if (TO_USER(pSkillCaster)->isRogue())
					{
						if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(ULONGLONG(pSkill->bCastTime) * 35))
							return SkillUseResult::Successful;
						else
							TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
					}
					else if (TO_USER(pSkillCaster)->isPriest())
					{
						if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(ULONGLONG(pSkill->bCastTime) * 69))
							return SkillUseResult::Successful;
						else
							TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
					}
					else if (TO_USER(pSkillCaster)->isMage())
					{
						if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(ULONGLONG(pSkill->bCastTime) * 60))
							return SkillUseResult::Successful;
						else
							TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
					}
					else if (TO_USER(pSkillCaster)->isPortuKurian())
					{
						if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(ULONGLONG(pSkill->bCastTime) * 65))
							return SkillUseResult::Successful;
						else
							TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
					}
				}
				else if (!TO_USER(pSkillCaster)->isTransformed())
				{

					if (TO_USER(pSkillCaster)->isPortuKurian())
					{
						uint8 reskill = 0;
						if (nSkillID == 115810 || nSkillID == 215810)
							reskill = 200;

						if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(TO_USER(pSkillCaster)->isDevil() ? ULONGLONG(pSkill->bCastTime) * 50 : ULONGLONG(pSkill->bCastTime) * 100 - reskill))
							return SkillUseResult::Successful;
						else
							TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
					}
					else if (TO_USER(pSkillCaster)->isMage() || TO_USER(pSkillCaster)->isPriest())
					{
						//printf("system skill %d ( old %llu < %llu )\n", nSkillID, ULONGLONG(UNIXTIME2 - itr->second), ULONGLONG(ULONGLONG(pSkill->bCastTime) * 100));
						if (ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(ULONGLONG(pSkill->bCastTime) * 80))
							return SkillUseResult::Successful;
						else
							TO_USER(pSkillCaster)->m_SkillCastList.erase(nSkillID);
					}
				}
			}
		}

	}



	/**********codigo para skill de armor de mago 75***********/
	if (TO_USER(pSkillCaster)->GetClass() == (pSkill->iNum - 80000) / 1000)
		return SkillUseResult::Successful;


	if (pSkillCaster->isPlayer()
		&& bOpcode != MAGIC_CANCEL
		&& bOpcode != MAGIC_CANCEL2
		&& bOpcode != MAGIC_CANCEL_TRANSFORMATION
		&& pSkill->iNum < 300000 && (TO_USER(pSkillCaster)->GetNation() != pSkill->iNum / 100000))
	{
		TRACE("#### %s is trying to use skill[%d] of other class. ####\n", pSkillCaster->GetName().c_str(), pSkill->iNum);
		return SkillUseResult::Failed;
	}




	if (bOpcode != MAGIC_FLYING && bOpcode != MAGIC_EFFECTING)
	{
		if (bOpcode == MAGIC_CASTING
			&& pSkillTarget
			&& (pSkill->sRange > 0
				&& pSkill->sUseStanding == 1
				&& !(pSkillCaster->GetDistanceSqrt(pSkillTarget) > float(pSkill->sRange))))  // Skill Range Check for Casting
			return SkillUseResult::Failed;
		else
			return SkillUseResult::Successful;
	}

	if (pSkillCaster->isPlayer())
	{
		CUser* pCaster = TO_USER(pSkillCaster);

		if (pCaster)
		{
			if (pSkill->sUseStanding == 1 && pCaster->m_sSpeed != 0) // Hacking prevention!
				return SkillUseResult::Failed;

			if ((pSkill->bType[0] == 3 || pSkill->bType[1] == 3) && !pCaster->isPortuKurian())
			{
				_MAGIC_TYPE3* pType = g_pMain->m_Magictype3Array.GetData(nSkillID);

				if (pType == nullptr)
					TRACE("[%s] Used skill %d but it does not exist in MagicType3.\n", pSkillCaster->GetName().c_str(), nSkillID);

				else if (pType->bAttribute == AttributeNone)
				{
					if (pSkill->bType[0] != 0)
						pCaster->m_MagicTypeCooldownList.erase(pSkill->bType[0]);

					if (pSkill->bType[1] != 0)
						pCaster->m_MagicTypeCooldownList.erase(pSkill->bType[1]);
				}
			}

			if (pSkill->bType[0] == 4 && pSkill->bType[1] == 0)
			{
				_MAGIC_TYPE4* pType4 = g_pMain->m_Magictype4Array.GetData(nSkillID);

				if (pType4 == nullptr)
					TRACE("[%s] Used skill %d but it does not exist in MagicType4.\n", pSkillCaster->GetName().c_str(), nSkillID);
				else if (pType4->bBuffType == BUFF_TYPE_FISHING)
				{
					if (!pSkillCaster->isPlayer())
						return SkillUseResult::Failed;

					if (pSkillCaster->isInTempleQuestEventZone())
						return SkillUseResult::Failed;

					if (TO_USER(pSkillCaster)->GetPremium() != 10 && pType4->sSpecialAmount == 1)
						return SkillUseResult::Failed;

					if (TO_USER(pSkillCaster)->GetPremium() != 11 && pType4->sSpecialAmount == 2)
						return SkillUseResult::Failed;

					if (TO_USER(pSkillCaster)->GetPremium() != 12 && pType4->sSpecialAmount == 3)
						return SkillUseResult::Failed;

					if (TO_USER(pSkillCaster)->m_FlashDcBonus == 100)
						return SkillUseResult::Failed;

					if (TO_USER(pSkillCaster)->m_FlashExpBonus == 100)
						return SkillUseResult::Failed;

					if (TO_USER(pSkillCaster)->m_FlashWarBonus == 10)
						return SkillUseResult::Failed;
				}
			}

			/**********Checa el skill rango de staff e mago************/
			if (pSkill->bType[0] == 1 && pSkill->bType[1] == 3)
			{
				uint16_t DuslerinSokagiRange = (uint16_t)sqrtf(pSkillCaster->GetDistance(pSkillTarget->GetX(), pSkillTarget->GetZ())) % 100;

				if (pSkill->sRange > 0 && DuslerinSokagiRange > pSkill->sRange * 2)

				{
					//printf("rango %d\n", DuslerinSokagiRange);
					return SkillUseResult::Failed;
				}

			}

			// Skil Range, Safety Area, Temple Zones and Event Room Checks...
			if ((pSkillTarget != nullptr && bOpcode != MAGIC_EFFECTING) || nSkillID == 108575 || nSkillID == 208575)
			{
				if ((pSkillCaster != pSkillTarget
					&& !pSkillCaster->isInAttackRange(pSkillTarget, pSkill))  // Skill Range Check
					|| (pCaster->isInEnemySafetyArea() && nSkillID < 400000) // Disable Skill in Safety Area 
					|| (pCaster->isInTempleEventZone() && !pCaster->isSameEventRoom(pSkillTarget)) // BDW CHAOS JR If EventRoom is not same disable Skill.
					|| (pCaster->isInTempleQuestEventZone() && !pCaster->isSameEventRoom(pSkillTarget))) // Monster Squard 81 / 82 / 83 If EventRoom is not same disable Skill.
					return SkillUseResult::Failed;
			}

			/***********controla los skill de area sus rangos*********/
			if (pSkillTarget == nullptr && pSkillCaster != nullptr)
			{
				if (pSkill->bMoral == MORAL_AREA_ALL
					|| pSkill->bMoral == MORAL_AREA_ENEMY
					|| pSkill->bMoral == MORAL_AREA_FRIEND
					|| (pSkill->bMoral == MORAL_SELF_AREA && !pCaster->isPortuKurian()))
				{
					if (!pSkillCaster->isInRangeSlow(this->sData[0], this->sData[2], (float)(pSkill->sRange)))
						return SkillUseResult::Failed;
				}
			}

			// Skill Cooldown Checks...
			if (bOpcode != MAGIC_TYPE4_EXTEND && pCaster->m_CoolDownList.find(nSkillID) != pCaster->m_CoolDownList.end() && pSkill->sReCastTime > 1)
			{
				SkillCooldownList::iterator itr = pCaster->m_CoolDownList.find(nSkillID);

				ULONGLONG DiffrentMilliseconds = UNIXTIME2 - itr->second;

				if (DiffrentMilliseconds > 0 && (DiffrentMilliseconds < ULONGLONG(pSkill->sReCastTime * 85)) && pSkill->bType[0] != 9 && !bIsRecastingSavedMagic)
					return SkillUseResult::Failed;
				else
					pCaster->m_CoolDownList.erase(nSkillID);
			}

			if (pCaster->isRogue() && pSkill->bType[0] == 2 && pSkill->iUseItem != 370007000)
			{
				_ITEM_TABLE* pLeftHand = TO_USER(pSkillCaster)->GetItemPrototype(TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND);
				_ITEM_TABLE* pRightHand = TO_USER(pSkillCaster)->GetItemPrototype(TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND);

				if ((pLeftHand == nullptr && pRightHand == nullptr)
					|| ((pLeftHand && !pLeftHand->isBow())
						|| (pRightHand && !pRightHand->isBow()))) // Hacking prevention!
					return SkillUseResult::Failed;

				if (bOpcode == MAGIC_EFFECTING)
					return SkillUseResult::Successful;

				if (bOpcode == MAGIC_EFFECTING)
					return SkillUseResult::Successful;
			}

			if (pSkill->iNum > 400000 // using hp and mana potion at the same time!!! not at all!
				&& pSkill->bType[0] == 3
				&& pSkill->bType[1] == 0
				&& pSkill->bMoral == MORAL_SELF
				&& pSkill->bItemGroup == 9)
			{
				_MAGIC_TYPE3* pType3 = g_pMain->m_Magictype3Array.GetData(pSkill->iNum);
				if (pType3 == nullptr)
					return SkillUseResult::Failed;

				// Skill mustn't do any damage or such.
				if ((pType3->bDirectType == 1 || pType3->bDirectType == 2)
					&& pType3->sFirstDamage > 0)
				{
					if ((UNIXTIME2 - pCaster->t_timeLastPotionUse) < PLAYER_POTION_REQUEST_INTERVAL)
						return SkillUseResult::Failed;

				}
			}

			if (pSkill->iNum == 208685 || pSkill->iNum == 108685)
				return SkillUseResult::Successful;


			// Same time skill checks...
			MagicTypeCooldownList::iterator itr;
			if ((pSkill->bType[0] == 1
				|| pSkill->bType[0] == 3
				|| pSkill->bType[0] == 4
				|| pSkill->bType[0] == 5
				|| pSkill->bType[0] == 6
				|| pSkill->bType[0] == 7)
				&& nSkillID < 400000
				&& pCaster->m_MagicTypeCooldownList.size() > 0
				&& pCaster->m_MagicTypeCooldownList.find(pSkill->bType[0]) != pCaster->m_MagicTypeCooldownList.end())
			{
				itr = pCaster->m_MagicTypeCooldownList.find(pSkill->bType[0]);
				if (!isStaffSkill() && (ULONGLONG(UNIXTIME2 - itr->second) < PLAYER_SKILL_REQUEST_INTERVAL - 200))
					return SkillUseResult::Failed;
				else if (isStaffSkill() && (ULONGLONG(UNIXTIME2 - itr->second) < PLAYER_SKILL_REQUEST_INTERVAL - 100))
					return SkillUseResult::NoFunction;
				else
					pCaster->m_MagicTypeCooldownList.erase(pSkill->bType[0]);
			}

			if ((pSkill->bType[1] == 1
				|| pSkill->bType[1] == 3
				|| pSkill->bType[1] == 4
				|| pSkill->bType[1] == 5
				|| pSkill->bType[1] == 6
				|| pSkill->bType[1] == 7)
				&& nSkillID < 400000
				&& pCaster->m_MagicTypeCooldownList.size() > 0
				&& pCaster->m_MagicTypeCooldownList.find(pSkill->bType[1]) != pCaster->m_MagicTypeCooldownList.end())
			{
				itr = pCaster->m_MagicTypeCooldownList.find(pSkill->bType[1]);

				if (!isStaffSkill() && (ULONGLONG(UNIXTIME2 - itr->second) < PLAYER_SKILL_REQUEST_INTERVAL - 200))
					return SkillUseResult::Failed;
				else if (isStaffSkill() && (ULONGLONG(UNIXTIME2 - itr->second) < PLAYER_SKILL_REQUEST_INTERVAL - 100))
					return SkillUseResult::NoFunction;
				else
					pCaster->m_MagicTypeCooldownList.erase(pSkill->bType[1]);
			}

			if (bOpcode == MAGIC_FLYING && pSkill->bType[0] == 2)
			{
				_MAGIC_TYPE2* pType = g_pMain->m_Magictype2Array.GetData(nSkillID);
				if (pType != nullptr)
				{


					if (pType->bNeedArrow == 1)
					{

						if (pSkill->bType[0] == 2
							&& nSkillID < 400000
							&& pCaster->m_MagicTypeCooldownList.size() > 0
							&& pCaster->m_MagicTypeCooldownList.find(pSkill->bType[0]) != pCaster->m_MagicTypeCooldownList.end())
						{
							itr = pCaster->m_MagicTypeCooldownList.find(pSkill->bType[0]);

							//printf("system1 skill %d ( %llu <  %llu)\n", nSkillID, ULONGLONG(UNIXTIME2 - itr->second), ULONGLONG(pSkill->bCastTime * 62));

							if ((ULONGLONG(UNIXTIME2 - itr->second) < ULONGLONG(pSkill->bCastTime * 62)/*PLAYER_SKILL_REQUEST_INTERVAL - 450*/))
								return SkillUseResult::NoFunction;
							else
								pCaster->m_MagicTypeCooldownList.erase(pSkill->bType[0]);
						}
					}
					else
					{
						if (pCaster->m_SkillCastimeList.find(nSkillID) != pCaster->m_SkillCastimeList.end())
						{
							SkillCastimeList::iterator itr = pCaster->m_SkillCastimeList.find(nSkillID);

							ULONGLONG DifeSeconds = UNIXTIME2 - itr->second;
							//printf("m_CoolDownList skill %d ( %llu <  %llu)\n", nSkillID, DifeSeconds, ULONGLONG(pSkill->bCastTime * 100));
							if (DifeSeconds > 0 && (DifeSeconds < ULONGLONG(pSkill->bCastTime * (pType->bNeedArrow == 3 ? 92 : 80))))
								return SkillUseResult::NoFunction;
							else
								pCaster->m_SkillCastimeList.erase(nSkillID);
						}
					}
				}
			}


			if (pCaster->GetZoneID() != ZONE_DRAKI_TOWER)
			{
				if (pSkill->iNum == 492070
					|| pSkill->iNum == 492071)
					return SkillUseResult::Failed;
			}
		}
	}

	if (pSkillTarget != nullptr)
	{
		if (pSkillTarget->isNPC())
		{
			if (pSkillCaster->GetMap()->isWarZone())
			{
				if (TO_NPC(pSkillTarget)->GetType() == NPC_GATE
					&& g_pMain->m_byBattleOpen != NATION_BATTLE)
					return SkillUseResult::NoFunction;

				if (TO_NPC(pSkillTarget)->GetType() == NPC_GATE
					&& TO_NPC(pSkillTarget)->m_byGateOpen == 1
					&& g_pMain->m_byBattleOpen == NATION_BATTLE)
					return SkillUseResult::NoFunction;
			}

			if (pSkillCaster->GetZoneID() == ZONE_DELOS)
			{
				if (TO_NPC(pSkillTarget)->GetType() == NPC_GATE
					&& g_pMain->m_byBattleOpen != SIEGE_BATTLE)
					return SkillUseResult::NoFunction;

				if (TO_NPC(pSkillTarget)->GetType() == NPC_GATE
					&& TO_NPC(pSkillTarget)->m_byGateOpen == 1
					&& g_pMain->m_byBattleOpen == SIEGE_BATTLE)
					return SkillUseResult::NoFunction;
			}
		}

		if (pSkillTarget->isNPC() && !pSkillCaster->isAttackable(pSkillTarget))
			return SkillUseResult::Failed;
		if (pSkillTarget->isBlinking() && !bIsRecastingSavedMagic)
			return SkillUseResult::NoFunction;
		else
		{
			if (pSkillTarget->isPlayer() && TO_USER(pSkillTarget)->hasBuff(BUFF_TYPE_FREEZE)) // Effect Sorunu ve Blink Sorunu Var...
				return SkillUseResult::Failed;
		}
	}

	return SkillUseResult::Successful;
}