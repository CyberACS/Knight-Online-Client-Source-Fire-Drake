#include "stdafx.h"
#include <sstream>
#include "../shared/Ini.h"
#include "../shared/DateTime.h"
using namespace std;

extern bool g_bRunning;
std::vector<Thread*> g_timerThreadsLogin;

LoginServer::LoginServer() : m_sLastVersion(__VERSION), m_fpLoginServer(nullptr)
{
}

bool LoginServer::Startup()
{
	DateTime time;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	// Server Start
	SetConsoleTextAttribute(h, FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_INTENSITY);
	printf("Login Server started on %04d-%02d-%02d at %02d:%02d\n\n", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());

	// License System Stard System..
	/*m_HardwareIDArray.push_back(230662953965053);
	if (!g_HardwareInformation.IsValidHardwareID(m_HardwareIDArray))
	{
		printf("Lisans Number: %lld\n", g_HardwareInformation.GetHardwareID());
		return false;
	}*/
	// License System End System..

	GetInfoFromIni();

	CreateDirectory("Logs", NULL);

	m_fpLoginServer = fopen("./Logs/LoginServer.log", "a");
	if (m_fpLoginServer == nullptr)
	{
		printf("ERROR: Unable to open log file.\n");
		return false;
	}

	m_fpUser = fopen(string_format("./Logs/Login_%d_%d_%d.log", time.GetDay(), time.GetMonth(), time.GetYear()).c_str(), "a");
	if (m_fpUser == nullptr)
	{
		printf("ERROR: Unable to open user log file.\n");
		return false;
	}

	if (!m_DBProcess.Connect(m_ODBCName, m_ODBCLogin, m_ODBCPwd))
	{
		printf("ERROR: Unable to connect to the database using the details configured.\n");
		return false;
	}

	printf("Database connection is established successfully.\n");

	if (!m_DBProcess.LoadVersionList())
	{
		printf("ERROR: Unable to load the version list.\n");
		return false;
	}

	printf("Latest version in database: %d\n", GetVersion());
	if (!m_DBProcess.LoadServerList())
	{
		printf("ERROR: Unable to load the server list.\n");
		return false;
	}
	InitPacketHandlers();

	for (int i = 0; i < 10; i++)
	{
		if (!m_socketMgr[i].Listen((uint16)m_LoginServerPort + i, MAX_USER / 5))
		{
			printf("ERROR: Failed to listen on server port.\n");
			return false;
		}

		m_socketMgr[i].RunServer();
	}


	g_timerThreadsLogin.push_back(new Thread(Timer_UpdateUserCount));
	g_timerThreadsLogin.push_back(new Thread(Timer_UpdateKingNotice));
	printf("\n\n");
	return true;
}

uint32 LoginServer::Timer_UpdateUserCount(void* lpParam)
{
	while (g_bRunning)
	{
		g_pLoginMain->UpdateServerList();
		sleep(60 * SECOND);
	}
	return 0;
}

uint32 LoginServer::Timer_UpdateKingNotice(void* lpParam)
{
	while (g_bRunning)
	{
		sleep(600 * SECOND);
		g_pLoginMain->m_DBProcess.LoadServerList();
	}
	return 0;
}

void LoginServer::GetServerList(Packet& result)
{
	Guard lock(m_serverListLock);
	result.append(m_serverListPacket.contents(), m_serverListPacket.size());
}

void LoginServer::UpdateServerList()
{
	// Update the user counts first
	m_DBProcess.LoadUserCountList();

	Guard lock(m_serverListLock);
	Packet& result = m_serverListPacket;

	result.clear();
	result << uint8(m_ServerList.GetSize());
	foreach_stlmap(itr, m_ServerList)
	{
		_SERVER_INFO* pServer = itr->second;
		if (pServer == nullptr)
			continue;

		result << pServer->strLanIP;
		result << pServer->strServerIP;
		result << pServer->strServerName;

		if (pServer->sUserCount <= pServer->sPlayerCap)
			result << pServer->sUserCount;
		else
			result << int16(-1);

		result << pServer->sServerID << pServer->sGroupID;
		result << pServer->sPlayerCap << pServer->sFreePlayerCap;
		result << uint8(0);

		// we read all this stuff from ini, TODO: make this more versatile.
		result << pServer->strKarusKingName << pServer->strKarusNotice
			<< pServer->strElMoradKingName << pServer->strElMoradNotice;
	}
}

