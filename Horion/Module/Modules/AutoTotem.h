#pragma once

#include "Module.h"

class AutoTotem : public IModule {
public:
	AutoTotem();
	~AutoTotem();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
