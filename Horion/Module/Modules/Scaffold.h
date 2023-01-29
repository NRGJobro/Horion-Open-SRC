#pragma once
#include "Module.h"
class Scaffold : public IModule {
private:
	bool autoSelect = false;
	bool down = false;
	bool highlight = true;
	bool hive = false;
	bool rotations = true;
	int extend = 0;
	bool Ylock = false;
	bool tryScaffold(Vec3 blockBelow);
	bool tryClutchScaffold(Vec3 blockBelow);
	bool findBlock();
	float YCoord;

public:
	Scaffold();
	~Scaffold(){};

	virtual const char* getModuleName() {
		return "Scaffold";
	};
	virtual void onEnable() {
		auto player = Game.getLocalPlayer();
		if (player == nullptr) return;
		float yPos = player->eyePos0.y;  // Block 1 block below the player
		yPos -= player->height;
		yPos -= 0.5f;
		YCoord = yPos;
	};
	virtual void onPostRender(MinecraftUIRenderContext* ctx) override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onPlayerTick(Player* player) override;
};
