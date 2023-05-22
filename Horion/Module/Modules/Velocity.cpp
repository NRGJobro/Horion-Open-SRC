#include "Velocity.h"

Velocity::Velocity() : IModule(0x0, Category::MOVEMENT, "Don't ever take knockback again.") {
	registerFloatSetting("Linear Modifier", &xModifier, xModifier, 0.f, 1.f);
	registerFloatSetting("Height Modifier", &yModifier, yModifier, 0.f, 1.f);
}

Velocity::~Velocity() {
}

const char* Velocity::getModuleName() {
	return ("Velocity");
}
