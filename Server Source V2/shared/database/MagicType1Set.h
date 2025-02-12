#pragma once

class CMagicType1Set : public OdbcRecordset
{
public:
	CMagicType1Set(OdbcConnection* dbConnection, Magictype1Array* pMap)
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAGIC_TYPE1"); }
	virtual tstring GetColumns() { return _T("iNum, Type, HitRate, Hit, AddDamage, Range, ComboType, ComboCount, ComboDamage, Delay"); }

	virtual bool Fetch()
	{
		_MAGIC_TYPE1* pData = new _MAGIC_TYPE1;

		_dbCommand->FetchUInt32(1, pData->iNum);
		_dbCommand->FetchByte(2, pData->bHitType);
		_dbCommand->FetchUInt16(3, pData->sHitRate);
		_dbCommand->FetchUInt16(4, pData->sHit);
		_dbCommand->FetchUInt16(5, pData->sAddDamage);
		_dbCommand->FetchUInt16(6, pData->sRange);
		_dbCommand->FetchByte(7, pData->bComboType);
		_dbCommand->FetchByte(8, pData->bComboCount);
		_dbCommand->FetchUInt16(9, pData->sComboDamage);
		_dbCommand->FetchByte(10, pData->bDelay);

		if (!m_pMap->PutData(pData->iNum, pData))
			delete pData;

		return true;
	}

	Magictype1Array* m_pMap;
};