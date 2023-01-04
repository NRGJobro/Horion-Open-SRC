#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "A simple autoclicker, automatically clicks for you.") {
	registerBoolSetting("RightClick", &rightclick, rightclick);
	registerBoolSetting("Only Weapons", &weapons, weapons);
	registerBoolSetting("Break Blocks", &breakBlocks, breakBlocks);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	registerBoolSetting("Hold", &hold, hold);
}

AutoClicker::~AutoClicker() {
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");
}

void AutoClicker::onTick(GameMode* gm) {
	if ((GameData::isLeftClickDown() || !hold) && GameData::canUseMoveKeys()) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		Level* level = Game.getLocalPlayer()->level;
		Odelay++;

		if (Odelay >= delay) {
			auto selectedItem = localPlayer->getSelectedItem();
			if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
				return;

			Game.leftclickCount++;

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
			Level* level = Game.getLocalPlayer()->level;
			Odelay++;
			if (Odelay >= delay) {
				Game.rightclickCount++;
				bool idk = true;
				gm->buildBlock(new Vec3i(level->block), level->blockSide, idk);
				Odelay = 0;
			}
		}
	}
}