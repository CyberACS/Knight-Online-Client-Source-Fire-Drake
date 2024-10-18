#include "stdafx.h"
#include "MagicProcess.h"
#include "MagicInstance.h"




SkillUseResult MagicInstance::UserCanCast()
{
	CBot* pPriest = nullptr;

	if (pSkill == nullptr)
		return SkillUseResult::Failed;

	pPriest = g_pMain->m_BotArray.GetData(sCasterID);

	if (pPriest != nullptr && !pPriest->isNPC())
		return SkillUseResult::Successful;

	if (pSkillCaster->isBlinking() && !bIsRecastingSavedMagic)
		return SkillUseResult::NoFunction;

	// We don't need to check anything as we're just canceling our buffs.
	if (bOpcode == MAGIC_CANCEL || bOpcode == MAGIC_CANCEL2
		// Also don't need to check anything if we're forwarding a fail packet.
		|| bOpcode == MAGIC_FAIL
		// Or are reapplying persistent buffs.
		|| bIsRecastingSavedMagic)
		return SkillUseResult::Successful;

	// Ensure the caster can use skills (i.e. they're not incapacitated, or have skills blocked, etc).
	// Additionally, unless it's resurrection-related, dead players cannot use skills.
	if (!pSkillCaster->canUseSkills()
		|| (pSkillCaster->isDead() && pSkill->bType[0] != 5))
		return SkillUseResult::Failed;

	if (!pSkillCaster->GearSkills())
		return SkillUseResult::Failed;

	// If we're using an AOE, and the target is specified... something's not right.
	if ((pSkill->bMoral >= MORAL_AREA_ENEMY && pSkill->bMoral <= MORAL_SELF_AREA) && sTargetID != -1)
	{
		// Items that proc skills require the target ID to be fixed up.
		// There's no other means of detecting who to target.
		if (!bIsItemProc)
			return SkillUseResult::Failed;

		sTargetID = -1;
	}

	if (isKrowazWeaponSkill() && !bIsItemProc) /*_ITEM_OP*/
	{
		printf("banned por skill hack \n");
		return SkillUseResult::Failed;
	}

	if (TO_USER(pSkillCaster)->GetClass() == (pSkill->iNum - 80000) / 1000)
		return SkillUseResult::Successful;

	// NPCs do not need most of these checks.
	if (pSkillCaster->isPlayer())
	{
		if (pSkill->sSkill != 0 && !bIsRunProc)
		{
			if (pSkillCaster->GetZoneID() != ZONE_CHAOS_DUNGEON
				&& pSkillCaster->GetZoneID() != ZONE_DUNGEON_DEFENCE
				&& pSkillCaster->GetZoneID() != ZONE_KNIGHT_ROYALE)
			{
				if (TO_USER(pSkillCaster)->m_sClass != (pSkill->sSkill / 10)
					|| pSkillCaster->GetLevel() < pSkill->sSkillLevel)
					return SkillUseResult::Failed;
			}
		}

		if (pSkillCaster->isInTempleEventZone() &&
			!g_pMain->pTempleEvent.isAttackable &&
			TO_USER(pSkillCaster)->isEventUser())
			return SkillUseResult::Failed;

		if (pSkillTarget != nullptr)
		{
			if (!pSkillTarget->isPlayer())
			{
				if (TO_NPC(pSkillTarget)->GetType() == NPC_TREE)
					return SkillUseResult::Failed;

				else if (TO_NPC(pSkillTarget)->GetType() == NPC_FOSIL)
					return SkillUseResult::Failed;

				else if (TO_NPC(pSkillTarget)->GetType() == NPC_OBJECT_FLAG
					&& TO_NPC(pSkillTarget)->GetProtoID() == 511)
					return SkillUseResult::Failed;

				else if (TO_NPC(pSkillTarget)->GetType() == NPC_REFUGEE)
					return SkillUseResult::Failed;

				else if (TO_NPC(pSkillTarget)->GetType() == NPC_PRISON)
					return SkillUseResult::Failed;

				else if (TO_NPC(pSkillTarget)->GetType() == NPC_DESTROYED_ARTIFACT
					|| TO_NPC(pSkillTarget)->isCswDoors())
				{
					_KNIGHTS_SIEGE_WARFARE* pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(Aktive);
					if (pKnightSiege == nullptr
						|| !TO_USER(pSkillCaster)->isInClan()
						|| !g_pMain->pCswEvent.Started
						|| g_pMain->pCswEvent.Status != CswOperationStatus::CastellanWar
						|| g_pMain->m_byBattleOpen != SIEGE_BATTLE)
						return SkillUseResult::Failed;

					if (pKnightSiege->sMasterKnights != 0)
					{
						if (TO_USER(pSkillCaster)->GetClanID() == pKnightSiege->sMasterKnights)
							return SkillUseResult::Failed;
					}

					_CASTELLAN_WAR_INFO* pCastellanInfo = g_pMain->m_KnightSiegeWarCastellanClanList.GetData(TO_USER(pSkillCaster)->GetClanID());
					if (pCastellanInfo == nullptr)
					{
						_CASTELLAN_OLD_CSW_WINNER* pOldWinnerInfo = g_pMain->m_KnightSiegeWarCastellanOldWinner.GetData(TO_USER(pSkillCaster)->GetClanID());
						if (pOldWinnerInfo == nullptr)
						{
							return SkillUseResult::Failed;
						}
					}
				}

				else if (TO_NPC(pSkillTarget)->GetType() == Nation::NONE &&
					TO_NPC(pSkillTarget)->GetType() == NPC_PARTNER_TYPE)
					return SkillUseResult::Failed;

				else if (TO_NPC(pSkillTarget)->GetType() == NPC_BORDER_MONUMENT)
					return SkillUseResult::Failed;

				if (pSkillCaster->GetZoneID() == ZONE_UNDER_CASTLE
					&& isUnderTheCastleDisableSkill())
					return SkillUseResult::Failed;
			}

			if (pSkillCaster->isPlayer())
			{
				if (pSkillCaster->isInTempleQuestEventZone()
					&& (!pSkillCaster->isSameEventRoom(pSkillTarget)
						&& TO_USER(pSkillCaster)->isQuestEventUser()))
					return SkillUseResult::Failed;
			}
		}

		if (TO_USER(pSkillCaster)->isTrading())
			return SkillUseResult::Failed;

		if (TO_USER(pSkillCaster)->isMerchanting())
			return SkillUseResult::Failed;

		if (TO_USER(pSkillCaster)->isStoreOpen())
			return SkillUseResult::Failed;

		if ((pSkillCaster->GetMana() - pSkill->sMsp) < 0)
			return SkillUseResult::Failed;

		if (pSkillCaster->isPlayer())
		{
			if (TO_USER(pSkillCaster)->isPortuKurian())
			{
				if ((TO_USER(pSkillCaster)->GetStamina() - pSkill->sSp) < 0)
					return SkillUseResult::Failed;
			}
		}

		// If we're in a snow war, we're only ever allowed to use the snowball skill.
		if (pSkillCaster->GetZoneID() == ZONE_SNOW_BATTLE && g_pMain->m_byBattleOpen == SNOW_BATTLE && nSkillID != SNOW_EVENT_SKILL)
			return SkillUseResult::Failed;

		// Handle death taunts (i.e. pressing the spacebar on a corpse).
		// NOTE: These skills don't really have any other generic means of identification.
		if (pSkillTarget != nullptr
			&& pSkill->bMoral == MORAL_ENEMY
			&& pSkill->bType[0] == 3
			&& pSkill->bType[1] == 0
			// Target player must be a corpse.
			&& pSkillTarget->isDead())
		{
			_MAGIC_TYPE3* pType3 = g_pMain->m_Magictype3Array.GetData(pSkill->iNum);
			if (pType3 == nullptr)
				return SkillUseResult::Failed;

			// Skill mustn't do any damage or such.
			if (pType3->bDirectType == 0
				&& pType3->sFirstDamage == 0
				&& pType3->sTimeDamage == 0)
			{
				// We also need to tweak the packet being sent.
				bOpcode = MAGIC_EFFECTING;
				sData[1] = 1;
				SendSkill();
				return SkillUseResult::NoValue;
			}
		}

		if (pSkill->bType[0] == 1
			&& pSkill->bType[1] == 0)
		{
			_MAGIC_TYPE1* pType1 = g_pMain->m_Magictype1Array.GetData(pSkill->iNum);

			if (pType1 == nullptr)
				return SkillUseResult::Failed;

			if (pSkill->sUseStanding == 0
				&& pSkill->sRange > 0
				&& pSkillCaster->GetDistanceSqrt(pSkillTarget) > float(pSkill->sRange))
				return SkillUseResult::Failed;
		}

		if (pSkill->bMoral == MORAL_PARTY
			&& pSkill->bType[0] == 3
			&& pSkill->bType[1] == 0)
		{

			if (pSkillTarget == nullptr)
				return SkillUseResult::Failed;

			_MAGIC_TYPE3* pType3 = g_pMain->m_Magictype3Array.GetData(pSkill->iNum);
			if (pType3 == nullptr)
				return SkillUseResult::Failed;

			if (pSkillTarget->GetID() == pSkillCaster->GetID())
				return SkillUseResult::Failed;

			if (pSkill->sUseStanding == 0
				&& pSkill->sRange > 0
				&& !pSkillCaster->isInRangeSlow(pSkillTarget, float(pSkill->sRange)))
				return SkillUseResult::Failed;
		}

		if (nSkillID == 490214
			&& TO_USER(pSkillCaster)->isInPVPZone()) // Abyss Fire are disabled in PVP Zones
			return SkillUseResult::Failed;

		// Familiar only use safe zone. (Zone)

		if (nSkillID == 500117 && TO_USER(pSkillCaster)->isInPetZone())
			return SkillUseResult::Failed;

		// Familiar only use this premium type's. (Premiumsuz Pet Basamaz)

		if (nSkillID == 500117 && TO_USER(pSkillCaster)->m_bPremiumInUse == NO_PREMIUM && !g_pMain->PetFree)//i add this for free pet
			return SkillUseResult::Failed;

		_MAGIC_TYPE5* pType5;

		uint8 bType = 0;
		uint16 sNeedStone = 0;

		if (pSkill->bType[0] == 5)
		{
			pType5 = g_pMain->m_Magictype5Array.GetData(pSkill->iNum);

			if (pType5)
			{
				bType = pType5->bType;
				sNeedStone = pType5->sNeedStone;
			}
		}

		// Archer & transformation skills will handle item checking themselves
		if ((pSkill->bType[0] != 2 && pSkill->bType[0] != 6)
			// The user does not meet the item's requirements or does not have any of said item.
			&& (pSkill->iUseItem != 0
				&& !TO_USER(bType == RESURRECTION ? pSkillTarget : pSkillCaster)->CanUseItem(pSkill->iUseItem, (bType == RESURRECTION ? sNeedStone : 1))))
			return SkillUseResult::Failed;

		// Some skills also require class-specific stones which are taken instead of UseItem.
		// In this case, UseItem is considered a required item and not consumed on skill use.
		if (pSkill->nBeforeAction >= ClassWarrior && pSkill->nBeforeAction <= ClassPriest)
			nConsumeItem = CLASS_STONE_BASE_ID + (pSkill->nBeforeAction * 1000);
		else if (pSkill->nBeforeAction != 0 && (pSkill->nBeforeAction == 379090000 || pSkill->nBeforeAction == 379093000))
			nConsumeItem = pSkill->iUseItem;
		else if (pSkill->nBeforeAction == 381001000)
			nConsumeItem = pSkill->nBeforeAction;
		else
			nConsumeItem = pSkill->iUseItem;

		if ((pSkill->bType[0] != 2 && pSkill->bType[0] != 6)
			// The user does not meet the item's requirements or does not have any of said item.
			&& (pSkill->iUseItem != 0
				&& !TO_USER(pSkillCaster)->CanUseItem(nConsumeItem))
			&& bType != RESURRECTION)
			return SkillUseResult::Failed;

		// For skills that are unlocked via quests, ensure the user has actually 
		// completed the quest...
		// NOTE: GMs are excluded.
		if (!TO_USER(pSkillCaster)->isGM()
			&& pSkill->sEtc != 0
			&& !TO_USER(pSkillCaster)->CheckExistEvent(pSkill->sEtc, 2))
			return SkillUseResult::Failed;

		if (bOpcode == MAGIC_CASTING
			&& pSkill->bType[0] < 4
			&& pSkillTarget != nullptr
			&& !pSkillCaster->isInAttackRange(pSkillTarget, pSkill))
			return SkillUseResult::Failed;
		else if (bOpcode == MAGIC_EFFECTING
			&& (pSkill->bType[0] == 1 || pSkill->bType[0] == 2)
			&& pSkillTarget != nullptr
			&& !pSkillCaster->isInAttackRange(pSkillTarget, pSkill))
			return SkillUseResult::Failed;

		if (pSkill->iNum == 210004
			|| pSkill->iNum == 209004
			|| pSkill->iNum == 109004
			|| pSkill->iNum == 110004) // TYPE 8 TP SKILLS [SUMMON FRIEND]
		{
			if (pSkillTarget == nullptr
				|| pSkillCaster == pSkillTarget
				|| pSkillTarget->isDead()) // cannot tp her/hisself
				return SkillUseResult::Failed;

			if (pSkillTarget->isBlinking())
				return SkillUseResult::NoFunction;

			if (pSkillTarget->isNPC())
				return SkillUseResult::Failed;

			if (!TO_USER(pSkillCaster)->isInParty())
				return SkillUseResult::Failed;

			if (TO_USER(pSkillCaster)->GetPartyID() != TO_USER(pSkillTarget)->GetPartyID())
				return SkillUseResult::Failed;
		}

		if (pSkill->iNum == 500306
			|| pSkill->iNum == 500038
			|| pSkill->iNum == 500106) // Teleport Item Skill (PUS)
		{
			if (pSkillTarget == nullptr
				|| pSkillCaster == pSkillTarget
				|| pSkillTarget->isDead()) // cannot tp her/hisself
				return SkillUseResult::Failed;

			if (pSkillTarget->isBlinking())
				return SkillUseResult::NoFunction;

			if (pSkillCaster->GetZoneID() != pSkillTarget->GetZoneID()
				|| pSkillCaster->GetNation() != pSkillTarget->GetNation()
				|| pSkillCaster->GetZoneID() == ZONE_RONARK_LAND
				|| pSkillCaster->GetZoneID() == ZONE_ARDREAM
				|| pSkillCaster->GetZoneID() == ZONE_RONARK_LAND_BASE
				|| pSkillCaster->GetZoneID() == ZONE_BIFROST)
				return SkillUseResult::Failed;

			if (pSkillTarget->isNPC())
				return SkillUseResult::Failed;
		}

		if (pSkill->iNum == 490301
			|| pSkill->iNum == 490302
			|| pSkill->iNum == 490303
			|| pSkill->iNum == 490304
			|| pSkill->iNum == 490305
			|| pSkill->iNum == 490306
			|| pSkill->iNum == 490307) // Bifrost Warp Taþý
		{
			if (pSkillCaster->isDead()) // cannot tp her/hisself
				return SkillUseResult::Failed;

			if (pSkillTarget->isBlinking())
				return SkillUseResult::NoFunction;

			if (pSkillCaster->GetZoneID() != ZONE_BIFROST
				&& pSkillCaster->GetZoneID() != ZONE_BATTLE2)
				return SkillUseResult::Failed;

			if (!pSkillCaster->isPlayer())
				return SkillUseResult::Failed;

		}

		if (pSkill->iNum == 610090
			|| pSkill->iNum == 610091
			|| pSkill->iNum == 610100
			|| pSkill->iNum == 610124) // Casusluk Görevi
		{

			if (pSkillCaster == pSkillTarget
				|| pSkillCaster->isDead()) // cannot tp her/hisself
				return SkillUseResult::Failed;

			if (pSkillTarget->isBlinking())
				return SkillUseResult::NoFunction;

			if (pSkillCaster->GetNation() == KARUS && pSkillCaster->GetZoneID() != ZONE_KARUS)
				return SkillUseResult::Failed;
			else if (pSkillCaster->GetNation() == ELMORAD && pSkillCaster->GetZoneID() != ZONE_ELMORAD)
				return SkillUseResult::Failed;

			if (pSkillCaster->GetLevel() < 60)
				return SkillUseResult::Failed;

			if (!pSkillCaster->isPlayer())
				return SkillUseResult::Failed;
		}

		_MAGIC_TYPE6* pType6;

		if (pSkill->bType[0] == 6)
		{
			pType6 = g_pMain->m_Magictype6Array.GetData(pSkill->iNum);

			if (pType6 == nullptr)
				return SkillUseResult::Failed;

			if (pType6->bUserSkillUse == TransformationSkillUseMonster)
			{
				if (!pSkillCaster->isTransformationMonsterInZone() || pSkillCaster->isBuffed(true))
					return SkillUseResult::Failed;
			}

			if (pType6->bUserSkillUse == TransformationSkillUseSiege)
			{
				if (pSkillCaster->isBuffed(true))
					return SkillUseResult::Failed;

				if (pSkillCaster->GetZoneID() != ZONE_DELOS)
					return SkillUseResult::Failed;

				if (!g_pMain->pCswEvent.Started
					|| g_pMain->pCswEvent.Status != CswOperationStatus::CastellanWar)
					return SkillUseResult::Failed;
			}

			if (pType6->bUserSkillUse == TransformationSkillMovingTower)
			{
				if (!g_pMain->pCswEvent.Started
					|| g_pMain->pCswEvent.Status != CswOperationStatus::CastellanWar)
					return SkillUseResult::Failed;

				if (pSkillCaster->isBuffed(true) || pSkillCaster->GetZoneID() != ZONE_DELOS)
					return SkillUseResult::Failed;
			}
		}

		_MAGIC_TYPE8* pType8;
		if (pSkill->bType[0] == 8)
		{
			pType8 = g_pMain->m_Magictype8Array.GetData(pSkill->iNum);
			if (pType8 == nullptr)
				return SkillUseResult::Failed;

			if (pSkillTarget != nullptr)
			{
				if (pSkillCaster->isPlayer())
				{
					if (pType8->bWarpType == 25)
					{
						if (pSkillTarget == pSkillCaster)
							return SkillUseResult::Failed;
					}

					if (pType8->sRadius > 0)
					{
						if (pType8->bWarpType == 25)
						{
							if (pSkillCaster->GetDistanceSqrt(pSkillTarget) > (float)pType8->sRadius / 2)
								return SkillUseResult::Failed;
						}
						else
						{
							if (pSkillCaster->GetDistanceSqrt(pSkillTarget) > (float)pType8->sRadius)
								return SkillUseResult::Failed;
						}
					}
					else
					{
						if (pType8->bWarpType == 30 || pType8->bWarpType == 31)
							return SkillUseResult::Failed;

						if (pType8->iNum == 114509
							|| pType8->iNum == 115509
							|| pType8->iNum == 214509
							|| pType8->iNum == 215509
							|| pType8->iNum == 115570
							|| pType8->iNum == 215570)
						{
							if (pSkillCaster->GetDistanceSqrt(pSkillTarget) > (float)pSkill->sRange + 5)
								return SkillUseResult::Failed;
						}
						else
						{
							if (pSkillCaster->GetDistanceSqrt(pSkillTarget) > (float)pSkill->sRange)
								return SkillUseResult::Failed;
						}
					}
				}
			}
		}

		_MAGIC_TYPE9* pType9;

		if (pSkill->bType[0] == 9)
		{
			pType9 = g_pMain->m_Magictype9Array.GetData(pSkill->iNum);
			if (pType9 == nullptr)
				return SkillUseResult::Failed;

			if (pType9->bStateChange <= 2
				&& !pSkillCaster->canStealth()
				&& pSkillCaster->GetZoneID() != ZONE_LOST_TEMPLE
				&& pSkillCaster->GetZoneID() != ZONE_FORGOTTEN_TEMPLE)
			{
				return SkillUseResult::Failed;
			}
			if (pSkillCaster->isPlayer())
			{
				Guard lock(pSkillCaster->m_buffLock);
				Type9BuffMap& buffMap = pSkillCaster->m_type9BuffMap;

				// Ensure this type of skill isn't already in use.
				if (buffMap.find(pType9->bStateChange) != buffMap.end() && nSkillID != 208680 && nSkillID != 108680)
				{
					return SkillUseResult::Failed;
				}
			}
		}
	}

	if (pSkillCaster->isNPC())
	{
		if (TO_NPC(pSkillCaster)->isPet())
		{
			_MAGIC_TYPE3* pType3;

			pType3 = g_pMain->m_Magictype3Array.GetData(pSkill->iNum);
			if (pType3 == nullptr && pSkill->bMoral == 1)
				return SkillUseResult::Failed;

			if (pSkill->bType[0] == 3
				&& (pType3->bDirectType == 1 || pType3->bDirectType == 2)
				&& pSkill->iUseItem != 0)
			{

				CUser* pUser = g_pMain->GetUserPtr(pSkillCaster->m_oSocketID);
				if (pUser != nullptr)
				{
					// The user does not meet the item's requirements or does not have any of said item.
					if (!pUser->CanUseItem(pSkill->iUseItem))
						return SkillUseResult::Failed;

					pUser->RobItem(pSkill->iUseItem);
				}
			}
		}

		if ((pSkillCaster->GetMana() - pSkill->sMsp) < 0)
			return SkillUseResult::Failed;

		if ((pSkill->bType[0] == 7 && pSkill->bType[1] == 0)
			|| (pSkill->bType[0] == 4 && pSkill->bType[1] == 0))
			pSkillCaster->MSpChange(-(pSkill->sMsp));
	}

	if ((bOpcode == MAGIC_EFFECTING || bOpcode == MAGIC_CASTING) && !IsAvailable() && !bIsRunProc)
		return SkillUseResult::Failed;

	// Instant casting affects the next cast skill only, and is then removed.
	if (bOpcode == MAGIC_EFFECTING && pSkillCaster->canInstantCast())
		bInstantCast = true;

	// In case we made it to here, we can cast! Hurray!
	return SkillUseResult::Successful;
}

