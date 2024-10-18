#include "stdafx.h"
#include "Map.h"
#include "KnightsManager.h"
#include "KingSystem.h"
#include "MagicInstance.h"
#include "DBAgent.h"
#include <algorithm>
#include "../shared/DateTime.h"
#include <boost\foreach.hpp>	
#include <iostream> 
#include <sstream> 

using namespace std;


void CUser::GmTools(Packet& pkt)
{
	/*if (!isGM())
		return;*/
	Packet Lottery = pkt;


	if (m_GameMastersGmTools != 1)
	{
		// send description
		g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
		return;
	}


	uint8 sOpCode;
	pkt >> sOpCode;

	//GAMEMASTER TOOLS EKRANI

	if (sOpCode == 1)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		Packet result(PL_GMTOOLS);

		result << uint8(1);

		result << GetName()
			<< m_bAuthority
			<< GetLevel()
			<< GetNation()
			<< m_iMaxExp
			<< m_iExp
			<< GetLoyalty()
			<< GetMonthlyLoyalty()
			<< uint32(m_nKnightCash)
			<< uint32(m_nKnightCashBonus);


		Send(&result);
	}

	if (sOpCode == 2)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}


		if (m_bAuthority == AUTHORITY_GM_USER)
		{
			m_bAuthority = AUTHORITY_GAME_MASTER;


			SendMyInfo();
			UserInOut(INOUT_OUT);
			SetRegion(GetNewRegionX(), GetNewRegionZ());

			UserInOut(INOUT_WARP);
			RemoveRegion(GetNewRegionX(), GetNewRegionZ());

			g_pMain->RegionUserInOutForMe(this);
			g_pMain->RegionNpcInfoForMe(this);
			g_pMain->MerchantUserInOutForMe(this);

			ResetWindows();
			InitType4();
			RecastSavedMagic();
		}
		else if (m_bAuthority == AUTHORITY_GAME_MASTER)
		{
			m_bAuthority = AUTHORITY_GM_USER;


			SendMyInfo();
			UserInOut(INOUT_OUT);
			SetRegion(GetNewRegionX(), GetNewRegionZ());

			UserInOut(INOUT_WARP);
			RemoveRegion(GetNewRegionX(), GetNewRegionZ());

			g_pMain->RegionUserInOutForMe(this);
			g_pMain->RegionNpcInfoForMe(this);
			g_pMain->MerchantUserInOutForMe(this);

			ResetWindows();
			InitType4();
			RecastSavedMagic();

		}

	}

	if (sOpCode == 3)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		if (m_bAbnormalType == ABNORMAL_INVISIBLE)
		{

			m_bAbnormalType = ABNORMAL_NORMAL;

			SendMyInfo();
			UserInOut(INOUT_OUT);
			SetRegion(GetNewRegionX(), GetNewRegionZ());

			UserInOut(INOUT_WARP);
			RemoveRegion(GetNewRegionX(), GetNewRegionZ());

			g_pMain->RegionUserInOutForMe(this);
			g_pMain->RegionNpcInfoForMe(this);
			g_pMain->MerchantUserInOutForMe(this);

			ResetWindows();
			InitType4();
			RecastSavedMagic();
		}
		else if (m_bAbnormalType == ABNORMAL_NORMAL)
		{

			m_bAbnormalType = ABNORMAL_INVISIBLE;

			SendMyInfo();
			UserInOut(INOUT_OUT);
			SetRegion(GetNewRegionX(), GetNewRegionZ());

			UserInOut(INOUT_WARP);
			RemoveRegion(GetNewRegionX(), GetNewRegionZ());

			g_pMain->RegionUserInOutForMe(this);
			g_pMain->RegionNpcInfoForMe(this);
			g_pMain->MerchantUserInOutForMe(this);

			ResetWindows();
			InitType4();
			RecastSavedMagic();

		}


	}

	//GAMEMASTER TOOLS EKRANI


	//USER EDITOR

	if (sOpCode == 4)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		Packet result(PL_GMTOOLS);

		result << uint8(2);

		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			if (TO_USER(itr->second)->isInGame()) {


				result << GetName()
					<< m_bAuthority
					<< GetLevel()
					<< GetNation()
					<< m_iMaxExp
					<< m_iExp
					<< GetLoyalty()
					<< GetMonthlyLoyalty()
					<< uint32(m_nKnightCash)
					<< uint32(m_nKnightCashBonus)
					<< GetStat(STAT_STR)
					<< GetStat(STAT_STA)
					<< GetStat(STAT_DEX)
					<< GetStat(STAT_INT)
					<< GetStat(STAT_CHA)
					<< uint8(m_sFireR) << uint8(m_sColdR) << uint8(m_sLightningR)
					<< uint8(m_sMagicR) << uint8(m_sDiseaseR) << uint8(m_sPoisonR);

				TO_USER(itr->second)->Send(&result);


			}
		}
	}

	if (sOpCode == 5)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string query, name;
		pkt >> name >> query;

		uint16 sCount = atoi(query.c_str());

		std::string strUserID = name;


		CUser* pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		if (pUser == nullptr) {
			g_pMain->SendHelpDescription(this, string_format("%s nick'li karakter oyunda deðil!", pUser->GetName().c_str()).c_str());
			return;
		}

		if (sCount == 0) {
			g_pMain->SendHelpDescription(this, string_format("Lütfen 0'dan farklý bir sayý giriniz.!", pUser->GetName().c_str()).c_str());
			return;
		}


		pUser->SendKnightCash(sCount);

		g_pMain->SendHelpDescription(this, string_format("Cash added successfully to %s!", pUser->GetName().c_str()).c_str());

		return;
	}

	if (sOpCode == 6)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string strUserID, guncelword;
		pkt >> strUserID;

		guncelword = "User Editorden banlanmistir";

		Packet result(WIZ_AUTHORITY_CHANGE);
		DateTime time;

		result << GetName() << strUserID << uint16(AUTHORITY_BANNED) << uint8(BanTypes::BANNED) << uint32(999) << guncelword;
		g_pMain->AddDatabaseRequest(result, this);

		g_pMain->WriteBanListLogFile(string_format("%d:%d:%d || Sender=%s,Command=%s,User=%s,Period=Permanent,Zone=%d(%d,%d)\n",
			time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), guncelword.c_str(), strUserID.c_str(), GetZoneID(), uint16(GetX()), uint16(GetZ())));
	}
	if (sOpCode == 7)
	{

		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string strUserID;
		pkt >> strUserID;

		Packet result(WIZ_AUTHORITY_CHANGE);
		DateTime time;
		std::string sOperatorCommandType = "Mute";

		result << GetName() << strUserID << uint16(AUTHORITY_MUTED) << uint8(BanTypes::MUTE) << uint32(3) << sOperatorCommandType;
		g_pMain->AddDatabaseRequest(result, this);

		g_pMain->WriteBanListLogFile(string_format("%d:%d:%d || Sender=%s,Command=%s,User=%s,Period=Permanent,Zone=%d(%d,%d)\n",
			time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), sOperatorCommandType.c_str(), strUserID.c_str(), GetZoneID(), uint16(GetX()), uint16(GetZ())));


	}
	if (sOpCode == 8)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}


		string strUserID;
		pkt >> strUserID;

		Packet result(WIZ_AUTHORITY_CHANGE);
		DateTime time;
		std::string sOperatorCommandType = "UnMute";

		result << GetName() << strUserID << uint16(AUTHORITY_PLAYER) << uint8(BanTypes::UNMUTE) << uint32(0) << sOperatorCommandType;
		g_pMain->AddDatabaseRequest(result, this);

		g_pMain->WriteBanListLogFile(string_format("%d:%d:%d || Sender=%s,Command=%s,User=%s,Period=Permanent,Zone=%d(%d,%d)\n",
			time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), sOperatorCommandType.c_str(), strUserID.c_str(), GetZoneID(), uint16(GetX()), uint16(GetZ())));

	}
	/*if (sOpCode == 9)//süreli ban fonks. yok bende
	{
		if (!isGM())
			return;

		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string query, name;
		pkt >> name >> query;

		uint16 sCount = atoi(query.c_str());




	}*/
	if (sOpCode == 10)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		CUser* pUser;
		string strUserID;
		pkt >> strUserID;
		pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		pUser->m_bAuthority = AUTHORITY_PLAYER;

		pUser->SendMyInfo();
		pUser->UserInOut(INOUT_OUT);
		pUser->SetRegion(GetNewRegionX(), GetNewRegionZ());

		pUser->UserInOut(INOUT_WARP);
		pUser->RemoveRegion(GetNewRegionX(), GetNewRegionZ());

		g_pMain->RegionUserInOutForMe(pUser);
		g_pMain->RegionNpcInfoForMe(pUser);
		g_pMain->MerchantUserInOutForMe(pUser);

		pUser->ResetWindows();
		pUser->InitType4();
		pUser->RecastSavedMagic();

	}

	if (sOpCode == 11)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		CUser* pUser;
		string strUserID;
		pkt >> strUserID;
		pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		pUser->m_bAuthority = AUTHORITY_GM_USER;

		pUser->SendMyInfo();
		pUser->UserInOut(INOUT_OUT);
		pUser->SetRegion(GetNewRegionX(), GetNewRegionZ());

		pUser->UserInOut(INOUT_WARP);
		pUser->RemoveRegion(GetNewRegionX(), GetNewRegionZ());

		g_pMain->RegionUserInOutForMe(pUser);
		g_pMain->RegionNpcInfoForMe(pUser);
		g_pMain->MerchantUserInOutForMe(pUser);

		pUser->ResetWindows();
		pUser->InitType4();
		pUser->RecastSavedMagic();

	}

	/*if (sOpCode == 12) // sheriff kodu ne bilmiyorum
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		CUser* pUser;
		string strUserID;
		pkt >> strUserID;
		pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		pUser->m_bAuthority = sheriff;
	}*/

	if (sOpCode == 13)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		CUser* pUser;
		string strUserID;
		pkt >> strUserID;
		pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		pUser->Disconnect();
	}

	if (sOpCode == 14)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		CUser* pUser;
		string strUserID;
		pkt >> strUserID;
		pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		if (pUser == nullptr)
		{
			g_pMain->SendHelpDescription(this, "Error : User is not online");
			return;
		}

		pUser->ZoneChange(GetZoneID(), GetX(), GetZ());
	}

	if (sOpCode == 15)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		CUser* pUser;
		string strUserID;
		pkt >> strUserID;
		pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		if (pUser == nullptr)
		{
			g_pMain->SendHelpDescription(this, "Error : User is not online");
			return;
		}

		ZoneChange(pUser->GetZoneID(), pUser->GetX(), pUser->GetZ());
	}
	//USER EDITOR


	//Reload_tables
	if (sOpCode == 16)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_CyberACSPusItemArray.DeleteAllData();
		g_pMain->LoadCyberACSPusLoad();
		printf("Tables are loaded \n");

	}if (sOpCode == 17)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_ItemExchangeArray.DeleteAllData();
		g_pMain->LoadItemExchangeTable();

		g_pMain->m_ItemExchangeExpArray.DeleteAllData();
		g_pMain->LoadItemExchangeExpTable();

		g_pMain->m_ItemSpecialExchangeArray.DeleteAllData();
		g_pMain->LoadItemSpecialExchangeTable();

		g_pMain->m_ItemExchangeCrashArray.DeleteAllData();
		g_pMain->LoadItemExchangeCrashTable();
		printf("Tables are loaded \n");
	}
	if (sOpCode == 18)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_IsMagicTableInUpdateProcess = true;
		g_pMain->m_MagictableArray.DeleteAllData();
		g_pMain->m_Magictype1Array.DeleteAllData();
		g_pMain->m_Magictype2Array.DeleteAllData();
		g_pMain->m_Magictype3Array.DeleteAllData();
		g_pMain->m_Magictype4Array.DeleteAllData();
		g_pMain->m_Magictype5Array.DeleteAllData();
		g_pMain->m_Magictype6Array.DeleteAllData();
		g_pMain->m_Magictype8Array.DeleteAllData();
		g_pMain->m_Magictype9Array.DeleteAllData();
		g_pMain->LoadMagicTable();
		g_pMain->LoadMagicType1();
		g_pMain->LoadMagicType2();
		g_pMain->LoadMagicType3();
		g_pMain->LoadMagicType4();
		g_pMain->LoadMagicType5();
		g_pMain->LoadMagicType6();
		g_pMain->LoadMagicType7();
		g_pMain->LoadMagicType8();
		g_pMain->LoadMagicType9();
		g_pMain->m_IsMagicTableInUpdateProcess = false;
		printf("Tables are loaded \n");

	}if (sOpCode == 19)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		/*g_pMain->m_CyberACSSettingsArray.DeleteAllData();
		g_pMain->LoadCyberACSSettings();*/

		g_pMain->m_ItemPremiumGiftArray;
		g_pMain->LoadItemPremiumGiftTable();

		printf("Tables are loaded \n");

	}if (sOpCode == 20)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_ItemUpgradeArray.DeleteAllData();
		g_pMain->LoadItemUpgradeTable();

		printf("Tables are loaded \n");
	}

	if (sOpCode == 21)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_WarBanishOfWinnerArray.DeleteAllData();
		g_pMain->LoadBanishWinnerTable();

		g_pMain->m_DungeonDefenceMonsterListArray.DeleteAllData();
		g_pMain->LoadDungeonDefenceMonsterTable();

		g_pMain->m_DungeonDefenceStageListArray.DeleteAllData();
		g_pMain->LoadDungeonDefenceStageTable();

		g_pMain->m_DrakiMonsterListArray.DeleteAllData();
		g_pMain->m_DrakiRoomListArray.DeleteAllData();
		g_pMain->LoadDrakiTowerTables();

		g_pMain->m_LuaGiveItemExchangeArray.DeleteAllData();
		g_pMain->LoadGiveItemExchangeTable();

		g_pMain->m_HackToolListArray.DeleteAllData();
		g_pMain->LoadUserHackToolTables();
		printf("Tables are loaded \n");
	}
	if (sOpCode == 22)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_MonsterVeriableRespawnListArray.DeleteAllData();
		g_pMain->LoadMonsterRespawnVeriableListTable();

		g_pMain->m_MonsterStableRespawnListArray.DeleteAllData();
		g_pMain->LoadMonsterRespawnStableListTable();

		g_pMain->m_MonsterSummonList.DeleteAllData();
		g_pMain->LoadMonsterSummonListTable();

		g_pMain->m_MonsterUnderTheCastleArray.DeleteAllData();
		g_pMain->LoadMonsterUnderTheCastleTable();

		g_pMain->m_MonsterStoneListInformationArray.DeleteAllData();
		g_pMain->LoadMonsterStoneListInformationTable();

		g_pMain->m_JuraidMountionListInformationArray.DeleteAllData();
		g_pMain->LoadJuraidMountionListInformationTable();
		printf("Tables are loaded \n");
	}

	if (sOpCode == 23)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_EventTriggerArray.DeleteAllData();
		g_pMain->LoadEventTriggerTable();

		g_pMain->m_EventScheduleArray.DeleteAllData();
		g_pMain->LoadEventScheduleStatusTable();

		g_pMain->m_RoomEventPlayTimerArray.DeleteAllData();
		g_pMain->LoadRoomEventPlayTimerTable();

		g_pMain->m_RoyaleEventPlayTimerArray.DeleteAllData();
		g_pMain->LoadRoyaleEventPlayTimerTable();

		g_pMain->m_BeefEventPlayTimerArray.DeleteAllData();
		g_pMain->LoadBeefEventPlayTimerTable();

		g_pMain->m_CswEventPlayTimerArray.DeleteAllData();
		g_pMain->LoadCswEventPlayTimerTable();
		printf("Tables are loaded \n");
		return;

	}
	if (sOpCode == 24)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_KnightRoayleStatsListArray.DeleteAllData();
		g_pMain->LoadKnightRoyaleStatsTable();

		g_pMain->m_KnightRoyaleBeginnerItemListArray.DeleteAllData();
		g_pMain->LoadKnightRoyaleBeginnerItemTable();

		g_pMain->m_KnightRoyaleTreasureChestListArray.DeleteAllData();
		g_pMain->LoadKnightRoyaleChestListTable();

		g_pMain->m_KnStartPositionRandomArray.DeleteAllData();
		g_pMain->LoadKnightRoyaleStartPositionRandomTable();

		g_pMain->m_KnightCastellanCapeArray.DeleteAllData();
		g_pMain->LoadKnightsCastellanCapeTable();
		printf("Tables are loaded \n");

		return;

	}

	if (sOpCode == 25)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_MiningExchangeArray.DeleteAllData();
		g_pMain->LoadMiningExchangeListTable();

		g_pMain->m_MiningFishingItemArray.DeleteAllData();
		g_pMain->LoadMiningFishingItemTable();
		g_pMain->SetMiningDrops();
		printf("Tables are loaded \n");
		return;
	}
	if (sOpCode == 26)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_MiningFishingItemArray.DeleteAllData();
		g_pMain->LoadMiningFishingItemTable();
		g_pMain->SetMiningDrops();

		printf("Tables are loaded \n");

		return;

	}if (sOpCode == 27)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_MakeItemGroupArray.DeleteAllData();
		g_pMain->LoadMakeItemGroupTable();
		g_pMain->m_NpcItemArray.DeleteAllData();
		g_pMain->LoadNpcItemTable();
		g_pMain->m_MonsterItemArray.DeleteAllData();
		g_pMain->LoadMonsterItemTable();
		printf("Tables are loaded \n");
		return;

	}if (sOpCode == 28)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_ChaosStoneSummonListArray.DeleteAllData();
		g_pMain->LoadChaosStoneMonsterListTable();

		g_pMain->m_ChaosStoneRespawnCoordinateArray.DeleteAllData();
		g_pMain->LoadChaosStoneCoordinateTable();

		g_pMain->m_ChaosStoneStageArray.DeleteAllData();
		g_pMain->LoadChaosStoneStage();
		printf("Tables are loaded \n");
		return;

	}if (sOpCode == 29)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->ReloadKnightAndUserRanks();
		printf("Tables are loaded \n");
		return;

	}if (sOpCode == 30)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_ItemOpArray.DeleteAllData();
		g_pMain->LoadItemOpTable();
		printf("Tables are loaded \n");
		return;

	}if (sOpCode == 31)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_ItemSellTableArray.DeleteAllData();
		g_pMain->LoadItemSellTable();
		printf("Tables are loaded \n");
		return;

	}if (sOpCode == 32)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_CyberACSCollectionRaceSettingsArray.DeleteAllData();
		g_pMain->LoadCyberACSCollectionRaceSettings();
		printf("Tables are loaded \n");
		return;

	}if (sOpCode == 33)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->m_CyberACSCollectionRaceHuntListArray.DeleteAllData();
		g_pMain->LoadCyberACSCollectionRaceHuntList();
		printf("Tables are loaded \n");
		return;

	}if (sOpCode == 34)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_ServerResourceArray.DeleteAllData();
		g_pMain->LoadServerResourceTable();

		g_pMain->m_MonsterResourceArray.DeleteAllData();
		g_pMain->LoadMonsterResourceTable();

		g_pMain->m_MonsterChallengeArray.DeleteAllData();
		g_pMain->LoadMonsterChallengeTable();

		g_pMain->m_MonsterChallengeSummonListArray.DeleteAllData();
		g_pMain->LoadMonsterChallengeSummonListTable();
		printf("Tables are loaded \n");
		return;

	}/*if (sOpCode == 35)// fazladan sOpCode
	{


	}*/
	//Reload_tables

	//START EVENTS
	if (sOpCode == 36)// START WAR
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string Zone;
		pkt >> Zone;

		uint16 ZoneID = atoi(Zone.c_str());


		if (ZoneID == 61)
			g_pMain->BattleZoneOpen(BATTLEZONE_OPEN, 1);
		if (ZoneID == 62)
			g_pMain->BattleZoneOpen(BATTLEZONE_OPEN, 2);
		if (ZoneID == 63)
			g_pMain->BattleZoneOpen(BATTLEZONE_OPEN, 3);
		if (ZoneID == 64)
			g_pMain->BattleZoneOpen(BATTLEZONE_OPEN, 4);
		if (ZoneID == 65)
			g_pMain->BattleZoneOpen(BATTLEZONE_OPEN, 5);
		if (ZoneID == 66)
			g_pMain->BattleZoneOpen(BATTLEZONE_OPEN, 6);
		return;
	}if (sOpCode == 37)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->BattleZoneClose();
	}
	if (sOpCode == 38)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->BattleZoneSelectCommanders();

	}if (sOpCode == 39)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->KnightRoyaleManuelOpening();

	}if (sOpCode == 40)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->JuraidMountainManuelOpening();

	}if (sOpCode == 41)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->BorderDefenceWarManuelOpening();

	}if (sOpCode == 42)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->ChaosExpansionManuelOpening();


	}if (sOpCode == 43)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->DeathMatchManuelOpening();

	}if (sOpCode == 44)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}



		g_pMain->m_bUnderTheCastleIsActive = true;
		g_pMain->m_bUnderTheCastleMonster = true;
		g_pMain->m_nUnderTheCastleEventTime = 180 * MINUTE;

		std::string noticeStr = "UTC Event Baþladý Giriþler Emc ve Luferson Castle'dendir. ";

		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		BOOST_FOREACH(auto itr, sessMap)
		{
			CUser* pUser = TO_USER(itr.second);
			if (pUser != nullptr && pUser->isInGame()) {
				Packet NoticePkt;
				char s = NULL;
				ChatPacket::Construct(&NoticePkt, 13, noticeStr.c_str(), &s, pUser->GetNation());
				pUser->Send(&NoticePkt);
			}
		}


	}if (sOpCode == 45)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->BeefEventManuelOpening();

	}if (sOpCode == 46)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->BattleZoneOpen(SNOW_BATTLE);

	}if (sOpCode == 47)
	{

		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string dakika, hedef_id, odulid;
		pkt >> dakika >> hedef_id >> odulid;

		uint16 dakika1 = atoi(dakika.c_str());
		uint16 hedef_id1 = atoi(hedef_id.c_str());
		uint16 odulid1 = atoi(odulid.c_str());



		if (dakika1 < 10
			|| dakika1 > 500)
		{
			g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Süre  10 ile 500 dakika arasýnda olmalý."));
			return;
		}


		g_pMain->m_CyberACSCollectionRaceSetting = g_pMain->m_CyberACSCollectionRaceSettingsArray.GetData(odulid1);
		g_pMain->m_CyberACSCollectionRaceHuntList = g_pMain->m_CyberACSCollectionRaceHuntListArray.GetData(hedef_id1);
		g_pMain->m_CyberACSCollectionRaceSelectedMonster = hedef_id1;

		if (g_pMain->m_CyberACSCollectionRaceSetting == nullptr) {
			g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Ödül verisi bulunamadý."));
			return;
		}
		if (g_pMain->m_CyberACSCollectionRaceCounts < g_pMain->m_CyberACSCollectionRaceSelectedMonster)
		{
			g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Görev hedefi verisi bulunamadý."));
			return;
		}
		if (g_pMain->m_CyberACSCollectionRaceHuntList == nullptr) {
			g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Hunt verisi bulunamadý."));
			return;
		}

		g_pMain->m_CyberACSCollectionRaceFinishTime = dakika1 * 60;

		uint16 m_sMaxCount = 0;

		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser* pUser = TO_USER(itr->second);
			if (pUser == nullptr)
				continue;

			/*if (pUser->m_CyberACSCollectionRaceRegister)
				continue;*/

			if (pUser->GetLevel() < g_pMain->m_CyberACSCollectionRaceSetting->CREventMinLevel)
				continue;

			if (pUser->GetZoneID() != g_pMain->m_CyberACSCollectionRaceHuntList->QuestZone)
				continue;

			if (m_sMaxCount == g_pMain->m_CyberACSCollectionRaceSetting->CREventMaxUser)
				continue;

			pUser->CyberACSCollectionRaceLoadProcess(); m_sMaxCount++;
		}
		if (m_sMaxCount > 0)
		{
			g_pMain->m_CyberACSCollectionRaceGameServerSatus = true;
			g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Event baþladý! | Katýlýmcý : %d | Süre : %d", m_sMaxCount, g_pMain->m_CyberACSCollectionRaceFinishTime / 60));
		}
		else
		{
			g_pMain->m_CyberACSCollectionRaceFinishTime = 0;
			g_pMain->m_CyberACSCollectionRaceGameServerSatus = false;
			g_pMain->SendHelpDescription(this, string_format("[CR EVENT] : Event Baþlatýlamadý.! | Katýlýmcý : %d | Süre : %d", m_sMaxCount, g_pMain->m_CyberACSCollectionRaceFinishTime / 60));
		}

	}if (sOpCode == 48)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_CyberACSCollectionRaceFinishTime = 0;
		g_pMain->m_CyberACSCollectionRaceGameServerSatus = false;

		SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
		foreach(itr, sessMap)
		{
			CUser* pUser = TO_USER(itr->second);

			if (pUser == nullptr)
				continue;

			pUser->CyberACSCollectionRaceEndProcess();
		}

	}

	//adream event
	if (sOpCode == 49)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string dakika;
		pkt >> dakika;

		uint16 dakika1 = atoi(dakika.c_str());




		int Sure = dakika1;

		if (Sure < 10 || Sure > 500)
		{
			g_pMain->SendHelpDescription(this, "Ardream Event: Sure En Az 10 En Fazla 500 Dakika Olabilir");
			return;
		}

		g_pMain->ArdreamEventFinishTime = Sure * 60;
		g_pMain->ArdreamEventGameServerSatus = true;
		g_pMain->ArdreamEventOpen();
		g_pMain->SendFormattedNotice("%s, Ardream Event Aktif Edildi | Event Suresi : %d Dakika", Nation::ALL, GetName().c_str(), g_pMain->ArdreamEventFinishTime / 60);




	}if (sOpCode == 50)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}
		g_pMain->ArdreamEventZoneClose();

	}
	//adream event

	//cz event
	if (sOpCode == 51)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string dakika;
		pkt >> dakika;

		uint16 dakika1 = atoi(dakika.c_str());






	}if (sOpCode == 52)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}




	}
	//cz event

	//base event
	if (sOpCode == 53)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string dakika;
		pkt >> dakika;
		uint16 dakika1 = atoi(dakika.c_str());


	}if (sOpCode == 54)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}








	}
	//base event


	if (sOpCode == 55)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string warzone, clan1, clan2;
		pkt >> warzone >> clan1 >> clan2;
		uint16 zone = atoi(warzone.c_str());

		g_pMain->HandleTournamentStart(clan1, clan2, zone);

	}
	if (sOpCode == 56)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->CastleSiegeDeathmatchBarrackCreated();

	}
	if (sOpCode == 57)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->CastleSiegeWarGenelClose();

	}
	if (sOpCode == 58)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string nation;
		pkt >> nation;
		uint8 nt = atoi(nation.c_str());


		if (!g_pMain->isWarOpen())
		{
			// send description
			printf("Warning : Battle is not open.\n");
			return;
		}
		if (nt > 0 && nt < 3)
			g_pMain->BattleZoneResult(nt);

	}
	if (sOpCode == 59)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		ResetTempleEventData();
		g_pMain->TempleEventReset(g_pMain->pTempleEvent.ActiveEvent);
		g_pMain->TempleEventFinish();;
		g_pMain->TempleEventRoomClose();
	}

	//START EVENTS

	//GM COMMANDS
	if (sOpCode == 60)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string mesaj;
		uint8 type1;
		pkt >> mesaj >> type1;



		const char* cstr = mesaj.c_str();


		if (type1 == 1)
		{
			g_pMain->SendChat<GENERAL_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 2)
		{
			g_pMain->SendChat<PRIVATE_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 3)
		{
			g_pMain->SendChat<PARTY_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 4)
		{
			g_pMain->SendChat<FORCE_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 5)
		{
			g_pMain->SendChat<SHOUT_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 6)
		{
			g_pMain->SendChat<KNIGHTS_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 7)
		{
			g_pMain->SendChat<PUBLIC_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 8)
		{
			g_pMain->SendChat<WAR_SYSTEM_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 12)
		{
			g_pMain->SendChat<GM_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 13)
		{
			g_pMain->SendChat<COMMAND_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 14)
		{
			g_pMain->SendChat<MERCHANT_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 15)
		{
			g_pMain->SendChat<ALLIANCE_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 29)
		{
			g_pMain->SendChat<ANNOUNCEMENT_WHITE_CHAT>(cstr, ALL, true);
		}
		else if (type1 == 36)
		{
			g_pMain->LogosYolla("", cstr, 132, 112, 255);
		}
		return;



	}
	/*if (sOpCode == 61)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_GameServerShutDownOK = true;
		g_pMain->m_GameServerShutDownSeconds = 300;

		Packet x;
		x.Initialize(WIZ_LOGOSSHOUT);
		x << uint8(2) << uint8(6) << uint8(5);
		g_pMain->Send_All(&x);
		printf("Server Shut Down in (%d) Seconds \n", g_pMain->m_GameServerShutDownSeconds);

	}*/
	if (sOpCode == 62)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_sDiscount = 1;//savaþý kazanan ýrka indirim

	}
	if (sOpCode == 63)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_sDiscount = 2;//herese indirim

	}
	if (sOpCode == 64)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_bSantaOrAngel = FLYING_SANTA;
	}
	if (sOpCode == 65)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_bSantaOrAngel = FLYING_NONE;

	}
	if (sOpCode == 66)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_bSantaOrAngel = FLYING_ANGEL;
	}
	if (sOpCode == 67)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		g_pMain->m_bSantaOrAngel = FLYING_NONE;
	}
	if (sOpCode == 68)
	{


		if (m_GameMastersGmTools != 1)
		{
			// send description
			g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
			return;
		}

		string zone1, zone2;
		pkt >> zone1 >> zone2;

		uint8 z1 = atoi(zone1.c_str());
		uint8 z2 = atoi(zone2.c_str());


		if (z1 > 0 || z2 > 0)
			g_pMain->KickOutZoneUsers(z1, z2);

	}

	//GM COMMANDS


	if (sOpCode == 69)//lottery event
	{
		LotteryEvent(Lottery);

	}


	if (sOpCode == 70)//Bot Sistemi
	{
		uint8 count1;
		uint16 Time1;
		uint8 lvl;
		uint8 type1;


		pkt >> count1 >> Time1 >> lvl >> type1;


		for (int i = 0; i < count1; i++)
		{
			float Bonc = myrand(1, 15) * 1.0f;
			float Bonc2 = myrand(1, 15) * 1.0f;

			g_pMain->SpawnUserBot(Time1, GetZoneID(), GetX() + Bonc, GetY(), GetZ() + Bonc2, type1, lvl);
		}
		return;

	}
}








