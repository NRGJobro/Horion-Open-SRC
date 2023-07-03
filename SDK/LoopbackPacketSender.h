#pragma once

#include "Packet.h"

class NetworkHandler {
private:
	char pad_0x0[0x70];  // 0x0
public:
	class RakNetInstance* rakNetInstance;  // 0x70
};

class LoopbackPacketSender {
private:
	char pad_0x8[0x8];  // 0x8
public:
	class NetworkHandler* networkHandler;  // 0x10

	virtual ~LoopbackPacketSender();
	//Duplicate contructor
	virtual __int64 send(Packet* packet);
	virtual __int64 sendToServer(Packet* packet);
	virtual __int64 sendToClient(const void* networkIdentifier, const Packet* packet, int a4);
	virtual __int64 sendToClients(const void* networkIdentifier, const Packet* packet);
	virtual __int64 sendBroadcast(const Packet* packet);
	virtual __int64 sendBroadcast(const void* networkIdentifier, int a3, const Packet* packet);
	virtual __int64 flush(void* networkIdentifier, int a3);
};