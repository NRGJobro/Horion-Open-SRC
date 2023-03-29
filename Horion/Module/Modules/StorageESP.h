#pragma once

#include "Module.h"

class StorageESP : public IModule {
private:
	int tickTimeout = 0;
	std::vector<AABB> bufferedChestList;
	std::mutex listLock;
	bool fill = false;
	float opacity = 0.5f;
	SettingEnum mode{this};

public:
	StorageESP();
	~StorageESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(GameMode* gm) override;
};
