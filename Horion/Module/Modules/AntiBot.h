#pragma once
#include "Module.h"
class AntiBot : public IModule {
private:
	bool hitboxCheck = true;
	bool nameCheck = false;
	bool invisibleCheck = true;
	bool entityIdCheck = true;
	bool otherCheck = true;
	bool extraCheck = false;

public:
	AntiBot();
	~AntiBot();

	inline bool isHitboxCheckEnabled() {
		return hitboxCheck && isEnabled();
	}
	inline bool isNameCheckEnabled() {
		return false && isEnabled();
	}
	inline bool isInvisibleCheckEnabled() {
		return invisibleCheck && isEnabled();
	}
	inline bool isEntityIdCheckEnabled() {
		return entityIdCheck && isEnabled();
	}
	inline bool isOtherCheckEnabled() {
		return otherCheck && isEnabled();
	}
	inline bool isExtraCheckEnabled() {
		return extraCheck && isEnabled();
	}

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
