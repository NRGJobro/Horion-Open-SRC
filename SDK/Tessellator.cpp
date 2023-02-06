#include "Tessellator.h"

#include "../Utils/Utils.h"
#include "BlockLegacy.h"
#include "ClientInstance.h"

void Tessellator::addPostTransformOffset(float x, float y, float z) {
	this->transformOffset = this->transformOffset.add(x, y, z);
}

Mesh* BlockTessellator::getMeshForBlockInWorld(Tessellator *t, Block *block, Vec3i &pos) {
	using BT_getMeshForBlockInWorld_t = Mesh* (*)(BlockTessellator *, Tessellator *, Block *, Vec3i &, int, __int64);
	static BT_getMeshForBlockInWorld_t getMeshForBlock = reinterpret_cast<BT_getMeshForBlockInWorld_t>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4D 8B F1 4D 8B F8 48"));

	return getMeshForBlock(this, t, block, pos, block->getRenderLayer(), 0);
}
