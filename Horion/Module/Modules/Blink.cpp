#include "Blink.h"

Blink::Blink() : IModule(0x0, Category::MISC, "Stops you from sending packets and then sends them in a bunch.") {
}

Blink::~Blink() {
	getMovePlayerPacketHolder()->clear();
	getPlayerAuthInputPacketHolder()->clear();
}

const char* Blink::getModuleName() {
	return ("Blink");
}

void Blink::onTick(GameMode* gm) {
	gm->player->fallDistance = 0.f;
}
