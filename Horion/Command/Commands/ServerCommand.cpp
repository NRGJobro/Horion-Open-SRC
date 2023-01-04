#include "ServerCommand.h"

ServerCommand::ServerCommand() : IMCCommand("server", "Show information about the server you're playing on", "") {
	registerAlias("ip");
}

ServerCommand::~ServerCommand() {
}

bool ServerCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);

	if (Game.getRakNetInstance()->isonaServer())
		clientMessageF("You're currently playing on:\nIP: %s\nPort: %s", Game.getRakNetInstance()->serverIp.getText(), std::to_string(Game.getRakNetInstance()->serverPort).c_str());
	else
		clientMessageF("%sYou're not playing on a server.", RED);
	return true;
}
