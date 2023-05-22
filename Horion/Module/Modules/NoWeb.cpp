#include "NoWeb.h"

NoWeb::NoWeb() : IModule(0x0, Category::MOVEMENT, "Ignore cobwebs slowdown.") {
}

NoWeb ::~NoWeb() {
}

const char* NoWeb::getModuleName() {
	return ("NoWeb");
}

void NoWeb::onTick(GameMode* gm) {
	gm->player->slowdownFactor = {0, 0, 0};
}
