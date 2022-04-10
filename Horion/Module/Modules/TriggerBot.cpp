#include "TriggerBot.h"

#include "../../../Utils/Target.h"

TriggerBot::TriggerBot() : IModule(0, Category::COMBAT, "Attacks entities you're looking at") {
	registerIntSetting("delay", &delay, delay, 0, 20);
}

TriggerBot::~TriggerBot() {
}

const char* TriggerBot::getModuleName() {
	return ("TriggerBot");
}
void TriggerBot::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_Entity* target = g_Data.getLocalPlayer()->level->getEntity();
	
	Odelay++;
	if (target != 0 && Odelay >= delay) {
		if (!Target::isValidTarget(target))
			return;

		localPlayer->swingArm();
		gm->attack(target);

		Odelay = 0;
	}
}