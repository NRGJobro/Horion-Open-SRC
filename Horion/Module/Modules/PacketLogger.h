#pragma once

#include "Module.h"

class PacketLogger : public IModule {
public:
	bool packetadd;
	bool authchunk = false;
	SettingEnum addressType;
	PacketLogger();
	~PacketLogger();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
};