#pragma once

#include "../../DrawUtils.h"
#include "Module.h"

class NoFall : public IModule {
public:
	SettingEnum mode;

	NoFall();
	~NoFall();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onTick(GameMode* gm) override;
};