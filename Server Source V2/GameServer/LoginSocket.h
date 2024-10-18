#pragma once

#include "../shared/KOSocket.h"

class LoginSocket : public KOSocket
{
public:
	LoginSocket(uint16 socketID, SocketMgr* mgr) : KOSocket(socketID, mgr, 0, 262144, 262144, true), m_bHasConnected(false) {}

	INLINE bool IsReconnecting() { return m_bHasConnected; }

	virtual void OnConnect();
	virtual bool HandlePacket(Packet& pkt);

	void LoginServerRequest(Packet& pkt);
	void LoginServerTest(Packet& pkt);

	// Login related Methods
	void RecvConnectUser(Packet& pkt);
	void RecvDisconnectUser(Packet& pkt);

	virtual void OnDisconnect();
	virtual ~LoginSocket() {}

private:
	bool m_bHasConnected;
};
