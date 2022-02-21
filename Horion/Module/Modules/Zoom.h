#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class Zoom : public IModule {
public:
	float strength = 0.5f;
	float OGFov;
	bool zooming = false;
	float target = 20.f;
	float modifier = 20.f;

	Zoom() : IModule(0x0, Category::VISUAL, "Zoom in or out!") {
		registerFloatSetting("Strength", &strength, strength, 0.f, 1.f);
	};
	~Zoom(){};

	bool Zoom::isFlashMode() {
		return true;
	}
	void onEnable() {
		zooming = true;
	}
	void onLevelRender() {
		if (g_Data.getLocalPlayer() != nullptr && g_Data.isInGame())
			g_Data.getLocalPlayer()->setFieldOfViewModifier(strength);
	}
	void onDisable() {
		g_Data.getLocalPlayer()->setFieldOfViewModifier(OGFov);
		target = g_Data.fov;
	}
	virtual const char* getModuleName() override {
		return "Zoom";
	}
};