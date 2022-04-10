#include "InventoryMove.h"

InventoryMove::InventoryMove() : IModule(0, Category::MOVEMENT, "Move even though you have your inventory open") {
}

InventoryMove::~InventoryMove() {
}

const char* InventoryMove::getModuleName() {
	return ("InvMove");
}

void InventoryMove::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer()->canOpenContainerScreen())
		return;
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return;

	float speed = 0.325f;
	float yaw = gm->player->yaw;

	if (GameData::isKeyDown(*input->spaceBarKey) && gm->player->onGround)
		gm->player->jumpFromGround();

	if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
		return;
	else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
		yaw += 45.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
		yaw -= 45.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
		yaw += 135.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
		yaw -= 135.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->forwardKey)) {
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->backKey)) {
		yaw += 180.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
		yaw += 90.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
		yaw -= 90.f;
		keyPressed = true;
	}
	if (yaw >= 180)
		yaw -= 360.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	//float calcPitch = (gm->player->pitch) * -(PI / 180);
	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = gm->player->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if (keyPressed) {
		gm->player->lerpMotion(moveVec);
		keyPressed = false;
	}
}