bool CGameServerDlg::HandleTournamentStart(std::string& clan1, std::string& clan2, uint8 zone)
{

	std::string TournamentName1 = clan1;

	std::string TournamentName2 = clan2;


	uint8 TournamentStartZoneID = zone;

	bool ClaniGM = false;
	bool SucsessZoneID = (TournamentStartZoneID == 77
		|| TournamentStartZoneID == 78
		|| TournamentStartZoneID == 96
		|| TournamentStartZoneID == 97
		|| TournamentStartZoneID == 98
		|| TournamentStartZoneID == 99);

	if (!SucsessZoneID)
	{
		// Send Game Server Description
		printf("Error: Invalid Tournament Zone(%d) \n", TournamentStartZoneID);
		return true;
	}

	if (TournamentName1.empty() || TournamentName1.size() > 21)
	{
		// Send Game Server Description
		printf("Error: TournamentName1 is empty or size > 21 \n");
		return true;
	}

	if (TournamentName2.empty() || TournamentName2.size() > 21)
	{
		// Send Game Server Description
		printf("Error: TournamentName2 is empty or size > 21 \n");
		return true;
	}

	if (TournamentName1.c_str() == TournamentName2.c_str())
	{
		// Send Game Server Description
		printf("Error: Two clan names are the same. \n");
		return true;
	}

	_TOURNAMENT_DATA* TournamentClanInfo = g_pMain->m_ClanVsDataList.GetData(TournamentStartZoneID);
	if (TournamentClanInfo != nullptr)
	{
		// Send Game Server Description
		printf("Error: Tournament is Zone(%d) Already Active \n", TournamentStartZoneID);
		return true;
	}

	_TOURNAMENT_DATA* Tour77 = g_pMain->m_ClanVsDataList.GetData(77);
	_TOURNAMENT_DATA* Tour78 = g_pMain->m_ClanVsDataList.GetData(78);
	_TOURNAMENT_DATA* Tour96 = g_pMain->m_ClanVsDataList.GetData(96);
	_TOURNAMENT_DATA* Tour97 = g_pMain->m_ClanVsDataList.GetData(97);
	_TOURNAMENT_DATA* Tour98 = g_pMain->m_ClanVsDataList.GetData(98);
	_TOURNAMENT_DATA* Tour99 = g_pMain->m_ClanVsDataList.GetData(99);

	CKnights* pAlreadyFirstClan = nullptr, * pAlreadySecondClan = nullptr;
	foreach_stlmap(itr, g_pMain->m_KnightsArray)
	{
		if (!itr->second->GetName().compare(TournamentName1))
			pAlreadyFirstClan = itr->second;

		if (!itr->second->GetName().compare(TournamentName2))
			pAlreadySecondClan = itr->second;
	}

	if (pAlreadyFirstClan != nullptr)
	{
		if (Tour77 != nullptr)
		{
			if (pAlreadyFirstClan->GetID() == Tour77->aTournamentClanNum[0]
				|| pAlreadyFirstClan->GetID() == Tour77->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour77->aTournamentZoneID);
				return true;

			}
		}

		if (Tour78 != nullptr)
		{
			if (pAlreadyFirstClan->GetID() == Tour78->aTournamentClanNum[0]
				|| pAlreadyFirstClan->GetID() == Tour78->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour78->aTournamentZoneID);
				return true;

			}
		}

		if (Tour96 != nullptr)
		{
			if (pAlreadyFirstClan->GetID() == Tour96->aTournamentClanNum[0]
				|| pAlreadyFirstClan->GetID() == Tour96->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour96->aTournamentZoneID);
				return true;

			}
		}

		if (Tour97 != nullptr)
		{
			if (pAlreadyFirstClan->GetID() == Tour97->aTournamentClanNum[0]
				|| pAlreadyFirstClan->GetID() == Tour97->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour97->aTournamentZoneID);
				return true;

			}
		}

		if (Tour98 != nullptr)
		{
			if (pAlreadyFirstClan->GetID() == Tour98->aTournamentClanNum[0]
				|| pAlreadyFirstClan->GetID() == Tour98->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour98->aTournamentZoneID);
				return true;

			}
		}

		if (Tour99 != nullptr)
		{
			if (pAlreadyFirstClan->GetID() == Tour99->aTournamentClanNum[0]
				|| pAlreadyFirstClan->GetID() == Tour99->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadyFirstClan->GetName().c_str(), Tour99->aTournamentZoneID);
				return true;

			}
		}
	}

	if (pAlreadySecondClan != nullptr)
	{
		if (Tour77 != nullptr)
		{
			if (pAlreadySecondClan->GetID() == Tour77->aTournamentClanNum[0]
				|| pAlreadySecondClan->GetID() == Tour77->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour77->aTournamentZoneID);
				return true;

			}
		}

		if (Tour78 != nullptr)
		{
			if (pAlreadySecondClan->GetID() == Tour78->aTournamentClanNum[0]
				|| pAlreadySecondClan->GetID() == Tour78->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour78->aTournamentZoneID);
				return true;

			}
		}

		if (Tour96 != nullptr)
		{
			if (pAlreadySecondClan->GetID() == Tour96->aTournamentClanNum[0]
				|| pAlreadySecondClan->GetID() == Tour96->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour96->aTournamentZoneID);
				return true;

			}
		}

		if (Tour97 != nullptr)
		{
			if (pAlreadySecondClan->GetID() == Tour97->aTournamentClanNum[0]
				|| pAlreadySecondClan->GetID() == Tour97->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour97->aTournamentZoneID);
				return true;

			}
		}

		if (Tour98 != nullptr)
		{
			if (pAlreadySecondClan->GetID() == Tour98->aTournamentClanNum[0]
				|| pAlreadySecondClan->GetID() == Tour98->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour98->aTournamentZoneID);
				return true;

			}
		}

		if (Tour99 != nullptr)
		{
			if (pAlreadySecondClan->GetID() == Tour99->aTournamentClanNum[0]
				|| pAlreadySecondClan->GetID() == Tour99->aTournamentClanNum[1])
			{
				// Send Game Server Description
				printf("Error: Second Specified clan (%s) is fighting against another clan. Other ZoneID: (%d) \n", pAlreadySecondClan->GetName().c_str(), Tour99->aTournamentZoneID);
				return true;

			}
		}
	}

	bool isClanVs = (TournamentStartZoneID == 77 || TournamentStartZoneID == 78);
	bool isPartyVs = (TournamentStartZoneID == 96 || TournamentStartZoneID == 97 || TournamentStartZoneID == 98 || TournamentStartZoneID == 99);
	if (isClanVs)
	{
		CKnights* pFirstClan = nullptr, * pSecondClan = nullptr;
		g_pMain->m_KnightsArray.m_lock.lock();
		foreach_stlmap_nolock(itr, g_pMain->m_KnightsArray)
		{
			if (!itr->second->GetName().compare(TournamentName1))
				pFirstClan = itr->second;

			if (!itr->second->GetName().compare(TournamentName2))
				pSecondClan = itr->second;
		}
		g_pMain->m_KnightsArray.m_lock.unlock();

		if (pFirstClan == nullptr)
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: First clan was not found in database \n");
			return true;
		}

		if (pSecondClan == nullptr)
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: Second clan was not found in database \n");
			return true;
		}

		uint16 FirstClanOnlineCount = 0;
		pFirstClan->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pFirstClan->m_arKnightsUser)
		{
			_KNIGHTS_USER* pFirstClanKnightUser = itr->second;
			if (pFirstClanKnightUser == nullptr)
				continue;

			CUser* pFirstClanUser = g_pMain->GetUserPtr(pFirstClanKnightUser->strUserName, TYPE_CHARACTER);
			if (pFirstClanUser == nullptr)
				continue;

			if (pFirstClanUser->isGM())
			{
				ClaniGM = true;
				break;
			}

			if (pFirstClanUser != nullptr
				&& pFirstClanUser->isInGame()
				&& pFirstClanUser->GetZoneID() != TournamentStartZoneID)
				FirstClanOnlineCount++;
		}
		pFirstClan->m_arKnightsUser.m_lock.unlock();

		if (ClaniGM == true)
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: There is an administrator in Clan (%s) \n", TournamentName1.c_str());
			return true;
		}

		if (FirstClanOnlineCount > TOURNAMENT_MAX_CLAN_COUNT)
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: %s Clan is clan high count. (count: %d) Maxmium Count: (%d)\n", TournamentName1.c_str(), FirstClanOnlineCount, TOURNAMENT_MAX_CLAN_COUNT);
			return true;
		}

		if (FirstClanOnlineCount < 1) //TOURNAMENT_MIN_CLAN_COUNT
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: %s Clan is clan low count. (count: %d) Minumum Count: (%d)\n", TournamentName1.c_str(), FirstClanOnlineCount, TOURNAMENT_MIN_CLAN_COUNT);
			return true;
		}

		uint16 SecondClanOnlineCount = 0;
		pSecondClan->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pSecondClan->m_arKnightsUser)
		{
			_KNIGHTS_USER* pSecondClanKnightUser = itr->second;
			if (pSecondClanKnightUser == nullptr)
				continue;

			CUser* pSecondClanUser = g_pMain->GetUserPtr(pSecondClanKnightUser->strUserName, TYPE_CHARACTER);
			if (pSecondClanUser == nullptr)
				continue;

			if (pSecondClanUser->isGM())
			{
				ClaniGM = true;
				break;
			}

			if (pSecondClanUser != nullptr
				&& pSecondClanUser->isInGame()
				&& pSecondClanUser->GetZoneID() != TournamentStartZoneID)
				SecondClanOnlineCount++;
		}
		pSecondClan->m_arKnightsUser.m_lock.unlock();

		if (ClaniGM == true)
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: There is an administrator in Clan (%s) \n", TournamentName2.c_str());
			return true;
		}

		if (SecondClanOnlineCount > TOURNAMENT_MAX_CLAN_COUNT)
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: %s Clan is clan high count. (count: %d) Maxmium Count: (%d) \n", TournamentName2.c_str(), SecondClanOnlineCount, TOURNAMENT_MAX_CLAN_COUNT);
			return true;
		}

		if (SecondClanOnlineCount < 1) //TOURNAMENT_MIN_CLAN_COUNT
		{
			// Send Game Server Description
			printf("Error : Clan Tournament: %s Clan is clan low count. (count: %d) Minumum Count: (%d) \n", TournamentName2.c_str(), SecondClanOnlineCount, TOURNAMENT_MIN_CLAN_COUNT);
			return true;
		}

		KickOutZoneUsers(TournamentStartZoneID);

		if (TournamentClanInfo == nullptr)
		{
			_TOURNAMENT_DATA* cTournamentSign = new _TOURNAMENT_DATA;
			cTournamentSign->aTournamentZoneID = TournamentStartZoneID;
			cTournamentSign->aTournamentClanNum[0] = pFirstClan->GetID();
			cTournamentSign->aTournamentClanNum[1] = pSecondClan->GetID();
			cTournamentSign->aTournamentScoreBoard[0] = 0;
			cTournamentSign->aTournamentScoreBoard[1] = 0;
			cTournamentSign->aTournamentTimer = 30 * 60;
			cTournamentSign->aTournamentMonumentKilled = 0;
			cTournamentSign->aTournamentOutTimer = UNIXTIME;
			cTournamentSign->aTournamentisAttackable = false;
			cTournamentSign->aTournamentisStarted = true;
			cTournamentSign->aTournamentisFinished = false;

			if (!m_ClanVsDataList.PutData(cTournamentSign->aTournamentZoneID, cTournamentSign))
				delete cTournamentSign;
		}

		pFirstClan->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pFirstClan->m_arKnightsUser)
		{
			_KNIGHTS_USER* pFirstClanKnightUser = itr->second;
			if (pFirstClanKnightUser == nullptr)
				continue;

			CUser* pFirstClanUser = g_pMain->GetUserPtr(pFirstClanKnightUser->strUserName, TYPE_CHARACTER);
			if (pFirstClanUser == nullptr
				|| !pFirstClanUser->isInGame()
				|| pFirstClanUser->GetZoneID() == TournamentStartZoneID)
				continue;

			pFirstClanUser->ZoneChange(TournamentStartZoneID, 0.0f, 0.0f);
		}
		pFirstClan->m_arKnightsUser.m_lock.unlock();

		pSecondClan->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pSecondClan->m_arKnightsUser)
		{
			_KNIGHTS_USER* pSecondClanKnightUser = itr->second;
			if (pSecondClanKnightUser == nullptr)
				continue;

			CUser* pSecondClanUser = g_pMain->GetUserPtr(pSecondClanKnightUser->strUserName, TYPE_CHARACTER);
			if (pSecondClanUser == nullptr
				|| !pSecondClanUser->isInGame()
				|| pSecondClanUser->GetZoneID() == TournamentStartZoneID)
				continue;

			pSecondClanUser->ZoneChange(TournamentStartZoneID, 0.0f, 0.0f);
		}
		pSecondClan->m_arKnightsUser.m_lock.unlock();
	}
	else if (isPartyVs)
	{
		CKnights* pFirstClanParty = nullptr, * pSecondClanParty = nullptr;
		g_pMain->m_KnightsArray.m_lock.lock();
		foreach_stlmap(itr, g_pMain->m_KnightsArray)
		{
			if (!itr->second->GetName().compare(TournamentName1))
				pFirstClanParty = itr->second;

			if (!itr->second->GetName().compare(TournamentName2))
				pSecondClanParty = itr->second;
		}
		g_pMain->m_KnightsArray.m_lock.unlock();

		if (pFirstClanParty == nullptr)
		{
			// Send Game Server Description
			printf("Error :Party Tournament: First clan was not found in database \n");
			return true;
		}

		if (pSecondClanParty == nullptr)
		{
			// Send Game Server Description
			printf("Error :Party Tournament: Second clan was not found in database \n");
			return true;
		}

		uint16 FirstClanOnlineCount = 0;
		pFirstClanParty->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pFirstClanParty->m_arKnightsUser)
		{
			_KNIGHTS_USER* pFirstClanPartyKnightUser = itr->second;
			if (pFirstClanPartyKnightUser == nullptr)
				continue;

			CUser* pFirstClanUser = g_pMain->GetUserPtr(pFirstClanPartyKnightUser->strUserName, TYPE_CHARACTER);
			if (pFirstClanUser != nullptr
				&& pFirstClanUser->isInGame()
				&& pFirstClanUser->GetZoneID() != TournamentStartZoneID)
				FirstClanOnlineCount++;
		}
		pFirstClanParty->m_arKnightsUser.m_lock.unlock();

		if (FirstClanOnlineCount > TOURNAMENT_MAX_PARTY_COUNT)
		{
			// Send Game Server Description
			printf("Error : Party Tournament: %s Clan is clan high count. (count: %d) Maximum Count (%d) \n", TournamentName1.c_str(), FirstClanOnlineCount, TOURNAMENT_MAX_PARTY_COUNT);
			return true;
		}

		if (FirstClanOnlineCount < TOURNAMENT_MIN_PARTY_COUNT)
		{
			// Send Game Server Description
			printf("Error : Party Tournament: %s Clan is clan low count. (count: %d) Minumum Count (%d)\n", TournamentName1.c_str(), FirstClanOnlineCount, TOURNAMENT_MIN_PARTY_COUNT);
			return true;
		}

		uint16 SecondClanOnlineCount = 0;
		pSecondClanParty->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pSecondClanParty->m_arKnightsUser)
		{
			_KNIGHTS_USER* pSecondClanPartyKnightUser = itr->second;
			if (pSecondClanPartyKnightUser == nullptr)
				continue;

			CUser* pSecondClanUser = g_pMain->GetUserPtr(pSecondClanPartyKnightUser->strUserName, TYPE_CHARACTER);
			if (pSecondClanUser != nullptr
				&& pSecondClanUser->isInGame()
				&& pSecondClanUser->GetZoneID() != TournamentStartZoneID)
				SecondClanOnlineCount++;
		}
		pSecondClanParty->m_arKnightsUser.m_lock.unlock();

		if (SecondClanOnlineCount > TOURNAMENT_MAX_PARTY_COUNT)
		{
			// Send Game Server Description
			printf("Error : Party Tournament: %s Clan is clan high count. (count: %d) Maximum Count (%d) \n", TournamentName2.c_str(), SecondClanOnlineCount, TOURNAMENT_MAX_PARTY_COUNT);
			return true;
		}

		if (SecondClanOnlineCount < TOURNAMENT_MIN_PARTY_COUNT)
		{
			// Send Game Server Description
			printf("Error : Party Tournament: %s Clan is clan low count. (count: %d) Minumum Count (%d)\n", TournamentName2.c_str(), SecondClanOnlineCount, TOURNAMENT_MIN_PARTY_COUNT);
			return true;
		}

		KickOutZoneUsers(TournamentStartZoneID);

		if (TournamentClanInfo == nullptr)
		{
			_TOURNAMENT_DATA* cTournamentSign = new _TOURNAMENT_DATA;
			cTournamentSign->aTournamentZoneID = TournamentStartZoneID;
			cTournamentSign->aTournamentClanNum[0] = pFirstClanParty->GetID();
			cTournamentSign->aTournamentClanNum[1] = pSecondClanParty->GetID();
			cTournamentSign->aTournamentScoreBoard[0] = 0;
			cTournamentSign->aTournamentScoreBoard[1] = 0;
			cTournamentSign->aTournamentTimer = 0;
			cTournamentSign->aTournamentMonumentKilled = 0;
			cTournamentSign->aTournamentOutTimer = UNIXTIME;
			cTournamentSign->aTournamentisAttackable = false;
			cTournamentSign->aTournamentisStarted = true;
			cTournamentSign->aTournamentisFinished = false;

			if (!m_ClanVsDataList.PutData(cTournamentSign->aTournamentZoneID, cTournamentSign))
				delete cTournamentSign;
		}

		pFirstClanParty->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pFirstClanParty->m_arKnightsUser)
		{
			_KNIGHTS_USER* pFirstClanPartyKnightUser = itr->second;
			if (pFirstClanPartyKnightUser == nullptr)
				continue;

			CUser* pFirstClanUser = g_pMain->GetUserPtr(pFirstClanPartyKnightUser->strUserName, TYPE_CHARACTER);
			if (pFirstClanUser == nullptr
				|| !pFirstClanUser->isInGame()
				|| pFirstClanUser->GetZoneID() == TournamentStartZoneID)
				continue;

			pFirstClanUser->ZoneChange(TournamentStartZoneID, 0.0f, 0.0f);
		}
		pFirstClanParty->m_arKnightsUser.m_lock.unlock();

		pSecondClanParty->m_arKnightsUser.m_lock.lock();
		foreach_stlmap_nolock(itr, pSecondClanParty->m_arKnightsUser)
		{
			_KNIGHTS_USER* pSecondClanPartyKnightUser = itr->second;
			if (pSecondClanPartyKnightUser == nullptr)
				continue;

			CUser* pSecondClanUser = g_pMain->GetUserPtr(pSecondClanPartyKnightUser->strUserName, TYPE_CHARACTER);
			if (pSecondClanUser == nullptr
				|| !pSecondClanUser->isInGame()
				|| pSecondClanUser->GetZoneID() == TournamentStartZoneID)
				continue;

			pSecondClanUser->ZoneChange(TournamentStartZoneID, 0.0f, 0.0f);
		}
		pSecondClanParty->m_arKnightsUser.m_lock.unlock();

	}
	printf("Final : Tournament is Start: Red Clan: (%s) - (%s) :Blue Clan Tournament Zone (%d) \n", TournamentName1.c_str(), TournamentName1.c_str(), TournamentStartZoneID);
	return true;
}