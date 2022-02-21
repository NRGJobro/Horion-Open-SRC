#pragma once
#include "Module.h"
class Freelook : public IModule {
public:
	vec2_t oldPos;
	vec2_t Pos;
	bool hold = true;

	Freelook() : IModule(0, Category::VISUAL, "Look around freely without moving your rotation") {
		registerBoolSetting("Hold", &hold, hold);
	}
	~Freelook(){};

	virtual const char* getModuleName() override { return "Freelook"; }
	void onEnable() override { oldPos = g_Data.getLocalPlayer()->viewAngles; }
	void onDisable() override { g_Data.getLocalPlayer()->setRot(oldPos);
	}
	bool callWhenDisabled() override {
		return true;
	}
};