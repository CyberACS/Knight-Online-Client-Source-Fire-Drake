#include "stdafx.h"
#include "Map.h"
#include "../shared/KOSocketMgr.h"
#include "MagicProcess.h"
#include "MagicInstance.h"

using std::string;
using std::vector;

void MagicInstance::Run()
{
	bool isBot = false;
	CBot* botCheck = nullptr;


	if (pSkill == nullptr)
		pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);

	if (pSkillCaster == nullptr)
		pSkillCaster = g_pMain->GetUnitPtr(sCasterID, sSkillCasterZoneID);

	botCheck = g_pMain->m_BotArray.GetData(sCasterID);
	if (botCheck != nullptr)
	{
		isBot = true;
	}
		


	if (pSkillCaster != nullptr && pSkill != nullptr && pSkillCaster->isPlayer() && !ArmorDefens())
	{
		if (pSkillCaster->isPlayer())
		{
			switch (pSkill->bMoral)
			{
			case MORAL_SELF:
				if (pSkillCaster->isPlayer())
				{
					
					pSkillTarget = pSkillCaster;
					sTargetID = pSkillCaster->GetID();
				}
				break;

			case MORAL_FRIEND_WITHME:
				if (sTargetID != -1)
				{
					Unit* pTarget = g_pMain->GetUnitPtr(sTargetID, sSkillCasterZoneID);
					if (pTarget != nullptr && pTarget->isNPC())
					{
						CNpc* pNpcInfo = g_pMain->GetNpcPtr(sTargetID, sSkillCasterZoneID);
						if (pNpcInfo != nullptr && !pTarget->isPlayer())
						{
							if (!pNpcInfo->isMonster() && pTarget != pSkillCaster)
							{
								pSkillTarget = pSkillCaster;
								if (!TO_NPC(pTarget)->isPet())
									sTargetID = pSkillCaster->GetID();
							}
						}
					}
				}
				break;
			}
		}
	}


	if (sTargetID != -1 && pSkillTarget == nullptr)
		pSkillTarget = g_pMain->GetUnitPtr(sTargetID, sSkillCasterZoneID);

	if (pSkill == nullptr || pSkillCaster == nullptr)
	{
		SendSkillFailed();
		return;
	}

	if (pSkill && pSkillCaster->isPlayer())
	{
		CBot* pUserBot = nullptr;
		pUserBot = g_pMain->m_BotArray.GetData(sCasterID);
		if (pUserBot != nullptr) // bot ise
		{
			
			if (pUserBot == nullptr )
			{
				CUser* pUser = g_pMain->GetUserPtr(sCasterID);

				if (pUser == nullptr)
					return;

				if (pUser->isGM())
					g_pMain->SendHelpDescription(pUser, string_format("[Skill] ID -> : %d || [Successrate] -> : %d || [Type] -> : %d", pSkill->iNum, pSkill->bSuccessRate, pSkill->bType[0]));

			}

		}
		else
		{
			CUser* pUser = g_pMain->GetUserPtr(sCasterID);

			if (pUser == nullptr)
				return;

			if (pUser->isGM())
				g_pMain->SendHelpDescription(pUser, string_format("[Skill] ID -> : %d || [Successrate] -> : %d || [Type] -> : %d", pSkill->iNum, pSkill->bSuccessRate, pSkill->bType[0]));

		}
	}

	SkillUseResult CanCast, CheckSkill;
	CheckSkill = CheckSkillPrerequisites();

	


	switch (CheckSkill)
	{
	case Successful:
		break;
	case Failed:
	{
		SendSkillFailed();
		printf("CheckSkillPrerequisites %d\n ", CheckSkillPrerequisites());
		return;
	}
	break;
	case NoValue:
		return;
		break;
	case NoFunction:
	{
		SendSkillNoFunction();
		printf("CheckSkillPrerequisites %d\n ", CheckSkillPrerequisites());
		return;
	}
	break;
	}

	CanCast = UserCanCast();
	switch (CanCast)
	{
	case Successful:
		break;
	case Failed:
	{
		SendSkillFailed();
		printf("UserCanCast %d \n ", UserCanCast());
		return;
	}
	break;
	case NoValue:
		return;
		break;
	case NoFunction:
	{
		SendSkillNoFunction();
		printf("UserCanCast %d \n ", UserCanCast());
		return;
	}
	break;
	}


	bool bInitialResult;
	switch (bOpcode)
	{
	case MAGIC_CASTING:
		SendSkill(bOpcode == MAGIC_CASTING); // only send casting packets to the region, not fail packets.
		break;
	case MAGIC_FAIL:
		SendSkill(bOpcode == MAGIC_FAIL); // only send casting packets to the region, not fail packets.
		break;

	case MAGIC_FLYING:
	{
		// Handle arrow & mana checking/removals.
		if (pSkillCaster->isPlayer())
		{
			CUser* pCaster = TO_USER(pSkillCaster);
			_MAGIC_TYPE2* pType = g_pMain->m_Magictype2Array.GetData(nSkillID);

			// NOTE: Not all skills that use MAGIC_FLYING are type 2 skills.
			// Some type 3 skills use it (such as "Vampiric Fire"). 
			// For these we should really apply the same flying logic, but for now we'll just ignore it.
			if (pType != nullptr)
			{
				// Throwing knives are differentiated by the fact "NeedArrow" is set to 0.
				// We still need to check for & take 1 throwing knife in this case however.
				uint8 bCount = pType->bNeedArrow;

				if (!bCount)
					bCount = 1;
				if (pType == nullptr
					// The user does not have enough arrows! We should point them in the right direction. ;)
					|| (!pCaster->CheckExistItem(pSkill->iUseItem, bCount))
					// Ensure user has enough mana for this skill
					|| pSkill->sMsp > pSkillCaster->GetMana()
					/*|| pSkill->sSp > pCaster->GetStamina()*/)
				{
					SendSkillFailed();
					return;
				}

				// Add all flying arrow instances to the user's list for hit detection
				Guard lock(pCaster->m_arrowLock);
				for (size_t i = 0; i < bCount; i++)
					pCaster->m_flyingArrows.push_back(Arrow(pType->iNum, UNIXTIME2));

				// Remove the arrows
				pCaster->RobItem(pSkill->iUseItem, bCount);
			}
			// for non-type 2 skills, ensure we check the user's mana.
			else if (pSkill->sMsp > pSkillCaster->GetMana())
			{
				SendSkillFailed();
				return;
			}

			// Take the required mana for this skill
			if (!pCaster->isBlinking())
				pCaster->MSpChange(-(pSkill->sMsp));

		}
		SendSkill(true); // send this to the region
	} break;

	case MAGIC_EFFECTING:
		// Hacky check for a transformation item (Disguise Totem, Disguise Scroll)
		// These apply when first type's set to 0, second type's set and obviously, there's a consumable item.
		// Need to find a better way of handling this.
		if (!bIsRecastingSavedMagic
			&& (pSkill->bType[0] == 0 && pSkill->bType[1] != 0 && pSkill->iUseItem != 0
				&& (pSkillCaster->isPlayer() && TO_USER(pSkillCaster)->CheckExistItem(pSkill->iUseItem)))
			&& !TO_USER(pSkillCaster)->isInPKZone())
		{
			SendTransformationList();
			return;
		}

		bInitialResult = ExecuteSkill(pSkill->bType[0]);

		if (bInitialResult)
		{
			if (pSkillCaster->isPlayer() && !isBot)
			{
				CUser* pCaster = TO_USER(pSkillCaster);
				if (nSkillID == 110820 || nSkillID == 210820)
				{
					pCaster->m_CoolDownList.insert(std::make_pair(nSkillID, UNIXTIME2));
				}
				else if (!pSkillCaster->hasBuff(BUFF_TYPE_INSTANT_MAGIC))
				{
					/*Recasting time all skill*/
					pCaster->m_CoolDownList.insert(std::make_pair(nSkillID, UNIXTIME2));

					/*arrow casting time*/
					pCaster->m_SkillCastimeList.insert(std::make_pair(nSkillID, UNIXTIME2));

					if (pSkill->bType[0] != 0 && pSkill->iNum < 490000)
						pCaster->m_MagicTypeCooldownList.insert(std::make_pair(pSkill->bType[0], UNIXTIME2));

					if (pSkill->bType[1] != 0 && pSkill->iNum < 490000)
						pCaster->m_MagicTypeCooldownList.insert(std::make_pair(pSkill->bType[1], UNIXTIME2));
				}
			}

			ExecuteSkill(pSkill->bType[1]);

			if (pSkill->bType[0] != 2)
				ConsumeItem();
		}
		break;

	case MAGIC_CANCEL:
	case MAGIC_CANCEL2:
		Type3Cancel();	//Damage over Time skills.
		Type4Cancel();	//Buffs
		Type6Cancel();	//Transformations
		Type9Cancel();	//Stealth, lupine etc.
		break;

	case MAGIC_TYPE4_EXTEND:
		Type4Extend();
		break;

	default:
		printf("MagicInstance Unhandled packets %d \n", bOpcode);
		TRACE("MagicInstance Unhandled packets %d \n", bOpcode);
		break;
	}
}

void MagicInstance::RunBot()
{
	
	CBot* pPriest = nullptr;
	pPriest = g_pMain->m_BotArray.GetData(sCasterID);

	if (pSkill == nullptr)
		pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);

	if (pSkillCaster == nullptr)
		pSkillCaster = g_pMain->GetUnitPtr(pPriest->GetID(), sSkillCasterZoneID);

	

	

	if (pPriest != nullptr && pSkill != nullptr)
	{
		
			switch (pSkill->bMoral)
			{
			case MORAL_FRIEND_WITHME:
				if (sTargetID != -1)
				{
					Unit* pTarget = g_pMain->GetUnitPtr(sTargetID, sSkillCasterZoneID);
					if (pTarget != nullptr && pTarget->isNPC())
					{
						CNpc* pNpcInfo = g_pMain->GetNpcPtr(sTargetID, sSkillCasterZoneID);
						if (pNpcInfo != nullptr && !pTarget->isPlayer())
						{
							if (!pNpcInfo->isMonster() && pTarget != pSkillCaster)
							{
								pSkillTarget = pSkillCaster;
								if (!TO_NPC(pTarget)->isPet())
									sTargetID = pSkillCaster->GetID();
							}
						}
					}
				}
				break;
			}
		
	}


	if (sTargetID != -1 && pSkillTarget == nullptr)
		pSkillTarget = g_pMain->GetUnitPtr(sTargetID, sSkillCasterZoneID);

	if (pSkill == nullptr || pPriest == nullptr)
	{
		SendSkillFailed();
		return;
	}



	SkillUseResult CanCast, CheckSkill;
	CheckSkill = Successful;
	switch (CheckSkill)
	{
	case Successful:
		break;
	case Failed:
	{
		SendSkillFailed();
		printf("CheckSkillPrerequisites %d\n ", CheckSkillPrerequisites());
		return;
	}
	break;
	case NoValue:
		return;
		break;
	case NoFunction:
	{
		SendSkillNoFunction();
		printf("CheckSkillPrerequisites %d\n ", CheckSkillPrerequisites());
		return;
	}
	break;
	}

	CanCast = Successful;
	switch (CanCast)
	{
	case Successful:
		break;
	case Failed:
	{
		SendSkillFailed();
		printf("UserCanCast %d \n ", UserCanCast());
		return;
	}
	break;
	case NoValue:
		return;
		break;
	case NoFunction:
	{
		SendSkillNoFunction();
		printf("UserCanCast %d \n ", UserCanCast());
		return;
	}
	break;
	}

	bool bInitialResult;
	switch (bOpcode)
	{
	case MAGIC_CASTING:
		SendSkill(bOpcode == MAGIC_CASTING); // only send casting packets to the region, not fail packets.
		break;
	case MAGIC_FAIL:
		SendSkill(bOpcode == MAGIC_FAIL); // only send casting packets to the region, not fail packets.
		break;

	case MAGIC_FLYING:
	{
		// Handle arrow & mana checking/removals.
		if (pSkillCaster->isPlayer())
		{
			CUser* pCaster = TO_USER(pSkillCaster);
			_MAGIC_TYPE2* pType = g_pMain->m_Magictype2Array.GetData(nSkillID);

			// NOTE: Not all skills that use MAGIC_FLYING are type 2 skills.
			// Some type 3 skills use it (such as "Vampiric Fire"). 
			// For these we should really apply the same flying logic, but for now we'll just ignore it.
			if (pType != nullptr)
			{
				// Throwing knives are differentiated by the fact "NeedArrow" is set to 0.
				// We still need to check for & take 1 throwing knife in this case however.
				uint8 bCount = pType->bNeedArrow;

				if (!bCount)
					bCount = 1;
				if (pType == nullptr
					// The user does not have enough arrows! We should point them in the right direction. ;)
					|| (!pCaster->CheckExistItem(pSkill->iUseItem, bCount))
					// Ensure user has enough mana for this skill
					|| pSkill->sMsp > pSkillCaster->GetMana()
					/*|| pSkill->sSp > pCaster->GetStamina()*/)
				{
					SendSkillFailed();
					return;
				}

				// Add all flying arrow instances to the user's list for hit detection
				Guard lock(pCaster->m_arrowLock);
				for (size_t i = 0; i < bCount; i++)
					pCaster->m_flyingArrows.push_back(Arrow(pType->iNum, UNIXTIME2));

				// Remove the arrows
				pCaster->RobItem(pSkill->iUseItem, bCount);
			}
			// for non-type 2 skills, ensure we check the user's mana.
			else if (pSkill->sMsp > pSkillCaster->GetMana())
			{
				SendSkillFailed();
				return;
			}

			// Take the required mana for this skill
			if (!pCaster->isBlinking())
				pCaster->MSpChange(-(pSkill->sMsp));

		}
		SendSkill(true); // send this to the region
	} break;

	case MAGIC_EFFECTING:
		// Hacky check for a transformation item (Disguise Totem, Disguise Scroll)
		// These apply when first type's set to 0, second type's set and obviously, there's a consumable item.
		// Need to find a better way of handling this.
		if (!bIsRecastingSavedMagic
			&& (pSkill->bType[0] == 0 && pSkill->bType[1] != 0 && pSkill->iUseItem != 0
				&& (pSkillCaster->isPlayer() && TO_USER(pSkillCaster)->CheckExistItem(pSkill->iUseItem)))
			&& !TO_USER(pSkillCaster)->isInPKZone())
		{
			SendTransformationList();
			return;
		}

		bInitialResult = ExecuteSkill(pSkill->bType[0]);

		if (bInitialResult)
		{
			if (pSkillCaster->isPlayer())
			{
				CUser* pCaster = TO_USER(pSkillCaster);
				if (nSkillID == 110820 || nSkillID == 210820)
				{
					pCaster->m_CoolDownList.insert(std::make_pair(nSkillID, UNIXTIME2));
				}
				else if (!pSkillCaster->hasBuff(BUFF_TYPE_INSTANT_MAGIC))
				{
					/*Recasting time all skill*/
					pCaster->m_CoolDownList.insert(std::make_pair(nSkillID, UNIXTIME2));

					/*arrow casting time*/
					pCaster->m_SkillCastimeList.insert(std::make_pair(nSkillID, UNIXTIME2));

					if (pSkill->bType[0] != 0 && pSkill->iNum < 490000)
						pCaster->m_MagicTypeCooldownList.insert(std::make_pair(pSkill->bType[0], UNIXTIME2));

					if (pSkill->bType[1] != 0 && pSkill->iNum < 490000)
						pCaster->m_MagicTypeCooldownList.insert(std::make_pair(pSkill->bType[1], UNIXTIME2));
				}
			}

			ExecuteSkill(pSkill->bType[1]);

			if (pSkill->bType[0] != 2)
				ConsumeItem();
		}
		break;

	case MAGIC_CANCEL:
	case MAGIC_CANCEL2:
		Type3Cancel();	//Damage over Time skills.
		Type4Cancel();	//Buffs
		Type6Cancel();	//Transformations
		Type9Cancel();	//Stealth, lupine etc.
		break;

	case MAGIC_TYPE4_EXTEND:
		Type4Extend();
		break;

	default:
		printf("MagicInstance Unhandled packets %d \n", bOpcode);
		TRACE("MagicInstance Unhandled packets %d \n", bOpcode);
		break;
	}
}


bool MagicInstance::ExecuteSkill(uint8 bType)
{
	if (pSkillCaster != nullptr)
	{
		CBot* pPriest = nullptr;
		pPriest = g_pMain->m_BotArray.GetData(sCasterID);

		if (pPriest == nullptr)
		{
			if (bType == 0 || (pSkillCaster->isBlinking() && bType != 4 && pSkill->iNum < 300000))
				return false;

			// Implement player-specific logic before skills are executed.
			if (pSkillCaster->isPlayer())
			{
				// If a player is stealthed, and they are casting a type 1/2/3/7 skill
				// it is classed as an attack, so they should be unstealthed.
				if ((bType >= 1 && bType <= 3) || (bType == 7))
					TO_USER(pSkillCaster)->RemoveStealth();
			}
		}	
	}

	

	switch (bType)
	{
	case 1: return ExecuteType1();
	case 2: return ExecuteType2();
	case 3: return ExecuteType3();
	case 4: return ExecuteType4();
	case 5: return ExecuteType5();
	case 6: return ExecuteType6();
	case 7: return ExecuteType7();
	case 8: return ExecuteType8();
	case 9: return ExecuteType9();
	}

	return false;
}

void MagicInstance::SendTransformationList()
{
	if (!pSkillCaster->isPlayer())
		return;

	Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_TRANSFORM_LIST));
	result << nSkillID;
	TO_USER(pSkillCaster)->Send(&result);
}

/**
* @brief	Sends the skill failed packet to the caster.
*
* @param	sTargetID	Target ID to override with, as some use cases
* 						require.
*/
void MagicInstance::SendSkillFailed(int16 sTargetID)
{
	if (pSkillCaster == nullptr)
		return;

	CUser* pUser = g_pMain->GetUserPtr(pSkillCaster->GetID());
	_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(pUser->GetPartyID());

	if (pUser->hasPriestBot == false && nSkillID == 495146 && pParty != nullptr)
	{
		for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
		{
			auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
			if (PartyUser != nullptr && PartyUser->m_bUserPriestBotID > 0)
			{
				Packet result2(PL_MESSAGE);
				result2 << uint8(0) << "Error" << "There is a priest in the Party.";
				pUser->Send(&result2);
				break;
			}
		}
	}
	else if (pUser->hasPriestBot == true && nSkillID == 495146)
	{
		Packet result2(PL_MESSAGE);
		result2 << uint8(0) << "Error" << "You have a priest.";
		pUser->Send(&result2);
	}
	

	Packet result;
	sData[3] = (bOpcode == MAGIC_CASTING ? SKILLMAGIC_FAIL_CASTING : SKILLMAGIC_FAIL_NOEFFECT);
	BuildSkillPacket(result, sCasterID, sTargetID == -1 ? this->sTargetID : sTargetID, MAGIC_FAIL, nSkillID, sData);

	// No need to proceed if we're not sending fail packets.
	if (!bSendFail || !pSkillCaster->isPlayer())
		return;

	TO_USER(pSkillCaster)->Send(&result);
}

