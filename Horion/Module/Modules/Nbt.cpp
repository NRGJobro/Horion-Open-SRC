#include "Nbt.h"
#include "../../../SDK/Tag.h"
#include "../../../Utils/Utils.h"

Nbt::Nbt() : IModule(0, Category::PLAYER, "Print NBT tags, right click on mobs (Only works on local worlds!)") {
}

Nbt::~Nbt() {
}

const char* Nbt::getModuleName() {
	return ("Nbt");
}

void Nbt::onTick(C_GameMode* gm) {
	if (!GameData::canUseMoveKeys()) {
		return;
	}
	Level* level = g_Data.getLocalPlayer()->level;
	
	if (GameData::isRightClickDown()) {  // && Utils::getClipboardText() != lastCopy) {
		if (level->getEntity() != nullptr) {

			if (!(g_Data.getRakNetInstance()->serverIp.getTextLength() < 1))
				return;
			std::unique_ptr<CompoundTag> tag = std::make_unique<CompoundTag>();
			level->getEntity()->save(tag.get());
			std::stringstream build;
			tag->write(build);
			auto str = build.str();
			if (lastCopy == str)
				return;
			lastCopy = str;
			Utils::setClipboardText(lastCopy);
			g_Data.getGuiData()->displayClientMessageF("%s%s", GREEN, "CompoundTag copied:");
			g_Data.getClientInstance()->getGuiData()->displayClientMessage(&str);
		}
	}
}
