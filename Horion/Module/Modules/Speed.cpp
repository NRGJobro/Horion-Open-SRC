#include "Speed.h"

Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed up!") {
	registerFloatSetting("Speed", &speed, 1, 0.1f, 3.f);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");  // 48 8D 15 ?? ?? ?? ?? 48 8B CB FF 90 ?? ?? ?? ?? 48 8B D8
}

void Speed::onTick(GameMode* gm) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();

	float* speedAdr = reinterpret_cast<float*>(Game.getLocalPlayer()->getSpeed() + 0x84);
	*speedAdr = speed;
}

void Speed::onEnable() {
	if (Game.getLocalPlayer() == nullptr) {
		setEnabled(false);
		return;
	} else {
		origSpeed = *reinterpret_cast<float*>(Game.getLocalPlayer()->getSpeed() + 0x84);
	}
}

void Speed::onDisable() {
	if (Game.getLocalPlayer() != nullptr)
		*reinterpret_cast<float*>(Game.getLocalPlayer()->getSpeed() + 0x84) = origSpeed;
}
