#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	mode = (*new SettingEnum(this))
		//.addEntry(EnumEntry("Vanilla", 0))
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

void NoFall::onSendPacket(C_Packet* packet) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->fallDistance > 2.f && mode.selected == 1) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
}

void NoFall::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer->fallDistance > 2.f) {
		switch (mode.selected) {
				/*
		case 0:{
			C_PlayerActionPacket actionPacket;
			actionPacket.action = 7; //Respawn
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		}
			break;
			*/
		case 2:{
			localPlayer->velocity.y = 0.f;
			localPlayer->setPos((*localPlayer->getPos()).add(0, 0.2, 0));
		}
			break;
		case 3:{
			C_PlayerActionPacket actionPacket;
			actionPacket.action = 15;  //Open Elytra
			actionPacket.entityRuntimeId = localPlayer->entityRuntimeId;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
		}
		}
	}
}
