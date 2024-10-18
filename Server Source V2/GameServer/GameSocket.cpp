#include "stdafx.h"
#include "GameSocket.h"
#include "../shared/globals.h"

CGameSocket::~CGameSocket() {}

void CGameSocket::OnConnect()
{
	KOSocket::OnConnect();
	Initialize();
}

void CGameSocket::Initialize()
{

}

void CGameSocket::OnDisconnect()
{
	TRACE("*** CloseProcess - socketID=%d ... server=%s *** \n", GetSocketID(), GetRemoteIP().c_str());
}

bool CGameSocket::HandlePacket(Packet& pkt)
{
	switch (pkt.GetOpcode())
	{
	case LOGIN_SERVER_CONNECT:
		RecvServerConnect(pkt);
		break;
	case LOGIN_SERVER_TEST:
		RecvServerTest(pkt);
		break;
	}
	return true;
}

void CGameSocket::RecvServerTest(Packet& pkt)
{
	printf("Login Server - Game Server  | TEST pkt received.\n");
}

void CGameSocket::RecvServerConnect(Packet& pkt)
{
	uint8 byReconnect = pkt.read<uint8>();
	printf("Game Server connected - %s\n", GetRemoteIP().c_str());

	Packet result(LOGIN_SERVER_CONNECT, byReconnect);
	Send(&result);

	/*for(int i=0; i<10 ; i++)
		g_pMain->m_socketMgr[i].RunServer();*/


	if (byReconnect == 1)
		TRACE("**** ReConnect - server=%s,  socket = %d ****\n ", GetRemoteIP().c_str(), GetSocketID());
	else
		TRACE("**** Connect - server=%s,  socket = %d ****\n ", GetRemoteIP().c_str(), GetSocketID());

}