bool MagicInstance::IsAvailable()
{
	CUser* pParty = nullptr;
	int modulator = 0, Class = 0, skill_mod = 0;

	if (pSkill == nullptr)
		goto fail_return;

	switch (pSkill->bMoral)
	{
		// Enforce self-casting skills *unless* we're an NPC.
		// Quest NPCs, naturally, use skills set as self-buffs.
	case MORAL_SELF:
		if (pSkillCaster->m_bReflectArmorType > 0)
		{

		}
		else if (nSkillID == 492032)
		{
			if (!pSkillCaster->isPlayer())
				goto fail_return;
		}
		else
			if (pSkillCaster->isPlayer() && pSkillCaster != pSkillTarget)
				goto fail_return;
		break;

	case MORAL_FRIEND_WITHME:
		if (pSkillTarget != pSkillCaster)
		{
			if (pSkillCaster->isMoral2Checking(pSkillTarget, pSkill))
			{
				pSkillTarget = pSkillCaster;
				sTargetID = pSkillCaster->GetID();
			}
		}
		break;

	case MORAL_FRIEND_EXCEPTME:
		if (pSkillCaster == pSkillTarget || pSkillCaster->isHostileTo(pSkillTarget))
			goto fail_return;
		break;

	case MORAL_PARTY:
	{
		// NPCs can't *be* in parties.
		if (pSkillCaster->isNPC()
			|| (pSkillTarget != nullptr && pSkillTarget->isNPC()))
			goto fail_return;

		// We're definitely a user, so...
		CUser* pCaster = TO_USER(pSkillCaster);

		// we cannot use party skills if we are not in a party unless it is a buff skill.
		if (pSkill->bType[0] != 4)
		{
			if (pSkillTarget == nullptr)
				return false;

			// Ensure that there is a target, they're in the same party.
			if (!pCaster->isInParty()
				|| !TO_USER(pSkillTarget)->isInParty()
				|| pSkillTarget == pCaster
				|| TO_USER(pSkillTarget)->GetPartyID() != pCaster->GetPartyID())
				return false;
		}
		else
		{
			// If the caster's not in a party, make sure the target's not someone other than themselves.
			if ((!pCaster->isInParty() && pSkillCaster != pSkillTarget)
				// Also ensure that if there is a target, they're in the same party.
				|| (pSkillTarget != nullptr &&
					TO_USER(pSkillTarget)->isInParty() &&
					TO_USER(pSkillTarget)->GetPartyID() != pCaster->GetPartyID()))
				goto fail_return;
		}
	} break;

	case MORAL_NPC:
		if (pSkillTarget == nullptr
			|| !pSkillCaster->isHostileTo(pSkillTarget))
			goto fail_return;

		if (nSkillID == 490214)
		{
			if (pSkillTarget == nullptr
				|| !pSkillTarget->isNPC()
				|| pSkillCaster->isHostileTo(pSkillTarget)
				&& pSkill->iUseItem != 379105000)
				goto fail_return;
		}
		break;
	case MORAL_ENEMY:
		// Allow for archery skills with no defined targets (e.g. an arrow from 'multiple shot' not hitting any targets). 
		// These should be ignored, not fail.
		if (pSkillTarget != nullptr
			// Nation alone cannot dictate whether a unit can attack another.
				// As such, we must check behaviour specific to these entities.
					// For example: same nation players attacking each other in an arena.
			&& !pSkillCaster->isHostileTo(pSkillTarget))
			goto fail_return;
		break;

	case MORAL_CORPSE_FRIEND:
		// We need to revive *something*.
		if (pSkillTarget == nullptr
			// Are we allowed to revive this person?
			|| pSkillCaster->isHostileTo(pSkillTarget)
			// We cannot revive ourselves.
			|| pSkillCaster == pSkillTarget
			// We can't revive living targets.
			|| pSkillTarget->isAlive())
			goto fail_return;
		break;

	case MORAL_CLAN:
	{
		// NPCs cannot be in clans.
		if (pSkillCaster->isNPC()
			|| (pSkillTarget != nullptr && pSkillTarget->isNPC()))
			goto fail_return;

		// We're definitely a user, so....
		CUser* pCaster = TO_USER(pSkillCaster);

		// If the caster's not in a clan, make sure the target's not someone other than themselves.
		if ((!pCaster->isInClan() && pSkillCaster != pSkillTarget)
			// If we're targeting someone, that target must be in our clan.
			|| (pSkillTarget != nullptr
				&& TO_USER(pSkillTarget)->GetClanID() != pCaster->GetClanID()))
			goto fail_return;
	} break;

	case MORAL_SIEGE_WEAPON:
		if (pSkillCaster->isNPC()
			|| (pSkillCaster->isPlayer()
				&& !TO_USER(pSkillCaster)->isSiegeTransformation()))
			goto fail_return;
		break;

	case MORAL_DRAWBRIDGE:
		if (!pSkillTarget->isNPC() || (pSkillCaster->isPlayer() && !TO_USER(pSkillCaster)->isSiegeTransformation()))
			goto fail_return;
		break;
	}

	// Check skill prerequisites
	for (int i = 0; i < 2; i++)
	{
		switch (pSkill->bType[i])
		{
		case 3:
			if (!CheckType3Prerequisites())
				return false;
			break;

		case 4:
			if (!CheckType4Prerequisites())
				return false;
			break;

		case 6:
			if (!CheckType6Prerequisites())
				return false;
			break;
		}
	}

	// This only applies to users casting skills. NPCs are fine and dandy (we can trust THEM at least).
	if (pSkillCaster->isPlayer())
	{
		if (pSkill->bType[0] == 3)
		{
			_MAGIC_TYPE3* pType3 = g_pMain->m_Magictype3Array.GetData(pSkill->iNum);
			_ITEM_TABLE* pTable;
			if (pType3 == nullptr)
				goto fail_return;

			// Allow for skills that block potion use.
			if (!pSkillCaster->canUsePotions()
				&& pType3->bDirectType == 1		// affects target's HP (magic numbers! yay!)
				&& pType3->sFirstDamage > 0		// healing only
				&& pSkill->iUseItem != 0		// requiring an item (i.e. pots) 
				// To avoid conflicting with priest skills that require items (e.g. "Laying of hands")
				// we need to lookup the item itself for the information we need to ignore it.
				&& (pTable = g_pMain->GetItemPtr(pSkill->iUseItem)) != nullptr
				// Item-required healing skills are class-specific. 
				// We DO NOT want to block these skills.
				&& pTable->m_bClass == 0)
				goto fail_return;
		}

		modulator = pSkill->sSkill % 10;     // Hacking prevention!
		if (modulator != 0) {

			if (TO_USER(pSkillCaster)->GetClass() == (pSkill->iNum - 80000) / 1000)
				return true;

			Class = pSkill->sSkill / 10;
			if (Class != TO_USER(pSkillCaster)->GetClass())
				goto fail_return;

			if ((pSkill->sSkillLevel > TO_USER(pSkillCaster)->m_bstrSkill[modulator]) && TO_USER(pSkillCaster)->GetFame() != COMMAND_CAPTAIN) goto fail_return;
		}

		else if (pSkill->sSkillLevel > pSkillCaster->GetLevel()
			&& pSkill->sSkillLevel != 30010
			&& pSkill->sSkillLevel != 30050)
			goto fail_return;

		if (pSkill->bType[0] == 1)
		{
			// Weapons verification in case of COMBO attack (another hacking prevention).
			if (pSkill->sSkill == 1055 || pSkill->sSkill == 2055) // Weapons verification in case of dual wielding attacks !
			{
				if (TO_USER(pSkillCaster)->isWeaponsDisabled())
					return false;

				_ITEM_TABLE* pLeftHand = TO_USER(pSkillCaster)->GetItemPrototype(TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND);
				_ITEM_TABLE* pRightHand = TO_USER(pSkillCaster)->GetItemPrototype(TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND);

				if (pLeftHand != nullptr)
				{
					if (!pLeftHand->isSword()
						&& !pLeftHand->isAxe()
						&& !pLeftHand->isMace()
						&& !pLeftHand->isSpear()
						&& !pLeftHand->isClub()
						&& !pLeftHand->isMace()
						&& !pLeftHand->isShield())
						return false;
				}

				if (pRightHand != nullptr)
				{
					if (!pRightHand->isSword()
						&& !pRightHand->isAxe()
						&& !pRightHand->isMace()
						&& !pRightHand->isSpear()
						&& !pRightHand->isClub()
						&& !pRightHand->isMace()
						&& !pRightHand->isShield())
						return false;
				}
			}
			else if (pSkill->sSkill == 1056 || pSkill->sSkill == 2056)
			{	// Weapons verification in case of 2 handed attacks !
				if (TO_USER(pSkillCaster)->isWeaponsDisabled())
					return false;

				_ITEM_TABLE* pRightHand = TO_USER(pSkillCaster)->GetItemPrototype(TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND);

				if (TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE)
				{
					if (TO_USER(pSkillCaster)->GetKnightRoyaleItem(LEFTHAND)->nNum != 0)
						return false;
				}
				else
				{
					if (TO_USER(pSkillCaster)->GetItem(KNIGHT_ROYAL_LEFTHAND)->nNum != 0)
						return false;
				}

				if (pRightHand != nullptr)
				{
					if (!pRightHand->isSword()
						&& !pRightHand->isAxe()
						&& !pRightHand->isMace()
						&& !pRightHand->isSpear()
						&& !pRightHand->isClub()
						&& !pRightHand->isMace()
						&& !pRightHand->isShield())
						return false;
				}
			}
		}

		// Handle MP/HP/item loss.
		if (bOpcode == MAGIC_EFFECTING)
		{
			CUser* pCaster = TO_USER(pSkillCaster);

			if (pCaster == nullptr)
				return false;

			int total_hit = pSkillCaster->m_sTotalHit;

			if (pSkill->bType[0] == 2
				&& pSkill->bFlyingEffect != 0) // Type 2 related...
				return true;		// Do not reduce MP/SP when flying effect is not 0.

			if (pSkill->sMsp > pSkillCaster->GetMana())
				goto fail_return;


			if (TO_USER(pSkillCaster)->isPortuKurian())
			{
				if (pSkill->sSp > TO_USER(pSkillCaster)->GetStamina())
					goto fail_return;
			}


			// If the PLAYER uses an item to cast a spell.
			if (pSkillCaster->isPlayer() && (pSkill->bType[0] == 3 || pSkill->bType[0] == 4))
			{
				if (pSkill->iUseItem != 0) {
					_ITEM_TABLE* pItem = nullptr;				// This checks if such an item exists.
					pItem = g_pMain->GetItemPtr(pSkill->iUseItem);
					if (!pItem) return false;

					if ((pItem->m_bClass != 0 && !TO_USER(pSkillCaster)->JobGroupCheck(pItem->m_bClass))
						|| (pItem->m_bReqLevel != 0 && TO_USER(pSkillCaster)->GetLevel() < pItem->m_bReqLevel))
						return false;
				}
			}



			/*
						if ((pSkill->bType[0] != 4
							|| (pSkill->bType[0] == 4 && sTargetID == -1))
							&& !pSkillCaster->isBlinking() && pSkill->sMsp != 0)*/
			if (!pSkillCaster->isBlinking())
			{
				CUser* pSkilled = TO_USER(pSkillCaster);

				if (pSkilled->isDevil()
					&& pSkilled->CheckSkillPoint(PRO_SKILL3, 45, 83))
				{
					int16 nDivide = pSkill->sMsp / 2;
					pSkillCaster->MSpChange(-(nDivide));
				}
				else
					pSkillCaster->MSpChange(-(pSkill->sMsp));

				if (pCaster != nullptr)
				{
					if (pCaster->isPortuKurian())
						pCaster->SpChange(-(pSkill->sSp));
				}
			}


			// Skills that require HP rather than MP.
			if (pSkill->sHP > 0
				&& pSkill->sMsp == 0
				&& pSkill->sHP < 10000) // Hack (used officially) to allow for skills like "Sacrifice"
			{
				if (pSkill->sHP > pSkillCaster->GetHealth())
					goto fail_return;

				pSkillCaster->HpChange(-pSkill->sHP);
			}

			// Support skills like "Sacrifice", that sacrifice your HP for another's.
			if (pSkill->sHP >= 10000)
			{
				// Can't cast this on ourself.
				if (pSkillCaster == pSkillTarget)
					return false;

				// Take 10,000 HP from the caster (note: DB is set to 10,0001 but server will always take 10,000...)
				pSkillCaster->HpChange(-10000);
			}
		}
	}
	return true;

fail_return:
	SendSkillFailed();
	return false;
}



