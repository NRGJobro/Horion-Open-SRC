#include "AirJump.h"

AirJump::AirJump() : IModule('G', Category::MOVEMENT, "Jump even you're not on the ground.") {
	registerBoolSetting("Legacy", &legacyMode, legacyMode);
}

AirJump::~AirJump() {
}

const char* AirJump::getModuleName() {
	return ("AirJump");
}

void AirJump::onTick(GameMode* gm) {
	if (legacyMode) {
		gm->player->onGround = true;
		return;
	}
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return;

	if (GameData::isKeyDown(*input->spaceBarKey) && hasJumped == 0) {
		gm->player->onGround = true;
		hasJumped = 1;
	} else if (!GameData::isKeyDown(*input->spaceBarKey)) {
		hasJumped = 0;
	}
}