void MagicInstance::SendSkillNoFunction(int16 sTargetID)
{
	if (pSkillCaster == nullptr)
		return;

	Packet result;
	sData[3] = (bOpcode == MAGIC_CASTING ? SKILLMAGIC_FAIL_CASTING : SKILLMAGIC_FAIL_ENDCOMBO);
	BuildSkillPacket(result, sCasterID, sTargetID == -1 ? this->sTargetID : sTargetID, MAGIC_FAIL, nSkillID, sData);

	// No need to proceed if we're not sending fail packets.
	if (!bSendFail || !pSkillCaster->isPlayer())
		return;

	TO_USER(pSkillCaster)->Send(&result);
}

void MagicInstance::SendSkillNotEffect()
{
	if (pSkillCaster == nullptr)
		return;

	Packet result;
	sData[3] = (bOpcode == MAGIC_CASTING ? SKILLMAGIC_FAIL_CASTING : SKILLMAGIC_FAIL_NOEFFECT);
	BuildSkillPacket(result, sCasterID, sTargetID, MAGIC_EFFECTING, nSkillID, sData);

	// No need to proceed if we're not sending fail packets.
	if (!bSendFail || !pSkillCaster->isPlayer())
		return;

	TO_USER(pSkillCaster)->Send(&result);
}

/**
* @brief	Builds skill packet using the specified data.
*
* @param	result			Buffer to store the packet in.
* @param	sSkillCaster	Skill caster's ID.
* @param	sSkillTarget	Skill target's ID.
* @param	opcode			Skill system opcode.
* @param	nSkillID		Identifier for the skill.
* @param	sData			Array of additional misc skill data.
*/
void MagicInstance::BuildSkillPacket(Packet& result, int16 sSkillCaster, int16 sSkillTarget, int8 opcode,
	uint32 nSkillID, int16 sData[7])
{
	// On skill failure, flag the skill as failed.
	if (opcode == MAGIC_FAIL)
	{
		bSkillSuccessful = false;

		// No need to proceed if we're not sending fail packets.
		if (!bSendFail)
			return;
	}

	result.Initialize(WIZ_MAGIC_PROCESS);
	result << opcode << nSkillID << sSkillCaster << sSkillTarget
		<< sData[0] << sData[1] << sData[2] << sData[3]
		<< sData[4] << sData[5] << sData[6];
}

/**
* @brief	Builds and sends skill packet using the specified data to pUnit.
*
* @param	pUnit			The unit to send the packet to. If an NPC is specified,
* 							bSendToRegion is assumed true.
* @param	bSendToRegion	true to send the packet to pUser's region.
* @param	sSkillCaster	Skill caster's ID.
* @param	sSkillTarget	Skill target's ID.
* @param	opcode			Skill system opcode.
* @param	nSkillID		Identifier for the skill.
* @param	sData			Array of additional misc skill data.
*/
void MagicInstance::BuildAndSendSkillPacket(Unit* pUnit, bool bSendToRegion, int16 sSkillCaster, int16 sSkillTarget, int8 opcode, uint32 nSkillID, int16 sData[7])
{
	Packet result;
	BuildSkillPacket(result, sSkillCaster, sSkillTarget, opcode, nSkillID, sData);

	// No need to proceed if we're not sending fail packets.
	if (opcode == MAGIC_FAIL && !bSendFail)
		return;

	
	if (bSendToRegion || !pUnit->isPlayer())
		pUnit->SendToRegion(&result);
	else
		TO_USER(pUnit)->SendToRegion(&result, nullptr, pUnit->GetEventRoom());


	
}

/**
* @brief	Sends the skill data in the current context to pUnit.
* 			If pUnit is nullptr, it will assume the caster.
*
* @param	bSendToRegion	true to send the packet to pUnit's region.
* 							If pUnit is an NPC, this is assumed true.
* @param	pUser		 	The unit to send the packet to.
* 							If pUnit is an NPC, it will send to pUnit's region regardless.
*/
void MagicInstance::SendSkill(bool bSendToRegion, Unit* pUnit)
{
	// If pUnit is nullptr, it will assume the caster.
	if (pUnit == nullptr)
		pUnit = pSkillCaster;

	// Build the skill packet using the skill data in the current context.
	BuildAndSendSkillPacket(pUnit, bSendToRegion,
		this->sCasterID, this->sTargetID, this->bOpcode, this->nSkillID, this->sData);
}



bool MagicInstance::ExecuteType1()
{
	if (pSkill == nullptr)
		return false;

	_MAGIC_TYPE1* pType = g_pMain->m_Magictype1Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	int damage = 0;
	bool bResult = false;

	if (pSkillTarget != nullptr)
	{
		if ((pSkillCaster == nullptr)
			|| (pSkillTarget->GetEventRoom() != pSkillCaster->GetEventRoom()))
			return false;

		if (pSkillTarget->isDead()
			|| pSkillTarget->GetZoneID() != pSkillCaster->GetZoneID())
			return false;


		bResult = true;
		uint16 sAdditionalDamage = pType->sAddDamage;

		// Give increased damage in war zones (as per official 1.298~1.325)
		// This may need to be revised to use the last nation to win the war, or more accurately,
		// the nation that last won the war 3 times in a row (whether official behaves this way now is unknown).
		if (pSkillCaster->isPlayer()
			&& pSkillTarget->isPlayer())
		{
			if (pSkillCaster->GetMap()->isWarZone())
				sAdditionalDamage /= 2;
			else
				sAdditionalDamage /= 3;
		}


		damage = pSkillCaster->GetDamage(pSkillTarget, pSkill);
		// Only add additional damage if the target's not currently blocking it.
		// NOTE: Not sure whether to count this as physical or magic damage.
		// Using physical damage, due to the nature of these skills.
		if (!pSkillTarget->m_bBlockPhysical)
			damage += sAdditionalDamage;

		if (pSkillCaster->GetZoneID() == ZONE_CHAOS_DUNGEON && nSkillID != 490226)
			damage = 100;
		else if (pSkillCaster->GetZoneID() == ZONE_CHAOS_DUNGEON && nSkillID == 490226)
			damage = 1000;


		if (pSkillCaster->isPlayer() && pSkillTarget->m_bReflectArmorType != 0 && pSkillCaster != pSkillTarget)
		{
			pSkillTarget->HpChange(-damage, pSkillCaster);
			ReflectDamage(damage, pSkillTarget);
		}
		else
			pSkillTarget->HpChange(-damage, pSkillCaster);
	}
	else if (sTargetID == -1)
	{
		switch (pType->bHitType)
		{
		case 0:
		{
			vector<Unit*> casted_member;
			std::vector<uint16> unitList;
			g_pMain->GetUnitListFromSurroundingRegions(pSkillCaster, &unitList);
			uint16 sAdditionalDamage = pType->sAddDamage;
			bResult = true;

			foreach(itr, unitList)
			{
				Unit* pTarget = g_pMain->GetUnitPtr(*itr, sSkillCasterZoneID);

				if (pTarget == nullptr)
					continue;

				if (pTarget->isPlayer() && TO_USER(pTarget)->isGM())
					continue;

				if (pSkillCaster != pTarget
					&& !pTarget->isDead() && !pTarget->isBlinking() && pTarget->isAttackable()
					&& CMagicProcess::UserRegionCheck(pSkillCaster, pTarget, pSkill, pType->sRange, sData[0], sData[2]))
					casted_member.push_back(pTarget);
			}

			// If you managed to not hit anything with your AoE, you're still gonna have a cooldown (You should l2aim)
			if (casted_member.empty() || (sTargetID == -1 && casted_member.empty()))
			{
				SendSkill();
				return true;
			}

			sData[1] = 1;
			foreach(itr, casted_member)
			{
				Unit* pTarget = *itr; // it's checked above, not much need to check it again

				if (pTarget == nullptr)
					continue;

				if (pTarget->isPlayer()
					&& !TO_USER(pTarget)->isInGame())
					continue;

				if (pSkill->sRange > 0
					&& (pSkillCaster->isPlayer()
						&& pSkillCaster->GetDistanceSqrt(pTarget) >= (float)pSkill->sRange))
					continue;

				uint16 sAdditionalDamage = pType->sAddDamage;
				damage = pSkillCaster->GetDamage(pTarget, pSkill, true);

				// Only add additional damage if the target's not currently blocking it.
				// NOTE: Not sure whether to count this as physical or magic damage.
				// Using physical damage, due to the nature of these skills.
				if (!pTarget->m_bBlockPhysical)
					damage += sAdditionalDamage;

				if (pSkillCaster->GetZoneID() == ZONE_CHAOS_DUNGEON && nSkillID != 490226)
					damage = 100;
				else if (pSkillCaster->GetZoneID() == ZONE_CHAOS_DUNGEON && nSkillID == 490226)
					damage = 1000;



				if (pSkillCaster->isPlayer() && pTarget->m_bReflectArmorType != 0 && pSkillCaster != pTarget)
				{
					pTarget->HpChange(-damage, pSkillCaster);
					ReflectDamage(damage, pTarget);
				}
				else
					pTarget->HpChange(-damage, pSkillCaster);
			}
		}
		break;
		case 3:
		{
			vector<Unit*> casted_member;
			std::vector<uint16> unitList;
			g_pMain->GetUnitListFromSurroundingRegions(pSkillCaster, &unitList);
			uint16 sAdditionalDamage = pType->sAddDamage;
			bResult = true;

			foreach(itr, unitList)
			{
				Unit* pTarget = g_pMain->GetUnitPtr(*itr, sSkillCasterZoneID);

				if (pTarget == nullptr
					|| pTarget->isDead())
					continue;

				if (pTarget->isPlayer() && TO_USER(pTarget)->isGM())
					continue;

				if (pSkillCaster != pTarget
					&& !pTarget->isDead() && !pTarget->isBlinking() && pTarget->isAttackable()
					&& CMagicProcess::UserRegionCheck(pSkillCaster, pTarget, pSkill, pType->sRange, sData[0], sData[2]))
					casted_member.push_back(pTarget);
			}

			// If you managed to not hit anything with your AoE, you're still gonna have a cooldown (You should l2aim)
			if (casted_member.empty() || (sTargetID == -1 && casted_member.empty()))
			{
				SendSkill();
				return true;
			}

			sData[1] = 1;
			foreach(itr, casted_member)
			{
				Unit* pTarget = *itr; // it's checked above, not much need to check it again

				if (pTarget == nullptr
					|| pTarget->isDead())
					continue;

				if (pTarget->isPlayer()
					&& !TO_USER(pTarget)->isInGame())
					continue;

				if (pSkill->sRange > 0
					&& (pSkillCaster->GetDistanceSqrt(pTarget) >= (float)pSkill->sRange))
					continue;

				uint16 sAdditionalDamage = pType->sAddDamage;
				damage = pSkillCaster->GetDamage(pTarget, pSkill, true);

				if (!pTarget->m_bBlockPhysical)
					damage += sAdditionalDamage;

				pTarget->HpChange(-damage, pSkillCaster);
			}
		}
		break;
		}
	}

	if (pSkillTarget != nullptr)
	{
		if (pSkillTarget->isNPC()
			&& (pSkill->iNum == 108821 || pSkill->iNum == 208821))
			bOpcode = MAGIC_CASTING;
	}

	// This should only be sent once. I don't think there's reason to enforce this, as no skills behave otherwise
	sData[3] = (damage == 0 ? SKILLMAGIC_FAIL_ATTACKZERO : 0);

	// Send the skill data in the current context to the caster's region
	if (pSkill->iNum == 106520
		|| pSkill->iNum == 106802
		|| pSkill->iNum == 206520
		|| pSkill->iNum == 105520
		|| pSkill->iNum == 205520
		|| pSkill->iNum == 206802
		|| pSkill->iNum == 106820
		|| pSkill->iNum == 206820
		|| pSkill->iNum == 490220)
		return bResult;

	SendSkill();

	return bResult;
}

bool MagicInstance::ExecuteType2()
{
	/*
	NOTE:
	Archery skills work differently to most other skills.

	When an archery skill is used, the client sends MAGIC_FLYING (instead of MAGIC_CASTING)
	to show the arrows flying in the air to their targets.

	The client chooses the target(s) to be hit by the arrows.

	When an arrow hits a target, it will send MAGIC_EFFECTING which triggers this handler.
	An arrow sent may not necessarily hit a target.

	As such, for archery skills that use multiple arrows, not all n arrows will necessarily
	hit their target, and thus they will not necessarily call this handler all n times.

	What this means is, we must remove all n arrows from the user in MAGIC_FLYING, otherwise
	it is not guaranteed all arrows will be hit and thus removed.
	(and we can't just go and take all n items each time an arrow hits, that could potentially
	mean 25 arrows are removed [5 each hit] when using "Arrow Shower"!)

	However, via the use of hacks, this MAGIC_FLYING step can be skipped -- so we must also check
	to ensure that there arrows are indeed flying, to prevent users from spamming the skill
	without using arrows.
	*/
	if (pSkill == nullptr)
		return false;

	_MAGIC_TYPE2* pType = g_pMain->m_Magictype2Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	int damage = 0;
	bool bResult = false;
	float total_range = 0.0f;	// These variables are used for range verification!
	int sx, sz, tx, tz;
	int range = 0;

	// If we need arrows, then we require a bow.
	// This check is needed to allow for throwing knives (the sole exception at this time.
	// In this case, 'NeedArrow' is set to 0 -- we do not need a bow to use throwing knives, obviously.


	if (pType->bNeedArrow > 0)
	{
		_ITEM_TABLE* pTable = nullptr;
		if (pSkillCaster->isPlayer())
		{
			if (TO_USER(pSkillCaster)->isWeaponsDisabled())
				return false;

			// Not wearing a left-handed bow
			pTable = TO_USER(pSkillCaster)->GetItemPrototype(TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND);
			if (pTable == nullptr || !pTable->isBow())
			{
				pTable = TO_USER(pSkillCaster)->GetItemPrototype(TO_USER(pSkillCaster)->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND);

				// Not wearing a right-handed (2h) bow either!
				if (pTable == nullptr || !pTable->isBow())
					return false;
			}
		}
		else
		{
			// TODO: Verify this. It's more a placeholder than anything. 
			pTable = g_pMain->GetItemPtr(TO_NPC(pSkillCaster)->m_iWeapon_1);
			if (pTable == nullptr)
				return false;
		}

		// For arrow skills, we require a bow & its range.
		range = pTable->isBow() ? pTable->m_sRange : pSkill->sRange + pTable->m_sRange;
	}
	else
	{
		// For non-arrow skills (i.e. throwing knives) we should use the skill's range.
		range = pSkill->sRange;
	}


	// is this checked already?
	if (pSkillTarget == nullptr || pSkillTarget->isDead())
		goto packet_send;

	total_range = pow(((pType->sAddRange * range) / 100.0f), 2.0f);     // Range verification procedure.
	sx = (int)pSkillCaster->GetX(); tx = (int)pSkillTarget->GetX();
	sz = (int)pSkillCaster->GetZ(); tz = (int)pSkillTarget->GetZ();
	float distance = pow((float)(sx - tx), 2.0f) + pow((float)(sz - tz), 2.0f);

	if (distance > total_range)	   // Target is out of range, exit.
		goto packet_send;


	if (pSkillCaster->isPlayer())
	{
		Guard lock(TO_USER(pSkillCaster)->m_arrowLock);

		CUser* pUser = TO_USER(pSkillCaster);

		// No arrows currently flying.
		if (pUser->m_flyingArrows.empty())
			goto packet_send;

		ArrowList::iterator arrowItr;
		bool bFoundArrow = false;
		for (auto itr = pUser->m_flyingArrows.begin(); itr != pUser->m_flyingArrows.end();)
		{
			if (UNIXTIME2 >= itr->tFlyingTime + ARROW_EXPIRATION_TIME)
			{
				itr = pUser->m_flyingArrows.erase(itr);
			}
			else
			{
				if (itr->nSkillID == nSkillID)
				{
					arrowItr = itr; /* don't break out here to ensure we remove all expired arrows */
					bFoundArrow = true;
				}
				++itr;
			}
		}

		// No flying arrow matching this skill's criteria was found.
		// User's probably cheating.
		if (!bFoundArrow)
			goto packet_send;

		// Remove this instance's arrow now that we've found it.
		pUser->m_flyingArrows.erase(arrowItr);
	}


	damage = pSkillCaster->GetDamage(pSkillTarget, pSkill);  // Get damage points of enemy.	


	if (pSkillCaster->isPlayer() && pSkillTarget->m_bReflectArmorType != 0 && pSkillCaster != pSkillTarget)
	{
		ReflectDamage(damage, pSkillTarget);
		pSkillTarget->HpChange(-damage, pSkillCaster);     // Reduce target health point.
	}
	else
		pSkillTarget->HpChange(-damage, pSkillCaster);     // Reduce target health point.


	bResult = true;

packet_send:
	// This should only be sent once. I don't think there's reason to enforce this, as no skills behave otherwise
	sData[3] = (damage == 0 ? SKILLMAGIC_FAIL_ATTACKZERO : 0);

	// Send the skill data in the current context to the caster's region
	// Send the skill data in the current context to the caster's region
	if (pSkill->bType[0] == 2 && (pSkill->bType[1] != 3 && pSkill->bType[1] != 4))
	{
		if (pSkillTarget == nullptr)
			SendSkill();
		else
		{
			sData[1] = 1;
			BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, pSkillTarget->GetID(), bOpcode, nSkillID, sData);
		}
	}

	return bResult;
}

