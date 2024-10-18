#include "stdafx.h"
#include "DBAgent.h"



void CUser::VsEvent(Packet& pkt)
{
	uint8 sOpCode;
	pkt >> sOpCode;
	if (sOpCode == 1)
	{
		C3DMap* pMap = g_pMain->GetZoneByID(ZONE_ARENA);
		std::string Loyality, Kill;
		Loyality = "";
		Kill = "";
		uint16 GetID = 0;

		pkt >> Loyality >> Kill >> GetID;

		uint32 Loyality1 = atoi(Loyality.c_str());
		uint16 Kill1 = atoi(Kill.c_str());

		CUser* pUser = g_pMain->GetUserPtr(GetID);


		if (pUser == nullptr)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Kullanıcı Adi Hatali Yada Oyunda Değil");
			return;
		}

		if (GetLevel() < pMap->GetMinLevelReq()  || pUser->GetLevel() < pMap->GetMinLevelReq())
		{
			g_pMain->SendHelpDescription(this, string_format("[1vs1 Event] = Karşı Tarafın Yada Senin Level'ın %d'den Küçüktür.", pMap->GetMinLevelReq()));
			return ;
		}

		if (GetLoyalty() < Loyality1)
		{
			g_pMain->SendHelpDescription(this, string_format("[1vs1 Event] = 1vs1 Yapabilmek Icin Uzerinizde En Az %d NP Olmasi Gereklidir", Loyality1));
			return ;
		}

		if (Loyality == "")
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Kullanım : Np Miktarını Düzgün Giriniz.");
			return ;
		}

		//if (pUser->VsYapilacakKullanici )

		

		if (pUser->GetLoyalty() < Loyality1)
		{
			g_pMain->SendHelpDescription(this, string_format("[1vs1 Event] = Karsi Tarafin Np Puani 1vs1 Event Yapmak Icin Yeterli Degil"));
			return ;
		}

		if (GetName() == pUser->GetName())
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Kendinize Vs Istegi Gonderemezsiniz");
			return ;
		}

		if (pUser->isGM())
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Oyun Yöneticisine Teklif Yapamazsınız");
			return ;
		}

	

		if (Loyality1 > g_pMain->vsEventMaxNp)
		{
			g_pMain->SendHelpDescription(this, string_format("[1vs1 Event] = NP Maksimum %d Olabilir", g_pMain->vsEventMaxNp));
			return ;
		}

		if (pUser->m_VsDurumu == false)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Karsi Taraf Suanda Vs Yapamaz");
			return ;
		}

		if (pUser->GetLoyalty() < Loyality1)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Karsi Tarafin Np Puani Vs Icin Yeterli Degil");
			return ;
		}

		if (GetLoyalty() < Loyality1)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Np Puaniniz Vs Icin Yeterli Degil");
			return ;
		}


		g_pMain->SendHelpDescription(this, string_format("[Vs Event] = %s Adlı Kullanıcıya Vs Teklifiniz Gönderildi", pUser->GetName().c_str()));

		pUser->VsYapilacakKullanici = GetName();
		VsYapilacakKullanici = pUser->GetName();
		pUser->VsMiktari = Loyality1;
		VsMiktari = Loyality1;
		pUser->VsKillMiktari = Kill1;
		VsKillMiktari = Kill1;

		Packet result1(PL_VS_EVENT);
		result1 << GetName() << Loyality1;
		pUser->Send(&result1);

	}	


	if (sOpCode == 2)//vs event kabul işlemleri
	{
		if (GetZoneID() == ZONE_ARENA)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Vs Kabul Edebilmek İçin Maradona Geçiniz");
			return ;
		}

		CUser* pUser = g_pMain->GetUserPtr(VsYapilacakKullanici, TYPE_CHARACTER);

		if (pUser == nullptr)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Kullanıcı Adı Hatalı Yada Oyunda Değil");
			return ;
		}

		if (!pUser->m_VsDurumu)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Kullanici Zaten Bir Vs De");
			return ;
		}

		if (!m_VsDurumu)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Zaten Şuanda Aktif Bir Vs Yapıyorsunuz");
			return ;
		}

		++g_pMain->m_VsEventLastRoom; // SANAL ODA 

		printf("[Event Manager 1vs1] = %s Vs %s Np : %d Basladi\n", pUser->GetName().c_str(), GetName().c_str(), VsMiktari);

		pUser->m_bEventRoom = g_pMain->m_VsEventLastRoom;
		m_bEventRoom = g_pMain->m_VsEventLastRoom;
		g_pMain->LogosYolla("[VS Event]", string_format("%s vs %s Karşılaşması Başladı | Bahis : %d National Point", GetName().c_str(), pUser->GetName().c_str(), VsMiktari), 255, 156, 3);
		pUser->m_VsDurumu = false;
		m_VsDurumu = false;

		ZoneChange(ZONE_ARENA, 195, 84);
		pUser->ZoneChange(ZONE_ARENA, 178, 50);

		

		return ;

	}

	if (sOpCode == 3)//vs event iptal işlemleri
	{
		CUser* pUser = g_pMain->GetUserPtr(VsYapilacakKullanici, TYPE_CHARACTER);

		if (pUser == nullptr)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Kullanıcı Adı Hatalı Yada Oyunda Değil");
			return ;
		}

		if (pUser->m_VsDurumu == false)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Kullanıcı Şuanda Başka Bir Vs De");
			return ;
		}

		if (m_VsDurumu == false)
		{
			g_pMain->SendHelpDescription(this, "[1vs1 Event] = Karşılaşmadayken Vs Red Yapamazsınız");
			return ;
		}

		g_pMain->SendHelpDescription(pUser, string_format("[1vs1 Event] = %s Vs İsteğini Kabul Etmedi", GetName().c_str()));

		pUser->m_VsDurumu = true;
		pUser->m_bEventRoom = 0;
		pUser->VsMiktari = 0;
		pUser->VsYapilacakKullanici.clear();

		m_VsDurumu = true;
		m_bEventRoom = 0;
		VsMiktari = 0;
		VsYapilacakKullanici.clear();

		g_pMain->SendHelpDescription(this, "[1vs1 Event] = Vs İsteğini Kabul Etmediniz");
		return ;
	}
}



