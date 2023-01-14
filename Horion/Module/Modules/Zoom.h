#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class Zoom : public IModule {
public:
	float strength = 0.5f;

	Zoom() : IModule(0x0, Category::VISUAL, "Zoom in or out!") {
		registerFloatSetting("Strength", &strength, strength, 0.f, 1.f);
	};
	~Zoom(){};

	virtual const char* getModuleName() override {
		return "Zoom";
	}

	void onLevelRender() override {
		if (auto localPlayer = Game.getLocalPlayer())
			localPlayer->setFieldOfViewModifier(1.f - strength);
	}

	void onDisable() override {
		if (auto localPlayer = Game.getLocalPlayer())
			localPlayer->setFieldOfViewModifier(1.0f);
	}

	bool isFlashMode() override {
		return true;
	}
};