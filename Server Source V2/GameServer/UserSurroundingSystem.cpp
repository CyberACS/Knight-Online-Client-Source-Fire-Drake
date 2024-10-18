#include "stdafx.h"

void CUser::UserRegionChat(Packet &pkt)
{
	uint8 opcode;
	pkt >> opcode;

	switch (opcode)
	{
	case UserInfo:
		HandleSurroundingAllUserInfo(UserInfo);
		break;
	case UserInfoDetail:
		HandleSurroundingUserInfoDetail(pkt);
		break;
	case UserInfoAll:
		HandleSurroundingAllUserInfo(UserInfoAll);
		break;
	case UserInfoShow:
		HandleSurroundingUserRegionUpdate();
		break;
	case UserInfoView:
		HandleSurroundingUserInfoDetailV2(pkt);
		break;
	case UserHookInfo:
		HandleHookUser(pkt);
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 12:
	case 13:
	case 14:
	case 15:
		break;
	default:
		printf("User region chat unhandled packets %d \n",opcode);
		TRACE("User region chat unhandled packets %d \n", opcode);
		break;
	}
}

void CUser::HandleSurroundingAllUserInfo(uint8 type)
{
	if (UNIXTIME2 - m_tLastSurroundingUpdate < 500)
		return;
	
	KOMap* pMap = GetMap();
	if (pMap == nullptr)
		return;

	uint16 sCount = 0;
	size_t wpos = 0;
	m_tLastSurroundingUpdate = UNIXTIME2;

	Packet result(WIZ_USER_INFORMATIN, uint8(type));
	result << uint8(1) << uint16(GetZoneID());
	sCount = 0;
	wpos = result.wpos();
	result << sCount;
	
	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach(itr, sessMap)
	{
		CUser* pUser = TO_USER(itr->second);
		if (pUser == nullptr
			|| !pUser->isInGame()
			|| pUser->GetZoneID() != GetZoneID())
			continue;

		if (GetEventRoom() >= 0
			&& GetEventRoom() != pUser->GetEventRoom())
			continue;

		if (pUser->isGM())
			continue;

		CKnights* pKnight = g_pMain->GetClanPtr(pUser->GetClanID());

		result.SByte();
		result << pUser->GetName()
			<< uint8(pUser->GetNation())
			<< uint16(1)
			<< uint16(pUser->GetSPosX())
			<< uint16(pUser->GetSPosZ())
			<< uint16(pUser->GetClanID()) // clanid
			<< (pKnight != nullptr ? uint16(pKnight->m_sMarkVersion) : uint16(0))
			<< (pKnight != nullptr ? uint8(pKnight->m_byFlag) : uint8(0))  // clan flag
			<< (pKnight != nullptr ? uint8(pKnight->m_byGrade) : uint8(0)) // clan grade
			<< uint16(0);
		sCount++;

		std::string name = pUser->GetName();
	}
	foreach_stlmap(itr, g_pMain->m_BotArray)
	{
		CBot* pUser = itr->second;

		if (pUser == nullptr
			|| !pUser->isInGame()
			|| pUser->GetZoneID() != GetZoneID())
			continue;

		CKnights* pKnight = g_pMain->GetClanPtr(pUser->GetClanID());

		result.SByte();
		result << pUser->GetName()
			<< uint8(pUser->GetNation())
			<< uint16(1)
			<< uint16(pUser->GetSPosX())
			<< uint16(pUser->GetSPosZ())
			<< uint16(pUser->GetClanID()) // clanid
			<< (pKnight != nullptr ? uint16(pKnight->m_sMarkVersion) : uint16(0))
			<< (pKnight != nullptr ? uint8(pKnight->m_byFlag) : uint8(0))  // clan flag
			<< (pKnight != nullptr ? uint8(pKnight->m_byGrade) : uint8(0)) // clan grade
			<< uint16(0);
		sCount++;
	}
	result.put(wpos, sCount);
	Send(&result);
}

