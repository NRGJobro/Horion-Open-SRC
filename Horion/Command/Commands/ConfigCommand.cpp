#include "ConfigCommand.h"

ConfigCommand::ConfigCommand() : IMCCommand("config", "Load/save configs", "<load/new/save> <name>") {
	registerAlias("cfg");
	registerAlias("c");
}

ConfigCommand::~ConfigCommand() {
}

bool ConfigCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "load" || args->at(1) == "l") {
		std::string name = args->at(2);
		configMgr->loadConfig(name, false);
		return true;
	} else if (args->at(1) == "new" || args->at(1) == "n") {
		std::string name = args->at(2);
		configMgr->loadConfig(name, true);
		return true;
	} else if (args->at(1) == "save" || args->at(1) == "s") {
		configMgr->saveConfig();
		clientMessageF("[%sHorion%s] %sSuccessfully saved config %s%s%s!", GOLD, WHITE, GREEN, GRAY, configMgr->currentConfig.c_str(), GREEN);
		return true;
	}
	return false;
}