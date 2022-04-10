#include "Timer.h"

Timer::Timer() : IModule(0, Category::MISC, "Increase the ticks per second.") {
	registerIntSetting("TPS", &timer, timer, 0, 500);
}

Timer::~Timer() {
}

const char* Timer::getModuleName() {
	return ("Timer");
}

void Timer::onTick(C_GameMode* gm) {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
}

void Timer::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}