void CUser::HandleHookUser(Packet& pkt)
{
	std::string strUserID;
	pkt >> strUserID;
	//username = GetName();

	Packet result(WIZ_USER_INFORMATIN, uint8(2));
	result.SByte();
	pkt.SByte();

	CUser* pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
	CKnights* pKnight = nullptr;

	if (pUser == nullptr || !pUser->isInGame())
	{
		CBot* pBot = g_pMain->GetBotPtr(strUserID, TYPE_CHARACTER);
		if (pBot == nullptr)
			return;
		else if (pBot->isInGame())
		{

			result << uint8(1)
				<< pBot->GetName()
				<< uint8(pBot->m_bLevel)
				<< uint16(pBot->m_sClass)
				<< uint32(100)
				<< uint32(pBot->GetMonthlyLoyalty())
				<< uint8(0x01);


			if (pBot->GetClanID())
			{

				CKnights* pKnights = g_pMain->GetClanPtr(pBot->GetClanID());
				if (pKnights == nullptr)
					result << uint16(0) << uint16(0) << uint16(0) << uint16(0);
				else
					result << pBot->GetClanID()
					<< pKnights->m_sMarkVersion
					<< pKnights->m_byFlag
					<< pKnights->m_byGrade
					<< pKnights->m_strName
					<< pKnights->m_strChief;

			}
			else
				result << uint64(0);


			result << pBot->GetRebLevel();

			Send(&result);

			Packet result2(WIZ_ITEM_UPGRADE, uint8(ITEM_CHARACTER_SEAL)); // User Inventory View,By Terry.17.04.02..

			result2.DByte();
			result2 << uint8(CharacterSealOpcodes::Preview) << uint8(1)
				<< pBot->GetName()
				//<< pBot->m_reblvl
				<< pBot->GetRace()
				<< pBot->GetClass()
				<< pBot->GetLevel()
				<< uint32(1750)

				<< pBot->GetStat(STAT_STR)
				<< pBot->GetStat(STAT_STA)
				<< pBot->GetStat(STAT_DEX)
				<< pBot->GetStat(STAT_INT)
				<< pBot->GetStat(STAT_CHA)

				<< pBot->GetCoins()
				<< pBot->m_bstrSkill[SkillPointFree]
				<< uint32(1)
				<< pBot->m_bstrSkill[SkillPointCat1]
				<< pBot->m_bstrSkill[SkillPointCat2]
				<< pBot->m_bstrSkill[SkillPointCat3]
				<< pBot->m_bstrSkill[SkillPointMaster];

			for (int i = 0; i < SLOT_MAX + HAVE_MAX; i++)
			{
				_ITEM_DATA* pItem = pBot->GetItem(i);

				result2 << pItem->nNum << pItem->sDuration << pItem->sCount << pItem->bFlag;
			}
			Send(&result2);

			return;

		}
		else
			return;
		//result << uint8(0) << strUserID << uint64(0) << uint64(0) << uint64(0) << uint8(0);
	}
	else
	{
		result << uint8(pUser->GetState())
			<< pUser->m_strUserID
			<< uint8(pUser->GetLevel())
			<< uint16(pUser->GetClass())
			<< uint32(pUser->GetLoyalty())
			<< uint32(pUser->GetMonthlyLoyalty())
			<< uint8(pUser->m_bResHpType);

		if (pUser->isInClan())
		{
			pKnight = g_pMain->GetClanPtr(pUser->GetClanID());
			if (pKnight != nullptr)
			{
				result << uint16(pUser->GetClanID()) // clanid
					<< uint16(pKnight->m_sMarkVersion)
					<< uint8(pKnight->m_byFlag)  // clan flag
					<< uint8(pKnight->m_byGrade) // clan grade
					<< pKnight->GetName()       // clan name
					<< pKnight->m_strChief;     // clan leader
			}
		}
		else
			result << uint64(0);

		result << uint8(pUser->GetRebirthLevel()) << uint32(0);

		Packet result(WIZ_ITEM_UPGRADE, uint8(ITEM_CHARACTER_SEAL));

		result.DByte();
		result << uint8(CharacterSealOpcodes::Preview) << uint8(1)
			<< pUser->m_strUserID
			//<< pUser->m_bRebirthLevel
			<< pUser->m_bRace
			<< pUser->m_sClass
			<< pUser->m_bLevel
			<< pUser->m_iLoyalty

			<< pUser->m_bStats[STAT_STR]
			<< pUser->m_bStats[STAT_STA]
			<< pUser->m_bStats[STAT_DEX]
			<< pUser->m_bStats[STAT_INT]
			<< pUser->m_bStats[STAT_CHA]

			<< pUser->m_iGold
			<< pUser->m_bstrSkill[SkillPointFree]
			<< uint32(1)
			<< pUser->m_bstrSkill[SkillPointCat1]
			<< pUser->m_bstrSkill[SkillPointCat2]
			<< pUser->m_bstrSkill[SkillPointCat3]
			<< pUser->m_bstrSkill[SkillPointMaster];

		for (int i = 0; i < INVENTORY_COSP; i++)
		{
			_ITEM_DATA* pItem = pUser->GetItem(i);

			if (pItem == nullptr)
				continue;


			result << pItem->nNum << pItem->sDuration << pItem->sCount << pItem->bFlag;
		}
		Send(&result);


	}

	Send(&result);
}
void CUser::HandleSurroundingUserInfoDetailV2(Packet& pkt)
{
	//std::string strUserID1;
	uint16 strUserID;
	pkt >> strUserID;
	
	
	Packet result(WIZ_USER_INFORMATIN, uint8(2));
	result.SByte();
	pkt.SByte();
	
	
	CUser* pUser = g_pMain->GetUserPtr(strUserID);
	CKnights* pKnight = nullptr;
	
	if (pUser == nullptr || !pUser->isInGame())
	{
		foreach_stlmap(itr, g_pMain->m_BotArray)
		{
			CBot* pUserBot = itr->second;
			if (pUserBot == nullptr)
				return;

			if (pUserBot->GetID() == strUserID)
			{
				CBot* pBot = g_pMain->GetBotPtr(pUserBot->GetName() , TYPE_CHARACTER);
				if (pBot == nullptr)
					return;
				else if (pBot->isInGame())
				{

					result << uint8(1)
						<< pBot->GetName()
						<< uint8(pBot->m_bLevel)
						<< uint16(pBot->m_sClass)
						<< uint32(100)
						<< uint32(pBot->GetMonthlyLoyalty())
						<< uint8(0x01);


					if (pBot->GetClanID())
					{

						CKnights* pKnights = g_pMain->GetClanPtr(pBot->GetClanID());
						if (pKnights == nullptr)
							result << uint16(0) << uint16(0) << uint16(0) << uint16(0);
						else
							result << pBot->GetClanID()
							<< pKnights->m_sMarkVersion
							<< pKnights->m_byFlag
							<< pKnights->m_byGrade
							<< pKnights->m_strName
							<< pKnights->m_strChief;

					}
					else
						result << uint64(0);


					result << pBot->GetRebLevel();

					Send(&result);

					Packet result2(WIZ_ITEM_UPGRADE, uint8(ITEM_CHARACTER_SEAL)); // User Inventory View,By Terry.17.04.02..

					result2.DByte();
					result2 << uint8(CharacterSealOpcodes::Preview) << uint8(1)
						<< pBot->GetName()
						//<< pBot->m_reblvl
						<< pBot->GetRace()
						<< pBot->GetClass()
						<< pBot->GetLevel()
						<< uint32(1750)

						<< pBot->GetStat(STAT_STR)
						<< pBot->GetStat(STAT_STA)
						<< pBot->GetStat(STAT_DEX)
						<< pBot->GetStat(STAT_INT)
						<< pBot->GetStat(STAT_CHA)

						<< pBot->GetCoins()
						<< pBot->m_bstrSkill[SkillPointFree]
						<< uint32(1)
						<< pBot->m_bstrSkill[SkillPointCat1]
						<< pBot->m_bstrSkill[SkillPointCat2]
						<< pBot->m_bstrSkill[SkillPointCat3]
						<< pBot->m_bstrSkill[SkillPointMaster];

					for (int i = 0; i < SLOT_MAX + HAVE_MAX; i++)
					{
						_ITEM_DATA* pItem = pBot->GetItem(i);

						result2 << pItem->nNum << pItem->sDuration << pItem->sCount << pItem->bFlag;
					}
					Send(&result2);

					return;

				}
				else
					return;
			}
			
		}

		
		//result << uint8(0) << strUserID << uint64(0) << uint64(0) << uint64(0) << uint8(0);
	}
	else
	{
		result << uint8(pUser->GetState())
			<< pUser->m_strUserID
			<< uint8(pUser->GetLevel())
			<< uint16(pUser->GetClass())
			<< uint32(pUser->GetLoyalty())
			<< uint32(pUser->GetMonthlyLoyalty())
			<< uint8(pUser->m_bResHpType);

		if (pUser->isInClan())
		{
			pKnight = g_pMain->GetClanPtr(pUser->GetClanID());
			if (pKnight != nullptr)
			{
				result << uint16(pUser->GetClanID()) // clanid
					<< uint16(pKnight->m_sMarkVersion)
					<< uint8(pKnight->m_byFlag)  // clan flag
					<< uint8(pKnight->m_byGrade) // clan grade
					<< pKnight->GetName()       // clan name
					<< pKnight->m_strChief;     // clan leader
			}
		}
		else
			result << uint64(0);

		result << uint8(pUser->GetRebirthLevel()) << uint32(0);

		Packet result(WIZ_ITEM_UPGRADE, uint8(ITEM_CHARACTER_SEAL));

		result.DByte();
		result << uint8(CharacterSealOpcodes::Preview) << uint8(1)
			<< pUser->m_strUserID
			//<< pUser->m_bRebirthLevel
			<< pUser->m_bRace
			<< pUser->m_sClass
			<< pUser->m_bLevel
			<< pUser->m_iLoyalty

			<< pUser->m_bStats[STAT_STR]
			<< pUser->m_bStats[STAT_STA]
			<< pUser->m_bStats[STAT_DEX]
			<< pUser->m_bStats[STAT_INT]
			<< pUser->m_bStats[STAT_CHA]

			<< pUser->m_iGold
			<< pUser->m_bstrSkill[SkillPointFree]
			<< uint32(1)
			<< pUser->m_bstrSkill[SkillPointCat1]
			<< pUser->m_bstrSkill[SkillPointCat2]
			<< pUser->m_bstrSkill[SkillPointCat3]
			<< pUser->m_bstrSkill[SkillPointMaster];

		for (int i = 0; i < INVENTORY_COSP; i++)
		{
			_ITEM_DATA* pItem = pUser->GetItem(i);

			if (pItem == nullptr)
				continue;


			result << pItem->nNum << pItem->sDuration << pItem->sCount << pItem->bFlag;
		}
		Send(&result);


	}

	Send(&result);
}

