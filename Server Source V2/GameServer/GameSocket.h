#pragma once

#include "../shared/KOSocket.h"

class CUser;
class MAP;

class CGameSocket : public KOSocket
{
public:

	CGameSocket(uint16 socketID, SocketMgr* mgr) : KOSocket(socketID, mgr, -1, 262144, 262144) {}

	virtual void OnConnect();

	void Initialize();
	bool HandlePacket(Packet& pkt);

	virtual void OnDisconnect();
	void RecvServerConnect(Packet& pkt);
	void RecvServerTest(Packet& pkt);
	virtual ~CGameSocket();
};