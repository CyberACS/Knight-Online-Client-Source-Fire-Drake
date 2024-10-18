#pragma once
#include "../shared/database/OdbcConnection.h"
#include "LoadServerData.h"

class CMonsterBowlTable : public OdbcRecordset
{
public:

	class CMonsterBowlTable(OdbcConnection* dbConnection, MonsterBowlArray* pMap) : OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName()
	{
		return _T("K_BOWL_PK");
	}

	virtual tstring GetColumns()
	{
		return _T("sIndex, Cashbonus, PlayerEffectGive");
	}

	virtual bool Fetch()
	{
		_MONSTER_BOWL_TABLE* pData = new _MONSTER_BOWL_TABLE;

		_dbCommand->FetchUInt16(1, pData->sIndex);
		_dbCommand->FetchUInt32(2, pData->Cashbonus);
		_dbCommand->FetchUInt32(7, pData->sEffect);

		if (!m_pMap->PutData(pData->sIndex, pData))
			delete pData;

		return true;
	}

	MonsterBowlArray* m_pMap;
};