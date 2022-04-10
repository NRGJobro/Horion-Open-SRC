#pragma once

#include "Module.h"

class AirStuck : public IModule {
public:
	AirStuck();
	~AirStuck();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
