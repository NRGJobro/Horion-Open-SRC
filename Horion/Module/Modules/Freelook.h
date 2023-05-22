#pragma once

#include "Module.h"

class Freelook : public IModule {
public:
	Vec2 oldPos;
	Vec2 Pos;
	bool hold = true;

	Freelook() : IModule(0x0, Category::VISUAL, "Look around freely without moving your rotation.") {
		registerBoolSetting("Hold", &hold, hold);
	}
	~Freelook(){};

	virtual const char* getModuleName() override { return "Freelook"; }
	void onEnable() override { 
		if (Game.getLocalPlayer() != nullptr)
		oldPos = Game.getLocalPlayer()->viewAngles; 
	}
	void onDisable() override { Game.getLocalPlayer()->setRot(oldPos);
	}
	bool callWhenDisabled() override {
		return true;
	}
};