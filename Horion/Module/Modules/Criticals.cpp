#include "Criticals.h"

Criticals::Criticals() : IModule(0, Category::COMBAT, "Each hit becomes a critical hit.") {
}

Criticals::~Criticals() {
}

const char* Criticals::getModuleName() {
	return ("Criticals");
}

void Criticals::onSendPacket(Packet* packet) {
	Vec3 pos = *Game.getLocalPlayer()->getPos();
	pos.y += 2.f;
	if (packet->isInstanceOf<C_MovePlayerPacket>() && Game.getLocalPlayer() != nullptr) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->onGround = false;
		movePacket->Position = pos;
	}
	if (packet->isInstanceOf<PlayerAuthInputPacket>() && Game.getLocalPlayer() != nullptr) {
		PlayerAuthInputPacket* authInput = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authInput->pos = pos;
	}
}