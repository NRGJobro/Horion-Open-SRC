#include "Spammer.h"
#include "../../../Utils/Utils.h"

Spammer::Spammer() : IModule(0, Category::MISC, "Spams a message in a specified delay") {
	registerIntSetting("delay", &delay, delay, 1, 10);
	registerIntSetting("length", &length, length, 1, 60);
	registerBoolSetting("bypass", &bypass, bypass);
}

Spammer::~Spammer() {
}

const char* Spammer::getModuleName() {
	return ("Spammer");
}

void Spammer::onTick(C_GameMode* gm) {
	Odelay++;
	if (Odelay > delay * 20) {
		C_TextPacket textPacket;
		textPacket.message.setText(bypass ? (message + " | " + Utils::randomString(length)) : message);
		textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
		textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		Odelay = 0;
	}
}