void LoginServer::GetInfoFromIni()
{
	CIni ini(CONF_GAME_SERVER);

	ini.GetString("DOWNLOAD", "URL", "ftp.yoursite.net", m_strFtpUrl, false);
	ini.GetString("DOWNLOAD", "PATH", "/", m_strFilePath, false);

	ini.GetString("ODBC", "ACCOUNT_DSN", "KO_MAIN", m_ODBCName, false);
	ini.GetString("ODBC", "ACCOUNT_UID", "username", m_ODBCLogin, false);
	ini.GetString("ODBC", "ACCOUNT_PWD", "password", m_ODBCPwd, false);


	m_LoginServerPort = ini.GetInt("SETTINGS_LOGIN", "PORT", 15100);

	int nServerCount = ini.GetInt("CYBERACS_SERVER_LIST", "COUNT", 1);
	if (nServerCount <= 0)
		nServerCount = 1;

	char key[20];

	// Read news from INI (max 3 blocks)
#define BOX_START '#' << uint8(0) << '\n'
#define BOX_END '#'

	m_news.Size = 0;
	std::stringstream ss;
	for (int i = 0; i < 3; i++)
	{
		string title, message;

		_snprintf(key, sizeof(key), "TITLE_%02d", i);
		ini.GetString("NEWS", key, "", title);
		if (title.empty())
			continue;

		_snprintf(key, sizeof(key), "MESSAGE_%02d", i);
		ini.GetString("NEWS", key, "", message);
		if (message.empty())
			continue;

		size_t oldPos = 0, pos = 0;
		ss << title << BOX_START << message << BOX_END;
	}

	m_news.Size = ss.str().size();
	if (m_news.Size)
		memcpy(&m_news.Content, ss.str().c_str(), m_news.Size);
}

void LoginServer::WriteLogFile(string& logMessage)
{
	Guard lock(m_lock);
	fwrite(logMessage.c_str(), logMessage.length(), 1, m_fpLoginServer);
	fflush(m_fpLoginServer);
}

void LoginServer::WriteUserLogFile(string& logMessage)
{
	Guard lock(m_lock);
	fwrite(logMessage.c_str(), logMessage.length(), 1, m_fpUser);
	fflush(m_fpUser);
}

void LoginServer::ReportSQLError(OdbcError* pError)
{
	if (pError == nullptr)
		return;

	// This is *very* temporary.
	string errorMessage = string_format(_T("ODBC error occurred.\r\nSource: %s\r\nError: %s\r\nDescription: %s\n"),
		pError->Source.c_str(), pError->ExtendedErrorMessage.c_str(), pError->ErrorMessage.c_str());

	TRACE("%s", errorMessage.c_str());
	WriteLogFile(errorMessage);
	delete pError;
}

LoginServer::~LoginServer()
{
	printf("Waiting for timer threads to exit...");
	foreach(itr, g_timerThreadsLogin)
	{
		(*itr)->waitForExit();
		delete (*itr);
	}
	printf(" exited.\n");
	Sleep(1 * SECOND);

	m_ServerList.DeleteAllData();

	foreach(itr, m_VersionList)
		delete itr->second;
	m_VersionList.clear();

	if (m_fpLoginServer != nullptr)
		fclose(m_fpLoginServer);

	if (m_fpUser != nullptr)
		fclose(m_fpUser);

	printf("Shutting down socket system...");

	for (int i = 0; i < 10; i++)
		m_socketMgr[i].Shutdown();

	printf(" done.\n");
	Sleep(1 * SECOND);
}