/**
* @brief	Checks primary type 3 (DOT/HOT) skill prerequisites before executing the skill.
*
* @return	true if it succeeds, false if it fails.
*/
bool MagicInstance::CheckType3Prerequisites()
{
	_MAGIC_TYPE3* pType = g_pMain->m_Magictype3Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	// Handle AOE prerequisites
	if (sTargetID == -1)
	{
		// No need to handle any prerequisite logic for NPCs/mobs casting AOEs.
		if (!pSkillCaster->isPlayer())
			return true;

		if (pSkill->bMoral != MORAL_PARTY_ALL && pSkill->bMoral != MORAL_PARTY)
		{
			// Player can attack other players in the safety area.
			if (TO_USER(pSkillCaster)->isInEnemySafetyArea())
				return false;
		}

		if (pSkill->bMoral == MORAL_PARTY_ALL && pType->sTimeDamage > 0)
		{
			// Players may not cast group healing spells whilst transformed
			// into a monster (skills with IDs of 45###). 
			if (TO_USER(pSkillCaster)->isMonsterTransformation())
				return false;

			// Official behaviour means we cannot cast a group healing spell
			// if we currently have an active restoration spells on us.
			// This behaviour seems fairly illogical, but it's how it works.
			for (int i = 0; i < MAX_TYPE3_REPEAT; i++)
			{
				if (pSkillCaster->m_durationalSkills[i].m_sHPAmount > 0)
					return false;
			}
		}

		// No other reason to reject AOE spells.
		return true;
	}
	// Handle prerequisites for skills cast on NPCs.
	else if (sTargetID >= NPC_BAND)
	{
		if (pSkillTarget == nullptr)
			return false;

		// Unless the zone is Delos, or it's a healing skill, we can continue on our merry way.
		if (pSkillCaster->GetZoneID() != 30 || (pType->sFirstDamage <= 0 && pType->sTimeDamage <= 0))
			return true;

		// We cannot heal gates! That would be bad, very bad.
		if (TO_NPC(pSkillTarget)->GetType() == NPC_GATE) // note: official only checks byType 50
			return false;

		// Otherwise, officially there's no reason we can't heal NPCs (more specific logic later).
		return true;
	}
	// Handle prerequisites for skills cast on players.
	else
	{
		// We only care about friendly non-AOE spells.
		if (pSkill->bMoral > MORAL_PARTY)
			return true;

		if (pSkillTarget == nullptr
			|| !pSkillTarget->isPlayer()
			|| pSkillTarget->isDead())
			return false;

		// If the spell is a healing/restoration spell...
		if (pType->sTimeDamage > 0)
		{
			// Official behaviour means we cannot cast a restoration spell
			// if the target currently has an active restoration spell on them.
			for (int i = 0; i < MAX_TYPE3_REPEAT; i++)
			{
				if (pSkillTarget->m_durationalSkills[i].m_sHPAmount > 0)
					return false;
			}
		}

		// It appears that the server should reject any attacks or heals
		// on players that have transformed into monsters.
		if (TO_USER(pSkillTarget)->isSiegeTransformation()
			&& !pSkillCaster->CanAttack(pSkillTarget))
			return false;

		return true;
	}
}

