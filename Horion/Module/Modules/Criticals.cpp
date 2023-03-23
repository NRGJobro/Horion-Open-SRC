#include "Criticals.h"

Criticals::Criticals() : IModule(0, Category::COMBAT, "Each hit becomes a critical hit.") {
	registerBoolSetting("test", &test, test);
}

Criticals::~Criticals() {
}

const char* Criticals::getModuleName() {
	return ("Criticals");
}

void Criticals::onTick(GameMode* gm) {
	if (test) {
		LocalPlayer* player = Game.getLocalPlayer();
		Vec3 pos = player->eyePos0;
		pos.y += 2.f;
		C_MovePlayerPacket movePlayerPacket;
		movePlayerPacket.onGround = false;
		movePlayerPacket = C_MovePlayerPacket(player, pos);
		PlayerAuthInputPacket authInputPacket;
		authInputPacket = PlayerAuthInputPacket(pos, player->pitch, player->yaw, player->yawUnused1);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&authInputPacket);
	}
}

void Criticals::onSendPacket(Packet* packet) {
	LocalPlayer* player = Game.getLocalPlayer();
	Vec3 pos = player->eyePos0;
	pos.y += 2.f;
	if (packet->isInstanceOf<C_MovePlayerPacket>() && player != nullptr) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->onGround = false;
		movePacket->Position = pos;
	}
	if (packet->isInstanceOf<PlayerAuthInputPacket>() && player != nullptr) {
		PlayerAuthInputPacket* authInput = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authInput->pos = pos;
	}
}