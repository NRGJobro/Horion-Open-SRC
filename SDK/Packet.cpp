#include "Packet.h"

#include "../Utils/Utils.h"

TextHolder* Packet::getName() {
	return Utils::CallVFunc<2, TextHolder*>(this, new TextHolder());
}
LevelSoundEventPacket::LevelSoundEventPacket() {
	static uintptr_t** LevelSoundEventPacketVtable = 0x0;
	if (LevelSoundEventPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 4A 10 48 C7 42 ? ? ? ? ? 48 89 42 48 89 42 50 48 89 42 58 48 C7 42 ? ? ? ? ? 48 89 42 68 66 89 42 78 EB 03 48 8B D0 0F 57 C0 0F 11 03 48 8D 42 10 48 89 03 48 89 53 08 48 8B C3 48 83 C4 30 5B C3 CC CC CC CC CC CC CC 40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B C8 48 8B 10 4C 8B 42 08 BA ? ? ? ? 41 FF D0 48 89 44 24 ? 48 8B D0 48 85 C0 74 72");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		LevelSoundEventPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (LevelSoundEventPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(LevelSoundEventPacket));  // Avoid overwriting vtable
	vTable = LevelSoundEventPacketVtable;
	this->entityType.setText("minecraft:player");
}
PlayerAuthInputPacket::PlayerAuthInputPacket() {
	static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
	if (PlayerAuthInputPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 0F 57 C0 0F 11 00 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 89 02 33 C0 48 89 42");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerAuthInputPacket));  // Avoid overwriting vtable
	vTable = PlayerAuthInputPacketVtable;
}
PlayerAuthInputPacket::PlayerAuthInputPacket(Vec3 pos, float pitch, float yaw, float yawUnused) {
	static uintptr_t** PlayerAuthInputPacketVtable = 0x0;
	if (PlayerAuthInputPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 0F 57 C0 0F 11 00 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 89 02 33 C0 48 89 42");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		PlayerAuthInputPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (PlayerAuthInputPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerAuthInputPacket));  // Avoid overwriting vtable
	vTable = PlayerAuthInputPacketVtable;
	this->pos = pos;
	this->pitch = pitch;
	this->yaw = yaw;
	this->yawUnused = yawUnused;
	this->InputAD = 0.f;
	this->InputWS = 0.f;
}

MobEquipmentPacket::MobEquipmentPacket() {
	static uintptr_t** MobEquipmentPacketVtable = 0x0;
	if (MobEquipmentPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 06 48 8B C6 4C 8D 9C 24");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		MobEquipmentPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (MobEquipmentPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(MobEquipmentPacket));  // Avoid overwriting vtable
	vTable = MobEquipmentPacketVtable;
}

MobEquipmentPacket::MobEquipmentPacket(__int64 entityRuntimeId, ItemStack& item, int hotbarSlot, int inventorySlot) {
	memset(this, 0x0, sizeof(MobEquipmentPacket));
	using MobEquimentPacketConstructor_t = void(__fastcall*)(MobEquipmentPacket*, __int64, ItemStack&, int, int, char);
	static MobEquimentPacketConstructor_t MobEquimentPacketConstructor = reinterpret_cast<MobEquimentPacketConstructor_t>(FindSignature("48 8D 05 ? ? ? ? 48 89 06 48 8B C6 4C 8D 9C 24"));

	if (MobEquimentPacketConstructor != 0)
		MobEquimentPacketConstructor(this, entityRuntimeId, item, hotbarSlot, inventorySlot, 0);
}

InventoryTransactionPacket::InventoryTransactionPacket() {
	static uintptr_t** InventoryTransactionPacketVtable = 0x0;
	if (InventoryTransactionPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8d 05 ? ? ? ? 48 89 45 ? 48 8d 05 ? ? ? ? 48 89 45 ? 89 75");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		InventoryTransactionPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (InventoryTransactionPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(InventoryTransactionPacket));  // Avoid overwriting vtable
	vTable = InventoryTransactionPacketVtable;
}
C_TextPacket::C_TextPacket() {
	static uintptr_t** textPacketVtable = 0x0;
	if (textPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 44 24 ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 48 8B 4D");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		textPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (textPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_TextPacket));  // Avoid overwriting vtable
	vTable = textPacketVtable;

	messageType = 1;  // TYPE_CHAT
}
C_MovePlayerPacket::C_MovePlayerPacket() {
	static uintptr_t** movePlayerPacketVtable = 0x0;
	if (movePlayerPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 40 08 48 8B 47 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MovePlayerPacket));  // Avoid overwriting vtable
	vTable = movePlayerPacketVtable;
}
C_MovePlayerPacket::C_MovePlayerPacket(LocalPlayer* player, Vec3 pos) {
	static uintptr_t** movePlayerPacketVtable = 0x0;
	if (movePlayerPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 49 89 40 08 48 8B 47 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		movePlayerPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (movePlayerPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_MovePlayerPacket));  // Avoid overwriting vtable
	vTable = movePlayerPacketVtable;
	entityRuntimeID = player->entityRuntimeId;
	Position = pos;
	pitch = player->pitch;
	yaw = player->yaw;
	headYaw = player->yaw;
	onGround = true;
	mode = 0;
}
PlayerActionPacket::PlayerActionPacket() {
	static uintptr_t** playerActionPacketVtable = 0x0;
	if (playerActionPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 4D ? 66 0F 7F 45 ? C7 45 ? ? ? ? ? 48 89 45 ? 49 8B 8D");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerActionPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerActionPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerActionPacket));  // Avoid overwriting vtable
	vTable = playerActionPacketVtable;
}

C_SubChunkRequestPacket::C_SubChunkRequestPacket() {
	static uintptr_t** subChunkRequestPacketVtable = 0x0;
	if (subChunkRequestPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 8B F9 48 89 01 8B DA 48 83 C1 50 E8 ? ? ? ? 48 8D 4F 38 E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 07");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		subChunkRequestPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (subChunkRequestPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_SubChunkRequestPacket));  // Avoid overwriting vtable
	vTable = subChunkRequestPacketVtable;
}

C_EmotePacket::C_EmotePacket() {
	static uintptr_t** emotePacketVtable = 0x0;
	if (emotePacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 8D ? ? ? ? 48 89 85 ? ? ? ? 48 89 BD");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		emotePacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (emotePacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_EmotePacket));  // Avoid overwriting vtable
	vTable = emotePacketVtable;
}
//48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 89 48 40 48 89 48 48 48 89 48 58 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? 48 89 50 10 48 C7 40 ? ? ? ? ? EB 04 33 C9 8B C1 48 8D 48 10 48 8D 15 ? ? ? ? 48 89 51 20 48 89 43 08 48 8B C3 48 89 0B 48 83 C4 30 5B C3 48 8D 0D ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 30 65 48 8B 04 25 ? ? ? ? 48 8B D9 48 89 4C 24 ? BA ? ? ? ? 48 8B 08 8B 04 0A 39 05 ? ? ? ? 0F 8F ? ? ? ? E8 ? ? ? ? 48 8B C8 BA ? ? ? ? 48 8B 00 FF 50 08 48 89 44 24 ? 48 85 C0 74 4F
//Model Form Response packet
AnimatePacket::AnimatePacket() {
	static uintptr_t** animatePacketVtable = 0x0;
	if (animatePacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 48 89 48 40 48 89 48 48 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? 48 89 50 10 EB 04 33 C9 8B C1 48 8D 48 10 48 8D 15 ? ? ? ? 48 89 51 20 48 89 43 08 48 8B C3 48 89 0B 48 83 C4 30 5B C3 48 8D 0D ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 40");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		animatePacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (animatePacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(AnimatePacket));  // Avoid overwriting vtable
	vTable = animatePacketVtable;
}

AnimatePacket::AnimatePacket(int action, __int64 entityRuntimeID, float rowingTime) {
	static uintptr_t** animatePacketVtable = 0x0;
	if (animatePacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 48 89 48 40 48 89 48 48 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? 48 89 50 10 EB 04 33 C9 8B C1 48 8D 48 10 48 8D 15 ? ? ? ? 48 89 51 20 48 89 43 08 48 8B C3 48 89 0B 48 83 C4 30 5B C3 48 8D 0D ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 40");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		animatePacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (animatePacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(AnimatePacket));  // Avoid overwriting vtable
	vTable = animatePacketVtable;
	this->action = action;
	this->entityId = entityRuntimeID;
	this->rowingTime = rowingTime;
}

C_NPCRequestPacket::C_NPCRequestPacket() {
	static uintptr_t** npcRequestPacketVtable = 0x0;
	if (npcRequestPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 03 48 89 53 30");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		npcRequestPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (npcRequestPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_NPCRequestPacket));  // Avoid overwriting vtable
	vTable = npcRequestPacketVtable;
}

PlayerSkinPacket::PlayerSkinPacket() {
	static uintptr_t** playerSkinPacketVtable = 0x0;
	if (playerSkinPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 48 89 79 30 48 89 79 38 48 83 C1 40");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerSkinPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerSkinPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerSkinPacket));  // Avoid overwriting vtable
	vTable = playerSkinPacketVtable;
}

NetworkLatencyPacket::NetworkLatencyPacket() {
	static uintptr_t** networkLatencyPacketVtable = 0x0;
	if (networkLatencyPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 C7 41 ? ? ? ? ? 66 C7 41 ? ? ? FF 15");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		networkLatencyPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (networkLatencyPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(NetworkLatencyPacket));  // Avoid overwriting vtable
	vTable = networkLatencyPacketVtable;
}

CommandRequestPacket::CommandRequestPacket() {
	static uintptr_t** commandRequestPacketVtable = 0x0;
	if (commandRequestPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 0F 11 00 C7 40 ?? ?? ?? ?? ?? C7 40 ?? ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ?? 48 89 02 33 C0 48 89 42 28 48 89 42 30 89 42 38 48 89 42 40 48 89 42 50 88 42 60");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		commandRequestPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (commandRequestPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(CommandRequestPacket));  // Avoid overwriting vtable
	vTable = commandRequestPacketVtable;
}

CommandRequestPacket::CommandRequestPacket(std::string cmd) {
	static uintptr_t** commandRequestPacketVtable = 0x0;
	if (commandRequestPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 0F 11 00 C7 40 ?? ?? ?? ?? ?? C7 40 ?? ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ?? 48 89 02 33 C0 48 89 42 28 48 89 42 30 89 42 38 48 89 42 40 48 89 42 50 88 42 60");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		commandRequestPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (commandRequestPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(CommandRequestPacket));  // Avoid overwriting vtable
	vTable = commandRequestPacketVtable;
	this->origin = Origin::PLAYER;
	this->isExternal = 0;
}

C_InteractPacket::C_InteractPacket(/*enum InteractPacket::Action, class ActorRuntimeID, Vec3 const&*/) {
	static uintptr_t** interactPacketVtable = 0x0;
	if (interactPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 88 48 40 48 89 48 48 48 89 48 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		interactPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (interactPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(C_InteractPacket));  // Avoid overwriting vtable
	vTable = interactPacketVtable;
}

SetPlayerGameTypePacket::SetPlayerGameTypePacket() {
	static uintptr_t** setPlayerGameTypePacketVtable = 0x0;
	if (setPlayerGameTypePacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 48 10 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? C7 40 ? ? ? ? ? EB 04 33 C9 8B C1 48 8D 48 10 48 8D 15 ? ? ? ? 48 89 51 20 48 89 43 08 48 8B C3 48 89 0B 48 83 C4 30 5B C3 48 8D 0D ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 30 65 48 8B 04 25 ? ? ? ? 48 8B D9 48 89 4C 24 ? BA ? ? ? ? 48 8B 08 8B 04 0A 39 05 ? ? ? ? 0F 8F ? ? ? ? E8 ? ? ? ? 48 8B C8 BA ? ? ? ? 48 8B 00 FF 50 08 48 89 44 24 ? 48 85 C0 74 5B");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		setPlayerGameTypePacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (setPlayerGameTypePacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(SetPlayerGameTypePacket));  // Avoid overwriting vtable
	vTable = setPlayerGameTypePacketVtable;
}

ActorEventPacket::ActorEventPacket() {
	static uintptr_t** actorEvenPacketVtable = 0x0;
	if (actorEvenPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 48 89 48 40 88 48 48 89 48 4C");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		actorEvenPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (actorEvenPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(ActorEventPacket));  // Avoid overwriting vtable
	vTable = actorEvenPacketVtable;
}

ActorEventPacket::ActorEventPacket(uint64_t entityRuntimeId, char eventId, int16_t itemId) {
	static uintptr_t** actorEvenPacketVtable = 0x0;
	if (actorEvenPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 48 89 48 40 88 48 48 89 48 4C");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		actorEvenPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (actorEvenPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(ActorEventPacket));  // Avoid overwriting vtable
	vTable = actorEvenPacketVtable;
	this->entityRuntimeId = entityRuntimeId;
	this->eventId = eventId;
	this->itemId = itemId;
}

PlayerHotbarPacket::PlayerHotbarPacket() { //Client
	static uintptr_t** playerHotbarPacketVtable = 0x0;
	if (playerHotbarPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 89 48 40 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? 48 89 50 10 66 C7 40 ? ? ?");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerHotbarPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerHotbarPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerHotbarPacket));  // Avoid overwriting vtable
	vTable = playerHotbarPacketVtable;
}

PlayerHotbarPacket::PlayerHotbarPacket(int selectedHotbarSlot, char containerId, bool selectHotbarSlot) { //Client
	static uintptr_t** playerHotbarPacketVtable = 0x0;
	if (playerHotbarPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 89 48 40 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? 48 89 50 10 66 C7 40 ? ? ?");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerHotbarPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerHotbarPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerHotbarPacket));  // Avoid overwriting vtable
	vTable = playerHotbarPacketVtable;
	this->selectedHotbarSlot = selectedHotbarSlot;
	this->containerId = containerId;
	this->selectHotbarSlot = selectHotbarSlot;
}

MobEffectPacket::MobEffectPacket() { //Client
	static uintptr_t** mobEffectPacketVtable = 0x0;
	if (mobEffectPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 48 89 48 40 89 48 48 88 48 4C 48 89 48 50 88 48 58");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		mobEffectPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (mobEffectPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(MobEffectPacket));  // Avoid overwriting vtable
	vTable = mobEffectPacketVtable;
}

MobEffectPacket::MobEffectPacket(__int64 entityRuntimeId, char event, int effectId, int amplifier, bool showParticles, int duration) { //Client
	static uintptr_t** mobEffectPacketVtable = 0x0;
	if (mobEffectPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 15 ? ? ? ? C7 40 ? ? ? ? ? 33 C9 C7 40 ? ? ? ? ? 48 89 48 28 48 89 48 30 89 48 38 48 89 48 40 89 48 48 88 48 4C 48 89 48 50 88 48 58");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		mobEffectPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (mobEffectPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(MobEffectPacket));  // Avoid overwriting vtable
	vTable = mobEffectPacketVtable;
	this->eid = entityRuntimeId;
	this->event = event;
	this->effectId = effectId;
	this->amplifier = amplifier;
	this->duration = duration;
	this->showParticles = showParticles;
}

RemoveActorPacket::RemoveActorPacket() { //Client
	static uintptr_t** removeActorPacketVtable = 0x0;
	if (removeActorPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 48 89 48 10 C7 40 ? ? ? ? ? C7 40 ? ? ? ? ? 66 C7 40 ? ? ? 48 C7 40 ? ? ? ? ? EB 04 33 C9 8B C1 48 8D 48 10 48 8D 15 ? ? ? ? 48 89 51 20 48 89 43 08 48 8B C3 48 89 0B 48 83 C4 30 5B C3 48 8D 0D ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 0F 85 ? ? ? ? 48 8D 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC 40 53");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		removeActorPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (removeActorPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(RemoveActorPacket));  // Avoid overwriting vtable
	vTable = removeActorPacketVtable;
}

PlayerArmorDamagePacket::PlayerArmorDamagePacket() { //Client
	static uintptr_t** playerArmorDamagePacketVtable = 0x0;
	if (playerArmorDamagePacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8d 0d ? ? ? ? 48 89 48 ? 33 c9 c7 40");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		playerArmorDamagePacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (playerArmorDamagePacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(PlayerArmorDamagePacket));  // Avoid overwriting vtable
	vTable = playerArmorDamagePacketVtable;
}

SetHealthPacket::SetHealthPacket() {  // Client
	static uintptr_t** setHealthPacketVtable = 0x0;
	if (setHealthPacketVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8d 15 ? ? ? ? c7 40 ? ? ? ? ? 33 c9 c7 40 ? ? ? ? ? 48 89 48 ? 48 89 48 ? 89 48 ? 89 48 ? c7 40 ? ? ? ? ? c7 40 ? ? ? ? ? 66 c7 40 ? ? ? 48 89 50 ? eb ? 33 c9 8b c1 48 8d 48 ? 48 8d 15 ? ? ? ? 48 89 51 ? 48 89 43 ? 48 8b c3 48 89 0b 48 83 c4 ? 5b c3 48 8d 0d ? ? ? ? e8 ? ? ? ? 83 3d ? ? ? ? ? 0f 85 ? ? ? ? 48 8d 05 ? ? ? ? 48 8d 0d ? ? ? ? 48 89 05 ? ? ? ? e8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? e9 ? ? ? ? cc cc cc cc cc cc cc cc cc cc cc cc cc cc cc 40 53 48 83 ec ? 48 8b 05");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		setHealthPacketVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (setHealthPacketVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(SetHealthPacket));  // Avoid overwriting vtable
	vTable = setHealthPacketVtable;
}
