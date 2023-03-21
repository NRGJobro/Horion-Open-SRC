#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	mode.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Mineplex", 1))
		.addEntry(EnumEntry("CubeCraft", 2))
		.addEntry(EnumEntry("Nukkit", 3))
		.addEntry(EnumEntry("AuthPos", 4));
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
	if (mode.selected == 4) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket* authInput = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInput->pos = authPos;
		}
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->Position = authPos;
		}
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
			Vec3 pos = *localPlayer->getPos();
			Vec3 blockBelow;
			blockBelow = Vec3(pos.x, pos.y, pos.z);
			blockBelow.y -= 1.0;

			// Check if the block below the player is solid and not air
			while (localPlayer->region->getBlock(blockBelow)->blockLegacy->blockId == 0) {
				blockBelow.y -= 1.0;  // move blockBelow down until a solid non-air block is found
			}

			authPos = Vec3(blockBelow.x, blockBelow.y, blockBelow.z);  // set authPos to the position just above the solid non-air block
			authPos.y += 2.0;
		}
		}
	}
}