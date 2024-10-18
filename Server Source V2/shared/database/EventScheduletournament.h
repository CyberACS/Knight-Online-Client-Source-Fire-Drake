#pragma once

class CEventTournamentScheduleStatus : public OdbcRecordset
{
public:
	CEventTournamentScheduleStatus(OdbcConnection* dbConnection, EventTournamentScheduleArray* pMap)
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("CYBERACS_TOURNUMENT_TIMES"); }
	virtual tstring GetColumns() { return _T("EventLocalID,Start,EventStatus,StartHour1, StartMinute1,TimeActive1"); }

	virtual bool Fetch()
	{
		int i = 1;
		_TOURNAMENT_SCHEDULE* pData = new _TOURNAMENT_SCHEDULE;
		_dbCommand->FetchByte(1, pData->EventLocalID);
		_dbCommand->FetchString(2, pData->Start);
		_dbCommand->FetchByte(3, pData->EventStatus);
		_dbCommand->FetchUInt32(4, pData->EventStartHour);
		_dbCommand->FetchUInt32(5, pData->EventStartMinutes);
		_dbCommand->FetchByte(6, pData->TimeActive);
		
		if (!m_pMap->PutData(pData->EventLocalID, pData))
			delete pData;
	

		return true;
	}
	EventTournamentScheduleArray* m_pMap;
};