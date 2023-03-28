#include "PacketLogger.h"

PacketLogger::PacketLogger() : IModule(0x0, Category::WORLD, "Logs packets, PacketAddr shows the CE or IDA address for packet and AuthChunk shows PAIP and SCRP") {
	registerBoolSetting("Packet Addr", &packetadd, &packetadd);
	registerBoolSetting("Auth & Chunk", &authchunk, authchunk);
	registerEnumSetting("Mode", &addressType, 0);
	addressType = SettingEnum(this)
					  .addEntry(EnumEntry("CE Address", 0))
					  .addEntry(EnumEntry("IDA Address", 1));
}

PacketLogger::~PacketLogger() {}

const char* PacketLogger::getModuleName() {
	return ("PacketLogger");
}

void PacketLogger::onSendPacket(Packet* packet) {
	LocalPlayer* player = Game.getLocalPlayer();
	uint64_t currVTable = *(UINT64*)packet;
	std::stringstream stream;
	if (packetadd && addressType.selected == 0) {
		if (!authchunk && (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") == 0 || strcmp(packet->getName()->getText(), "SubChunkRequestPacket") == 0)) {
			return;
		}
		stream << std::hex << (currVTable - Utils::getBase());
		std::string result(stream.str());
		std::string packetText = packet->getName()->getText() + (std::string) " (Minecraft.Windows.exe+" + result + ")";
		TextHolder txt = TextHolder(packetText);
		player->displayClientMessage(&txt);
	}
	if (packetadd && addressType.selected == 1) {
		if (!authchunk && (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") == 0 || strcmp(packet->getName()->getText(), "SubChunkRequestPacket") == 0)) {
			return;
		}
		stream << std::hex << (currVTable - Utils::getBase() + 0x140000000);
		std::string result(stream.str());
		std::string packetText = packet->getName()->getText() + std::string(" = ") + result;
		TextHolder txt = TextHolder(packetText);
		player->displayClientMessage(&txt);
	}

	if (!authchunk) {
		if (!packetadd) {
			if (strcmp(packet->getName()->getText(), "PlayerAuthInputPacket") != 0 && strcmp(packet->getName()->getText(), "SubChunkRequestPacket") != 0) {
				Game.getClientInstance()->getGuiData()->displayClientMessageF("%s", packet->getName()->getText());
			}
		}
	} else {
		// Code for displaying all packets if authchunk is true
		Game.getClientInstance()->getGuiData()->displayClientMessageF("%s", packet->getName()->getText());
	}
}