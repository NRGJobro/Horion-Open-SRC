#pragma once

#include "Module.h"

class Criticals : public IModule {
public:
	bool test = false;
	Criticals();
	~Criticals();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet) override;
};
