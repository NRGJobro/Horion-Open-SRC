#pragma once
#include "Module.h"

class ClientTheme : public IModule {
public:
	SettingEnum Theme;
	ClientTheme();
	~ClientTheme();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};