/**
* @brief	Checks primary type 4 (buff/debuff) skill prerequisites before executing the skill.
*
* @return	true if it succeeds, false if it fails.
*/
bool MagicInstance::CheckType4Prerequisites()
{
	_MAGIC_TYPE4* pType = g_pMain->m_Magictype4Array.GetData(nSkillID);

	// Certain transformation (type 6) skills state they have an associated
	// type 4 skill but do not have any entry in the table. Consider these OK.
	if (pType == nullptr)
		return (pSkill->bType[0] == 6);

	if (pType->bBuffType == BUFF_TYPE_FISHING)
	{
		if (pSkillCaster == nullptr || !pSkillCaster->isPlayer())
			return false;

		if (pType->sSpecialAmount == 1 && TO_USER(pSkillCaster)->GetPremium() == 10)
		{
			if (TO_USER(pSkillCaster)->m_FlashDcBonus >= 100)
				return false;

			return true;
		}
		else if (pType->sSpecialAmount == 2 && TO_USER(pSkillCaster)->GetPremium() == 11)
		{
			if (TO_USER(pSkillCaster)->m_FlashExpBonus >= 100)
				return false;

			return true;
		}
		else if (pType->sSpecialAmount == 3 && TO_USER(pSkillCaster)->GetPremium() == 12)
		{
			if (TO_USER(pSkillCaster)->m_FlashWarBonus >= 10)
				return false;

			return true;
		}
		return false;
	}

	if ((sTargetID < 0 || sTargetID >= MAX_USER))
	{
		if (sTargetID < NPC_BAND // i.e. it's an AOE
			|| pType->bBuffType != BUFF_TYPE_HP_MP // doesn't matter who we heal.
			|| pType->sMaxHPPct != 99) // e.g. skills like Dispel Magic / Sweet Kiss
			return true;

		return false;
	}

	// At this point, it can only be a user -- so ensure the ID was invalid (this was looked up earlier).
	if (pSkillTarget == nullptr
		|| !pSkillTarget->isPlayer())
		return false;

	if (TO_USER(pSkillTarget)->isTransformed())
	{
		// Can't use buff scrolls/pots when transformed into anything but NPCs.
		if (!TO_USER(pSkillTarget)->isNPCTransformation()
			&& (nSkillID >= 500000
				// Can't use bezoars etc when transformed 
				// (this should really be a whitelist, but this is blocked explicitly in 1.298)
				|| pType->bBuffType == BUFF_TYPE_SIZE))
		{
			SendSkillFailed();
			return false;
		}
	}

	// If the specified target already has this buff (debuffs are required to reset)
	// we should fail this skill. 
	// NOTE: AOE buffs are exempt.
	if (pType->isBuff())
	{
		Guard lock(pSkillTarget->m_buffLock);
		if (pSkillTarget->m_buffMap.find(pType->bBuffType) != pSkillTarget->m_buffMap.end())
			return false;

		if (pSkillTarget->isPlayer())
		{
			if (pType->bBuffType == BUFF_TYPE_SPEED)
			{
				if (pSkillTarget->m_buffMap.find(BUFF_TYPE_SPEED2) != pSkillTarget->m_buffMap.end())
					return false;
			}
		}
	}
	// TODO: Allow for siege-weapon only buffs (e.g. "Physical Attack Scroll")
	return true;
}

