#include "CEntity.h"

#include "../Memory/GameData.h"
#include "../Utils/Utils.h"
C_InventoryTransactionManager *C_Entity::getTransactionManager() {
	//return &this->transac;
	static unsigned int offset = 0x1210;
	//if (offset == 0) {
	// EnchantCommand::execute
	//offset = *reinterpret_cast<int *>(FindSignature("48 89 5C 24 18 55 56 57 41 56 41 57 48 83 EC 30 45 0F B6 F8 4C ") + 3);
	//}
	return reinterpret_cast<C_InventoryTransactionManager *>(reinterpret_cast<__int64>(this) + offset);
}
C_PlayerInventoryProxy *C_Player::getSupplies() {
	static unsigned int offset = 0xB70;
	/*if (offset == 0) {
		offset = *reinterpret_cast<int *>(FindSignature("48 8B 51 ?? 4C 8B 82 ?? ?? ?? ?? 48 8B B2 ?? ?? ?? ?? 41 80 B8") + 7);  // GameMode::startDestroyBlock -> GameMode::_canDestroy -> getSupplies
	}*/
	return *reinterpret_cast<C_PlayerInventoryProxy **>(reinterpret_cast<__int64>(this) + offset);
}
void C_LocalPlayer::unlockAchievements() {  // MinecraftEventing::fireEventAwardAchievement
	using fireEventAward = void(__fastcall *)(void *, int);
	static fireEventAward fireEventAwardFunc = reinterpret_cast<fireEventAward>(FindSignature("48 85 C9 0F 84 ? ? ? ? 48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 01"));
	for (int i = 0; i < 118; i++)
		fireEventAwardFunc(this, i);
}
void C_LocalPlayer::applyTurnDelta(vec2_t *viewAngleDelta) {
	using applyTurnDelta = void(__thiscall *)(void *, vec2_t *);
	static applyTurnDelta TurnDelta = reinterpret_cast<applyTurnDelta>(FindSignature("48 8B C4 48 89 58 18 48 89 68 20 56 57 41 56 48 81 EC ?? ?? ?? ?? 0F 29 70 D8 0F 29 78 C8 44 0F 29 40 ?? 48 8B 05 ?? ?? ?? ??"));
	TurnDelta(this, viewAngleDelta);
}
void C_LocalPlayer::setGameModeType(int gma) {
	// Player::setPlayerGameType
	//using setGameMode = void(__thiscall *)(void *, int);
	//static setGameMode Game_Mode = reinterpret_cast<setGameMode>(FindSignature("48 8B C4 56 57 41 56 48 81 EC ? ? ? ? 48 C7 44 24 ? ? ? ? ? 48 89 58 ? 48 89 68 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B EA 48 8B D9 44 8B B1 ? ? ? ? 83 FA ? 75 2D"));
	this->setC_PlayerGameType(gma);
}

bool Level::hasEntity() {
	return rayHitType == 1;
}

float C_Entity::getBlocksPerSecond() {
	return getTicksPerSecond() * *g_Data.getClientInstance()->minecraft->timer;
}

void C_Entity::lerpTo(vec3_t const &pos, vec2_t const &a2, int a3) { //lerpTo was removed from the Player vtable so this is how we are going to use it from now on
	using lerpTo = void(__fastcall *)(C_Entity *, vec3_t const &, vec2_t const &, int);
	static lerpTo lerp = reinterpret_cast<lerpTo>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 49 8B F8 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 48 8B 5C 24"));
	lerp(this, pos, a2, a3);
}

C_Entity *Level::getEntity() {
	if (rayHitType != 1) return nullptr;
	C_Entity *retval = nullptr;
	g_Data.forEachEntity([this, &retval](C_Entity *ent, bool b) {
		if (*(__int64 *)((__int64)ent + 0x10) == GamingEntityFinder) {
			retval = ent;
			return;
		}
	});
	return retval;
}