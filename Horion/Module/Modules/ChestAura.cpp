#include "ChestAura.h"

ChestAura::ChestAura() : IModule(0x0, Category::PLAYER, "Aura but for opening chests.") {
	registerIntSetting("Range", &range, range, 1, 10);
	registerBoolSetting("EnderChests", &enderchests, enderchests);
}

ChestAura::~ChestAura() {
}

const char* ChestAura::getModuleName() {
	return ("ChestAura");
}

void ChestAura::onTick(GameMode* gm) {
	if (Game.getLocalPlayer()->getSupplies()->inventory->isFull())
		return;

	Vec3* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				Vec3i pos = Vec3i(x, y, z);
				Block* block = gm->player->region->getBlock(pos);
				if (block != nullptr && Game.canUseMoveKeys()) {
					auto id = gm->player->region->getBlock(pos)->toLegacy()->blockId;
					bool open = false;
					if (id == 54) open = true;                  // Chests
					if (id == 130 && enderchests) open = true;  // EnderCheats
					if (open)
					if (!(std::find(chestlist.begin(), chestlist.end(), pos) != chestlist.end())) {
						bool idk = true;
						gm->buildBlock(&pos, 0, idk);
						chestlist.push_back(pos);
						return;
					}
				}
			}
		}
	}
}

void ChestAura::onDisable() {
	chestlist.clear();  // this code should be changed later, the chestlist has to be cleared when loading into a new world
}
