#pragma once

#include "Module.h"

class Freecam : public IModule {
private:
	Vec3 oldPos;
	Vec3 oldOffset = {};
	float speed = 0.325f;
public:
	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
