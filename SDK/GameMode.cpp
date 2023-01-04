#include "GameMode.h"
#include "../Memory/GameData.h"

void GameMode::survivalDestroyBlockHack(Vec3i const &block, int face, bool &isDestroyedOut, bool isFirst) {
	auto p = this->player;
	p->swing();
	if (isFirst)
		this->startDestroyBlock(block, face, isDestroyedOut);
	else {
		*reinterpret_cast<bool *>(reinterpret_cast<__int64>(p) + 0x1c5a) = 1;
		this->continueDestroyBlock(block, face, isDestroyedOut);
	}

	p->startDestroying();

	// prevents regular code from aborting block-break
	*reinterpret_cast<bool *>(reinterpret_cast<__int64>(p) + 0x1c5a) = 0;

	PlayerActionPacket action;
	action.action = 18;
	action.blockPosition = Vec3i(block.x, block.y, block.z);
	action.face = face;
	action.entityRuntimeId = p->entityRuntimeId;
	Game.getClientInstance()->loopbackPacketSender->sendToServer(&action);
}
