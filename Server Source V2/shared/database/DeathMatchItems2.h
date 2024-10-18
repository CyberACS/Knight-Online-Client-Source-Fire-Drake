#pragma once

class CDeathMatchItems2 : public OdbcRecordset
{
public:
	CDeathMatchItems2(OdbcConnection* dbConnection, DeathMatchItemsArray2* pMap)
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("CYBERACS_DEATHMATCH_ITEMS2"); }
	virtual tstring GetColumns() { return _T("SlotID, ItemID, ItemDuration, ItemCount"); }

	virtual bool Fetch()
	{
		_ITEM_DATA* pData = new _ITEM_DATA;
		uint8 slot = 0;
		memset(pData, 0x00, sizeof(_ITEM_DATA));
		_dbCommand->FetchByte(1, slot);
		_dbCommand->FetchUInt32(2, pData->nNum);
		_dbCommand->FetchInt16(3, pData->sDuration);
		_dbCommand->FetchUInt16(4, pData->sCount);




		if (!m_pMap->PutData(slot, pData))
			delete pData;

		return true;
	}

	DeathMatchItemsArray2* m_pMap;
};