#include "Fucker.h"

Fucker::Fucker() : IModule(VK_NUMPAD9, Category::MISC, "Destroys certain blocks around you") {
	registerIntSetting("Range", &range, range, 1, 10);
	registerBoolSetting("Beds", &beds, beds);
	registerBoolSetting("Eggs", &eggs, eggs);
	registerBoolSetting("Cakes", &cakes, cakes);
	registerBoolSetting("Treasures", &treasures, treasures);
	registerBoolSetting("Chests", &chests, chests);
	registerBoolSetting("Barrels", &barrels, barrels);
}

Fucker::~Fucker() {
}
const char* Fucker::getModuleName() {
	return ("Breaker");
}

void Fucker::onTick(C_GameMode* gm) {
	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				bool destroy = false;
				bool eat = false;
				auto id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if (id == 26 && beds) destroy = true;      // Beds
				if (id == 122 && eggs) destroy = true;     // Dragon Eggs
				if (id == 92 && cakes) eat = true;         // Cakes
				if (id == 54 && chests) destroy = true;    // Chests
				if (id == 458 && barrels) destroy = true;  // Barrels

				if (destroy) {
					gm->destroyBlock(&blockPos, 0);
					g_Data.getLocalPlayer()->swingArm();
					return;
				}
				
				if (eat) {
					bool idk = true;
					gm->buildBlock(&blockPos, 0, idk);
					g_Data.getLocalPlayer()->swingArm();
					return;
				}
			}
		}
	}

	if (treasures) {
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			std::string name = ent->getNameTag()->getText();
			int id = ent->getEntityTypeId();
			if (name.find("Treasure") != std::string::npos && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= 5) {
				g_Data.getLocalPlayer()->swingArm();
				g_Data.getCGameMode()->attack(ent);
			}
		});
	}
}
