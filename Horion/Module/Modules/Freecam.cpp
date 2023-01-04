#include "Freecam.h"

Freecam::Freecam() : IModule('V', Category::MISC, "Move your camera without moving the player.") {
	registerFloatSetting("Speed", &speed, speed, 0.50f, 1.25f);
}

Freecam::~Freecam() {
}

const char* Freecam::getModuleName() {
	return ("Freecam");
}

void Freecam::onTick(GameMode* gm) {
	gm->player->fallDistance = 0.f;
	gm->player->velocity = Vec3(0, 0, 0);
	gm->player->aabb.upper = gm->player->aabb.lower;
}

void Freecam::onEnable() {
	if (Game.getLocalPlayer() != nullptr) {
		oldPos = *Game.getLocalPlayer()->getPos();
		oldOffset = Game.getLocalPlayer()->aabb.upper.sub(Game.getLocalPlayer()->aabb.lower);
	}
}

void Freecam::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;

	float calcYaw = (player->yaw + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	moveVec.x = moveVec2d.x * speed;
	moveVec.y = player->velocity.y;
	moveVec.z = moveVec2d.y * speed;
	if (pressed) player->lerpMotion(moveVec);
	C_MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
	if (input->isJumping) {
		player->velocity.y += 0.50f;
	}
	if (input->isSneakDown) {
		player->velocity.y -= 0.50f;
	}
}

void Freecam::onDisable() {
	auto plr = Game.getLocalPlayer();
	if (plr) {
		plr->setPos(oldPos);
		*Game.getClientInstance()->minecraft->timer = 20.f;
		plr->aabb.upper = plr->aabb.lower.add(oldOffset);
	}
}