#pragma once

#include <stdint.h>

#include "../Utils/HMath.h"
#include "CClientInstance.h"
#include "CEntity.h"

class C_GameMode {
private:
	virtual __int64 destructorGameMode();
	// Duplicate destructor
public:
	virtual __int64 startDestroyBlock(Vec3i const &pos, unsigned char blockSide, bool &isDestroyedOut);
	virtual __int64 destroyBlock(Vec3i *, unsigned char);
	virtual __int64 continueDestroyBlock(Vec3i const &, unsigned char blockSide, bool &isDestroyedOut);
	virtual __int64 stopDestroyBlock(Vec3i const &);
	virtual __int64 startBuildBlock(Vec3i const &, unsigned char);
	virtual __int64 buildBlock(Vec3i *, unsigned char, bool);
	virtual __int64 continueBuildBlock(Vec3i const &, unsigned char);
	virtual __int64 stopBuildBlock(void);
	virtual __int64 tick(void);

public:
	virtual __int64 getPickRange(__int64 const &, bool);
	virtual __int64 useItem(C_ItemStack &);
	virtual __int64 useItemOn(__int64 &, Vec3i const &, unsigned char, Vec3 const &, __int64 const *);
	virtual __int64 interact(C_Entity &, Vec3 const &);

public:
	virtual __int64 attack(C_Entity *);

public:
	virtual __int64 releaseUsingItem(void);

public:
	virtual void setTrialMode(bool);
	virtual bool isInTrialMode(void);

private:
	virtual __int64 registerUpsellScreenCallback(__int64);

public:
	C_Player *player;

	void survivalDestroyBlockHack(Vec3i const &block, int face, bool &isDestroyedOut, bool isFirst);
};