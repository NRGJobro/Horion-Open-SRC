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

	bool Zoom::isFlashMode() {
		return true;
	}
	
	void onLevelRender() {
		if (Game.getLocalPlayer() != nullptr && Game.isInGame())
			Game.getLocalPlayer()->setFieldOfViewModifier(1.f - strength);
	}
	
	void onDisable() {
		Game.getLocalPlayer()->setFieldOfViewModifier(1.0f);
	}
	
	virtual const char* getModuleName() override {
		return "Zoom";
	}
};