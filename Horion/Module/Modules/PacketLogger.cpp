#include "PacketLogger.h"

PacketLogger::PacketLogger() : IModule(0, Category::MISC, "Logging Packets!") {
}

PacketLogger::~PacketLogger() {
}

const char* PacketLogger::getModuleName() {
	return ("PacketLogger");
}

void PacketLogger::onSendPacket(Packet* packet) {
	/* if (packet->isInstanceOf<PlayerActionPacket>()) {
		auto pk = reinterpret_cast<PlayerActionPacket*>(packet);
		Game.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i", packet->getName()->getText(), pk->action);
		return;
	}*/
#ifdef _DEBUG

	if (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") != 0) {
		Game.getClientInstance()->getGuiData()->displayClientMessageF("%s", packet->getName()->getText());
	}
#endif  // DEBUG

}
