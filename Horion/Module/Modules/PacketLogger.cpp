#include "PacketLogger.h"

PacketLogger::PacketLogger() : IModule(0, Category::MISC, "Logging Packets!") {
}

PacketLogger::~PacketLogger() {
}

const char* PacketLogger::getModuleName() {
	return ("PacketLogger");
}

void PacketLogger::onSendPacket(Packet* packet) {

	if (packet->isInstanceOf<PlayerAuthInputPacket>())
		return;
	LocalPlayer* player = Game.getLocalPlayer();
	uint64_t currVTable = *(UINT64*)packet;
	std::stringstream stream;
	stream << std::hex << (currVTable - Utils::getBase() + 0x140000000);
	std::string result(stream.str());
	std::string packetText = packet->getName()->getText() + std::string(" = ") + result;
	TextHolder txt = TextHolder(packetText);
	player->displayClientMessage(&txt);


	if (packet->isInstanceOf<PlayerActionPacket>()) {
		auto pk = reinterpret_cast<PlayerActionPacket*>(packet);
		Game.getClientInstance()->getGuiData()->displayClientMessageF("%s action=%i", packet->getName()->getText(), pk->action, BLUE, BLUE, BLUE);
		//return;
	}
#ifdef _DEBUG

	if (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") != 0) {
		Game.getClientInstance()->getGuiData()->displayClientMessageF("%s", packet->getName()->getText());
	}
#endif  // DEBUG

}
