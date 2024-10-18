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


void CUser::LotteryEvent(Packet& pkt)
{
	if (!isGM())
		return;

	if (m_GameMastersGmTools != 1)
	{
		// send description
		g_pMain->SendHelpDescription(this, "Gerekli Yetki Yok. Lütfen Yetki Sýnýrlandýrmasý için Yönetici ile konuþunuz.");
		return;
	}

	
	
	uint8 sOpCode;
	pkt >> sOpCode;

	if (sOpCode == 1)//lottery kayýt sistemi
	{
		if (!isInGame())
			return;

		uint8 sOpCode2;
		pkt >> sOpCode2;

		if (sOpCode2 == 1)//Kayýt
		{
			vector<LOTTERY_USERS> pLotteryUsers;

			if (PurchasedLottery >= 0) 
			{
				if (PurchasedLottery == g_pMain->TotalLottery)
					return;


				if (g_pMain->isKC == 1)
				{ 
					if (m_nKnightCash > g_pMain->RequiredMoney)
						SendKnightCash(-g_pMain->RequiredMoney);
					else
						return;
				}
				else if (g_pMain->isKC == 0)
				{
					if (m_iGold > g_pMain->RequiredMoney)
						GoldGain(-g_pMain->RequiredMoney);
					else
						return;
				}

				

				g_pMain->TotalPurchasedLottery++;
				PurchasedLottery++;
				for (auto users : pLotteryUsers)
				{
					if (users.Name == GetName())
					{

						users.TotalTicket = PurchasedLottery;
						Packet result(PL_LOTTERY);
						result << uint8(2) << PurchasedLottery << g_pMain->TotalPurchasedLottery;
						Send(&result);
						return;

					}
				}

				pLotteryUsers.push_back(LOTTERY_USERS(GetName(), PurchasedLottery,0));

				User = pLotteryUsers;

				Packet result(PL_LOTTERY);
				result << uint8(2) << PurchasedLottery << g_pMain->TotalPurchasedLottery;
				Send(&result);




			}
			

		}
		else if (sOpCode2 == 2)//Kayýt Silme
		{
			if (!isInGame())
				return;

			if (PurchasedLottery > 0) 
			{
				if (g_pMain->isKC == 1)
				{
					SendKnightCash(g_pMain->RequiredMoney);
				}
				else if (g_pMain->isKC == 0)
				{
					GoldGain(g_pMain->RequiredMoney);
				}

				g_pMain->TotalPurchasedLottery--;
				PurchasedLottery--;

				for (auto users : User)
				{
					if (users.Name == GetName())
					{

						users.TotalTicket = PurchasedLottery;
						Packet result(PL_LOTTERY);
						result << uint8(2) << PurchasedLottery << g_pMain->TotalPurchasedLottery;
						Send(&result);
						return;
					}
				}

				Packet result(PL_LOTTERY);
				result << uint8(2) << PurchasedLottery << g_pMain->TotalPurchasedLottery;
				Send(&result);
			}

			if (PurchasedLottery == 0)
			{
				for (auto iter = User.begin(); iter != User.end(); ++iter)
				{
					if (iter->TotalTicket == 0)
					{
						iter = User.erase(iter);
						break;
					}
				}
			}
			
		}
		else if (sOpCode2 == 3)//Kazananlar iþlemi
		{
			uint16 sans = myrand(10, g_pMain->TotalLottery);	
			uint16 UserChances[10000];
			
			uint16 random = myrand(0, g_pMain->TotalLottery);
			

			uint16 sira = 0;
			for (auto iter = User.begin(); iter != User.end(); ++iter)
			{
				
				for (int i = 0; i < g_pMain->TotalLottery; i += 10) 
				{
					if (iter->TotalTicket > 0+i && iter->TotalTicket < 10+i)
					{
						iter->chance = 10+i;	
					}
				}
				UserChances[sira] = iter->chance;
				sira++;
			}

		
			for (int i = 0; i < g_pMain->TotalWinner; i++) 
			{
				int n = sizeof(UserChances) / sizeof(UserChances[random]);
				int diff = abs(UserChances[0] - sans), idx{ 0 };
				for (int i = 0; i < n; i++) 
				{
					if (abs(UserChances[i] - sans) < diff) 
					{
						diff = abs(UserChances[i] - sans);
						idx = i;
					}
				}

				uint16 sira2 = 0;
				int b = 0;
				for (auto iter = User.begin(); iter != User.end(); ++iter)
				{

					if (sira2 == idx)
					{
						
							names[b] = iter->Name;

							i++;

					}
						sira2++;
				}

			}
			for (int i = 0; i < names->size(); i++)
			{
				uint8 rewardCount = 0;
				std::map<uint32, uint32> rewardMap;
				CUser* pWinner = g_pMain->GetUserPtr(names[i], TYPE_CHARACTER);

				for (int i = 0; i < 2; i++) {
					if (g_pMain->m_LotterySettings->NumReward[i] > 1 && g_pMain->m_LotterySettings->EventItemCount[i] > 0 && g_pMain->m_LotterySettings->EventItemTimed[i] > 0) {
						GiveItem(g_pMain->m_LotterySettings->NumReward[i], g_pMain->m_LotterySettings->EventItemCount[i], true, g_pMain->m_LotterySettings->EventItemTimed[i]);
						rewardMap.insert(std::make_pair(g_pMain->m_LotterySettings->NumReward[i] + (rewardCount++), g_pMain->m_LotterySettings->EventItemCount[i]));
					}
					else if (g_pMain->m_LotterySettings->NumReward[i] > 0 && g_pMain->m_LotterySettings->EventItemCount[i] > 0) {
						GiveItem(g_pMain->m_LotterySettings->NumReward[i], g_pMain->m_LotterySettings->EventItemCount[i]);
						rewardMap.insert(std::make_pair(g_pMain->m_LotterySettings->NumReward[i] + (rewardCount++), g_pMain->m_LotterySettings->EventItemCount[i]));
					}
				}

				Packet result(WIZ_QUEST);
				result << uint8(10);

				foreach(itr, rewardMap)
					result << itr->first << itr->second;

				result << uint32(0);
				Send(&result);


				g_pMain->SendFormattedNotice("Lottery event bitmiþtir. Kazananlar: [%d]", Nation::ALL, pWinner->GetName().c_str());
			}

			User.clear();
			g_pMain->OpenLottery == false;
			g_pMain->Time = 0;
			g_pMain->TotalWinner = 0;
			g_pMain->RequiredMoney = 0;
			g_pMain->rewardid = 0;
			g_pMain->TotalPurchasedLottery = 0;
			memset(&PurchasedLottery, 0, sizeof(PurchasedLottery));
			Packet result(PL_LOTTERY);
			result << uint8(0);
			Send(&result);
		}


		


	}
	else if (sOpCode == 69)//lottery ayarlarý
	{
		uint8 sOpCode;
		pkt >> sOpCode;

		if (sOpCode == 1) {
			if (g_pMain->OpenLottery == true)
			{
				g_pMain->SendHelpDescription(this, "Lottery Event Aktif Lütfen Eventin bitmesini bekleyiniz.");
				return;
			}
			pkt >> g_pMain->TotalLottery >> g_pMain->Time >> g_pMain->TotalWinner >> g_pMain->RequiredMoney >> g_pMain->rewardid >> g_pMain->isKC;

			g_pMain->m_LotterySettings = g_pMain->m_LotteryArray.GetData(g_pMain->rewardid);

			if(g_pMain->m_LotterySettings == nullptr)
			{
				g_pMain->SendHelpDescription(this, "Lottery Event ID Eslesmedi ornek;1,1,1,1.");
				return;
			}

			if (g_pMain->isKC < 0 || g_pMain->isKC > 1)
				g_pMain->isKC = 0;//noah

			//Start Event
			Packet result(PL_LOTTERY);
			result << uint8(1) << g_pMain->TotalLottery << g_pMain->Time << g_pMain->TotalWinner << g_pMain->RequiredMoney << g_pMain->isKC;

			

			for (int i = 0; i < 3; i++)
				result << g_pMain->m_LotterySettings->NumReward[i] << g_pMain->m_LotterySettings->EventItemCount[i];
			Send(&result);

			g_pMain->OpenLottery = true;

			g_pMain->LogosYolla("", "Lottery Event baþlamýþtýr.Herkese þans dileriz.", 132, 112, 255);
			//Start Event
		}
		else if (sOpCode == 0)
		{
			//Close Event
			if (g_pMain->OpenLottery == true)
			{
				g_pMain->OpenLottery = false;

				g_pMain->Time = 0;
				g_pMain->TotalWinner = 0;
				g_pMain->RequiredMoney = 0;
				g_pMain->rewardid = 0;
				g_pMain->TotalPurchasedLottery = 0;
				memset(&PurchasedLottery, 0, sizeof(PurchasedLottery));
				g_pMain->LogosYolla("", "Lottery Event sona ermiþtir. Katýlýmcýlara teþekkürler.", 132, 112, 255);

				Packet result(PL_LOTTERY);
				result << uint8(0);
				Send(&result);

			}
			//Close Event
		}

	}

}


