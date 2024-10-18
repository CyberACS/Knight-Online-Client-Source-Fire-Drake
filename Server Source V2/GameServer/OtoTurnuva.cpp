#include "stdafx.h"
#include "DBAgent.h"

using std::string;
using std::unique_ptr;
extern CDBAgent g_DBAgent;

#pragma region CUser::TournumentCancelRegister()
uint8 CUser::TournumentCancelRegister()
{
	int nWeekDay = g_localTime.tm_wday;
	int nHour = g_localTime.tm_hour;
	int nMin = g_localTime.tm_min;
	int nSec = g_localTime.tm_sec;

	if (!isClanLeader())
		return 2;

	uint8 result = g_DBAgent.TournumentCancelChecking(GetClanID(), GetName());
	if (result == 3)
		return 3;

	uint8 result2 = g_DBAgent.TournumentCamcelSuccess(GetClanID(), GetName());
	if (result2 != 1)
		return 2;

	return 1;
}
#pragma endregion

#pragma region CUser::TournumenthRegister()
uint8 CUser::TournumentRegister()
{


	int nWeekDay = g_localTime.tm_wday;
	int nHour = g_localTime.tm_hour;
	int nMin = g_localTime.tm_min;
	int nSec = g_localTime.tm_sec;

	if (!isClanLeader()
		&& !isClanAssistant())
		return 2;

	CKnights* pClan = g_pMain->GetClanPtr(GetClanID());
	if (pClan == nullptr)
		return 3;

	/*if (pClan->m_byGrade > 3
		|| pClan->m_byFlag < ClanTypePromoted)
	{
		if (pClan->m_byFlag >= ClanTypeAccredited5)
			goto fail_return;

		return 3;
	}*/


	uint8 result = g_DBAgent.TournumentRegisterChecking(GetClanID(), GetName());
	if (result == 4)
		return 4;

	if (result == 5)
		return 5;

	uint8 result2 = g_DBAgent.TournumentRegisterSuccess(GetClanID(), GetName());
	if (result2 != 1)
		return 2;

	return 1;
}
#pragma endregion



/// DATABASE ÝÞLEMLERÝ ////

#pragma region CDBAgent::KnightsSiegeWarCancelChecking(uint16 sClanID, std::string & strUserID)
uint8 CDBAgent::TournumentCancelChecking(uint16 sClanID, std::string& strUserID)
{
	int8 bRet = -1;
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strUserID.c_str(), strUserID.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL TOURNUMENT_CANCEL_CHECK(%d, ?)}"), sClanID)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}
#pragma endregion

bool CDBAgent::NDPassword(std::string& strUserID, std::string& strAccountID)
{

	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	CUser* pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);


	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());

	if (!dbCommand->Execute(string_format(_T("{CALL ACCOUNT_2NDPASSWORD(?)}"))))
		ReportSQLError(m_GameDB->GetError());

	int field = 1;
	dbCommand->FetchUInt32(field++, pUser->ndpasswrd);


	return true;

}

bool CDBAgent::UserRankingALL()
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("{CALL LOAD_RATING_ALL}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	int b = 1;
	do
	{
		_USER_RANK_ALL* pData = new _USER_RANK_ALL;

		int i = 1;

		dbCommand->FetchString(i++, pData->strUserId);
		dbCommand->FetchByte(i++, pData->Nation);
		dbCommand->FetchUInt32(i++, pData->LoyaltyMonthly);

		if (!g_pMain->m_UserRankALL.PutData(b, pData))
			delete pData;
		b++;

	} while (dbCommand->MoveNext());

	return true;
}

