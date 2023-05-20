#include "NoSlowDown.h"

#include <Windows.h>

#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

NoSlowDown::NoSlowDown() : IModule(0, Category::MOVEMENT, "Don't get slowed down when blocking or eating.") {
}

NoSlowDown::~NoSlowDown() {
}

const char* NoSlowDown::getModuleName() {
	return ("NoSlowDown");
}

void NoSlowDown::onEnable() {
	if (opcode == 0 || opcode1 == 0) {
		opcode = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 ? F3 44 0F 10 46"));
		opcode1 = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 ? 45 89 26"));
	}

	DWORD oldProtect = 0;
	if (!VirtualProtect(opcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	} else {
		opcode[0] = 0x90;
		opcode[1] = 0x90;
		opcode[2] = 0x90;
		opcode[3] = 0x90;
		opcode[4] = 0x90;
		VirtualProtect(opcode, 5, oldProtect, &oldProtect);
	}

	if (!VirtualProtect(opcode1, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	} else {
		opcode1[0] = 0x90;
		opcode1[1] = 0x90;
		opcode1[2] = 0x90;
		opcode1[3] = 0x90;
		opcode1[4] = 0x90;
		VirtualProtect(opcode1, 5, oldProtect, &oldProtect);
	}
}

void NoSlowDown::onDisable() {
	if (opcode == 0 || opcode1 == 0) {
		opcode = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 ? F3 44 0F 10 46"));
		opcode1 = reinterpret_cast<uint8_t*>(FindSignature("F3 0F 11 46 ? 45 89 26"));
	}

	DWORD oldProtect = 0;
	if (!VirtualProtect(opcode, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	} else {
		opcode[0] = 0xF3;
		opcode[1] = 0x0F;
		opcode[2] = 0x11;
		opcode[3] = 0x46;
		opcode[4] = 0x0C;
		VirtualProtect(opcode, 5, oldProtect, &oldProtect);
	};

	if (!VirtualProtect(opcode1, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	} else {
		opcode1[0] = 0xF3;
		opcode1[1] = 0x0F;
		opcode1[2] = 0x11;
		opcode1[3] = 0x46;
		opcode1[4] = 0x0C;
		VirtualProtect(opcode1, 5, oldProtect, &oldProtect);
	};
}
