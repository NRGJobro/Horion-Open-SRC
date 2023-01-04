#pragma once

#include "Module.h"

class Scaffold : public IModule {
private:
	bool spoof = false;
	bool staircaseMode = false;
	bool tryScaffold(Vec3 blockBelow);
	bool findBlock();

public:
	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
