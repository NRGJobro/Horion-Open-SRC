#pragma once

#include "Module.h"

class StackableItem : public IModule {
private:
	int maxAmount = 64;
public:
	StackableItem();
	~StackableItem();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
