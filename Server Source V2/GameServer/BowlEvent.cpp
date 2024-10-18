#include "stdafx.h"

using namespace std;

#pragma region CUser::BowlXEvent1()

void CUser::BowlXEvent1()
{
	if (isInBowl())
	{
		if (BowlStatus == false)
		{
			g_pMain->SendHelpDescription(this, string_format("[TR] - Bowl'da durduðunuz sürece ödüller kazanabilirsiniz.", GetName().c_str()));
			g_pMain->SendHelpDescription(this, string_format("[EN] - You can win prizes as long as you stand in The Bowl.", GetName().c_str()));
			BowlStatus = true;
		}
	}

	if (BowlStatus == true && isInBowl())
	{
		uint32_t nRandonArray = myrand(1, g_pMain->m_MonsterBowlArray.GetSize());
		_MONSTER_BOWL_TABLE* PData = g_pMain->m_MonsterBowlArray.GetData(nRandonArray);

		if (PData == nullptr)
			return;

		PlayerEffect(PData->sEffect);
		SendKnightCash(1 + PData->Cashbonus);

	}
}

#pragma endregion


#pragma region CUser::isInBowl()

bool CUser::isInBowl()
{
	if (GetZoneID() != ZONE_RONARK_LAND)
		return false;

	return  ((GetX() > 892.0f && GetX() < 1166.0f)) && ((GetZ() > 969.0f && GetZ() < 1050.0f));
}

#pragma endregion

void CUser::EventBowlStatus()
{
	if (g_pMain->BownEvent)
	{
		if (isDead() || (bowlX == GetSPosX() && bowlz == GetSPosZ()))
			return;

		if (bowlxtimer1 + 1 * MINUTE < UNIXTIME && isInBowl())
		{
			bowlX = GetSPosX();
			bowlz = GetSPosZ();
			bowlxtimer1 = UNIXTIME;
			CUser::BowlXEvent1();
		}
	}
}