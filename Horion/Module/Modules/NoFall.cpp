#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	mode.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Mineplex", 1))
		.addEntry(EnumEntry("CubeCraft", 2))
		.addEntry(EnumEntry("Nukkit", 3));
	registerEnumSetting("Mode", &mode, 0);
}

NoFall::~NoFall() {
}

const char* NoFall::getModuleName() {
	return ("NoFall");
}

void NoFall::onSendPacket(Packet* packet) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->fallDistance > 2.f && mode.selected == 1) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
}

void NoFall::onTick(GameMode* gm) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();

	if (localPlayer->fallDistance > 2.f) {
		switch (mode.selected) {
		case 0:{
			PlayerActionPacket actionPacket;
			actionPacket.action = 7; //Respawn
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
			break;
		}
		case 2:{
			localPlayer->velocity.y = 0.f;
			localPlayer->setPos((*localPlayer->getPos()).add(0, (float)0.2, 0.f));
			break;
		}
		case 3:{
			PlayerActionPacket actionPacket;
			actionPacket.action = 15;  //Open Elytra
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		}
		}
	}
}
