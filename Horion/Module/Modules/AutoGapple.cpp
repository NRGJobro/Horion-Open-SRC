#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0, Category::MOVEMENT, "Auto eat gapples if you're at low health.") {
	registerIntSetting("Health", &health, 10, 1, 20);
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}

void AutoGapple::onTick(GameMode* gm) {
	
}
