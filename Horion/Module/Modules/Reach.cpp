#include "Reach.h"

#include <Windows.h>
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

Reach::Reach() : IModule(0x0, Category::COMBAT, "Increases your reach.") {
	registerFloatSetting("Reach Value", &reachValue, reachValue, 3.f, 7.f);
}

Reach::~Reach() {
}

const char* Reach::getModuleName() {
	return ("Reach");
}

void Reach::onTick(GameMode* gm) {
	if (reachPtr != 0) {
		*reachPtr = reachValue;
	}
}

void Reach::onEnable() {
	static uintptr_t sigOffset = 0x0;
	if (sigOffset == 0x0) {

		sigOffset = FindSignature("F3 0F 10 05 ? ? ? ? 41 0F 28 D9");

		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 4));  // Get Offset from code
			reachPtr = reinterpret_cast<float*>(sigOffset + offset + 8);
			originalReach = *reachPtr;
		}
	}
	if (!VirtualProtect(reachPtr, sizeof(float), PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
}

void Reach::onDisable() {
	*reachPtr = originalReach;
	if (reachPtr != 0)
		VirtualProtect(reachPtr, sizeof(float), oldProtect, &oldProtect);
}
