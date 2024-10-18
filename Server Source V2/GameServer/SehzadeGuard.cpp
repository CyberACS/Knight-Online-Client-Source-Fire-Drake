#include "stdafx.h"

#define __GUARD_VERSION 1394


void CUser::SehzadeGuard(Packet& pkt)
{
	uint8 subOpcode = pkt.read<uint8>();
	Packet resultGuard(WIZ_MASTER_SECURITY);
	DateTime time;
	switch (subOpcode)
	{
	case GuardOpCodes::INITIATE:
	{
		uint32 version;
		pkt >> version;
		if (version != uint32(__GUARD_VERSION))
		{
			TRACE("### CyberACS  %s (SocketID:%d) different version\n", GetRemoteIP().c_str(), GetSocketID());
			g_pMain->WriteCheatLogFile(string_format("### CyberACS - %d:%d:%d -  %s (SocketID:%d) different version\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID()));
			Disconnect();
			return;
		}

		m_tGuardAliveExpiryTime = uint32(UNIXTIME);
		isSexyGuardAlive = true;
		break;
	}
	case GuardOpCodes::ALIVE:
	{
	
		uint8 bAuthority = 0;
		uint32 version;
		pkt >> bAuthority >> version;

		if (version != uint32(__GUARD_VERSION))
		{
			TRACE("### CyberACS  %s (SocketID:%d) different version\n", GetRemoteIP().c_str(), GetSocketID());
			g_pMain->WriteCheatLogFile(string_format("### SexyGuard - %d:%d:%d -  %s (SocketID:%d) different version\n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID()));
			Disconnect();
			return;
		}


		if (bAuthority != m_bAuthority && !isGM())
		{
			TRACE("[%s] Disconnected. Reason: WALL HACK", GetName().c_str());
			g_pMain->WriteCheatLogFile(string_format("[CyberACS - WALL HACK | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
			Disconnect();
			return;
		}

		m_tGuardAliveExpiryTime = uint32(UNIXTIME);
		resultGuard << uint8(GuardOpCodes::ALIVE);
		Send(&resultGuard);
	}
	//}
	break;
	case GuardOpCodes::DISCONNECT:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = DISCONNECT \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - DISCONNECT | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::CLIENTNAME_ERROR:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = CLIENTNAME_ERROR \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - CLIENTNAME_ERROR | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::SPEED_HACK:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = SPEED_HACK \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - SPEED_HACK | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::WALL_HACK:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = WALL_HACK \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - WALL_HACK | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::DLL_INJECT:
		TRACE("### SexyGuard Disconnected = %s | SocketID = %d | Req = DLL_INJECT \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - DLL_INJECT | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::BYPASS:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = BYPASS \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - BYPASS | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::DEBUGGER:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = DEBUGGER \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - DEBUGGER | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::TBL:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = TBL \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - TBL | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::VIRTUAL_MACHINE:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = VIRTUAL_MACHINE \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - VIRTUAL_MACHINE | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::UGLY_PROCESS:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = UGLY_PROCESS \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - UGLY_PROCESS | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::UGLY_DRIVER:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = UGLY_DRIVER \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - UGLY_DRIVER | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::MULTI_CLIENT:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = MULTI_CLIENT \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - MULTI_CLIENT | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	case GuardOpCodes::TIMEOUT:
		TRACE("### CyberACS Disconnected = %s | SocketID = %d | Req = TIMEOUT \n", GetName().c_str(), GetSocketID());
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - TIMEOUT | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	default:
		g_pMain->WriteCheatLogFile(string_format("[CyberACS - Unknown Subopcode | %d:%d:%d ] Disconnected = %s | SocketID = %d | Req = Default(%d)  \n", time.GetHour(), time.GetMinute(), time.GetSecond(), GetName().c_str(), GetSocketID(), subOpcode));
		Disconnect();
		break;
	}

}



void CUser::InitiateGuard()
{
	Packet guardInitiate(WIZ_MASTER_SECURITY, uint8(GuardOpCodes::INITIATE));
	Send(&guardInitiate);
}