void CUser::VSEventProcess(int Code)
{
	if (Code == 1)
	{
		if (m_VsDurumu || GetZoneID() != ZONE_ARENA || GetEventRoom() < 1)
			return;

		CUser* VsEventUser = g_pMain->GetUserPtr(VsYapilacakKullanici, TYPE_CHARACTER);

		if (VsEventUser == nullptr)
			return;

		if (VsEventUser->m_VsDurumu || VsEventUser->GetZoneID() != ZONE_ARENA || VsEventUser->GetEventRoom() < 1)
			return;

		if (VsEventUser->GetSocketID() == GetSocketID() || VsEventUser->GetName() == GetName())
			return;

		g_DBAgent.UpdateVSEventLog(VsEventUser->GetName(), GetName(), VsMiktari);

		short Kaybeden = 0;
		Kaybeden -= VsMiktari;
		
		SendLoyaltyChange(Kaybeden, false, false, false);
		VsEventUser->SendLoyaltyChange(VsMiktari, false);

		VsEventUser->YourKill++;
	}

	else if (Code == 2)
	{
		CUser* pUserKarsi = g_pMain->GetUserPtr(VsYapilacakKullanici, TYPE_CHARACTER);

		if (pUserKarsi->VsKillMiktari > 0 && VsKillMiktari > 0 )
		{
			pUserKarsi->VsKillMiktari--;
			VsKillMiktari--;
		}

		if (pUserKarsi->VsKillMiktari == 0  && VsKillMiktari == 0)
		{
		

			m_bEventRoom = 0;
			VsMiktari = 0;
			m_VsDurumu = true;
			ZoneChange(21, 0.0f, 0.0f);
			YourKill = 0;
			VsYapilacakKullanici.clear();

			

			if (pUserKarsi != nullptr)
			{
				pUserKarsi->m_bEventRoom = 0;
				pUserKarsi->VsMiktari = 0;
				pUserKarsi->m_VsDurumu = true;
				pUserKarsi->ZoneChange(21, 0.0f, 0.0f);
				pUserKarsi->YourKill = 0;
				pUserKarsi->VsYapilacakKullanici.clear();
			}	

			if (pUserKarsi->YourKill > YourKill) {
				g_pMain->LogosYolla("[VS Event]", string_format("%s vs %s Karsilasmasinin Galibi : %s Tebrikler!", GetName().c_str(), pUserKarsi->GetName().c_str(), pUserKarsi->GetName().c_str()), 255, 156, 3);
			}
			else
			{
				g_pMain->LogosYolla("[VS Event]", string_format("%s vs %s Karsilasmasinin Galibi : %s Tebrikler!", GetName().c_str(), pUserKarsi->GetName().c_str(), pUserKarsi->GetName().c_str()), 255, 156, 3);
			}
		}
	}

}