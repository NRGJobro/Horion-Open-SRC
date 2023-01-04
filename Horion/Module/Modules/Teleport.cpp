#include "Teleport.h"

Teleport::Teleport() : IModule(0, Category::MISC, "Click a block to teleport to it.") {
	registerBoolSetting("Only Hand", &onlyHand, onlyHand);
	registerBoolSetting("Push", &bypass, bypass);
}

Teleport::~Teleport() {
}

const char* Teleport::getModuleName() {
	return "Teleport";
}

void Teleport::onTick(GameMode* gm) {
	if (!GameData::canUseMoveKeys())
		return;
	if (onlyHand && Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->item != nullptr)
		return;

	if (GameData::isRightClickDown() && !hasClicked) {
		hasClicked = true;

		Vec3i block = Game.getLocalPlayer()->getlevel()->block;
		if (block == Vec3i(0, 0, 0)) return;
		Vec3 pos = block.toFloatVector();
		pos.x += 0.5f;
		pos.z += 0.5f;

		tpPos = pos;
		shouldTP = true;

		Game.getGuiData()->displayClientMessageF("%sTeleport position set to %sX: %.1f Y: %.1f Z: %.1f%s. Sneak to teleport!", GREEN, GRAY, pos.x, pos.y, pos.z, GREEN);
	}
	if (!GameData::isRightClickDown())
		hasClicked = false;

	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (shouldTP && GameData::isKeyDown(*input->sneakKey)) {
		tpPos.y += (gm->player->getPos()->y - gm->player->getAABB()->lower.y) + 1;  // eye height + 1
		if (bypass) {
			/*int dist = (int)gm->player->getPos()->dist(tpPos);
			int i = (int)dist / 5;
			for (int n = 0; n < i; n++) {
				Vec3 offs = tpPos.sub(*gm->player->getPos()).div(i).mul(n);
				C_MovePlayerPacket p = C_MovePlayerPacket(Game.getLocalPlayer(), gm->player->getPos()->add(offs));
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			}
			gm->player->setPos(tpPos);*/
			float dist = gm->player->getPos()->dist(tpPos);
			Game.getLocalPlayer()->lerpTo(tpPos, Vec2(1, 1), (int)fmax((int)dist * 0.1, 1));
		} else
			gm->player->setPos(tpPos);
		shouldTP = false;
	}
}