bool MagicInstance::CheckType6Prerequisites()
{
	if (!pSkillCaster->isPlayer())
		return true;

	_MAGIC_TYPE6* pType = g_pMain->m_Magictype6Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	CUser* pCaster = TO_USER(pSkillCaster);
	switch (pType->bUserSkillUse)
	{
		// For monster transformations (TransformationSkillUseMonster), nBeforeAction is set to the item 
		// used for showing the transformation list & UseItem is the consumable item.
	case TransformationSkillUseMonster:
		// Ensure they have the item for showing the transformation list
		if ((!pCaster->CanUseItem(pSkill->nBeforeAction)
			// Ensure they have the required item for the skill.
			|| !pCaster->CanUseItem(pSkill->iUseItem)) && !pCaster->CheckExistItem(381001000))
			return false;

		break;
	case TransformationSkillOreadsGuard:
		if (!pCaster->isPlayer())
			return false;
		break;
		// For all other transformations, all we care about is UseItem (BeforeAction is set to 0 in these cases).
	default:
		// Ensure they have the item for showing the transformation list
		if (!pCaster->CanUseItem(pSkill->iUseItem))
			return false;
		break;
	}

	// Perform class check, if applicable.
	bool bAllowedClass = (pType->sClass == 0);
	if (bAllowedClass)
		return true;

	// NOTE: sClass is a 4 digit number (e.g. 1111) with a digit per class 
	// in the order warrior/rogue/mage/priest with '1' representing 'allowed' & 
	// anything else as forbidden.
	switch (pCaster->GetBaseClassType())
	{
	case ClassWarrior:
	case ClassWarriorNovice:
	case ClassWarriorMaster:
		bAllowedClass = ((pType->sClass / 1000)) == 1;
		break;

	case ClassRogue:
	case ClassRogueNovice:
	case ClassRogueMaster:
		bAllowedClass = ((pType->sClass % 1000) / 100) == 1;
		break;

	case ClassMage:
	case ClassMageNovice:
	case ClassMageMaster:
		bAllowedClass = (((pType->sClass % 1000) % 100) / 10) == 1;
		break;

	case ClassPriest:
	case ClassPriestNovice:
	case ClassPriestMaster:
		bAllowedClass = (((pType->sClass % 1000) % 100) % 10) == 1;
		break;

	case ClassPortuKurian:
	case ClassPortuKurianNovice:
	case ClassPortuKurianMaster:
		bAllowedClass = (((pType->sClass % 1000) % 100) % 10) == 1;
		break;
	}

	return bAllowedClass;
}