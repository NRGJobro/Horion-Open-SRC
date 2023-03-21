#include "TestModule.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/VoxelIntersector.h"
#include "../../DrawUtils.h"
#include "../../../SDK/MatrixStack.h"
#include <deque>
#include <array>
#include <unordered_set>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>			//radians
#include <glm/ext/matrix_transform.hpp>		// perspective, translate, rotate
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../../../Utils/Json.hpp"
#include "../../path/JoePathFinder.h"
#include "../../path/JoeMovementController.h"
#include "../../path/goals/JoeGoal.h"
#include "../../path/goals/JoeGoalXZ.h"
#include "../../ImmediateGui.h"

using json = nlohmann::json;


TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes only!") {
	enum1.addEntry(EnumEntry("1", 1)).addEntry(EnumEntry("2", 2)).addEntry(EnumEntry("3", 3));
	registerFloatSetting("float1", &float1, 0, -10, 10);
	registerIntSetting("int1", &int1, 0, -10, 10);
	registerEnumSetting("Enum1", &enum1, 0);
	registerBoolSetting("bool1", &bool1, true);
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

bool TestModule::isFlashMode() {
	return false;
}

void TestModule::onEnable() {
	/* LocalPlayer* player = Game.getLocalPlayer();
	Vec3 playerPos = *player->getPos();
	playerPos.y -= 1.f;
	for (int a = 0; a < 100; a++) {
		if (player->region->getBlock(playerPos)->blockLegacy->blockId != 0) {
			player->setPos(playerPos);
			break;
		}
	}
	logF("%f", playerPos.y);*/
}

void TestModule::onTick(GameMode* gm) {
	LocalPlayer* player = Game.getLocalPlayer();
	Vec3 pos = *player->getPos();
	Vec3 blockBelow;
	blockBelow = Vec3(pos.x, pos.y, pos.z);
	blockBelow.y -= 1.0;

	// Check if the block below the player is solid and not air
	while (player->region->getBlock(blockBelow)->blockLegacy->blockId == 0) {
		blockBelow.y -= 1.0;  // move blockBelow down until a solid non-air block is found
	}

	savepos = Vec3(blockBelow.x, blockBelow.y, blockBelow.z);  // set savepos to the position just above the solid non-air block
	savepos.y += 2.0;

	//if (player->fallDistance >= 2)
	//	player->setPos(savepos);
}

void TestModule::onMove(MoveInputHandler* hand){
}

void TestModule::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (HorionGui.Button("Test Button", Vec2(100, 100), true)) { //Check if button is pressed. When it gets pressed it sends the message.
		clientMessageF("Test Button Was Clicked");
	}
}

void TestModule::onPostRender(MinecraftUIRenderContext* renderCtx) {
}

void TestModule::onSendPacket(Packet* p) {
	if (p->isInstanceOf<PlayerAuthInputPacket>()) {
		PlayerAuthInputPacket* authInput = reinterpret_cast<PlayerAuthInputPacket*>(p);
		authInput->pos = savepos;
	}
}

void TestModule::onDisable() {
}

void TestModule::onLevelRender() {
}

void TestModule::onKey(int key, bool isDown, bool& cancel) {
	//Cancel W key for testing to make sure this works
	//if (key == 'W' && isDown) cancel = true;
}