bool CDBAgent::LoadTickets(std::string& strUserID)
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strUserID.c_str(), strUserID.length());
	if (!dbCommand->Execute(_T("{CALL LOAD_CYBERACS_TICKETS(?)}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	int b = 1;
	do
	{
		_LOAD_CYBERACS_TICKETS* pData = new _LOAD_CYBERACS_TICKETS;

		int i = 1;

		dbCommand->FetchString(i++, pData->baslik);
		dbCommand->FetchString(i++, pData->icerik);
		dbCommand->FetchString(i++, pData->cevap);
		dbCommand->FetchUInt16(i++, pData->status);


		if (!g_pMain->m_Tickets.PutData(b, pData))
			delete pData;
		b++;

	} while (dbCommand->MoveNext());

	return true;
}

bool CDBAgent::InsertTickets(std::string& strCharID, std::string& Title, std::string& Content)
{

	unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, Title.c_str(), Title.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, Content.c_str(), Content.length());
	if (!dbCommand->Execute(_T("INSERT INTO CYBERACS_TICKETS VALUES (?,?,?,'',0)")))
	{
		ReportSQLError(m_AccountDB->GetError());
		return false;
	}

	return true;
}

bool CDBAgent::UpdateTickets(std::string& strCharID, std::string& Title, std::string& Content)
{

	unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;


	if (!dbCommand->Execute(string_format(_T("UPDATE CYBERACS_TICKETS SET Title = '%s', [Content] = '%s', Answer = ' ', Statu = 0 WHERE strName = '%s'"), Title.c_str(), Content.c_str(), strCharID.c_str())))
	{
		ReportSQLError(m_AccountDB->GetError());
		return false;
	}

	return true;
}

bool CDBAgent::UpdatePassword(std::string& strCharID, uint32 password)
{

	unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;


	if (!dbCommand->Execute(string_format(_T("UPDATE TB_USER SET OTPPassword = '%d' WHERE strAccountID = '%s'"), password, strCharID.c_str())))
	{
		ReportSQLError(m_AccountDB->GetError());
		return false;
	}

	return true;
}

bool CDBAgent::UserRankingEL()
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("{CALL LOAD_RATING_ALL_EL}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	int b = 1;
	do
	{
		_USER_RANK_EL* pData = new _USER_RANK_EL;

		int i = 1;

		dbCommand->FetchString(i++, pData->strUserId);
		dbCommand->FetchUInt32(i++, pData->LoyaltyMonthly);

		if (!g_pMain->m_UserRankEL.PutData(b, pData))
			delete pData;
		b++;

	} while (dbCommand->MoveNext());

	return true;
}


bool CDBAgent::UserRankingKA()
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("{CALL LOAD_RATING_ALL_KA}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	int b = 1;
	do
	{
		_USER_RANK_KA* pData = new _USER_RANK_KA;

		int i = 1;


		dbCommand->FetchString(i++, pData->strUserId);
		dbCommand->FetchUInt32(i++, pData->LoyaltyMonthly);

		if (!g_pMain->m_UserRankKA.PutData(b, pData))
			delete pData;
		b++;

	} while (dbCommand->MoveNext());

	return true;
}


bool CDBAgent::LoadKings()
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("{CALL LOAD_KINGS}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	do
	{
		_KINGS_LIST* pData = new _KINGS_LIST;

		int i = 1;

		dbCommand->FetchString(i++, pData->strKarusKingName);
		dbCommand->FetchString(i++, pData->strKarusNotice);
		dbCommand->FetchString(i++, pData->strElMoradKingName);
		dbCommand->FetchString(i++, pData->strElMoradNotice);


		if (!g_pMain->m_KingList.PutData(1, pData))
			delete pData;

	} while (dbCommand->MoveNext());

	return true;
}

uint8 CDBAgent::NDPasswordCheck(std::string& strAccountID)
{

	int8 bRet = -1;
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;


	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL [ACCOUNT_2NDPASSWORD_CHECK](?)}"))))
		ReportSQLError(m_GameDB->GetError());

	return bRet;

}


#pragma region CDBAgent::TournumentCamcelSuccess(uint16 sClanID, std::string & strUserID)
uint8 CDBAgent::TournumentCamcelSuccess(uint16 sClanID, std::string& strUserID)
{
	int8 bRet = -1;
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strUserID.c_str(), strUserID.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL TOURNUMENT_CANCEL(%d, ?)}"), sClanID)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}
#pragma endregion

#pragma region CDBAgent::KnightsSiegeWarRegisterChecking(uint16 sClanID, std::string & strUserID)
uint8 CDBAgent::TournumentRegisterChecking(uint16 sClanID, std::string& strUserID)
{
	int8 bRet = -1;
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strUserID.c_str(), strUserID.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL TOURNUMENT_REGISTER_CHECK(%d, ?)}"), sClanID)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}
#pragma endregion

#pragma region CDBAgent::KnightsSiegeWarRegisterSuccess(uint16 sClanID, std::string & strUserID)
uint8 CDBAgent::TournumentRegisterSuccess(uint16 sClanID, std::string& strUserID)
{
	int8 bRet = -1;
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strUserID.c_str(), strUserID.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL TOURNUMENT_DEATMATCH_REGISTER(%d, ?)}"), sClanID)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}
#pragma endregion

