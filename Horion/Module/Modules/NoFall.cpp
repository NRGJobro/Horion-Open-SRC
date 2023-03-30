#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	mode.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Mineplex", 1))
		.addEntry(EnumEntry("CubeCraft", 2))
		.addEntry(EnumEntry("Nukkit", 3))
		.addEntry(EnumEntry("AuthGroundPos", 4));
	registerEnumSetting("Mode", &mode, 0);
}

NoFall::~NoFall() {}

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
	if (mode.selected == 4) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>() && !Game.getLocalPlayer()->onGround) {
			PlayerAuthInputPacket* authInput = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInput->pos = closestGround;
		}
		/*if (packet->isInstanceOf<C_MovePlayerPacket>() && !Game.getLocalPlayer()->onGround) { I don't know if this is better to have or not
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->Position = closestGround;
		}*/
	}
}

void NoFall::onTick(GameMode* gm) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();

	if (localPlayer->fallDistance > 2.f) {
		switch (mode.selected) {
		case 0: {
			PlayerActionPacket actionPacket;
			actionPacket.action = 7;  // Respawn
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
			break;
		}
		case 2: {
			localPlayer->velocity.y = 0.f;
			localPlayer->setPos((*localPlayer->getPos()).add(0, (float)0.2, 0.f));
			break;
		}
		case 3: {
			PlayerActionPacket actionPacket;
			actionPacket.action = 15;  // Open Elytra
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		}
		case 4: {
			Vec3 blockBelow = localPlayer->eyePos0;
			blockBelow.y -= localPlayer->height;
			blockBelow.y -= 0.17999f;
			while (localPlayer->region->getBlock(blockBelow)->blockLegacy->blockId == 0 && !localPlayer->region->getBlock(blockBelow)->blockLegacy->material->isSolid) {
				blockBelow.y -= 1.f;
			}
			blockBelow.y += 2.62001f;
			//closestGround.y = blockBelow.y;
			Vec3 pos = *localPlayer->getPos();
			closestGround = {pos.x, blockBelow.y, pos.z};
		}
		}
	}
}