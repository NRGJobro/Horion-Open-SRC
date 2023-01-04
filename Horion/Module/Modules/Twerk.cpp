#include "Twerk.h"

Twerk::Twerk() : IModule(0x0, Category::MOVEMENT, "Crouch Spam.") {
	registerFloatSetting("Delay", &delay, delay, 0.05f, 10);
}

Twerk::~Twerk() {
}

const char* Twerk::getModuleName() {
	return ("Twerk");
}

void Twerk::onTick(GameMode* gm) {
	Odelay++;
	if (Odelay > delay * 20) {
		GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
		if (gm->player->isSneaking()) {
			isSneaking = false;
		} 
		else {
			if (!gm->player->isSneaking()) {
				isSneaking = true;
			}
		}
		Game.getClientInstance()->getMoveTurnInput()->isSneakDown = isSneaking;
		Odelay = 0;
	}
}