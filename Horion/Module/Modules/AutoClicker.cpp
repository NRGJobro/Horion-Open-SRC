#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "A simple autoclicker, automatically clicks for you.") {
	registerBoolSetting("rightclick", &rightclick, rightclick);
	registerBoolSetting("only weapons", &weapons, weapons);
	registerBoolSetting("break blocks", &breakBlocks, breakBlocks);
	registerIntSetting("delay", &delay, delay, 0, 20);
	registerBoolSetting("hold", &hold, hold);
}

AutoClicker::~AutoClicker() {
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");
}

void AutoClicker::onTick(C_GameMode* gm) {
	if ((GameData::isLeftClickDown() || !hold) && GameData::canUseMoveKeys()) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		Level* level = g_Data.getLocalPlayer()->level;
		Odelay++;

		if (Odelay >= delay) {
			auto selectedItem = localPlayer->getSelectedItem();
			if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
				return;

			g_Data.leftclickCount++;

			localPlayer->swingArm();

			if (level->hasEntity() != 0)
				gm->attack(level->getEntity());
			else if (breakBlocks) {
				bool isDestroyed = false;
				gm->startDestroyBlock(level->block, level->blockSide, isDestroyed);
				gm->stopDestroyBlock(level->block);
				if (isDestroyed && localPlayer->region->getBlock(level->block)->blockLegacy->blockId != 0)
					gm->destroyBlock(&level->block, 0);
			}
			Odelay = 0;
		}
	}

	if (rightclick) {
		if ((GameData::isRightClickDown() || !hold) && GameData::canUseMoveKeys()) {
			Level* level = g_Data.getLocalPlayer()->level;
			Odelay++;
			if (Odelay >= delay) {
				g_Data.rightclickCount++;
				bool idk = true;
				gm->buildBlock(new vec3_ti(level->block), level->blockSide, idk);
				Odelay = 0;
			}
		}
	}
}