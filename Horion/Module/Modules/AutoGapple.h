#pragma once

#include "Module.h"

class AutoGapple : public IModule {
private:
	int health;

public:
	MoveInputHandler* inputHandler = nullptr;
	AutoGapple();
	~AutoGapple();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