void CUser::HandleSurroundingUserInfoDetail(Packet & pkt)
{
	Packet result(WIZ_USER_INFORMATIN, uint8(UserInfoDetail));
	std::string strCharName;
	pkt.SByte();
	pkt >> strCharName;
	CUser* pUser = g_pMain->GetUserPtr(strCharName, TYPE_CHARACTER);
	CKnights* pKnights = nullptr;

	if (pUser == nullptr)
	{
		CBot* pUser = g_pMain->GetBotPtr(strCharName, TYPE_CHARACTER);
		if (pUser == nullptr)
			result << uint8(0) << strCharName << uint64(0) << uint64(0) << uint64(0) << uint8(0);
		else
		{
			result.SByte();
			result << uint8(pUser->m_state) << pUser->GetName() << pUser->GetLevel() << pUser->GetClass() << pUser->GetLoyalty() << pUser->GetMonthlyLoyalty() << pUser->m_bResHpType;

			pKnights = g_pMain->GetClanPtr(pUser->GetClanID());

			if (pKnights == nullptr)
				result << uint16(0) << uint16(0) << uint16(0) << uint16(0);
			else
				result << pUser->GetClanID() << pKnights->m_sMarkVersion << pKnights->m_byFlag << pKnights->m_byGrade << pKnights->m_strName << pKnights->m_strChief;

			result << uint8(pUser->GetRebirthLevel()) << uint32(0);
		}
	}
	else
	{
		result.SByte();
		result << uint8(pUser->GetState()) << pUser->GetName() << pUser->GetLevel() << pUser->GetClass() << pUser->GetLoyalty() << pUser->GetMonthlyLoyalty() << pUser->m_bResHpType;

		pKnights = g_pMain->GetClanPtr(pUser->GetClanID());

		if (pKnights == nullptr)
			result << uint16(0) << uint16(0) << uint16(0) << uint16(0);
		else
			result << pUser->GetClanID() << pKnights->m_sMarkVersion << pKnights->m_byFlag << pKnights->m_byGrade << pKnights->m_strName << pKnights->m_strChief;

		result << uint8(pUser->GetRebirthLevel()) << uint32(0);
	}
	Send(&result);
}

void CUser::HandleSurroundingUserRegionUpdate()
{
	Packet result(WIZ_USER_INFORMATIN, uint8(UserInfoShow));
	result.SByte();
	result << GetName();
	g_pMain->Send_Zone(&result, GetZoneID());
}