#include "Aimbot.h"

Aimbot::Aimbot() : IModule(0, Category::COMBAT, "Automatically aims at the nearest entity.") {
	registerFloatSetting("Range", &range, range, 3.f, 8.f);
	registerBoolSetting("Require Click", &click, click);
	registerBoolSetting("Only Swords/Axes", &sword, sword);
	registerBoolSetting("Vertical", &vertical, vertical);
	registerFloatSetting("Horizontal Speed", &horizontalspeed, horizontalspeed, 10.f, 90.f);
	registerFloatSetting("Vertical Speed", &verticalspeed, verticalspeed, 10.f, 90.f);
	registerFloatSetting("Horizontal Range", &horizontalrange, horizontalrange, 20.f, 180.f);
	registerFloatSetting("Vertical Range", &verticalrange, verticalrange, 20.f, 180.f);
	registerBoolSetting("Aimlock", &lock, lock);
}

Aimbot::~Aimbot() {
}

const char* Aimbot::getModuleName() {
	return ("Aimbot");
}

struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Aimbot::onPostRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr)
		return;
	EntityList* entList = Game.getEntityList();
	if (entList == nullptr)
		return;
	size_t listSize = entList->getListSize();

	if (listSize > 1000) {
		return;
	}

	Vec3 origin = Game.getClientInstance()->levelRenderer->getOrigin();

	//Loop through all our players and retrieve their information
	static std::vector<Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++) {
		Entity* currentEntity = entList->get(i);

		if (!Target::isValidTarget(currentEntity))
			continue;

		float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < range)
			targetList.push_back(currentEntity);
	}

	if (targetList.size() > 0) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		Vec2 angle = origin.CalcAngle(*targetList[0]->getPos());
		Vec2 appl = angle.sub(localPlayer->viewAngles).normAngles();
		appl.x = -appl.x;
		if ((appl.x < verticalrange && appl.x > -verticalrange) && (appl.y < horizontalrange && appl.y > -horizontalrange) && GameData::canUseMoveKeys()) {
			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
			ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			if (sword && !(item->getItem()->isWeapon()))
				return;

			if (click && !Game.isLeftClickDown())
				return;

			if (!lock) {
				appl.x /= (100.f - verticalspeed);
				appl.y /= (100.f - horizontalspeed);
				if (appl.x >= 1 || appl.x <= -1) appl.div(abs(appl.x));
				if (appl.y >= 1 || appl.y <= -1) appl.div(abs(appl.y));
			}
			if (!vertical)
				appl.x = 0;
			localPlayer->applyTurnDelta(&appl);
		}
	}
}
