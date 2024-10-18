#include "stdafx.h"
#include "DBAgent.h"
#include "md5.h"
#include "MagicInstance.h"

void CUser::CyberACS_BotPriestSystem()
{
	bool Partybuff = false;
	
	bool buff = false;
	bool buffacc = false;

	CBot* pPriest = nullptr;
	pPriest = g_pMain->m_BotArray.GetData(m_bUserPriestBotID);


	
	if (pPriest != nullptr && m_bUserPriestBotID > 0)
	{
		_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());

		if (pParty != nullptr)//toplu 10k eğer 2 kişinin hp si %80 altına düşerse toplu 10k at
		{
			uint8 sayi;
			sayi = 0;
			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr && PartyUser->m_sHp < (PartyUser->m_MaxHp * 8) / 10) // %80 in altındaysa 
				{
					sayi++; //%80 altında kişi sayısı
				}
			}
			if (sayi >= 2)
			{
				for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
				{
					auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
					if (PartyUser != nullptr)
					{
						MagicInstance instance;
						instance.bOpcode = 1;
						instance.nSkillID = 212560;

						instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


						instance.sCasterID = pPriest->GetID();
						instance.sTargetID = PartyUser->GetSocketID();
						instance.sData[0] = 0;
						instance.sData[1] = 0;
						instance.sData[2] = 0;
						instance.sData[3] = 0;
						instance.sData[4] = 0;
						instance.sData[5] = 0;
						instance.sData[6] = 0;


						instance.sSkillCasterZoneID = PartyUser->GetZoneID();
						instance.bIsRecastingSavedMagic = false;
						instance.Run(); //priest animasyonu


						//heal kısmı

						instance.bOpcode = 3;
						instance.nSkillID = 212560;

						instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


						instance.sCasterID = pPriest->GetID();
						instance.sTargetID = PartyUser->GetSocketID();
						instance.sData[0] = 0;
						instance.sData[1] = 0;
						instance.sData[2] = 0;
						instance.sData[3] = 0;
						instance.sData[4] = 0;
						instance.sData[5] = 0;
						instance.sData[6] = 0;


						instance.sSkillCasterZoneID = PartyUser->GetZoneID();
						instance.bIsRecastingSavedMagic = false;
						instance.Run(); //priest animasyonu


					}
				}
			}
		}

		if (pParty != nullptr)//tekli 1920 
		{
			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr && PartyUser->m_sHp < (PartyUser->m_MaxHp * 8) / 10) // %80 in altındaysa 
				{
					MagicInstance instance;
					instance.bOpcode = 1;
					instance.nSkillID = 212545;//1920 heal

					instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


					instance.sCasterID = pPriest->GetID();
					instance.sTargetID = PartyUser->GetSocketID();
					instance.sData[0] = 0;
					instance.sData[1] = 0;
					instance.sData[2] = 0;
					instance.sData[3] = 0;
					instance.sData[4] = 0;
					instance.sData[5] = 0;
					instance.sData[6] = 0;


					instance.sSkillCasterZoneID = PartyUser->GetZoneID();
					instance.bIsRecastingSavedMagic = false;
					instance.Run();//priest animasyonu

					//heal basma kısmı
					instance.bOpcode = 3;
					instance.nSkillID = 212545;//1920 heal

					instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


					instance.sCasterID = pPriest->GetID();
					instance.sTargetID = PartyUser->GetSocketID();
					instance.sData[0] = 0;
					instance.sData[1] = 0;
					instance.sData[2] = 0;
					instance.sData[3] = 0;
					instance.sData[4] = 0;
					instance.sData[5] = 0;
					instance.sData[6] = 0;


					instance.sSkillCasterZoneID = PartyUser->GetZoneID();
					instance.bIsRecastingSavedMagic = false;
					instance.Run();//priest animasyonu
					break;
				}
			}
		}
		else
		{
			if (m_sHp < (m_MaxHp * 8) / 10) // %80 in altındaysa 
			{

				MagicInstance instance;
				instance.bOpcode = 1;
				instance.nSkillID = 212545;//1920 heal

				instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


				instance.sCasterID = pPriest->GetID();
				instance.sTargetID = GetSocketID();
				instance.sData[0] = 0;
				instance.sData[1] = 0;
				instance.sData[2] = 0;
				instance.sData[3] = 0;
				instance.sData[4] = 0;
				instance.sData[5] = 0;
				instance.sData[6] = 0;


				instance.sSkillCasterZoneID = GetZoneID();
				instance.bIsRecastingSavedMagic = false;
				instance.Run();//priest animasyonus


				//heal basma kısmı
				instance.bOpcode = 3;
				instance.nSkillID = 212545;//1920 heal

				instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


				instance.sCasterID = pPriest->GetID();
				instance.sTargetID = GetSocketID();
				instance.sData[0] = 0;
				instance.sData[1] = 0;
				instance.sData[2] = 0;
				instance.sData[3] = 0;
				instance.sData[4] = 0;
				instance.sData[5] = 0;
				instance.sData[6] = 0;


				instance.sSkillCasterZoneID = GetZoneID();
				instance.bIsRecastingSavedMagic = false;
				instance.Run();//priest animasyonus
			}
		}

		
		
		foreach(itr, m_buffMap)
		{
			if (itr->second.m_nSkillID == 212675)
			{
				buff = true;
			}
			if (itr->second.m_nSkillID == 212674)
			{
				buffacc = true;
			}
		}

		
		if (pParty != nullptr)//buff
		{
			uint8 PartyMember,PartyUsedSkill;
			PartyMember = 0;
			PartyUsedSkill = 0;

			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr)
				{
					PartyMember++;
				}
			}

			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr) 
				{

					foreach(itr, PartyUser->m_buffMap)
					{
						if (itr->second.m_nSkillID == 212675)
						{
							PartyUsedSkill++;
						}
					}
				}
			}

			if(PartyMember == PartyUsedSkill)
				Partybuff = true;

			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr && !Partybuff) // %80 in altındaysa 
				{
					MagicInstance instance;
					instance.bOpcode = 1;
					instance.nSkillID = 212675;//buff

					instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


					instance.sCasterID = pPriest->GetID();
					instance.sTargetID = PartyUser->GetSocketID();
					instance.sData[0] = 0;
					instance.sData[1] = 0;
					instance.sData[2] = 0;
					instance.sData[3] = 0;
					instance.sData[4] = 0;
					instance.sData[5] = 0;
					instance.sData[6] = 0;


					instance.sSkillCasterZoneID = PartyUser->GetZoneID();
					instance.bIsRecastingSavedMagic = false;
					instance.Run();//priest animasyonu

					//heal basma kısmı
					instance.bOpcode = 3;
					instance.nSkillID = 212675;//buff

					instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


					instance.sCasterID = pPriest->GetID();
					instance.sTargetID = PartyUser->GetSocketID();
					instance.sData[0] = 0;
					instance.sData[1] = 0;
					instance.sData[2] = 0;
					instance.sData[3] = 0;
					instance.sData[4] = 0;
					instance.sData[5] = 0;
					instance.sData[6] = 0;


					instance.sSkillCasterZoneID = PartyUser->GetZoneID();
					instance.bIsRecastingSavedMagic = false;
					instance.Run();//priest animasyonu
				}
			}
		}
		else if (pParty == nullptr && !buff)//buff
		{


			MagicInstance instance;
			instance.bOpcode = 1;
			instance.nSkillID = 212675;//buf

			instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


			instance.sCasterID = pPriest->GetID();
			instance.sTargetID = GetSocketID();
			instance.sData[0] = 0;
			instance.sData[1] = 0;
			instance.sData[2] = 0;
			instance.sData[3] = 0;
			instance.sData[4] = 0;
			instance.sData[5] = 0;
			instance.sData[6] = 0;


			instance.sSkillCasterZoneID = GetZoneID();
			instance.bIsRecastingSavedMagic = false;
			instance.Run();//priest animasyonus


			//işlem
			instance.bOpcode = 3;
			instance.nSkillID = 212675;//buf

			instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


			instance.sCasterID = pPriest->GetID();
			instance.sTargetID = GetSocketID();
			instance.sData[0] = 0;
			instance.sData[1] = 0;
			instance.sData[2] = 0;
			instance.sData[3] = 0;
			instance.sData[4] = 0;
			instance.sData[5] = 0;
			instance.sData[6] = 0;


			instance.sSkillCasterZoneID = GetZoneID();
			instance.bIsRecastingSavedMagic = false;
			instance.Run();//priest animasyonus
		}
		if (pParty != nullptr)
		{
			bool Partybuffacc = false;
			uint8 PartyMember, PartyUsedSkill;
			PartyMember = 0;
			PartyUsedSkill = 0;

			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr)
				{
					PartyMember++;
				}
			}

			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr)
				{

					foreach(itr, PartyUser->m_buffMap)
					{
						if (itr->second.m_nSkillID == 212674)
						{
							PartyUsedSkill++;
						}
					}
				}
			}

			if (PartyMember == PartyUsedSkill)
				Partybuffacc = true;

			for (uint8 i = 0; i < MAX_PARTY_USERS; i++)
			{
				auto* PartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (PartyUser != nullptr && !Partybuffacc) // %80 in altındaysa 
				{
					MagicInstance instance;
					instance.bOpcode = 1;
					instance.nSkillID = 212674;//buff

					instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


					instance.sCasterID = pPriest->GetID();
					instance.sTargetID = PartyUser->GetSocketID();
					instance.sData[0] = 0;
					instance.sData[1] = 0;
					instance.sData[2] = 0;
					instance.sData[3] = 0;
					instance.sData[4] = 0;
					instance.sData[5] = 0;
					instance.sData[6] = 0;


					instance.sSkillCasterZoneID = PartyUser->GetZoneID();
					instance.bIsRecastingSavedMagic = false;
					instance.Run();//priest animasyonu

					//heal basma kısmı
					instance.bOpcode = 3;
					instance.nSkillID = 212674;//buff

					instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


					instance.sCasterID = pPriest->GetID();
					instance.sTargetID = PartyUser->GetSocketID();
					instance.sData[0] = 0;
					instance.sData[1] = 0;
					instance.sData[2] = 0;
					instance.sData[3] = 0;
					instance.sData[4] = 0;
					instance.sData[5] = 0;
					instance.sData[6] = 0;


					instance.sSkillCasterZoneID = PartyUser->GetZoneID();
					instance.bIsRecastingSavedMagic = false;
					instance.Run();//priest animasyonu
				}
			}
		}
		else if (pParty == nullptr && !buffacc)//buff  acccc
		{


			MagicInstance instance;
			instance.bOpcode = 1;
			instance.nSkillID = 212674;//buf

			instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


			instance.sCasterID = pPriest->GetID();
			instance.sTargetID = GetSocketID();
			instance.sData[0] = 0;
			instance.sData[1] = 0;
			instance.sData[2] = 0;
			instance.sData[3] = 0;
			instance.sData[4] = 0;
			instance.sData[5] = 0;
			instance.sData[6] = 0;


			instance.sSkillCasterZoneID = GetZoneID();
			instance.bIsRecastingSavedMagic = false;
			foreach(itr, m_buffMap)
			{
				if (itr->second.m_nSkillID == 212674)
					return;
			}
			instance.Run();//priest animasyonus


			//işlem
			instance.bOpcode = 3;
			instance.nSkillID = 212674;//buf

			instance.pSkill = g_pMain->m_MagictableArray.GetData(instance.nSkillID);


			instance.sCasterID = pPriest->GetID();
			instance.sTargetID = GetSocketID();
			instance.sData[0] = 0;
			instance.sData[1] = 0;
			instance.sData[2] = 0;
			instance.sData[3] = 0;
			instance.sData[4] = 0;
			instance.sData[5] = 0;
			instance.sData[6] = 0;


			instance.sSkillCasterZoneID = GetZoneID();
			instance.bIsRecastingSavedMagic = false;
			instance.Run();//priest animasyonus
		}
	}
}