// Applied when a magical attack, healing, and mana restoration is done.
bool MagicInstance::ExecuteType3()
{

	if (pSkill == nullptr)
		return false;

	_MAGIC_TYPE3* pType = g_pMain->m_Magictype3Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	int damage = 0, duration_damage = 0;
	vector<Unit*> casted_member;

	// If the target's a group of people...
	if (sTargetID == -1)
	{
		std::vector<uint16> unitList;

		g_pMain->GetUnitListFromSurroundingRegions(pSkillCaster, &unitList);
		foreach(itr, unitList)
		{
			Unit* pTarget = g_pMain->GetUnitPtr(*itr, sSkillCasterZoneID);

			if (pTarget == nullptr)
				continue;

			if (pTarget->isPlayer() && TO_USER(pTarget)->isGM())
				continue;

			if (!pTarget->isDead()
				&& !pTarget->isBlinking()
				&& pTarget->isAttackable()
				&& CMagicProcess::UserRegionCheck(pSkillCaster, pTarget, pSkill, pType->bRadius, sData[0], sData[2]))
				casted_member.push_back(pTarget);
		}

		// If you managed to not hit anything with your AoE, you're still gonna have a cooldown (You should l2aim)
		if (casted_member.empty() || (sTargetID == -1 && casted_member.empty()))
		{
			if (pSkill->bMoral == MORAL_SIEGE_WEAPON)
				casted_member.push_back(pSkillCaster);
			/*else
			{
				SendSkill();
				return true;
			}*/
		}
	}
	else
	{
		Unit* pTUser = g_pMain->GetUnitPtr(sTargetID, sSkillCasterZoneID);
		if (pTUser == nullptr
			|| (pTUser->isPlayer()
				&& !TO_USER(pTUser)->isInGame()))
			return false;

		// If the target was a single unit.
		if (pSkillTarget == nullptr
			|| pSkillTarget->isDead()
			|| (pSkillTarget->isPlayer()
				&& TO_USER(pSkillTarget)->isBlinking()))
			return false;

		casted_member.push_back(pTUser);
	}

	// Anger explosion requires the caster be a player
	// and a full anger gauge (which will be reset after use).
	if (pType->bDirectType == 18)
	{
		// Only players can cast this skill.
		if (/*!pSkillCaster->isPlayer()
			||*/ !TO_USER(pSkillCaster)->hasFullAngerGauge())
			return false;

		// Reset the anger gauge
		TO_USER(pSkillCaster)->UpdateAngerGauge(0);
	}

	sData[1] = 1;
	foreach(itr, casted_member)
	{
		Unit* pTarget = *itr; // it's checked above, not much need to check it again

		if (pTarget == nullptr)
			continue;

		if (pTarget->isPlayer() && !TO_USER(pTarget)->isInGame())
			continue;

		// If you are casting an attack spell.
		if ((pType->sFirstDamage < 0) && (pType->bDirectType == 1 || pType->bDirectType == 8)
			&& (nSkillID < 400000)
			&& (pType->bDirectType != 11 && pType->bDirectType != 13))
			damage = GetMagicDamage(pTarget, pType->sFirstDamage, pType->bAttribute);
		else
			damage = pType->sFirstDamage;

		// Allow for complete magic damage blocks.
		if (damage < 0 && pTarget->m_bBlockMagic)
			continue;
		if (pSkillCaster != nullptr)
		{
			if (pSkillCaster->isPlayer())
			{
				if (pSkillCaster->GetZoneID() == ZONE_SNOW_BATTLE && g_pMain->m_byBattleOpen == SNOW_BATTLE)
					damage = -10;

				// hacking prevention for potions
				if (pSkill->iNum > 400000 // using hp and mana potion at the same time!!! not at all!
					&& pSkill->bType[1] == 0
					&& pSkill->bMoral == MORAL_SELF
					&& pSkill->bItemGroup == 9
					&& (pType->bDirectType == 1 || pType->bDirectType == 2)
					&& pType->sFirstDamage > 0)
					TO_USER(pSkillCaster)->t_timeLastPotionUse = UNIXTIME2;
			}
		}

		bool bSendLightningStunSkill = true;

		if (pType->bDirectType < 2
			&& nSkillID < 400000
			&& pTarget->isPlayer()
			&& (pType->bAttribute == AttributeLightning || pType->bAttribute == AttributeIce))
			bSendLightningStunSkill = false;

		// Non-durational spells.
		if (pType->bDuration == 0)
		{
			switch (pType->bDirectType)
			{
				// Affects target's HP
			case 1:
			{

				if (pTarget->isNPC() || pTarget->isPlayer())
				{
					if (pType->sTimeDamage > 0 || pType->sFirstDamage > 0)
					{
						if (pSkillCaster != pTarget && pSkillCaster->isPlayer())
						{							
							CUser* pUser = g_pMain->GetUserPtr(pSkillCaster->GetID());
							if (pUser != nullptr)
							{
								_ITEM_TABLE* pTable = pUser->GetItemPrototype(RIGHTHAND);
								if (pTable != nullptr && pUser->isPriest())
								{
									if (pTable->isDarkKnightMace())
									{
										if (pTarget->isNPC() && isDKMToMonsterDamageSkills())
										{
											if (TO_NPC(pTarget)->isMonster())
											{
												damage = pType->sFirstDamage / 2;
												damage = -damage;

												if (damage > 0)
													break;

												pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
												break;
											}
										}
										else if (pTarget->isPlayer() && isDKMToUserDamageSkills())
										{
											if (pUser->isInArena() && TO_USER(pTarget)->isInArena())
											{
												if (pUser->isInMeleeArena() && TO_USER(pTarget)->isInMeleeArena())
												{
													damage = pType->sFirstDamage / 2;
													damage = -damage;

													if (damage > 0)
														break;

													pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
													break;
												}
												else if (pUser->isInPartyArena() && TO_USER(pTarget)->isInPartyArena())
												{
													if (pUser->isInParty()
														&& pUser->GetPartyID() != TO_USER(pTarget)->GetPartyID())
													{
														damage = pType->sFirstDamage / 2;
														damage = -damage;

														if (damage > 0)
															break;

														pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
														break;
													}

													if (!pUser->isInParty())
													{
														damage = pType->sFirstDamage / 2;
														damage = -damage;

														if (damage > 0)
															break;

														pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
														break;
													}
												}
												if (pUser->GetZoneID() == ZONE_ARENA)
												{
													if (!pUser->isInEnemySafetyArea())
													{
														if (pUser->isInClanArena() && TO_USER(pTarget)->isInClanArena())
														{
															if (pUser->isInClan() && TO_USER(pTarget)->isInClan()
																&& pUser->GetClanID() != TO_USER(pTarget)->GetClanID())
															{
																damage = pType->sFirstDamage / 2;
																damage = -damage;

																if (damage > 0)
																	break;

																pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																break;
															}
														}
														else if (!TO_USER(pTarget)->isInClanArena())
														{
															damage = pType->sFirstDamage / 2;
															damage = -damage;

															if (damage > 0)
																break;

															pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
															break;
														}
													}
												}
											}
											if (pUser->GetZoneID() == ZONE_DELOS)
											{
												if (g_pMain->pCswEvent.Started)
												{
													CKnights* pKnightsSource = g_pMain->GetClanPtr(pUser->GetClanID());
													CKnights* pKnightsTarget = g_pMain->GetClanPtr(TO_USER(pTarget)->GetClanID());

													if (pKnightsSource != nullptr
														&& pKnightsTarget != nullptr)
													{
														if (!pUser->isInEnemySafetyArea()
															&& !TO_USER(pTarget)->isInEnemySafetyArea())
														{
															if (g_pMain->pCswEvent.Status == CswOperationStatus::DeathMatch)
															{
																_KNIGHTS_SIEGE_WARFARE* pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(Aktive);
																_DEATHMATCH_WAR_INFO* pDeathmatchSource = g_pMain->m_KnightSiegeWarClanList.GetData(pUser->GetClanID());
																_DEATHMATCH_WAR_INFO* pDeathmatchTarget = g_pMain->m_KnightSiegeWarClanList.GetData(TO_USER(pTarget)->GetClanID());
																if (pKnightSiege != nullptr)
																{
																	if (pDeathmatchSource != nullptr && pDeathmatchTarget != nullptr)
																	{
																		if (pUser->GetClanID() != TO_USER(pTarget)->GetClanID())
																		{
																			damage = pType->sFirstDamage / 2;
																			damage = -damage;

																			if (damage > 0)
																				break;

																			pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																			break;
																		}
																	}
																}
															}
															else if (g_pMain->pCswEvent.Status == CswOperationStatus::CastellanWar)
															{
																_KNIGHTS_SIEGE_WARFARE* pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(Aktive);
																if (pKnightSiege != nullptr)
																{
																	_CASTELLAN_WAR_INFO* pInfo1 = g_pMain->m_KnightSiegeWarCastellanClanList.GetData(pUser->GetClanID());
																	if (pInfo1 != nullptr && (pKnightSiege->sMasterKnights == TO_USER(pTarget)->GetClanID()))
																	{
																		damage = pType->sFirstDamage / 2;
																		damage = -damage;

																		if (damage > 0)
																			break;

																		pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																		break;
																	}
																	else if (pInfo1 != nullptr)
																	{
																		_CASTELLAN_OLD_CSW_WINNER* pOldWinnerTarget = g_pMain->m_KnightSiegeWarCastellanOldWinner.GetData(TO_USER(pTarget)->GetClanID());
																		if (pOldWinnerTarget != nullptr)
																		{
																			if (TO_USER(pTarget)->GetClanID() == pOldWinnerTarget->m_oldClanID)
																			{
																				damage = pType->sFirstDamage / 2;
																				damage = -damage;

																				if (damage > 0)
																					break;

																				pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																				break;
																			}
																		}
																	}
																	_CASTELLAN_OLD_CSW_WINNER* pOldWinnerSource = g_pMain->m_KnightSiegeWarCastellanOldWinner.GetData(pUser->GetClanID());
																	if (pOldWinnerSource != nullptr)
																	{
																		_CASTELLAN_WAR_INFO* pCastellanTarget = g_pMain->m_KnightSiegeWarCastellanClanList.GetData(TO_USER(pTarget)->GetClanID());
																		if (pCastellanTarget != nullptr && pUser->GetClanID() != TO_USER(pTarget)->GetClanID())
																		{
																			damage = pType->sFirstDamage / 2;
																			damage = -damage;

																			if (damage > 0)
																				break;

																			pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																			break;
																		}
																	}
																	else
																	{
																		if (pKnightSiege->sMasterKnights == pUser->GetClanID())
																		{
																			_CASTELLAN_WAR_INFO* pCastellanTarget = g_pMain->m_KnightSiegeWarCastellanClanList.GetData(TO_USER(pTarget)->GetClanID());
																			if (pCastellanTarget != nullptr && pUser->GetClanID() != TO_USER(pTarget)->GetClanID())
																			{
																				damage = pType->sFirstDamage / 2;
																				damage = -damage;

																				if (damage > 0)
																					break;

																				pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																				break;
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
							
							else if (pUser == nullptr)
							{
							CBot* pUserBot = g_pMain->m_BotArray.GetData(pSkillCaster->GetID());

							_ITEM_TABLE* pTable = pUserBot->GetItemPrototype(RIGHTHAND);
								if (pTable != nullptr && pUserBot->isPriest())
								{
									if (pTable->isDarkKnightMace())
									{
										if (pTarget->isNPC() && isDKMToMonsterDamageSkills())
										{
											if (TO_NPC(pTarget)->isMonster())
											{
												damage = pType->sFirstDamage / 2;
												damage = -damage;

												if (damage > 0)
													break;

												pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
												break;
											}
										}
										else if (pTarget->isPlayer() && isDKMToUserDamageSkills())
										{
											if (pUserBot->isInArena() && TO_USER(pTarget)->isInArena())
											{
												if (pUserBot->isInMeleeArena() && TO_USER(pTarget)->isInMeleeArena())
												{
													damage = pType->sFirstDamage / 2;
													damage = -damage;

													if (damage > 0)
														break;

													pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
													break;
												}
												else if (pUserBot->isInPartyArena() && TO_USER(pTarget)->isInPartyArena())
												{
													if (pUserBot->isInParty()
														&& pUserBot->GetPartyID() != TO_USER(pTarget)->GetPartyID())
													{
														damage = pType->sFirstDamage / 2;
														damage = -damage;

														if (damage > 0)
															break;

														pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
														break;
													}

													if (!pUserBot->isInParty())
													{
														damage = pType->sFirstDamage / 2;
														damage = -damage;

														if (damage > 0)
															break;

														pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
														break;
													}
												}
												if (pUserBot->GetZoneID() == ZONE_ARENA)
												{
													if (!pUserBot->isInEnemySafetyArea())
													{
														if (pUserBot->isInClanArena() && TO_USER(pTarget)->isInClanArena())
														{
															if (pUserBot->isInClan() && TO_USER(pTarget)->isInClan()
																&& pUserBot->GetClanID() != TO_USER(pTarget)->GetClanID())
															{
																damage = pType->sFirstDamage / 2;
																damage = -damage;

																if (damage > 0)
																	break;

																pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																break;
															}
														}
														else if (!TO_USER(pTarget)->isInClanArena())
														{
															damage = pType->sFirstDamage / 2;
															damage = -damage;

															if (damage > 0)
																break;

															pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
															break;
														}
													}
												}
											}
											if (pUserBot->GetZoneID() == ZONE_DELOS)
											{
												if (g_pMain->pCswEvent.Started)
												{
													CKnights* pKnightsSource = g_pMain->GetClanPtr(pUserBot->GetClanID());
													CKnights* pKnightsTarget = g_pMain->GetClanPtr(TO_USER(pTarget)->GetClanID());

													if (pKnightsSource != nullptr
														&& pKnightsTarget != nullptr)
													{
														if (!pUserBot->isInEnemySafetyArea()
															&& !TO_USER(pTarget)->isInEnemySafetyArea())
														{
															if (g_pMain->pCswEvent.Status == CswOperationStatus::DeathMatch)
															{
																_KNIGHTS_SIEGE_WARFARE* pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(Aktive);
																_DEATHMATCH_WAR_INFO* pDeathmatchSource = g_pMain->m_KnightSiegeWarClanList.GetData(pUserBot->GetClanID());
																_DEATHMATCH_WAR_INFO* pDeathmatchTarget = g_pMain->m_KnightSiegeWarClanList.GetData(TO_USER(pTarget)->GetClanID());
																if (pKnightSiege != nullptr)
																{
																	if (pDeathmatchSource != nullptr && pDeathmatchTarget != nullptr)
																	{
																		if (pUserBot->GetClanID() != TO_USER(pTarget)->GetClanID())
																		{
																			damage = pType->sFirstDamage / 2;
																			damage = -damage;

																			if (damage > 0)
																				break;

																			pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																			break;
																		}
																	}
																}
															}
															else if (g_pMain->pCswEvent.Status == CswOperationStatus::CastellanWar)
															{
																_KNIGHTS_SIEGE_WARFARE* pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(Aktive);
																if (pKnightSiege != nullptr)
																{
																	_CASTELLAN_WAR_INFO* pInfo1 = g_pMain->m_KnightSiegeWarCastellanClanList.GetData(pUserBot->GetClanID());
																	if (pInfo1 != nullptr && (pKnightSiege->sMasterKnights == TO_USER(pTarget)->GetClanID()))
																	{
																		damage = pType->sFirstDamage / 2;
																		damage = -damage;

																		if (damage > 0)
																			break;

																		pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																		break;
																	}
																	else if (pInfo1 != nullptr)
																	{
																		_CASTELLAN_OLD_CSW_WINNER* pOldWinnerTarget = g_pMain->m_KnightSiegeWarCastellanOldWinner.GetData(TO_USER(pTarget)->GetClanID());
																		if (pOldWinnerTarget != nullptr)
																		{
																			if (TO_USER(pTarget)->GetClanID() == pOldWinnerTarget->m_oldClanID)
																			{
																				damage = pType->sFirstDamage / 2;
																				damage = -damage;

																				if (damage > 0)
																					break;

																				pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																				break;
																			}
																		}
																	}
																	_CASTELLAN_OLD_CSW_WINNER* pOldWinnerSource = g_pMain->m_KnightSiegeWarCastellanOldWinner.GetData(pUserBot->GetClanID());
																	if (pOldWinnerSource != nullptr)
																	{
																		_CASTELLAN_WAR_INFO* pCastellanTarget = g_pMain->m_KnightSiegeWarCastellanClanList.GetData(TO_USER(pTarget)->GetClanID());
																		if (pCastellanTarget != nullptr && pUserBot->GetClanID() != TO_USER(pTarget)->GetClanID())
																		{
																			damage = pType->sFirstDamage / 2;
																			damage = -damage;

																			if (damage > 0)
																				break;

																			pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																			break;
																		}
																	}
																	else
																	{
																		if (pKnightSiege->sMasterKnights == pUserBot->GetClanID())
																		{
																			_CASTELLAN_WAR_INFO* pCastellanTarget = g_pMain->m_KnightSiegeWarCastellanClanList.GetData(TO_USER(pTarget)->GetClanID());
																			if (pCastellanTarget != nullptr && pUserBot->GetClanID() != TO_USER(pTarget)->GetClanID())
																			{
																				damage = pType->sFirstDamage / 2;
																				damage = -damage;

																				if (damage > 0)
																					break;

																				pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
																				break;
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}

				// Disable to heal or minor NPC.
				if ((pTarget->isNPC() && pType->sTimeDamage > 0) || (pTarget->isNPC() && pType->sFirstDamage > 0))
				{
					if (TO_NPC(pTarget)->isPet())
					{
						pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);
						break;
					}
					else
						return false;
				}

				if (pSkillCaster->isPlayer())
				{
					CUser* pUserhasBot = TO_USER(pSkillCaster);

					if (pUserhasBot->hasPriestBot == true)
					{
						CBot* pUserBot = g_pMain->m_BotArray.GetData(pSkillCaster->GetID());
						if (pUserBot != nullptr)
						{
							if (TO_USERB(pSkillCaster)->isRogue())
							{
								if (damage > 0 && pSkillCaster->hasBuff(BUFF_TYPE_DAMAGE_DOUBLE))
									damage *= 2;
							}
						}
					}
					else if (TO_USER(pSkillCaster)->isRogue())
					{
						if (damage > 0 && pSkillCaster->hasBuff(BUFF_TYPE_DAMAGE_DOUBLE))
							damage *= 2;
					}
				}



				if (pSkillCaster->isPlayer() && pTarget->m_bReflectArmorType != 0 && pTarget != pSkillCaster && damage < 0)
					ReflectDamage(damage, pTarget);
				else if (pTarget->isPlayer() && damage > 0 && pTarget->isDevil())
				{
					if (TO_USER(pTarget)->CheckSkillPoint(PRO_SKILL3, 45, g_pMain->m_byMaxLevel))
						pTarget->HpChangeMagic(damage * 2, pSkillCaster, (AttributeType)pType->bAttribute);
				}
				else
					pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);

			}
			break;
			case 2:
				if (pTarget->isPlayer())
				{
					if (!pTarget->isDead())
						pTarget->MSpChange(pType->sFirstDamage);
				}
				else
				{
					if (!pTarget->isDead())
					{
						if (TO_NPC(pTarget)->isPet())
							pTarget->MSpChange(pType->sFirstDamage);
						else
							pTarget->HpChange(pType->sFirstDamage, pSkillCaster);
					}
				}
				break;
			case 3:
				damage = pTarget->GetMana() / pType->sFirstDamage;
				pTarget->MSpChange(damage);
				break;

				// "Magic Hammer" repairs equipped items.
			case 4:
				if (pTarget->isPlayer())
				{
					if (damage > 0)
						TO_USER(pTarget)->ItemWoreOut(REPAIR_ALL, damage);
					else
						TO_USER(pTarget)->ItemWoreOut(ACID_ALL, -damage);
				}
				break;

				// Increases/decreases target's HP by a percentage
			case 5:
				if (pType->sFirstDamage < 100)
					damage = (pType->sFirstDamage * pTarget->GetHealth()) / -100;
				else
					damage = (pTarget->GetMaxHealth() * (pType->sFirstDamage - 100)) / 100;

				pTarget->HpChangeMagic(damage, pSkillCaster);
				break;

				// Caster absorbs damage based on percentage of target's HP. Players only.
			case 8:
				if (pType->sFirstDamage > 0)
				{
					if (pType->sFirstDamage < 100)
						damage = (pTarget->GetHealth() * 100) / pType->sFirstDamage;
					else
						damage = (pTarget->GetMaxHealth() - 100 * 100) / pType->sFirstDamage;
				}

				if (!pTarget->isDead() && pTarget->isPlayer())
				{
					pTarget->HpChangeMagic(damage, pSkillCaster);
					pSkillCaster->HpChangeMagic(-(damage));
				}
				else
					pTarget->HpChange(damage, pSkillCaster);

				break;

				// Caster absorbs damage based on percentage of target's max HP
			case 9:
				if (pType->sFirstDamage < 100)
					damage = (pType->sFirstDamage * pTarget->GetHealth()) / -100;
				else
					damage = (pTarget->GetMaxHealth() * (pType->sFirstDamage - 100)) / 100;

				pTarget->HpChangeMagic(damage, pSkillCaster);
				if (pTarget->isPlayer())
					pSkillCaster->HpChangeMagic(-(damage));
				break;

				// Inflicts true damage (i.e. disregards Ac/resistances/buffs, etc).
			case 11:
				pTarget->HpChange(damage, pSkillCaster);
				break;

				// Used by "Destination scroll" (whatever that is)
			case 12:
				continue;

				// Chance (how often?) to reduce the opponent's armor and weapon durability by sFirstDamage
			case 13:
				if (pTarget->isPlayer() && CheckPercent(500)) // using 50% for now.
				{
					TO_USER(pTarget)->ItemWoreOut(ATTACK, damage);
					TO_USER(pTarget)->ItemWoreOut(DEFENCE, damage);
				}
				break;

				// Drains target's MP, gives half of it to the caster as HP. Players only.
				// NOTE: Non-durational form (as in 1.8xx). This was made durational later (database configured).
			case 16:
				if (pTarget->isPlayer())
				{
					pTarget->MSpChange(pType->sFirstDamage);
					pSkillCaster->HpChangeMagic(-(pType->sFirstDamage));
				}
				break;
			case 17:
				if (!pTarget->isNPC() && !pTarget->isDead() && pSkillCaster->GetZoneID() == ZONE_DELOS && !pSkillCaster->isDead())
				{
					pTarget->HpChangeMagic(pType->sFirstDamage, pSkillCaster, (AttributeType)pType->bAttribute);
				}
				break;
			case 19: // Chaos Dungeon Skills
				if (pTarget->isPlayer() || pTarget->isNPC())
				{
					if (pSkill->iNum == 490221)
						damage = (-3000);
					else if (pSkill->iNum == 490227)
						damage = (-2000);
					else if (pSkill->iNum == 490232)
						damage = (-100);

					if (pTarget != pSkillCaster)
						pTarget->HpChangeMagic(damage / 10, pSkillCaster, (AttributeType)pType->bAttribute);

					if (pTarget != pSkillCaster)
						ReflectDamage(damage, pTarget);
				}
				break;
			case 20: // hp master
				if (!pTarget->isDead() && pTarget->isPlayer())
				{
					if (pSkillCaster->isDevil() && TO_USER(pSkillCaster)->CheckSkillPoint(PRO_SKILL3, 45, g_pMain->m_byMaxLevel))
						pTarget->HpChangeMagic(damage * 2, pSkillCaster, (AttributeType)pType->bAttribute);
					else
						pTarget->HpChangeMagic(damage, pSkillCaster, (AttributeType)pType->bAttribute);

					if (TO_USER(pSkillCaster)->GetPremium() > 0)
						TO_USER(pSkillCaster)->GoldLose(pType->sTimeDamage);
					else
						TO_USER(pSkillCaster)->GoldLose(100000);
				}
				break;
			case 21: // MP master

				if (!pTarget->isDead() && pTarget->isPlayer())
				{
					pTarget->MSpChange(pType->sFirstDamage);

					if (TO_USER(pSkillCaster)->GetPremium() > 0)
						TO_USER(pSkillCaster)->GoldLose(pType->sTimeDamage);
					else
						TO_USER(pSkillCaster)->GoldLose(100000);
				}
				break;
				// Stat Scroll - MagicNum = 501011
			case 255:
				if (TO_USER(pSkillCaster)->isPlayer())
				{

				}
				break;
			}
		}
		// Durational spells! Durational spells only involve HP.
		else if (pType->bDuration != 0)
		{
			if (pType->bDirectType == 18)
				damage = -(int)(pSkillCaster->GetLevel() * 12.5);

			if (damage != 0)		// In case there was first damage......
				pTarget->HpChangeMagic(damage, pSkillCaster);			// Initial damage!!!

			if (pTarget->isAlive())
			{

				// HP booster (should this actually just be using sFirstDamage as the percent of max HP, i.e. 105 being 5% of max HP each increment?)
				if (pType->bDirectType == 14)
					duration_damage = pType->bDuration / 2 * ((int)(pSkillCaster->GetLevel() * (1 + pSkillCaster->GetLevel() / 30.0)) + 3);
				else if (pType->bDirectType == 19)
					duration_damage = pType->sTimeDamage;
				else if (pSkillCaster->isDevil() && TO_USER(pSkillCaster)->CheckSkillPoint(PRO_SKILL3, 45, g_pMain->m_byMaxLevel) && pType->sTimeDamage > 0)
					duration_damage = (pType->sTimeDamage * 3);
				else if (pType->sTimeDamage < 0 && pType->bAttribute != 4)
					duration_damage = GetMagicDamage(pTarget, pType->sTimeDamage, pType->bAttribute);
				else
					duration_damage = pType->sTimeDamage;

				// Allow for complete magic damage blocks.
				if (duration_damage < 0 && pTarget->m_bBlockMagic)
					continue;

				if (pType->bDirectType == 18)
					duration_damage = -(int)((pSkillCaster->GetLevel() * 12.5) * (pType->bDuration / 2));

				// Setup DOT (damage over time)
				for (int k = 0; k < MAX_TYPE3_REPEAT; k++)
				{
					Unit::MagicType3* pEffect = &pTarget->m_durationalSkills[k];

					if (pEffect == nullptr)
						continue;

					if (pEffect->m_byUsed)
						continue;

					pEffect->m_byUsed = true;
					pEffect->m_tHPLastTime = 0;
					pEffect->m_bHPInterval = 2;					// interval of 2s between each damage loss/HP gain 

					// number of ticks required at a rate of 2s per tick over the total duration of the skill
					float tickCount = (float)pType->bDuration / (float)pEffect->m_bHPInterval;

					if (pSkillCaster->GetZoneID() == ZONE_CHAOS_DUNGEON)
						tickCount *= 2;

					// amount of HP to be given/taken every tick at a rate of 2s per tick
					pEffect->m_sHPAmount = (int16)(duration_damage / tickCount);

					pEffect->m_bTickCount = 0;
					pEffect->m_bTickLimit = (uint8)tickCount;
					pEffect->m_sSourceID = sCasterID;
					pEffect->m_byAttribute = pType->bAttribute;
					break;
				}
				pTarget->m_bType3Flag = true;
			}

			// Send the status updates (i.e. DOT or position indicators) to the party/user
			if (pTarget->isPlayer()
				// Ignore healing spells, not sure if this will break any skills.
				&& pType->sTimeDamage < 0)
			{
				if (bSendLightningStunSkill)
					TO_USER(pTarget)->SendUserStatusUpdate(pType->bAttribute == POISON_R ? USER_STATUS_POISON : USER_STATUS_DOT, USER_STATUS_INFLICT);
			}
		}

		if (!bSendLightningStunSkill)
		{
			if (isStaffSkill(false) || isExtendedArcherSkill(false))
			{
				nSkillID += SKILLPACKET;
				pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);

				if (pSkill == nullptr)
					return false;

				ExecuteType4();
			}
		}
		else if ((pSkill->bType[0] != 1 || pSkill->bType[0] != 4) && (pSkill->bType[1] == 0 || pSkill->bType[0] == 2))
		{
			// Send the skill data in the current context to the caster's region, with the target explicitly set.
			// In the case of AOEs, this will mean the AOE will show the AOE's effect on the user (not show the AOE itself again).

			BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, pTarget->GetID(), bOpcode, nSkillID, sData);
		}

		if (!bSendLightningStunSkill && pSkill->bType[1] != 4 && pSkill->bType[0] != 4)
			SendSkill();



		if (pSkillCaster->isPlayer() && TO_USER(pSkillCaster)->isMage())
			TO_USER(pSkillCaster)->ItemWoreOut(ATTACK, -damage);

		// we're healing someone (so that they can choose to pick on the healer!)
		if (pSkillCaster->isPlayer()
			&& pType->bDirectType == 1
			&& damage > 0
			&& sCasterID != sTargetID)
		{
			TO_USER(pSkillCaster)->HealMagic();
		}
	}

	// Allow for AOE effects.
	if (sTargetID == -1 && pSkill->bType[1] != 4)
		SendSkill();

	return true;
}

bool MagicInstance::ExecuteType4()
{
	if (pSkill == nullptr)
		return false;


	_MAGIC_TYPE4* pType = g_pMain->m_Magictype4Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	int damage = 0;
	vector<Unit*> casted_member;

	if (!bIsRecastingSavedMagic
		&& sTargetID >= 0
		&& (pSkillTarget && pSkillTarget->HasSavedMagic(nSkillID)))
		return false;

	if (sTargetID == -1)
	{
		std::vector<uint16> unitList;

		g_pMain->GetUnitListFromSurroundingRegions(pSkillCaster, &unitList);
		foreach(itr, unitList)
		{
			Unit* pTarget = g_pMain->GetUnitPtr(*itr, sSkillCasterZoneID);

			if (pTarget == nullptr)
				continue;

			if (pTarget->isPlayer()
				&& TO_USER(pTarget)->isGM())
				continue;

			if (!pTarget->isDead() && !pTarget->isBlinking() && pTarget->isAttackable()
				&& CMagicProcess::UserRegionCheck(pSkillCaster, pTarget, pSkill, pType->bRadius, sData[0], sData[2])) {
				casted_member.push_back(pTarget);
			}
		}

		// If you managed to not hit anything with your AoE, you're still gonna have a cooldown (You should l2aim)
		if (casted_member.empty())
		{
			if (pSkillCaster->isPlayer())
			{
				if (pSkill->bMoral == MORAL_PARTY_ALL)
					casted_member.push_back(pSkillCaster);
				else if (pSkill->bMoral == MORAL_ENEMY_PARTY)
					casted_member.push_back(pSkillCaster);
				else if (pSkill->bMoral == MORAL_SIEGE_WEAPON)
					casted_member.push_back(pSkillCaster);
				else if (pSkill->bMoral == MORAL_SELF)
					casted_member.push_back(pSkillCaster);
				/*else if (pSkill->bType[0] == 4)
				{
					if (pSkill->iNum == 490403)
					{
						SendSkill();
						return true;
					}
					else
					{
						SendSkill();
						return false;
					}
				}
				else if (pSkill->bType[1] == 4)
					return false;*/
			}
		}
	}
	else
	{
		// If the target was another single unit.
		if (pSkillTarget == nullptr
			|| (pSkillTarget->isPlayer()
				&& pSkillTarget->isDead())
			|| (pSkillTarget->isPlayer()
				&& TO_USER(pSkillTarget)->isBlinking()
				&& !bIsRecastingSavedMagic))
			return false;

		casted_member.push_back(pSkillTarget);
	}

	foreach(itr, casted_member)
	{

		Unit* pTarget = *itr;

		if (pTarget == nullptr)
			continue;

		/*
				if (pSkill->bCastTime == 0
					&& pSkill->sRange > 0
					&& (pSkillCaster->GetDistanceSqrt(pTarget) >= (float)pSkill->sRange)
					&& pType->bBuffType != BUFF_TYPE_HP_MP
					&& pType->bBuffType != BUFF_TYPE_AC)
					continue;*/

		uint8 bResult = 1;
		_BUFF_TYPE4_INFO pBuffInfo;
		bool bAllowCastOnSelf = false;

		uint16 sDuration = pType->sDuration;

		if (pType->bBuffType == BUFF_TYPE_DEVIL_TRANSFORM && TO_USER(pSkillCaster)->CheckSkillPoint(PRO_SKILL3, 70, 83))
			sDuration = sDuration + 300;
		else
			sDuration = sDuration;

		// Speed Skills
		bool bSetSpeed = true;
		uint8 nTargetSpeedAmount = pType->bSpeed;

		// A handful of skills (Krowaz, currently) should use the caster as the target.
		// As such, we should correct this before any other buff/debuff logic is handled.
		switch (pType->bBuffType)
		{
		case BUFF_TYPE_UNDEAD:
		case BUFF_TYPE_UNSIGHT:
		case BUFF_TYPE_BLOCK_PHYSICAL_DAMAGE:
		case BUFF_TYPE_BLOCK_MAGICAL_DAMAGE:
		case BUFF_TYPE_ARMORED:
			if (!pSkillCaster->isPlayer())
				continue;

			pTarget = pSkillCaster;
			bAllowCastOnSelf = true;
			break;
		case BUFF_TYPE_DISABLE_TARGETING:
		{
			if (!pSkillCaster->isPlayer())
				continue;

			if (pSkill->iNum == 108780 || pSkill->iNum == 208780)
				bAllowCastOnSelf = true;
		}break;
		}

		bool bBlockingDebuffs = pTarget->m_bBlockCurses;

		// Skill description: Blocks all curses and has a chance to reflect the curse back onto the caster.
		// NOTE: the exact rate is undefined, so we'll have to guess and improve later.
		if (pType->isDebuff() && pTarget->m_bReflectCurses)
		{
			const short reflectChance = 25; // % chance to reflect.
			if (CheckPercent(reflectChance * 10))
			{
				pTarget = pSkillCaster; // skill has been reflected, the target is now the caster.
				bBlockingDebuffs = (pTarget->m_bBlockCurses || pTarget->m_bReflectCurses);
				bAllowCastOnSelf = true;
			}
			// Didn't reflect, so we'll just block instead.
			else
			{
				bBlockingDebuffs = true;
			}
		}

		if (pTarget->isPlayer())
		{
			if ((pSkill->bMoral == MORAL_ENEMY
				|| pSkill->bMoral == MORAL_AREA_ENEMY)
				&& (pType->bBuffType == BUFF_TYPE_SPEED2
					|| pType->bBuffType == BUFF_TYPE_SPEED
					|| pType->bBuffType == BUFF_TYPE_STUN)
				|| (pSkill->bMoral == MORAL_SELF
					&& (pType->bBuffType == BUFF_TYPE_SPEED2
						|| pType->bBuffType == BUFF_TYPE_STUN)))
			{
				uint16 nMaxRessitance = 700;
				uint16 resisice = uint16(pTarget->m_sColdR + pTarget->m_bResistanceBonus + pTarget->m_bAddColdR);
				uint16 resislight = uint16(pTarget->m_sLightningR + pTarget->m_bResistanceBonus + pTarget->m_bAddLightningR);
				uint16 nTargetResistance = pType->bBuffType == BUFF_TYPE_SPEED2 ? resisice : resislight;
				uint16 bonusrate = 0;
				uint16 succedrate = 0;

				bonusrate = pSkill->bSuccessRate / 25;
				succedrate = myrand(0, nTargetResistance + bonusrate);

				if (pSkill->iNum == 106820 || pSkill->iNum == 206820)
				{
					if (50 <= myrand(0, 300))
						bSetSpeed = false;
				}
				else if (pSkill->iNum == 105520 || pSkill->iNum == 205520 || pSkill->iNum == 106520 || pSkill->iNum == 206520)
				{
					if (25 <= myrand(0, 100))
						bSetSpeed = false;
				}
				else if (isStaffSkill(true))
				{
					if (succedrate < nTargetResistance)
						bSetSpeed = false;
				}
				else if (isExtendedArcherSkill(true))
				{
					if (succedrate < nTargetResistance)
						bSetSpeed = false;
				}
				else if (isRushSkill())
				{
					if (50 <= myrand(0, 100))
						bSetSpeed = false;
				}
				else
				{
					if (pType->bBuffType == BUFF_TYPE_SPEED2)
					{
						if (pSkillCaster->GetZoneID() == ZONE_CHAOS_DUNGEON || nSkillID == 492027)
							bSetSpeed = true;
						else if (succedrate < nTargetResistance)//if ((pSkill->bMoral == MORAL_AREA_ENEMY ? 35 : 65) <= myrand(0, nMaxRessitance + nTargetResistance))
							bSetSpeed = false;
					}
					else
					{
						if (pSkillCaster->GetZoneID() == ZONE_CHAOS_DUNGEON) {

							if (succedrate < nTargetResistance)
								bSetSpeed = false;
						}
						else if (succedrate < nTargetResistance)//if ((pSkill->bMoral == MORAL_AREA_ENEMY ? 30 : 55) <= myrand(0, nMaxRessitance + nTargetResistance))
							bSetSpeed = false;
					}
				}
			}
		}

		pTarget->m_buffLock.lock();
		Type4BuffMap::iterator buffItr = pTarget->m_buffMap.find(pType->bBuffType);

		// Identify whether or not a skill (buff/debuff) with this buff type was already cast on the player.
		// NOTE:	Buffs will already be cast on a user when trying to recast. 
		//			We should not error out in this case.
		bool bSkillTypeAlreadyOnTarget = (!bIsRecastingSavedMagic && buffItr != pTarget->m_buffMap.end());

		pTarget->m_buffLock.unlock();
		// Debuffs 'stack', in that the expiry time is reset each time.
		// Debuffs also take precedence over buffs of the same nature, so we should ensure they get removed 
		// rather than just stacking the modifiers, as the client only supports one (de)buff of that type active.
		if (bSkillTypeAlreadyOnTarget && pType->isDebuff())
		{
			if (pType->bBuffType == BUFF_TYPE_STATS)
			{
				if (CMagicProcess::RemoveType4Buff(BUFF_TYPE_BATTLE_CRY, pTarget, false) && pTarget->isPlayer())
				{
					Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_DURATION_EXPIRED));
					result << uint8(BUFF_TYPE_BATTLE_CRY);
					TO_USER(pTarget)->Send(&result);
				}

				CMagicProcess::RemoveType4Buff(BUFF_TYPE_STATS, pTarget, false);
				bSkillTypeAlreadyOnTarget = false;


			}
			else if (pType->bBuffType != BUFF_TYPE_SPEED2)
			{
				CMagicProcess::RemoveType4Buff(pType->bBuffType, pTarget, false);
				bSkillTypeAlreadyOnTarget = false;
			}
			else if (pType->bBuffType == BUFF_TYPE_SPEED2 && bSetSpeed)
			{
				CMagicProcess::RemoveType4Buff(pType->bBuffType, pTarget, true);
				bSkillTypeAlreadyOnTarget = false;
			}
		}

		// If this skill is a debuff, and we are in the crossfire, 
		// we should not bother debuffing ourselves (that would be bad!)
		// Note that we should allow us if there's an explicit override (i.e. with Krowaz self-debuffs)
		if (!bAllowCastOnSelf
			&& pType->isDebuff() && pTarget == pSkillCaster)
			continue;

		// If the user already has this buff type cast on them (debuffs should just reset the duration)
		if ((bSkillTypeAlreadyOnTarget && pType->isBuff())
			// or it's a curse (debuff), and we're blocking them 
			|| (pType->isDebuff() && bBlockingDebuffs)
			// or we couldn't grant the (de)buff...
			|| !CMagicProcess::GrantType4Buff(pSkill, pType, pSkillCaster, pTarget, bIsRecastingSavedMagic))
		{
			if (sTargetID != -1 // only error out when buffing a target, otherwise we break the mass-(de)buff.
				// Only buffs should error here, unless it's a debuff & the user's blocking it.
				&& (pType->isBuff() || (pType->isDebuff() && bBlockingDebuffs)))
			{
				bResult = 0;
				goto fail_return;
			}

			// Debuffs of any kind, or area buffs should be ignored and carry on.
			// Usually - debuffs specifically - they correspond with attack skills which should
			// not be reset on fail.
			continue;
		}

		CUser* pUser = g_pMain->GetUserPtr(pTarget->GetID());
		_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(pUser->GetPartyID());

		if (pUser->m_bUserPriestBotID == -1 && nSkillID == 495146 && pParty == nullptr)
		{
			uint16 m_bUserPriestBotID = g_pMain->SpawnPriestBot(60, pUser);
		}


		// Only players can store persistent skills.
		if (nSkillID > 500000 && pTarget->isPlayer())
		{
			// Persisting effects will already exist in the map if we're recasting it. 
			if (!bIsRecastingSavedMagic)
				pTarget->InsertSavedMagic(nSkillID, pType->sDuration);
			else
				sDuration = pTarget->GetSavedMagicDuration(nSkillID);
		}


		// We do not want to reinsert debuffs into the map (which had their expiry times reset above).
		if (!bSkillTypeAlreadyOnTarget
			&& pType->bBuffType != BUFF_TYPE_SPEED2
			&& pType->bBuffType != BUFF_TYPE_FISHING)
		{
			pBuffInfo.m_nSkillID = nSkillID;
			pBuffInfo.m_bBuffType = pType->bBuffType;
			pBuffInfo.m_bIsBuff = pType->bIsBuff;

			pBuffInfo.m_bDurationExtended = false;
			pBuffInfo.m_tEndTime = UNIXTIME + sDuration;

			// Add the buff into the buff map.
			pTarget->AddType4Buff(pType->bBuffType, pBuffInfo);
		}
		else if (pType->bBuffType == BUFF_TYPE_SPEED2 && bSetSpeed)
		{
			pBuffInfo.m_nSkillID = nSkillID;
			pBuffInfo.m_bBuffType = pType->bBuffType;
			pBuffInfo.m_bIsBuff = pType->bIsBuff;

			pBuffInfo.m_bDurationExtended = false;
			pBuffInfo.m_tEndTime = UNIXTIME + sDuration;

			// Add the buff into the buff map.
			pTarget->AddType4Buff(pType->bBuffType, pBuffInfo);
		}

		// Update character stats.
		if (pTarget->isPlayer())
		{
			TO_USER(pTarget)->SetUserAbility();

			if (pType->isBuff() && pType->bBuffType == BUFF_TYPE_HP_MP && bIsRecastingSavedMagic)
				TO_USER(pTarget)->HpChange(pTarget->m_sMaxHPAmount);

			
			
		}
	fail_return:
		if (pSkill->bType[1] == 0 || pSkill->bType[1] == 4)
		{
			Unit* pTmp = nullptr;
			if (pType->bBuffType == BUFF_TYPE_HP_MP)
			{
				if (!pSkillCaster->isInRangeSlow(pTarget, pType->bRadius))
					pTmp = pTarget;
				else
					pTmp = (pSkillCaster->isPlayer() ? pSkillCaster : pTarget);
			}
			else
				pTmp = (pSkillCaster->isPlayer() ? pSkillCaster : pTarget);

			int16 sDataCopy[] =
			{
				sData[0], bResult, sData[2], (int16)sDuration,
				sData[4], pType->bSpeed, sData[6]
			};

			if (bSetSpeed)
			{
				if (pTarget->isPlayer() && pType->bBuffType == BUFF_TYPE_SPEED2)
				{
					Guard lock(pTarget->m_buffLock);
					bool isSpeedBuffExist = false;
					foreach(sk, pTarget->m_buffMap)
					{
						if (sk->second.m_nSkillID == 490230
							|| sk->second.m_nSkillID == 208725
							|| sk->second.m_nSkillID == 207725
							|| sk->second.m_nSkillID == 107725
							|| sk->second.m_nSkillID == 108725
							|| sk->second.m_nSkillID == 490223)
						{
							isSpeedBuffExist = true;
							break;
						}
					}
					if (isSpeedBuffExist)
					{
						CMagicProcess::RemoveType4Buff(BUFF_TYPE_SPEED, pTarget, false);

						Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_DURATION_EXPIRED));
						result << BUFF_TYPE_SPEED;
						TO_USER(pTarget)->Send(&result);
					}
				}

				BuildAndSendSkillPacket(pTmp, true, sCasterID, pTarget->GetID(), bOpcode, nSkillID, sDataCopy);

				if (pSkillCaster->isDevil() && kurianDemolition())
				{
					CMagicProcess::RemoveType4Buff(BUFF_TYPE_DEVIL_TRANSFORM, TO_USER(pSkillCaster));
				}

				if ((pSkill->bMoral >= MORAL_ENEMY || pSkill->bMoral >= MORAL_AREA_ENEMY)
					&& pTarget->isPlayer())
				{
					UserStatus status = USER_STATUS_POISON;
					if (pType->bBuffType == BUFF_TYPE_SPEED
						|| pType->bBuffType == BUFF_TYPE_SPEED2)
						status = USER_STATUS_SPEED;

					if (!bBlockingDebuffs)
						TO_USER(pTarget)->SendUserStatusUpdate(status, USER_STATUS_INFLICT);
				}
			}
			else if (!bSetSpeed && (pSkill->bType[1] == 4 || pSkill->bType[1] == 0))
			{
				if (sTargetID != -1 || pSkill->sSkillLevel == 33)
				{

					Packet result(WIZ_EFFECT);
					result << pTarget->GetID() << nSkillID;
					pTarget->SendToRegion(&result);
				}
			}

			if (pType->bBuffType == BUFF_TYPE_DECREASE_RESIST
				|| pType->bBuffType == BUFF_TYPE_DISABLE_TARGETING
				|| pType->bBuffType == BUFF_TYPE_RESIS_AND_MAGIC_DMG)
				SendSkill();
		}

		if (bResult == 0
			&& pSkillCaster->isPlayer())
		{
			if (pType->isDebuff())
				return false;
			else
				SendSkillFailed((*itr)->GetID());
		}
	}

	if (pSkill->bMoral >= MORAL_ALL)
		MagicInstance::SendSkill();

	return true;
}

bool MagicInstance::ExecuteType5()
{
	// Disallow anyone that isn't a player.
	if (!pSkillCaster->isPlayer()
		|| pSkill == nullptr)
		return false;

	_MAGIC_TYPE5* pType = g_pMain->m_Magictype5Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	vector<CUser*> casted_member;

	// Targeting a group of people (e.g. party)
	if (sTargetID == -1)
	{
		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser* pTUser = TO_USER(itr->second);
			if (!pTUser->isInGame())
				continue;

			// If the target's dead, only allow resurrection/self-resurrection spells.
			if (pTUser->isDead()
				&& (pType->bType != RESURRECTION
					&& pType->bType != RESURRECTION_SELF
					&& pType->bType != LIFE_CRYSTAL))
				continue;

			// If the target's alive, we don't need to resurrect them.
			if (!pTUser->isDead()
				&& (pType->bType == RESURRECTION
					|| pType->bType == RESURRECTION_SELF
					|| pType->bType == LIFE_CRYSTAL))
				continue;

			// Ensure the target's applicable for this skill.
			if (CMagicProcess::UserRegionCheck(pSkillCaster, pTUser, pSkill, pSkill->sRange, sData[0], sData[2]))
				casted_member.push_back(pTUser);
		}
	}
	// Targeting a single person
	else
	{
		if (pSkillTarget == nullptr
			|| !pSkillTarget->isPlayer())
			return false;

		// If the target's dead, only allow resurrection/self-resurrection spells.
		if (pSkillTarget->isDead()
			&& (pType->bType != RESURRECTION
				&& pType->bType != RESURRECTION_SELF
				&& pType->bType != LIFE_CRYSTAL))
			return false;

		// If the target's alive, we don't need to resurrect them.
		if (!pSkillTarget->isDead()
			&& (pType->bType == RESURRECTION
				|| pType->bType == RESURRECTION_SELF
				|| pType->bType == LIFE_CRYSTAL))
			return false;

		casted_member.push_back(TO_USER(pSkillTarget));
	}

	foreach(itr, casted_member)
	{
		Type4BuffMap::iterator buffIterator;
		CUser* pTUser = (*itr);

		if (pTUser == nullptr)
			continue;

		int skillCount = 0;
		bool bRemoveDOT = false;

		switch (pType->bType)
		{
			// Remove all DOT skills
		case REMOVE_TYPE3:
			for (int i = 0; i < MAX_TYPE3_REPEAT; i++)
			{
				Unit::MagicType3* pEffect = &pTUser->m_durationalSkills[i];

				if (pEffect == nullptr)
					continue;

				if (!pEffect->m_byUsed)
					continue;

				// Ignore healing-over-time skills
				if (pEffect->m_sHPAmount >= 0)
				{
					skillCount++;
					continue;
				}

				pEffect->Reset();
				// TODO: Wrap this up (ugh, I feel so dirty)
				Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_DURATION_EXPIRED));
				result << uint8(200); // removes DOT skill
				pTUser->Send(&result);
				bRemoveDOT = true;
			}

			if (skillCount == 0)
			{
				pTUser->m_bType3Flag = false;
				if (bRemoveDOT)
					pTUser->SendUserStatusUpdate(USER_STATUS_DOT, USER_STATUS_CURE);
			}
			break;

		case REMOVE_TYPE4: // Remove type 4 debuffs
		{
			Guard lock(pTUser->m_buffLock);
			Type4BuffMap buffMap = pTUser->m_buffMap; // copy the map so we can't break it while looping

			foreach(itr, buffMap)
			{
				if (itr->second.isDebuff())
				{
					CMagicProcess::RemoveType4Buff(itr->first, pTUser);

					if (pTUser->isLockableScroll(itr->second.m_bBuffType))
						pTUser->RecastLockableScrolls(itr->second.m_bBuffType);
				}
			}

			// NOTE: This originally checked to see if there were any active debuffs.
			// As this seems pointless (as we're removing all of them), it was removed
			// however leaving this note in, in case this behaviour in certain conditions
			// is required.
			pTUser->SendUserStatusUpdate(USER_STATUS_POISON, USER_STATUS_CURE);
		} break;

		case RESURRECTION_SELF:
			if (pSkillCaster != pTUser)
				continue;

			pTUser->Regene(INOUT_IN, nSkillID);
			break;

		case LIFE_CRYSTAL:
			if (pSkillCaster != pTUser)
				continue;

			pTUser->Regene(INOUT_IN, nSkillID);
			pTUser->ItemWoreOut(REPAIR_ALL, MAX_DAMAGE);
			break;

		case RESURRECTION:
			if (pTUser->CheckExistItem(pSkill->iUseItem, pType->sNeedStone))
			{
				if (pTUser->RobItem(pSkill->iUseItem, pType->sNeedStone))
				{
					TO_USER(pSkillCaster)->GiveItem(pSkill->iUseItem, (pType->sNeedStone / 2) + 1);
					pTUser->Regene(INOUT_IN, nSkillID);
				}
			}
			break;

		case REMOVE_BLESS:
		{
			if (CMagicProcess::RemoveType4Buff(BUFF_TYPE_HP_MP, pTUser))
			{
				if (!pTUser->isDebuffed())
					pTUser->SendUserStatusUpdate(USER_STATUS_POISON, USER_STATUS_CURE);
			}
		} break;
		}

		if (pSkill->bType[1] == 0 || pSkill->bType[1] == 5)
		{
			// Send the packet to the caster.
			sData[1] = 1;
			BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
		}
	}

	return true;
}

bool MagicInstance::ExecuteType6()
{
	if (pSkill == nullptr
		|| !pSkillCaster->isPlayer())
		return false;

	_MAGIC_TYPE6* pType = g_pMain->m_Magictype6Array.GetData(nSkillID);

	CUser* pCaster = TO_USER(pSkillCaster);
	uint16 sDuration = 0;

	if (pType != nullptr)
	{
		if (pType->bUserSkillUse == TransformationSkillUseMonster)
		{
			if ((pSkillCaster->GetMap()->canAttackOtherNation() && !(pSkillCaster->isTransformationMonsterInZone()))
				|| (!bIsRecastingSavedMagic && pCaster->isTransformed())
				|| (pSkillCaster->isBuffed(true))
				|| (pType->bNation != 0 && pType->bNation != pCaster->GetNation()))
			{
				if (bIsRecastingSavedMagic)
					Type6Cancel(true);
				return false;
			}
		}

		if (pType->bUserSkillUse == TransformationSkillUseNPC
			|| pType->bUserSkillUse == TransformationSkillUseMamaPag
			|| pType->bUserSkillUse == TransformationSkillUseSpecial)
		{
			if (pType->bNation != pCaster->GetNation() || (!bIsRecastingSavedMagic && pCaster->isTransformed()))
			{
				if (bIsRecastingSavedMagic)
					Type6Cancel(true);
				return false;
			}
		}

		if (pType->bUserSkillUse == TransformationSkillUseSiege)
		{

			if (nSkillID == 450001)
			{
				if (pSkillCaster->GetZoneID() != ZONE_DELOS &&
					pSkillCaster->GetZoneID() != ZONE_BATTLE2 &&
					pSkillCaster->GetZoneID() != ZONE_BATTLE3)
					return false;
			}
			else if (nSkillID == 450003)
			{
				if (pSkillCaster->GetZoneID() != ZONE_BATTLE2)
					return false;

			}
			else
				if (pSkillCaster->GetZoneID() != ZONE_DELOS)
					return false;

			if (pSkillCaster->GetMap()->canAttackOtherNation()
				&& (!bIsRecastingSavedMagic && pCaster->isTransformed())
				|| (pSkillCaster->isBuffed(true))
				|| (pType->bNation != 0 && pType->bNation != pCaster->GetNation()))
			{
				if (bIsRecastingSavedMagic)
					Type6Cancel(true);
				return false;
			}
		}

		if (pType->bUserSkillUse == TransformationSkillMovingTower)
		{
			if (pSkillCaster->GetZoneID() != ZONE_DELOS)
				return false;

			if (!g_pMain->pCswEvent.Started
				|| g_pMain->pCswEvent.Status != CswOperationStatus::CastellanWar)
				return false;

			if ((pSkillCaster->GetMap()->canAttackOtherNation() && !(pSkillCaster->GetZoneID() == ZONE_DELOS))
				|| (!bIsRecastingSavedMagic && pCaster->isTransformed())
				|| (pSkillCaster->isBuffed(true))
				|| (pType->bNation != 0 && pType->bNation != pCaster->GetNation()))
			{
				if (bIsRecastingSavedMagic)
					Type6Cancel(true);
				return false;
			}
		}
	}

	// We can ignore all these checks if we're just recasting on relog.
	if (!bIsRecastingSavedMagic)
	{
		if (pSkillTarget->HasSavedMagic(nSkillID))
			return false;

		// User's casting a new skill. Use the full duration.
		sDuration = pType->sDuration;
	}
	else
	{
		// Server's recasting the skill (kept on relog, zone change, etc.)
		int16 tmp = pSkillCaster->GetSavedMagicDuration(nSkillID);

		// Has it expired (or not been set?) -- just in case.
		if (tmp <= 0)
			return false;

		// it's positive, so no harm here in casting.
		sDuration = tmp;
	}

	switch (pType->bUserSkillUse)
	{
	case TransformationSkillUseMonster:
		pCaster->m_transformationType = Unit::TransformationMonster;
		break;

	case TransformationSkillUseNPC:
	case TransformationSkillUseMamaPag:
		pCaster->m_transformationType = Unit::TransformationNPC;
		break;

	case TransformationSkillUseSiege:
	case TransformationSkillMovingTower:
	case TransformationSkillOreadsGuard:
		pCaster->m_transformationType = Unit::TransformationSiege;
		break;

	default: // anything 
		return false;
	}

	// TODO : Save duration, and obviously end
	pCaster->m_sTransformID = pType->sTransformID;
	pCaster->m_sTransformSkillID = pType->iNum;
	pCaster->m_tTransformationStartTime = UNIXTIME2; // in milliseconds
	pCaster->m_sTransformationDuration = ULONGLONG(sDuration) * 1000; // in milliseconds
	pCaster->m_sTransformHpchange = true;
	pCaster->m_sTransformMpchange = true;

	pSkillCaster->StateChangeServerDirect(3, nSkillID);

	sData[1] = 1;
	sData[3] = sDuration;
	SendSkill();

	// TODO : Give the users ALL TEH BONUSES!!
	if (pType->bUserSkillUse == TransformationSkillUseSiege
		|| pType->bUserSkillUse == TransformationSkillMovingTower
		|| pType->bUserSkillUse == TransformationSkillOreadsGuard)
	{
		pCaster->m_sTotalHit = pType->sTotalHit;
		pCaster->m_sTotalAc = pType->sTotalAc;
		pCaster->m_MaxHp = pType->sMaxHp;
		pCaster->m_MaxMp = pType->sMaxMp;
		pCaster->m_sSpeed = pType->bSpeed;
		pCaster->m_sFireR = pType->sTotalFireR;
		pCaster->m_sColdR = pType->sTotalColdR;
		pCaster->m_sLightningR = pType->sTotalLightningR;
		pCaster->m_sMagicR = pType->sTotalMagicR;
		pCaster->m_sDiseaseR = pType->sTotalDiseaseR;
		pCaster->m_sPoisonR = pType->sTotalPoisonR;
		pCaster->SetUserAbility();
	}

	pSkillCaster->InsertSavedMagic(nSkillID, sDuration);
	return true;
}

bool MagicInstance::ExecuteType7()
{
	if (pSkill == nullptr)
		return false;

	_MAGIC_TYPE7* pType = g_pMain->m_Magictype7Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	vector<Unit*> casted_member;

	int damage = pType->sDamage;

	if (sTargetID == -1)
	{
		std::vector<uint16> unitList;

		g_pMain->GetUnitListFromSurroundingRegions(pSkillCaster, &unitList);
		foreach(itr, unitList)
		{
			Unit* pTarget = g_pMain->GetUnitPtr(*itr, sSkillCasterZoneID);

			if (pTarget == nullptr)
				continue;

			if (pTarget->isPlayer() && TO_USER(pTarget)->isGM())
				continue;

			if (pSkillCaster != pTarget && !pTarget->isDead() && !pTarget->isBlinking() && pTarget->isAttackable()
				&& CMagicProcess::UserRegionCheck(pSkillCaster, pTarget, pSkill, pType->bRadius, sData[0], sData[2]))
				casted_member.push_back(pTarget);
		}

		// If you managed to not hit anything with your AoE, you're still gonna have a cooldown (You should l2aim)
		if (casted_member.empty() || (sTargetID == -1 && casted_member.empty()))
		{
			SendSkill();
			return false;
		}
	}
	else
	{
		// If the target was a single unit.
		if (pSkillTarget == nullptr
			|| pSkillTarget->isDead()
			|| (pSkillTarget->isPlayer()
				&& TO_USER(pSkillTarget)->isBlinking()))
			return false;

		casted_member.push_back(pSkillTarget);
	}

	foreach(itr, casted_member)
	{
		Unit* pTarget = *itr;

		if (pTarget == nullptr || pTarget->isDead())
			continue;

		/*if (pSkill->sRange > 0
			&& (pSkillCaster->GetDistanceSqrt(pTarget) >= (float)pSkill->sRange))
			continue;*/

		if (pType->bTargetChange == 0)
		{
			if (pSkillCaster->isNPC())
			{
				CUser* pUser = g_pMain->GetUserPtr(pTarget->GetID());
				if (pUser == nullptr)
				{
					CNpc* pNpc = g_pMain->GetNpcPtr(pTarget->GetID(), pTarget->GetZoneID());
					if (pNpc == nullptr)
						return false;

					TO_NPC(pSkillCaster)->ChangeNTarget(pNpc);
				}
				else
					TO_NPC(pSkillCaster)->ChangeTarget(0, pUser);
			}
		}

		if (pType->bTargetChange == 1)
		{
			if (pSkillCaster->isNPC())
			{
				if (TO_NPC(pSkillCaster)->isPet())
				{
					CUser* pUser = g_pMain->GetUserPtr(TO_NPC(pSkillCaster)->GetPetID());

					if (pUser == nullptr)
						return false;

					if (pUser->m_PettingOn == nullptr)
						return false;

					pUser->m_PettingOn->sAttackStart = true;
					pUser->m_PettingOn->sAttackTargetID = pTarget->GetID();
					return false;
				}
			}

			if (damage < 0)
				continue;

			if (pTarget->isPlayer())
				continue;

			pTarget->HpChange(-damage, pSkillCaster);
			BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, pTarget->GetID(), bOpcode, nSkillID, sData);
		}

		if (pType->bTargetChange == 2)
		{
			if (!pTarget->isNPC())
				return false;

			pTarget->StateChangeServerDirect(1, 4);
			TO_NPC(pTarget)->m_tFaintingTime = UNIXTIME + pType->sDuration;
			BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, TO_NPC(pTarget)->GetID(), bOpcode, nSkillID, sData);
		}
	}

	return true;
}

bool MagicInstance::ExecuteType8()
{
	if (pSkill == nullptr)
		return false;

	C3DMap* pTUserZoneInfo = g_pMain->m_ZoneArray.GetData(pSkillCaster->GetZoneID());
	if (pTUserZoneInfo == nullptr)
		return false;

	_MAGIC_TYPE8* pType = g_pMain->m_Magictype8Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	vector<Unit*> casted_member;

	if (sTargetID == -1)
	{
		// TODO: Localise this loop to make it not suck (the life out of the server).
		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			Unit* pTUser = TO_USER(itr->second);
			if (pTUser == nullptr
				|| (pTUser->isPlayer()
					&& !TO_USER(pTUser)->isInGame()))
				continue;

			if (CMagicProcess::UserRegionCheck(pSkillCaster, pTUser, pSkill, pType->sRadius, sData[0], sData[2]))
				casted_member.push_back(pTUser);
		}

		if (casted_member.empty())
			return false;
	}
	else
	{	// If the target was another single player.
		Unit* pTUser = g_pMain->GetUnitPtr(sTargetID, sSkillCasterZoneID);
		if (pTUser == nullptr
			|| (pTUser->isPlayer()
				&& !TO_USER(pTUser)->isInGame()))
			return false;

		if (pSkill->bMoral == MORAL_PARTY
			&& pSkillCaster == pTUser)
			casted_member.push_back(pTUser);
		else if (pSkill->bMoral == MORAL_PARTY_ALL
			&& CMagicProcess::UserRegionCheck(pSkillCaster, pTUser, pSkill, pType->sRadius, sData[0], sData[2]))
			casted_member.push_back(pTUser);
		else
			casted_member.push_back(pTUser);
	}

	foreach(itr, casted_member)
	{
		Unit* pTUser = *itr;

		if (pTUser == nullptr
			|| (pTUser->isPlayer()
				&& !TO_USER(pTUser)->isInGame()))
			continue;

		uint8 bResult = 0;
		_OBJECT_EVENT* pEvent = nullptr;

		if (pType->bWarpType != 11)
		{   // Warp or summon related: targets CANNOT be dead.
			if (pTUser->isDead()
				// Players can have their teleports blocked by skills.
				|| !pTUser->canTeleport())
			{
				sData[1] = 0;
				BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				continue;
			}
		}
		// Resurrection related: we're reviving DEAD targets.
		else if (!pTUser->isDead())
		{
			sData[1] = 0;
			BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
			continue;
		}

		// Is target already warping?			
		if (pTUser->isPlayer() && TO_USER(pTUser)->m_bWarp)
		{
			sData[1] = 0;
			BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
			continue;
		}

		switch (pType->bWarpType)
		{
		case 1:	// Send source to resurrection point. Disable gate escape etc for forgetten temple and pvp zones...
		{
			if (nSkillID == 109035
				|| nSkillID == 110035
				|| nSkillID == 209035
				|| nSkillID == 210035)
			{
				if (pSkillCaster->isInLufersonCastle()
					|| pSkillCaster->isInElmoradCastle())
				{
					if (g_pMain->m_byBattleOpen == NATION_BATTLE)
					{
						SendSkillFailed();
						return false;
					}
				}

				if (pTUserZoneInfo->isEscapeZone() != 1)
				{
					SendSkillFailed();
					return false;
				}
			}

			if (nSkillID == 109015
				|| nSkillID == 110015
				|| nSkillID == 111700
				|| nSkillID == 112700
				|| nSkillID == 209015
				|| nSkillID == 210015
				|| nSkillID == 211700
				|| nSkillID == 212700)
			{
				if (pTUserZoneInfo->isGateZone() != 1)
				{
					SendSkillFailed();
					return false;
				}
			}

			// Send the packet to the target.
			sData[1] = 1;
			BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);

			if (pTUser->GetMap() == nullptr)
				continue;

			pEvent = pTUser->GetMap()->GetObjectEvent(TO_USER(pTUser)->m_sBind);

			if (pEvent != nullptr)
				TO_USER(pTUser)->Warp(uint16(pEvent->fPosX * 10), uint16(pEvent->fPosZ * 10));
			else
			{
				if (pTUser->GetZoneID() == ZONE_JURAID_MOUNTAIN)
				{
					if (TO_USER(pTUser)->isEventUser())
					{
						_JURAID_ROOM_INFO* pRoomInfo = g_pMain->m_TempleEventJuraidRoomList.GetData(TO_USER(pTUser)->GetEventRoom());
						if (pRoomInfo != nullptr)
						{
							if (TO_USER(pTUser)->GetNation() == ELMORAD)
							{
								if (pRoomInfo->m_sElmoBridgesTimerOpen[0])
								{
									if (TO_USER(pTUser)->isBridgeStage2())
									{
										float x = 0.0f, z = 0.0f;
										x = float(695 + myrand(0, 3));
										z = float(171 + myrand(0, 3));

										TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
									}
									else if (TO_USER(pTUser)->isDevaStage())
									{
										float x = 0.0f, z = 0.0f;
										x = float(511 + myrand(0, 3));
										z = float(281 + myrand(0, 3));

										TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
									}
									else
									{
										if (!pRoomInfo->m_sElmoBridgesTimerOpen[1]
											&& !pRoomInfo->m_sElmoBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(800 + myrand(0, 3));
											z = float(349 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else if (pRoomInfo->m_sElmoBridgesTimerOpen[1]
											&& !pRoomInfo->m_sElmoBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(695 + myrand(0, 3));
											z = float(171 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else if (pRoomInfo->m_sElmoBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(511 + myrand(0, 3));
											z = float(281 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else
										{
											_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
											if (pStartPosition)
												TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
											else
												return false;
										}
									}
								}
								else
								{
									if (pRoomInfo->m_sElmoBridgesTimerOpen[1])
									{
										if (TO_USER(pTUser)->isDevaStage())
										{
											float x = 0.0f, z = 0.0f;
											x = float(511 + myrand(0, 3));
											z = float(281 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else
										{
											if (pRoomInfo->m_sElmoBridgesTimerOpen[1]
												&& !pRoomInfo->m_sElmoBridgesTimerOpen[2])
											{
												float x = 0.0f, z = 0.0f;
												x = float(695 + myrand(0, 3));
												z = float(171 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else if (pRoomInfo->m_sElmoBridgesTimerOpen[2])
											{
												float x = 0.0f, z = 0.0f;
												x = float(511 + myrand(0, 3));
												z = float(281 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else
											{
												_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
												if (pStartPosition)
													TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
												else
													return false;
											}
										}
									}
									else
									{
										if (pRoomInfo->m_sElmoBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(511 + myrand(0, 3));
											z = float(281 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else
										{
											if (TO_USER(pTUser)->isBridgeStage1())
											{
												float x = 0.0f, z = 0.0f;
												x = float(800 + myrand(0, 3));
												z = float(349 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else if (TO_USER(pTUser)->isBridgeStage2())
											{
												float x = 0.0f, z = 0.0f;
												x = float(695 + myrand(0, 3));
												z = float(171 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else if (TO_USER(pTUser)->isDevaStage())
											{
												float x = 0.0f, z = 0.0f;
												x = float(511 + myrand(0, 3));
												z = float(281 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else
											{
												_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
												if (pStartPosition)
													TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
												else
													return false;
											}
										}
									}
								}
							}
							else if (TO_USER(pTUser)->GetNation() == KARUS)
							{
								if (pRoomInfo->m_sKarusBridgesTimerOpen[0])
								{
									if (TO_USER(pTUser)->isBridgeStage2())
									{
										float x = 0.0f, z = 0.0f;
										x = float(333 + myrand(0, 3));
										z = float(848 + myrand(0, 3));

										TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
									}
									else if (TO_USER(pTUser)->isDevaStage())
									{
										float x = 0.0f, z = 0.0f;
										x = float(512 + myrand(0, 3));
										z = float(743 + myrand(0, 3));

										TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
									}
									else
									{
										if (!pRoomInfo->m_sKarusBridgesTimerOpen[1] && !pRoomInfo->m_sKarusBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(224 + myrand(0, 3));
											z = float(671 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else if (pRoomInfo->m_sKarusBridgesTimerOpen[1] && !pRoomInfo->m_sKarusBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(333 + myrand(0, 3));
											z = float(848 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else if (pRoomInfo->m_sKarusBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(512 + myrand(0, 3));
											z = float(743 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else
										{
											_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
											if (pStartPosition)
												TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
											else
												return false;
										}
									}
								}
								else
								{
									if (pRoomInfo->m_sKarusBridgesTimerOpen[1])
									{
										if (TO_USER(pTUser)->isDevaStage())
										{
											float x = 0.0f, z = 0.0f;
											x = float(512 + myrand(0, 3));
											z = float(743 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else
										{
											if (pRoomInfo->m_sKarusBridgesTimerOpen[1] && !pRoomInfo->m_sKarusBridgesTimerOpen[2])
											{
												float x = 0.0f, z = 0.0f;
												x = float(333 + myrand(0, 3));
												z = float(848 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else if (pRoomInfo->m_sKarusBridgesTimerOpen[2])
											{
												float x = 0.0f, z = 0.0f;
												x = float(512 + myrand(0, 3));
												z = float(743 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else
											{
												_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
												if (pStartPosition)
													TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
												else
													return false;
											}
										}
									}
									else
									{
										if (pRoomInfo->m_sKarusBridgesTimerOpen[2])
										{
											float x = 0.0f, z = 0.0f;
											x = float(512 + myrand(0, 3));
											z = float(743 + myrand(0, 3));

											TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
										}
										else
										{
											if (TO_USER(pTUser)->isBridgeStage1())
											{
												float x = 0.0f, z = 0.0f;
												x = float(224 + myrand(0, 3));
												z = float(671 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else if (TO_USER(pTUser)->isBridgeStage2())
											{
												float x = 0.0f, z = 0.0f;
												x = float(333 + myrand(0, 3));
												z = float(848 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else if (TO_USER(pTUser)->isDevaStage())
											{
												float x = 0.0f, z = 0.0f;
												x = float(512 + myrand(0, 3));
												z = float(743 + myrand(0, 3));

												TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
											}
											else
											{
												_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
												if (pStartPosition)
													TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
												else
													return false;
											}
										}
									}
								}
							}
							else
							{
								_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
								if (pStartPosition)
									TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
								else
									return false;
							}
						}
						else
						{
							_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
							if (pStartPosition)
								TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
							else
								return false;
						}
					}
					else
					{
						_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
						if (pStartPosition)
							TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
						else
							return false;
					}
				}
				else
				{
					if (pTUser->GetZoneID() == ZONE_DELOS)
					{
						if (g_pMain->pCswEvent.Status == CswOperationStatus::DeathMatch)
						{
							_DEATHMATCH_BARRACK_INFO* pData4 = g_pMain->m_KnightSiegeWarBarrackList.GetData(TO_USER(pTUser)->GetClanID());
							_DEATHMATCH_WAR_INFO* pKnightList = g_pMain->m_KnightSiegeWarClanList.GetData(TO_USER(pTUser)->GetClanID());
							if (pData4 != nullptr && pKnightList != nullptr)
							{
								if (pKnightList->m_tRemainBarracks == 1)
								{
									float x = 0.0f, z = 0.0f;
									x = float(pData4->m_tBaseX + myrand(1, 5));
									z = float(pData4->m_tBaseZ + myrand(1, 5));

									TO_USER(pTUser)->Warp(uint16(x * 10), uint16(z * 10));
								}
								else
								{
									_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
									if (pStartPosition)
										TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
									else
										return false;
								}
							}
							else
							{
								_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
								if (pStartPosition)
									TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
								else
									return false;
							}
						}
						else
						{
							_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
							if (pStartPosition)
								TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
							else
								return false;
						}
					}
					else
					{
						if (pTUser->GetZoneID() <= ELMORAD || pTUser->GetMap()->isWarZone() || pTUser->GetMap()->canAttackOtherNation())
						{
							_START_POSITION* pStartPosition = g_pMain->m_StartPositionArray.GetData(pTUser->GetZoneID());
							if (pStartPosition)
								TO_USER(pTUser)->Warp((uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX)) * 10, (uint16)((pTUser->GetNation() == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)) * 10);
							else
								return false;
						}
						else
						{
							TO_USER(pTUser)->Warp(uint16(pTUser->GetMap()->m_fInitX * 10), uint16(pTUser->GetMap()->m_fInitZ * 10));
						}
					}
				}
			}
		}
		break;
		case 2: // Chamber of Teleport Skills / BATTLE & BIFROST
		{
			_MAGIC_TYPE8* pType = g_pMain->m_Magictype8Array.GetData(nSkillID);
			if (pType == nullptr)
				return false;

			if (pSkillCaster->GetZoneID() != ZONE_BIFROST
				&& pSkillCaster->GetZoneID() != ZONE_BATTLE2)
				return false;

			if (pSkillCaster->isPlayer() && pSkillCaster->isAlive())
			{
				if (pSkillCaster->GetZoneID() == ZONE_BIFROST)
				{
					if (nSkillID == 490301) //Chamber of Arrogance Transport Scroll
					{
						uint16 x, z;
						x = 227;
						z = 819;

						TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
					}
					else if (nSkillID == 490302) //Chamber of Gluttony Transport Scroll
					{
						uint16 x, z;
						x = 770;
						z = 818;

						TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
					}
					else if (nSkillID == 490303) //Chamber of Rage Transport Scroll
					{
						uint16 x, z;
						x = 671;
						z = 355;

						TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
					}
					else if (nSkillID == 490304) //Chamber of Sloth Transport Scroll
					{
						uint16 x, z;
						x = 498;
						z = 396;

						TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
					}
					else if (nSkillID == 490305) //Chamber of Lechery Transport Scroll
					{
						uint16 x, z;
						x = 102;
						z = 140;

						TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
					}
					else if (nSkillID == 490306) //Chamber of Jealousy Transport Scroll
					{
						uint16 x, z;
						x = 440;
						z = 188;

						TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
					}
					else if (nSkillID == 490307) //Chamber of Avarice Transport Scroll
					{
						uint16 x, z;
						x = 712;
						z = 183;

						TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
					}
				}
				else if (pSkillCaster->GetZoneID() == ZONE_BATTLE2)
				{
					if (nSkillID == 490301 || nSkillID == 490302 || nSkillID == 490303
						|| nSkillID == 490304 || nSkillID == 490305 || nSkillID == 490306
						|| nSkillID == 490307)
					{
						if (pSkillCaster->GetNation() == ELMORAD)
						{
							uint16 x, z;
							x = 600;
							z = 340;

							TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
						}
						else if (pSkillCaster->GetNation() == KARUS)
						{
							uint16 x, z;
							x = 394;
							z = 632;

							TO_USER(pSkillCaster)->Warp(x * 10, z * 10);
						}
					}
				}
				else
				{
					sData[1] = 0;
					BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
					return false;
				}
			}
			else
			{
				sData[1] = 0;
				BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				return false;
			}
		}
		break;
		case 3:
			//CASUSLUK
			break;
		case 5:		// Send target to a hidden zone.
			// LATER!!!
			break;
		case 11:// Resurrect a dead player.
		{
			if (!pSkillTarget->isAlive())
			{
				if (pSkillCaster->isPlayer()
					&& pSkillTarget->isPlayer())
				{
					// Send the packet to the target.
					sData[1] = 1;
					BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);

					TO_USER(pTUser)->m_bResHpType = USER_STANDING;     // Target status is officially alive now.
					TO_USER(pTUser)->HpChange(TO_USER(pTUser)->m_MaxHp, pSkillCaster);	 // Refill HP.	
					TO_USER(pTUser)->ExpChange(pType->sExpRecover / 100, true);    // Increase target experience.
				}
				else
					return false;
			}
			else
				return false;
		}
		break;
		case 12:// Summon a target within the zone.	Disable telepert for forgetten temple...
		{
			if (nSkillID == 490050 || nSkillID == 490042)
			{
				if (pSkillCaster->isInLufersonCastle()
					|| pSkillCaster->isInElmoradCastle())
				{
					if (g_pMain->m_byBattleOpen == NATION_BATTLE)
					{
						SendSkillFailed();
						return false;
					}
				}
				else
				{
					if (pTUserZoneInfo->isCallingFriendZone() != 1)
					{
						SendSkillFailed();
						return false;
					}
				}
			}
			else
			{
				if (pTUserZoneInfo->isTeleportFriendZone() != 1)
				{
					SendSkillFailed();
					return false;
				}
			}

			// Cannot teleport users from other zones.
			if (pSkillCaster->GetZoneID() != pTUser->GetZoneID())
			{
				sData[1] = 0;
				BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				return false;
			}

			if (pSkillCaster != pTUser)
			{
				// Send the packet to the target.
				sData[1] = 1;
				BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				TO_USER(pTUser)->Warp(pSkillCaster->GetSPosX(), pSkillCaster->GetSPosZ());
			}
			else
			{
				SendSkillFailed();
				return false;
			}
		}
		break;
		case 13:// Summon a target outside the zone.		
			// Different zone? 
			if (pSkillCaster->GetZoneID() == pTUser->GetZoneID())
			{
				sData[1] = 0;
				BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				return false;
			}

			// Send the packet to the target.
			sData[1] = 1;
			BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);

			TO_USER(pTUser)->ZoneChange(pSkillCaster->GetZoneID(), pSkillCaster->GetX(), pSkillCaster->GetZ());
			TO_USER(pTUser)->UserInOut(INOUT_RESPAWN);
			break;

		case 20:	// Teleport the source (radius) meters forward
		{
			/*float	warp_x = (float(sData[0])) / float(12.5), warp_z = (float(sData[2])) / float(12.5);// here is by default 10 with camera defual

			sData[1] = 1;
			BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
			TO_USER(pTUser)->Warp(uint16(warp_x * 10), uint16(warp_z * 10));*/
			// Calculate difference between where user is now and where they were previously
// to figure out an orientation.
// Should really use m_sDirection, but not sure what the value is exactly.
			float warp_x = pTUser->GetX() - TO_USER(pTUser)->m_oldx,
				warp_z = pTUser->GetZ() - TO_USER(pTUser)->m_oldz;

			// Unable to work out orientation, so we'll just fail (won't be necessary with m_sDirection).
			float	distance = sqrtf(warp_x * warp_x + warp_z * warp_z);
			if (distance == 0.0f)
			{
				sData[1] = 0;
				BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				return false;
			}

			warp_x /= distance; warp_z /= distance;
			warp_x *= pType->sRadius; warp_z *= pType->sRadius;
			warp_x += TO_USER(pTUser)->m_oldx; warp_z += TO_USER(pTUser)->m_oldz;

			sData[1] = 1;
			BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
			TO_USER(pTUser)->Warp(uint16(warp_x * 10), uint16(warp_z * 10));
		} break;

		case 21:// Summon a monster within a zone with monster staff.
		{
			MonsterSummonList* pMonsterSummonListStaff = g_pMain->m_MonsterSummonList.GetData(1);

			//bType = 1 : Monster summon Item
			if (nSkillID == 490088
				|| nSkillID == 490093
				|| nSkillID == 490096
				|| nSkillID == 490097)
			{
				if (pMonsterSummonListStaff)
				{
					int nCurrentMonster = 0;
					int nRandom = myrand(0, (int32)pMonsterSummonListStaff->size());

					for (std::vector<_MONSTER_SUMMON_LIST>::iterator itr = pMonsterSummonListStaff->begin(); itr != pMonsterSummonListStaff->end(); ++itr)
					{
						if (nCurrentMonster == nRandom)
						{
							g_pMain->SpawnEventNpc(itr->sSid, true, pSkillCaster->GetZoneID(), pSkillCaster->GetX(), pSkillCaster->GetY(), pSkillCaster->GetZ(), 1, (pType->sRadius / 1000));
							break;
						}
						nCurrentMonster++;
					}
				}
			}
			//bType = 2: Monster Summon Special Item
			if (nSkillID == 492015)
			{
				MonsterSummonList* pMonsterSummonListSpecialStaff = g_pMain->m_MonsterSummonList.GetData(2);
				if (pMonsterSummonListSpecialStaff)
				{
					int nCurrentMonster = 0;
					int nRandom = myrand(0, (int32)pMonsterSummonListSpecialStaff->size());

					for (std::vector<_MONSTER_SUMMON_LIST>::iterator itr = pMonsterSummonListSpecialStaff->begin(); itr != pMonsterSummonListSpecialStaff->end(); ++itr)
					{
						if (nCurrentMonster == nRandom)
						{
							g_pMain->SpawnEventNpc(itr->sSid, true, pSkillCaster->GetZoneID(), pSkillCaster->GetX(), pSkillCaster->GetY(), pSkillCaster->GetZ(), 1, (pType->sRadius / 1000));
							break;
						}
						nCurrentMonster++;
					}
				}
			}
			//bType = 3: Monster Summon Higher Staff
			if (nSkillID == 500202)
			{
				MonsterSummonList* pMonsterSummonListHigherStaff = g_pMain->m_MonsterSummonList.GetData(3);
				if (pMonsterSummonListHigherStaff)
				{
					int nCurrentMonster = 0;
					int nRandom = myrand(0, (int32)pMonsterSummonListHigherStaff->size());

					for (std::vector<_MONSTER_SUMMON_LIST>::iterator itr = pMonsterSummonListHigherStaff->begin(); itr != pMonsterSummonListHigherStaff->end(); ++itr)
					{
						if (nCurrentMonster == nRandom)
						{
							g_pMain->SpawnEventNpc(itr->sSid, true, pSkillCaster->GetZoneID(), pSkillCaster->GetX(), pSkillCaster->GetY(), pSkillCaster->GetZ(), 1, (pType->sRadius / 1000));
							break;
						}
						nCurrentMonster++;
					}
				}
			}
		}
		break;
		case 25:// This is used by Wild Advent (70 rogue skill) and Descent, teleport the user to the target user (Moral check to distinguish between the 2 skills)
		{
			float dest_x, dest_z = 0.0f;
			// If we're not even in the same zone, I can't teleport to you!
			if ((pSkill->bMoral != MORAL_ENEMY && pSkill->bMoral != MORAL_PARTY)
				|| (pSkill->iNum > 500000 && pSkillCaster->GetZoneID() > ZONE_MORADON5))
				return false;

			if (pTUser->GetZoneID() != pSkillCaster->GetZoneID()
				|| (pSkill->bMoral < MORAL_ENEMY && pSkillCaster->isHostileTo(pTUser)))
				return false;

			if (pSkill->iNum != 108770 && pSkill->iNum != 208770)
			{
				if (pTUserZoneInfo->isTeleportZone() != 1)
					return false;
			}

			dest_x = pTUser->GetX();
			dest_z = pTUser->GetZ();

			if (pSkillCaster->isPlayer() && (pSkillCaster->GetDistanceSqrt(pSkillTarget) <= (float)pType->sRadius))
				TO_USER(pSkillCaster)->Warp(uint16(dest_x * 10), uint16(dest_z * 10));
			else
				SendSkillFailed();
		}
		break;
		case 26:// Teleport the source (radius) meters forward
		{
			float value = TO_USER(pTUser)->Camerazoom + 9;

			float	warp_x = (float(sData[0])) / float(value), warp_z = (float(sData[2])) / float(value);// here is by default 10 with camera defual

			sData[1] = 1;
			BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
			TO_USER(pTUser)->Warp(uint16(warp_x * 10), uint16(warp_z * 10));

		} break;
		case 27:
		{
			if (pTUserZoneInfo->isTeleportZone() != 1)
				return false;

			float dest_x, dest_z = 0.0f;
			// If we're not even in the same zone, I can't teleport to you!

			if (pSkillCaster->GetZoneID() != pSkillTarget->GetZoneID()
				|| pSkillCaster->GetNation() != pSkillTarget->GetNation())
			{
				SendSkillFailed();
				return false;
			}

			dest_x = (TO_USER(pTUser)->curX1 / 10.0f + TO_USER(pTUser)->m_oldwillx / 10.0f) / 2;
			dest_z = (TO_USER(pTUser)->curZ1 / 10.0f + TO_USER(pTUser)->m_oldwillz / 10.0f) / 2;

			if (pSkillCaster->isPlayer())
				TO_USER(pSkillCaster)->Warp(uint16(dest_x * 10), uint16(dest_z * 10));
			else
				SendSkillFailed();
		}
		break;

		case 28:
			break;
		case 29:
		{

			g_pMain->pSoccerEvent.m_SoccerSocketID = TO_USER(pSkillCaster)->GetSocketID();

			float	warp_x = float(sData[0]), warp_z = float(sData[2]);// here is by default 10 with camera defual
			sData[1] = 1;
			BuildAndSendSkillPacket(*itr, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);

			TO_NPC(pTUser)->SendMoveResult(warp_x, 0, warp_z/*, distance*/);
		}
		break;
		case 30: // For Kurian Rush
		{
			if (pSkillTarget->isPlayer() && pSkillTarget->isAlive() && pSkillCaster->isPlayer())
			{
				if (pSkillTarget->isPlayer() && TO_USER(pSkillCaster)->CheckSkillPoint(PRO_SKILL1, 45, 83))
				{
					_MAGIC_TABLE* pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);
					if (pSkill == nullptr)
						return false;

					nSkillID += SKILLPACKET;
					pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);
					if (pSkill == nullptr)
						return false;

					ExecuteType4();

					_MAGIC_TYPE4* pType2 = g_pMain->m_Magictype4Array.GetData(nSkillID);
					nSkillID -= SKILLPACKET;

					if (pType2 == nullptr)
						return false;

					sData[1] = 1;
					BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				}
				else
				{
					sData[1] = 1;
					BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
				}
			}
			else
				return false;
		}
		break;
		case 31:// For Kurian Pull
		{
			if (pSkillTarget->isPlayer() && pSkillTarget->isAlive() && pSkillCaster->isPlayer())
			{
				bool luck = false;
				_MAGIC_TABLE* pSkill = g_pMain->m_MagictableArray.GetData(nSkillID);
				if (pSkill == nullptr)
					return false;

				if (75 <= myrand(0, 100))
					luck = false;
				else
					luck = true;

				sData[1] = luck;
				BuildAndSendSkillPacket(pSkillCaster, true, sCasterID, (*itr)->GetID(), bOpcode, nSkillID, sData);
			}
			else
				return false;
		}
		break;
		default:
			printf("Magic Instance ExecuteType8 Not Warp Type %d \n", pType->bWarpType);
			break;
		}
	}
	return true;
}

bool MagicInstance::ExecuteType9()
{
	if (pSkill == nullptr
		/*// Only players can use these skills.
			|| (!pSkillCaster->isPlayer() && sTargetID != -1)*/)
		return false;

	C3DMap* pTUserZoneInfo = g_pMain->m_ZoneArray.GetData(pSkillCaster->GetZoneID());
	if (pTUserZoneInfo == nullptr)
		return false;

	_MAGIC_TYPE9* pType = g_pMain->m_Magictype9Array.GetData(nSkillID);
	if (pType == nullptr)
		return false;

	Unit* pCaster = TO_USER(pSkillCaster);
	//CUser* pCaster = TO_USER(pSkillCaster);

	if (pCaster == nullptr)
		return false;

	Guard lock(pCaster->m_buffLock);
	Type9BuffMap& buffMap = pCaster->m_type9BuffMap;

	// Ensure this type of skill isn't already in use.
	if (pCaster->isPlayer() && buffMap.find(pType->bStateChange) != buffMap.end())
		return false;

	sData[1] = 1;

	if (pType->bStateChange <= 2 && pCaster->canStealth() && pCaster->GetZoneID() != ZONE_FORGOTTEN_TEMPLE)
	{
		// Cannot stealth when already stealthed.
		// This prevents us from using both types 1 and 2 (i.e. dispel on move & dispel on attack)
		// at the same time.
		if (TO_USER(pCaster)->m_bInvisibilityType != INVIS_NONE)
		{
			sData[1] = 0;
			SendSkillFailed();
			return false;
		}

		// Invisibility perk does NOT apply when using these skills on monsters.
		if (pSkillTarget->isPlayer())
		{
			Guard buffLock(pSkillTarget->m_buffLock);

			// If this user already has this skill active, we don't need to reapply it.
			if (pSkillTarget->m_type9BuffMap.find(pType->bStateChange)
				!= pSkillTarget->m_type9BuffMap.end())
				return false;

			pCaster->StateChangeServerDirect(7, pType->bStateChange); // Update the client to be invisible
			pSkillTarget->m_type9BuffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
			buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
		}

		// Update all players nearby to tell them we're now invisible.
		SendSkill();
	}
	else if (pType->bStateChange >= 3 && pType->bStateChange <= 4)
	{
		Packet result(WIZ_STEALTH, uint8(1));
		result << pType->sRadius;

		// If the player's in a party, apply this skill to all members of the party.
		if (TO_USER(pCaster)->isInParty() && pType->bStateChange == 4)
		{
			_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(TO_USER(pCaster)->GetPartyID());
			if (pParty == nullptr)
				return false;

			for (int i = 0; i < MAX_PARTY_USERS; i++)
			{
				CUser* pUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (pUser == nullptr)
					continue;

				Guard buffLock(pUser->m_buffLock);

				// If this user already has this skill active, we don't need to reapply it.
				if (pUser->m_type9BuffMap.find(pType->bStateChange)
					!= pUser->m_type9BuffMap.end())
					continue;

				pUser->m_type9BuffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				pUser->Send(&result);

				// Ensure every user in the party is given the skill icon in the corner of the screen.
				BuildAndSendSkillPacket(pUser, false, sCasterID, pUser->GetID(), bOpcode, nSkillID, sData);
			}
		}
		else // not in a party, so just apply this skill to us.
		{
			buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
			TO_USER(pCaster)->Send(&result);

			// Ensure we are given the skill icon in the corner of the screen.
			SendSkill(false); // only update us, as only we need to know that we can see invisible players.
		}
	}
	else if (pType->bStateChange == 9)
	{
		if (pTUserZoneInfo->isGuardSummonZone() != 1)
			return false;

		buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));

		g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, 2, pType->sDuration, pCaster->GetNation(), pCaster->GetID(), pCaster->GetEventRoom());


		int16 sDataCopy[] =
		{
			sData[0],sData[1],sData[2],int16(pType->sDuration),
			sData[4],sData[5],sData[6]
		};
		BuildAndSendSkillPacket(pCaster, false, sCasterID, pCaster->GetID(), bOpcode, nSkillID, sDataCopy);


	}
	else if (pType->bStateChange == 7)
	{
		buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
		g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, 2, pType->sDuration, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
	}
	else if (pType->bStateChange == 8)
	{
		if (pCaster->GetMap()->m_bPetSpawn != 1)
		{
			sData[1] = 0;
			SendSkillFailed();
			return false;
		}

		Guard Lock(g_pMain->m_PetSystemlock);
		PetDataSystemArray::iterator itr = g_pMain->m_PetDataSystemArray.find(TO_USER(pCaster)->GetItem(SHOULDER)->nSerialNum);
		if (itr == g_pMain->m_PetDataSystemArray.end()
			|| itr->second->info == nullptr)
		{
			sData[1] = 0;
			SendSkillFailed();
			return false;
		}

		CNpc* pPet = g_pMain->GetPetPtr(TO_USER(pCaster)->GetSocketID(), TO_USER(pCaster)->GetZoneID());
		if (pPet != nullptr)
		{
			sData[1] = 0;
			SendSkillFailed();
			return false;
		}

		TO_USER(pCaster)->m_PettingOn = itr->second;

		g_pMain->SpawnEventPet(MONSTER_KAUL_PET_SYSTEM, false, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, 0, -1, TO_USER(pCaster)->GetNation(), TO_USER(pCaster)->GetSocketID(), 0, 1, nSkillID);

		if (pType->sDuration == -1)
			pCaster->m_type9BuffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, -1)));
		else
			pCaster->m_type9BuffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
	}
	else if (pType->bStateChange == 10
		|| pType->bStateChange == 11
		|| pType->bStateChange == 12
		|| pType->bStateChange == 13)
	{
		switch (pType->bStateChange)
		{
		case 10:
			switch (pType->iNum)
			{
			case 502022:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 4, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502013:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 4, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502014:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 5, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502026:
			case 502027:
			case 502017:
			case 502018:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			default:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, pType->sDuration, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			}
			break;
		case 11:
			switch (pType->iNum)
			{
			case 502024:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 4, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502025:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 3, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502021:
			case 502031:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502019:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 5, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502028:
			case 502029:
			case 502030:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			default:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, pType->sDuration, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			}
			break;
		case 12:
			switch (pType->iNum)
			{
			case 502016:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			default:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, pType->sDuration, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			}
			break;
		case 13:
			switch (pType->iNum)
			{
			case 502023:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 4, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502015:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 5, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			case 502020:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, 0, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			default:
				buffMap.insert(std::make_pair(pType->bStateChange, _BUFF_TYPE9_INFO(nSkillID, UNIXTIME + pType->sDuration)));
				g_pMain->SpawnEventNpc(pType->sMonsterNum, true, pCaster->GetZoneID(), pCaster->GetX(), pCaster->GetY(), pCaster->GetZ(), 1, pType->sRadius, pType->sDuration, pType->bNationChange, pCaster->GetID(), pCaster->GetEventRoom());
				break;
			}
			break;
		default:
			break;
		}
	}
	return true;
}

short MagicInstance::GetMagicDamage(Unit* pTarget, int total_hit, int attribute)
{
	short damage = 0, temp_hit = 0, righthand_damage = 0, attribute_damage = 0;
	int random = 0, total_r = 0;
	uint8 result;

	if (pTarget == nullptr
		|| pSkillCaster == nullptr
		|| pTarget->isDead()
		|| pSkillCaster->isDead())
		return 0;

	// Trigger item procs
	if (pTarget->isPlayer()
		&& pSkillCaster->isPlayer())
	{
		pSkillCaster->OnAttack(pTarget, AttackTypeMagic);
		pTarget->OnDefend(pSkillCaster, AttackTypeMagic);
	}

	if (pTarget->m_bBlockMagic)
		return 0;

	int16 sMagicAmount = 0;
	if (pSkillCaster->isNPC())
		result = pSkillCaster->GetHitRate(pTarget->m_fTotalHitrate / pSkillCaster->m_fTotalEvasionrate + 2.0f);
	else
	{
		CUser* pUser = TO_USER(pSkillCaster);
		uint8 bCha = pUser->GetStat(STAT_CHA);
		if (bCha > 86)
			sMagicAmount = bCha - 86;

		sMagicAmount += pUser->m_sMagicAttackAmount;
		total_hit = total_hit * bCha / 186;
		result = SUCCESS;
	}

	if (result != FAIL)
	{
		// In case of SUCCESS.... 
		switch (attribute)
		{
		case FIRE_R:
			total_r = (pTarget->m_sFireR + pTarget->m_bAddFireR) * pTarget->m_bPctFireR / 100;
			total_r += pTarget->m_bResistanceBonus * pTarget->m_bPctFireR / 100;
			break;
		case COLD_R:
			total_r = (pTarget->m_sColdR + pTarget->m_bAddColdR) * pTarget->m_bPctColdR / 100;
			total_r += pTarget->m_bResistanceBonus * pTarget->m_bPctColdR / 100;
			break;
		case LIGHTNING_R:
			total_r = (pTarget->m_sLightningR + pTarget->m_bAddLightningR) * pTarget->m_bPctLightningR / 100;
			total_r += pTarget->m_bResistanceBonus * pTarget->m_bPctLightningR / 100;
			break;
		case MAGIC_R:
			total_r = (pTarget->m_sMagicR + pTarget->m_bAddMagicR) * pTarget->m_bPctMagicR / 100;
			total_r += pTarget->m_bResistanceBonus * pTarget->m_bPctMagicR / 100;
			break;
		case DISEASE_R:
			total_r = (pTarget->m_sDiseaseR + pTarget->m_bAddDiseaseR) * pTarget->m_bPctDiseaseR / 100;
			total_r += pTarget->m_bResistanceBonus * pTarget->m_bPctDiseaseR / 100;
			break;
		case POISON_R:
			total_r = (pTarget->m_sPoisonR + pTarget->m_bAddPoisonR) * pTarget->m_bPctPoisonR / 100;
			total_r += pTarget->m_bResistanceBonus * pTarget->m_bPctPoisonR / 100;
			break;
		default:
			total_r += pTarget->m_bResistanceBonus;
			break;
		}

		if (pSkillCaster->isPlayer())
		{
			CUser* pUser = TO_USER(pSkillCaster);

			// double the staff's damage when using a skill of the same attribute as the staff
			_ITEM_TABLE* pRightHand = pUser->GetItemPrototype(pUser->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND);
			if (!pUser->isWeaponsDisabled()
				&& pRightHand != nullptr
				&& pRightHand->isStaff()
				&& pUser->GetItemPrototype(pUser->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND) == nullptr)
			{
				Guard lock(pSkillCaster->m_equippedItemBonusLock);
				righthand_damage = pRightHand->m_sDamage + pUser->m_bAddWeaponDamage;
				auto itr = pSkillCaster->m_equippedItemBonuses.find(pUser->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND);
				if (itr != pSkillCaster->m_equippedItemBonuses.end())
				{
					auto bonusItr = itr->second.find(attribute);
					if (bonusItr != itr->second.end())
						attribute_damage = short(bonusItr->second * 1.5f);
				}
			}
			else
			{
				righthand_damage = 0;
			}

			// Add on any elemental skill damage
			Guard lock(pSkillCaster->m_equippedItemBonusLock);
			int16 sAmount = 0;
			foreach(itr, pSkillCaster->m_equippedItemBonuses)
			{
				uint8 bSlot = itr->first;
				auto bonusAtt = itr->second.find(attribute);
				if (bonusAtt == itr->second.end()
					|| bSlot == (pUser->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_RIGHTHAND : RIGHTHAND)
					|| bSlot == (pUser->GetZoneID() == ZONE_KNIGHT_ROYALE ? KNIGHT_ROYAL_LEFTHAND : LEFTHAND))
					continue;

				sAmount = int16(bonusAtt->second * 1.1f);

				if (attribute >= ITEM_TYPE_FIRE
					&& attribute <= ITEM_TYPE_POISON)
				{
					// add attribute damage amount to right-hand damage instead of attribute
					// so it doesn't bother taking into account caster level (as it would with the staff attributes).
					attribute_damage += sAmount;
				}
			}

		}

		if (pTarget->isPlayer())
		{
			if (TO_USER(pTarget)->isWarrior())
			{
				damage = (445 * total_hit / (total_r + 510));
			}
			else if (TO_USER(pTarget)->isPriest())
			{
				damage = (400 * total_hit / (total_r + 510));
			}
			else if (TO_USER(pTarget)->isRogue())
			{
				damage = (485 * total_hit / (total_r + 510));
			}
			else if (TO_USER(pTarget)->isMage())
			{
				damage = (455 * total_hit / (total_r + 515));
			}
			else
				damage = (555 * total_hit / (total_r + 515));
		}
		else
			damage = (555 * total_hit / (total_r + 515));

		random = myrand(0, damage / 2);
		damage = (short)(random * 0.1f + (damage * 0.85f)) - sMagicAmount * 3;

		if (pSkillCaster->isNPC())
			damage -= ((3 * righthand_damage) + (3 * attribute_damage));
		else if (attribute != MAGIC_R)	// Only if the staff has an attribute.
			damage -= (short)(((righthand_damage * 0.8f) + (righthand_damage * pSkillCaster->GetLevel()) / 60) + ((attribute_damage * 0.8f) + (attribute_damage * pSkillCaster->GetLevel()) / 30));
		if (pTarget->m_bMagicDamageReduction < 100)
			damage = damage * pTarget->m_bMagicDamageReduction / 100;

	}

	// Apply boost for skills matching weather type.
	// This isn't actually used officially, but I think it's neat...
	GetWeatherDamage(damage, attribute);

	if (pTarget->isPlayer())
		damage /= 3;

	// Implement damage cap.
	if (damage > MAX_DAMAGE)
		damage = MAX_DAMAGE;

	return damage;
}

int32 MagicInstance::GetWeatherDamage(int32 damage, int attribute)
{
	// Give a 10% damage output boost based on weather (and skill's elemental attribute)
	if ((g_pMain->m_byWeather == WEATHER_FINE && attribute == AttributeFire)
		|| (g_pMain->m_byWeather == WEATHER_RAIN && attribute == AttributeLightning)
		|| (g_pMain->m_byWeather == WEATHER_SNOW && attribute == AttributeIce))
		damage = (damage * 110) / 100;

	return damage;
}

void MagicInstance::Type6Cancel(bool bForceRemoval)
{
	// NPCs cannot transform.
	if (!pSkillCaster->isPlayer()
		// Are we transformed? Note: if we're relogging, and we need to remove it, we should ignore this check.
		|| (!bForceRemoval
			&& !TO_USER(pSkillCaster)->isTransformed()))
		return;

	CUser* pUser = TO_USER(pSkillCaster);
	_MAGIC_TYPE6* pType = g_pMain->m_Magictype6Array.GetData(nSkillID);

	if (pType == nullptr)
		return;

	if (pUser->isSiegeTransformation())
		pUser->m_bRegeneType = REGENE_NORMAL;

	Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_CANCEL_TRANSFORMATION));
	// TODO: Reset stat changes, recalculate stats.
	pUser->m_transformationType = Unit::TransformationNone;
	pUser->m_sTransformHpchange = false;
	pUser->m_sTransformMpchange = false;
	pUser->Send(&result);
	pUser->SetUserAbility();
	pUser->RemoveSavedMagic(pUser->m_bAbnormalType);
	pUser->StateChangeServerDirect(3, ABNORMAL_NORMAL);
}

void MagicInstance::Type9Cancel(bool bRemoveFromMap)
{
	if (pSkillCaster == nullptr
		|| !pSkillCaster->isPlayer())
		return;

	_MAGIC_TYPE9* pType = g_pMain->m_Magictype9Array.GetData(nSkillID);
	if (pType == nullptr)
		return;

	uint8 bResponse = 0;
	CUser* pCaster = TO_USER(pSkillCaster);

	if (pCaster == nullptr)
		return;

	Guard lock(pCaster->m_buffLock);
	Type9BuffMap& buffMap = pCaster->m_type9BuffMap;

	// If this skill isn't already applied, there's no reason to remove it.
	if (buffMap.find(pType->bStateChange) == buffMap.end())
		return;

	// Remove the buff from the map
	if (bRemoveFromMap)
		buffMap.erase(pType->bStateChange);

	// Stealths
	if (pType->bStateChange <= 2
		|| (pType->bStateChange >= 5 && pType->bStateChange < 7))
	{
		pCaster->StateChangeServerDirect(7, INVIS_NONE);
		bResponse = 91;
	}
	// Lupine etc.
	else if (pType->bStateChange >= 3 && pType->bStateChange <= 4)
	{
		pCaster->InitializeStealth();
		bResponse = 92;
	}
	// Guardian pet related
	else if (pType->bStateChange >= 7 && pType->bStateChange <= 8)
	{
		if (pType->bStateChange == 8)
		{
			if (pCaster->m_PettingOn != nullptr)
				pCaster->PetOnDeath();
		}
		else {
			g_pMain->KillNpc(pCaster->GetSocketID(), pCaster->GetZoneID(), pCaster);
		}
		bResponse = 93;
	}
	else if (pType->bStateChange == 9)
	{
		g_pMain->KillNpc(pCaster->GetSocketID(), pCaster->GetZoneID(), pCaster);
		bResponse = 9;
	}

	Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_DURATION_EXPIRED));
	result << bResponse;
	pCaster->Send(&result);
}

void MagicInstance::Type4Cancel()
{
	if (pSkill == nullptr || pSkillTarget != pSkillCaster)
		return;

	_MAGIC_TYPE4* pType = g_pMain->m_Magictype4Array.GetData(nSkillID);
	if (pType == nullptr
		|| pType->isDebuff())
		return;

	if (nSkillID > 500000
		&& TO_USER(pSkillCaster)->isLockableScroll(pType->bBuffType)
		&& pSkillCaster->hasDebuff(pType->bBuffType))
		return;

	if (!CMagicProcess::RemoveType4Buff(pType->bBuffType, TO_USER(pSkillCaster)))
		return;

	TO_USER(pSkillCaster)->RemoveSavedMagic(nSkillID);

	if (nSkillID == 495146)
	{
		CBot* pPriest = nullptr;
		pPriest = g_pMain->m_BotArray.GetData(TO_USER(pSkillCaster)->m_bUserPriestBotID);
		pPriest->UserInOut(INOUT_OUT);
		TO_USER(pSkillCaster)->m_bUserPriestBotID = -1;
		TO_USER(pSkillCaster)->hasPriestBot = false;
	}

	

}

void MagicInstance::Type3Cancel()
{
	if (pSkill == nullptr
		|| pSkillCaster != pSkillTarget)
		return;

	// Should this take only the specified skill? I'm thinking so.
	_MAGIC_TYPE3* pType = g_pMain->m_Magictype3Array.GetData(nSkillID);
	if (pType == nullptr)
		return;

	for (int i = 0; i < MAX_TYPE3_REPEAT; i++)
	{
		Unit::MagicType3* pEffect = &pSkillCaster->m_durationalSkills[i];

		if (pEffect == nullptr)
			continue;

		if (!pEffect->m_byUsed
			// we can only cancel healing-over-time skills
				// damage-over-time skills must remain.
			|| pEffect->m_sHPAmount <= 0)
			continue;

		pEffect->Reset();
		break;	// we can only have one healing-over-time skill active.
		// since we've found it, no need to loop through the rest of the array.
	}

	if (pSkillCaster->isPlayer())
	{
		Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_DURATION_EXPIRED));
		result << uint8(100); // remove the healing-over-time skill.
		TO_USER(pSkillCaster)->Send(&result);
	}

	int buff_test = 0;
	for (int j = 0; j < MAX_TYPE3_REPEAT; j++)
	{
		if (pSkillCaster->m_durationalSkills[j].m_byUsed)
		{
			buff_test++;
			break;
		}
	}

	if (buff_test == 0)
		pSkillCaster->m_bType3Flag = false;

	if (pSkillCaster->isPlayer()
		&& !pSkillCaster->m_bType3Flag)
		TO_USER(pSkillCaster)->SendUserStatusUpdate(USER_STATUS_DOT, USER_STATUS_CURE);
}

void MagicInstance::Type4Extend()
{
	if (pSkill == nullptr
		// Only players can extend buffs.
		|| !pSkillCaster->isPlayer()
		// Can't use on special items.
		|| nSkillID >= 500000)
		return;

	_MAGIC_TYPE4* pType = g_pMain->m_Magictype4Array.GetData(nSkillID);
	if (pType == nullptr
		|| pType->isDebuff())
		return;

	Guard lock(pSkillTarget->m_buffLock);
	Type4BuffMap::iterator itr = pSkillTarget->m_buffMap.find(pType->bBuffType);

	// Can't extend a buff that hasn't been started.
	if (itr == pSkillCaster->m_buffMap.end()
		// Can't extend a buff that's already been extended.
		|| itr->second.m_bDurationExtended)
		return;

	// Require the "Duration Item" for buff duration extension.
	// The things we must do to future-proof things...
	bool bItemFound = false;
	for (int i = SLOT_MAX; i < INVENTORY_TOTAL; i++)
	{
		_ITEM_DATA* pItem = nullptr;
		_ITEM_TABLE* pTable = TO_USER(pSkillCaster)->GetItemPrototype(i, pItem);
		if (pTable == nullptr
			|| pTable->m_bKind != 255
			|| pTable->m_iEffect1 == 0)
			continue;

		_MAGIC_TABLE* pEffect = g_pMain->m_MagictableArray.GetData(pTable->m_iEffect1);
		if (pEffect == nullptr
			|| pEffect->bMoral != MORAL_EXTEND_DURATION
			|| !TO_USER(pSkillCaster)->RobItem(i, pTable))
			continue;

		bItemFound = true;
		break;
	}

	// No "Duration Item" was found.
	if (!bItemFound)
		return;

	// Extend the duration of a buff.
	itr->second.m_tEndTime += pType->sDuration;

	// Mark the buff as extended (we cannot extend it more than once).
	itr->second.m_bDurationExtended = true;

	Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_TYPE4_EXTEND));
	result << uint32(nSkillID);
	TO_USER(pSkillTarget)->Send(&result);
}

void MagicInstance::ReflectDamage(int32 damage, Unit* pTarget)
{
	if (pSkillCaster == nullptr || pTarget == nullptr)
		return;


	MagicInstance InitReflection;
	InitReflection.bOpcode = 3;
	InitReflection.nSkillID = pTarget->m_bReflectArmorType + 80000;
	InitReflection.sCasterID = pTarget->GetID();
	InitReflection.sTargetID = pSkillCaster->GetID();

	InitReflection.sData[0] = (uint16)pSkillCaster->GetX();
	InitReflection.sData[1] = 1;
	InitReflection.sData[2] = (uint16)pSkillCaster->GetZ();
	InitReflection.sData[3] = 0;
	InitReflection.sData[4] = 0;
	InitReflection.sData[5] = 0;
	InitReflection.sData[6] = 0;

	InitReflection.Run();

	CMagicProcess::RemoveType4Buff(BUFF_TYPE_MAGE_ARMOR, pTarget, true);
}

void MagicInstance::ConsumeItem()
{
	if (nConsumeItem != 0 && pSkillCaster->isPlayer())
		if (nConsumeItem == 370001000 ||
			nConsumeItem == 370002000 ||
			nConsumeItem == 370003000 ||
			nConsumeItem == 379069000 ||
			nConsumeItem == 379070000 ||
			nConsumeItem == 379063000 ||
			nConsumeItem == 379064000 ||
			nConsumeItem == 379065000 ||
			nConsumeItem == 379066000)
			TO_USER(pSkillCaster)->RobItem(0);
		else
			TO_USER(pSkillCaster)->RobItem(nConsumeItem);


	if (bInstantCast)
		CMagicProcess::RemoveType4Buff(BUFF_TYPE_INSTANT_MAGIC, pSkillCaster);
}

bool MagicInstance::isDKMToMonsterDamageSkills()
{
	uint32 iSkillID = nSkillID;

	if (iSkillID == 111554 ||
		iSkillID == 112554 ||
		iSkillID == 211554 ||
		iSkillID == 212554)
		return true;

	return false;
}

bool MagicInstance::isDKMToUserDamageSkills()
{
	uint32 iSkillID = nSkillID;

	if (iSkillID == 111545 ||
		iSkillID == 112545 ||
		iSkillID == 211545 ||
		iSkillID == 212545)
		return true;

	return false;
}

bool MagicInstance::isStaffSkill(bool isExtended)
{
	uint32 iSkillID = nSkillID;

	if (isExtended)
		iSkillID -= SKILLPACKET;

	if (iSkillID == 109742 //Lr Staff
		|| iSkillID == 110742
		|| iSkillID == 209742
		|| iSkillID == 210742
		|| iSkillID == 110772
		|| iSkillID == 210772
		|| iSkillID == 109542 //Fr Staff
		|| iSkillID == 110542
		|| iSkillID == 209542
		|| iSkillID == 210542
		|| iSkillID == 110572
		|| iSkillID == 210572
		|| iSkillID == 109642 //Ice Staff
		|| iSkillID == 110642
		|| iSkillID == 209642
		|| iSkillID == 210642
		|| iSkillID == 110672
		|| iSkillID == 210672)
		return true;

	return false;
}

bool MagicInstance::isRushSkill()
{

	if (nSkillID == 194509
		|| nSkillID == 195509
		|| nSkillID == 294509
		|| nSkillID == 295509)
		return true;

	return false;
}

bool MagicInstance::isExtendedArcherSkill(bool isExtended)
{
	uint32 iSkillID = nSkillID;

	if (isExtended)
		iSkillID -= SKILLPACKET;

	if (iSkillID == 108566
		|| iSkillID == 208566)
		return true;

	return false;
}

bool MagicInstance::isUnderTheCastleDisableSkill()
{
	uint32 iSkillID = nSkillID;

	if (iSkillID == 107650
		|| iSkillID == 108650
		|| iSkillID == 207650
		|| iSkillID == 208650 // Vampiric touch
		|| iSkillID == 107610
		|| iSkillID == 108610
		|| iSkillID == 207610
		|| iSkillID == 208610 // Blood drain
		|| iSkillID == 109554
		|| iSkillID == 110554
		|| iSkillID == 209554
		|| iSkillID == 210554 // Fire Thorn
		|| iSkillID == 109754
		|| iSkillID == 110754
		|| iSkillID == 209754
		|| iSkillID == 210754 // Static Thorn
		|| iSkillID == 111745
		|| iSkillID == 112745
		|| iSkillID == 211745
		|| iSkillID == 212745 // Parasite
		|| iSkillID == 112771
		|| iSkillID == 212771) // Super Parasite
		return true;

	return false;
}

bool MagicInstance::ArmorDefens() {

	if (nSkillID == 190773
		|| nSkillID == 290773
		|| nSkillID == 290673
		|| nSkillID == 190673
		|| nSkillID == 290573
		|| nSkillID == 190573)
		return true;

	return false;
}

bool MagicInstance::isKrowazWeaponSkill()
{
	if (nSkillID == 491058	//	Cannot use against the ones to be summoned.                                                                                                                                                             
		|| nSkillID == 491059	//	Reduction                                                                                                                                                                                               
		|| nSkillID == 491060	//	Full Skill Gear                                                                                                                                                                                         
		|| nSkillID == 491061	//	Sweet Kiss                                                                                                                                                                                              
		|| nSkillID == 491062	//	No Potion                                                                                                                                                                                               
		|| nSkillID == 491063	//	Splash                                                                                                                                                                                                  
		|| nSkillID == 491064	//	Strike Wrist                                                                                                                                                                                            
		|| nSkillID == 491065	//	Kaul Transformation                                                                                                                                                                                     
		|| nSkillID == 491066	//	Deadmans Call                                                                                                                                                                                           
		|| nSkillID == 491067	//	Unsight                                                                                                                                                                                                 
		|| nSkillID == 491068	//	Adamant                                                                                                                                                                                                 
		|| nSkillID == 491069	//	Divine Protection                                                                                                                                                                                       
		|| nSkillID == 492001	//	Anger Recovery                                                                                                                                                                                          
		|| nSkillID == 492027	//	Fear                                                                                                                                                                                                    
		|| nSkillID == 492028	//	Impact Reflection                                                                                                                                                                                       
		|| nSkillID == 492029	//	Venom                                                                                                                                                                                                   
		|| nSkillID == 492030	//	Anti-heal                                                                                                                                                                                               
		|| nSkillID == 492031	//	Splash                                                                                                                                                                                                  
		|| nSkillID == 492032)	//	Armored Skin    
		return true;

	return false;
}