#include "Bhop.h"
#include "../../../Utils/HMath.h"

Bhop::Bhop() : IModule(0, Category::MOVEMENT, "Hop around like a bunny!") {
	registerFloatSetting("Speed", &speed, speed, 0.1f, 0.8f);
	registerBoolSetting("Hive", &hive, hive);
}

Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("Bhop");
}

void Bhop::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (player->isInLava() == 1 || player->isInWater() == 1) 
		return;
	
	if (player->isSneaking()) 
		return;

	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	
	float calcYaw = (player->yaw + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};

	if (hive) {
		player->stepHeight = 0.f;
		static bool useVelocity = false;
		if (0.4000000059604645 >= 0.385) {
			if (player->onGround && pressed) player->jumpFromGround();
			useVelocity = false;
		} else
			useVelocity = true;

		speedFriction *= 0.9535499811172485;
		if (pressed) {
			player->setSprinting(true);
			if (player->onGround) {
				if (useVelocity && !input->isJumping) player->velocity.y = 0.4000000059604645;
				speedFriction = RandomFloat(0.4190652072429657, 0.48381298780441284);
			} else {
				moveVec.x = moveVec2d.x * speedFriction;
				moveVec.y = player->velocity.y;
				moveVec.z = moveVec2d.y * speedFriction;
				player->lerpMotion(moveVec);
			}
		}
	} else {
		if (player->onGround && pressed)
			player->jumpFromGround();
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
}
