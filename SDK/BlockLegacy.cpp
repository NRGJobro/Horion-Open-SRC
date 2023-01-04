#include "BlockLegacy.h"

#include "../Memory/GameData.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
Block* BlockSource::getBlock(const Vec3i& block) {
	using getBlock_t = Block*(__fastcall*)(BlockSource*, const Vec3i&);
	static getBlock_t getBlock = reinterpret_cast<getBlock_t>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B DA 8B 4A"));
	return getBlock(this, block);
}
BlockActor* BlockSource::getBlockEntity(const Vec3i& block) {
	using getBlockEntity_t = BlockActor*(__fastcall*)(BlockSource*, const Vec3i&);
	static getBlockEntity_t getBlockEntity = reinterpret_cast<getBlockEntity_t>(FindSignature("40 55 56 57 48 83 EC ?? 8B 02"));
	return getBlockEntity(this, block);
}
Block* BlockSource::getLiquidBlock(const Vec3i& block) {
	using getLiquidBlock_t = Block*(__fastcall*)(BlockSource*, const Vec3i&);  //E8 41 7D 6A 01 48 8B C8 48 8B 46 10
	static getLiquidBlock_t getLiquidBlock = Utils::FuncFromSigOffset<getLiquidBlock_t>(FindSignature("E8 ?? ?? ?? ?? 65 48 8B 0C 25 ?? ?? ?? ??"), 1);
	return getLiquidBlock(this, block);  //E8 41 7D 6A ?? ?? ?? ?? ?? ?? ?? ??
}
bool BlockLegacy::getCollisionShape(AABB* collShapeOut, Block* block, BlockSource* blockSource, const Vec3i* pos, Entity* actor) {
	return Utils::CallVFunc<5, bool, AABB*, Block*, BlockSource*, const Vec3i*, Entity*>(this, collShapeOut, block, blockSource, pos, 0);
}
void BlockLegacy::liquidGetFlow(Vec3* flowOut, BlockSource* reg, const Vec3i* pos) {
	using liquid_getFlow_t = int(__fastcall*)(BlockLegacy*, Vec3*, BlockSource*, const Vec3i*);
	static liquid_getFlow_t liquid_getDepth = Utils::FuncFromSigOffset<liquid_getFlow_t>(FindSignature("E8 ? ? ? ? 48 8B 4C 24 ? F3 0F 10 01"), 1);

	liquid_getDepth(this, flowOut, reg, pos);
}
bool BlockLegacy::hasWater(BlockSource* reg, const Vec3i& pos) {
	if (this->material->isLiquid) {
		return !this->material->isSuperHot;
	}

	auto liquidBlock = reg->getLiquidBlock(pos)->toLegacy();
	return this != liquidBlock && liquidBlock->material->isLiquid && !liquidBlock->material->isSuperHot;
}