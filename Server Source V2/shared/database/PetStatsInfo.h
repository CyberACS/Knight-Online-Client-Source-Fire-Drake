#pragma once

class CPetStatsInfoSet : public OdbcRecordset
{
public:
	CPetStatsInfoSet(OdbcConnection* dbConnection, PetInfoSystemArray* pMap)
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("PET_STATS_INFO"); }
	virtual tstring GetColumns() { return _T("PetLevel, PetMaxHp, PetMaxSp, PetAttack, PetDefence, PetRes, PetExp"); }

	virtual bool Fetch()
	{
		PET_INFO* pData = new PET_INFO;

		_dbCommand->FetchByte(1, pData->PetLevel);
		_dbCommand->FetchUInt16(2, pData->PetMaxHP);
		_dbCommand->FetchUInt16(3, pData->PetMaxMP);
		_dbCommand->FetchUInt16(4, pData->PetAttack);
		_dbCommand->FetchUInt16(5, pData->PetDefense);
		_dbCommand->FetchByte(6, pData->PetRes);
		_dbCommand->FetchUInt32(7, pData->PetExp);

		if (!m_pMap->PutData(pData->PetLevel, pData))
			delete pData;

		return true;
	}

	PetInfoSystemArray* m_pMap;
};