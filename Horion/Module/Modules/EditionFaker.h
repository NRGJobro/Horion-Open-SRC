#pragma once

#include "Module.h"

class EditionFaker : public IModule {
public:
	EditionFaker();
	~EditionFaker();
	
	SettingEnum edition;
	SettingEnum inputMode;

	int getFakedEditon();
	int getFakedInputMode();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
