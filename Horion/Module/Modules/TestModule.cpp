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
	SetPlayerGameTypePacket packet;
	packet.gameMode = 0;
	Game.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
}

void TestModule::onTick(GameMode* gm) {
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
		authInput->pos.y = -100.f;
	}
	if (p->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* authInput = reinterpret_cast<C_MovePlayerPacket*>(p);
		authInput->Position.y = -100.f;
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