#pragma region CDBAgent::LoadKnightSiegeWarRegister()
void CDBAgent::LoadTournumentRegister(Packet& pkt)
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(_T("{CALL LOAD_TOURNUMENT_REGISTER}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return;
	}

	if (!dbCommand->hasData())
		return;

	uint8 sCount = 0;
	size_t wpos = pkt.wpos();
	pkt << sCount;
	pkt.DByte();
	do
	{
		uint16 sBaseClanNum;
		int16 sMarkVersion;
		string sBaseClanCommander, strKnightsName;

		dbCommand->FetchString(1, sBaseClanCommander);
		dbCommand->FetchString(2, strKnightsName);
		dbCommand->FetchInt16(3, sMarkVersion);
		dbCommand->FetchUInt16(4, sBaseClanNum);

		pkt << sBaseClanNum
			<< sMarkVersion
			<< strKnightsName
			<< sBaseClanCommander;
		sCount++;

	} while (dbCommand->MoveNext());

	pkt.put(wpos, sCount);
}
#pragma endregion

#pragma region CDBAgent::LoadKnightSiegeWarRegister()
int8 CDBAgent::TotalCount(Packet& pkt)
{
	int i = 0;

	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return 0;

	if (!dbCommand->Execute(_T("{CALL LOAD_TOURNUMENT_REGISTER}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return 0;
	}

	if (!dbCommand->hasData())
		return 0;

	uint8 sCount = 0;
	size_t wpos = pkt.wpos();

	pkt.DByte();
	do
	{
		uint16 sBaseClanNum;
		int16 sMarkVersion;
		string sBaseClanCommander, strKnightsName;

		dbCommand->FetchString(1, sBaseClanCommander);
		dbCommand->FetchString(2, strKnightsName);
		dbCommand->FetchInt16(3, sMarkVersion);
		dbCommand->FetchUInt16(4, sBaseClanNum);

		g_pMain->clanlar[i] = strKnightsName;

		sCount++;
		i++;
	} while (dbCommand->MoveNext());

	return sCount;
}
#pragma endregion

#pragma region CDBAgent::KnightsSiegeWarRegisterSuccess(uint16 sClanID, std::string & strUserID)
uint8 CDBAgent::TournumentUpdate(uint16 Active)
{
	int8 bRet = -1;
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);

	if (!dbCommand->Execute(string_format(_T("{? = CALL TOURNUMENT_UPDATE_ACTIVE(%d)}"), Active)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}
#pragma endregion

#pragma region CDBAgent::KnightsSiegeWarRegisterSuccess(uint16 sClanID, std::string & strUserID)
uint8 CDBAgent::TournumentLoadUpdate()
{
	int8 bRet = -1;
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);


	if (!dbCommand->Execute(_T("{? = CALL TOURNUMENT_LOAD_ACTIVE}")))
		ReportSQLError(m_GameDB->GetError());


	return bRet;
}
#pragma endregion

#pragma region CDBAgent::CastleSiegeWarRegisterReset()
bool CDBAgent::TournumentRegisterReset()
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("{CALL TOURNUMENT_REGISTER_RESET}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	return true;
}
#pragma endregion


bool CDBAgent::GAME_SERVER_SETTINGS()
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("SELECT * FROM CYBERACS_SERVER_SETTINGS")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}


	int IsOpen = 0;
	uint8 sCount = 0;

	if (dbCommand->hasData())
	{

		do
		{
			_SETTINGS_INFO* pInfo = new _SETTINGS_INFO();

			int i = 1;

			dbCommand->FetchUInt32(i++, pInfo->SelectChar);

			if (!g_pMain->m_MaintainceSettingsArray.PutData(1, pInfo))
				delete pInfo;

		} while (dbCommand->MoveNext());
	}

	return true;
}

uint8 CDBAgent::TBUSER_SETTINGS(std::string AccountID)
{
	unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;


	dbCommand->AddParameter(SQL_PARAM_INPUT, AccountID.c_str(), AccountID.length());
	if (!dbCommand->Execute(_T("SELECT Authority FROM USERDATA WHERE strUserID = ?")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}


	int IsOpen = 0;
	uint8 sCount = 0;

	if (dbCommand->hasData())
	{

		do
		{
			_TBUSER_INFO* pInfo = new _TBUSER_INFO();

			int i = 1;

			dbCommand->FetchInt16(i++, pInfo->Authority);

			IsOpen = pInfo->Authority;

		} while (dbCommand->MoveNext());
	}

	return IsOpen;
}
#pragma endregion

/// DATABASE ÝÞLEMLERÝ ////