#include "RainbowSky.h"

RainbowSky::RainbowSky() : IModule(0x0, Category::VISUAL, "Makes your sky look rainbow!") {
}

RainbowSky::~RainbowSky() {
}

const char* RainbowSky::getModuleName() {
	return ("RainbowSky");
}
