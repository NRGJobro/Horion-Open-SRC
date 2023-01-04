#pragma once

#include "Module.h"

class Teleport : public IModule {
private:
	bool hasClicked = false;
	bool onlyHand = false;
	bool bypass = false;
	bool shouldTP = false;
	Vec3 tpPos;

public:
	Teleport();
	~Teleport();

	// Inherited via IModule
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
};
