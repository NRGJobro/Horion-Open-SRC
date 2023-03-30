#pragma once
#include "../Utils/HMath.h"
#include "ComplexInventoryTransaction.h"
#include "Entity.h"
#include "TextHolder.h"

class Packet {
public:
	uintptr_t** vTable;  //0x0000
private:
	char pad_0x8[0x28];
public:
	TextHolder* getName();
	template <class T>
	bool isInstanceOf() {
		T packet;
		if (packet.vTable == this->vTable)
			return true;
		else
			return false;
	}
};

class LevelSoundEventPacket : public Packet {
public:
	int sound;           //0x28
	Vec3 pos;          //0x2C
	int extraData = -1;  //0x38
private:
	int unknown = 0;  //0x3C
public:
	TextHolder entityType;               //0x40
	bool isBabyMod = false;              //0x60
	bool disableRelativeVolume = false;  //0x61
	LevelSoundEventPacket();
};

class PlayerAuthInputPacket : public Packet {
public:
	//uint64_t entityRuntimeId; //0x0020
	float pitch;  //0x0028
	float yaw;
	Vec3 pos;       //0x0030
	float yawUnused;  //0x0038
	Vec3 velocity;
	float InputAD;  // 1 for A, -1 for D, multiply by sqrt(2)/2 if mixed with InputWS
	float InputWS;  // 1 for W, -1 for S, multiply by sqrt(2)/2 if mixed with InputAD
private:
	uint8_t padshit[12];
public:
	uint32_t inputKeys;
	char padding[0x256];

public:
	PlayerAuthInputPacket();
	PlayerAuthInputPacket(Vec3 pos, float pitch, float yaw, float yawUnused);
};

class PlayerActionPacket : public Packet {
public:
	PlayerActionPacket();
	
	Vec3i blockPosition;    // 0x28
	int face;                 // 0x34
	int action;               // 0x38
	__int64 entityRuntimeId;  // 0x40
};

class C_SubChunkRequestPacket : public Packet {
public:
	C_SubChunkRequestPacket();
};

class C_EmotePacket : public Packet {
public:
	C_EmotePacket();
};

class AnimatePacket : public Packet {
public:
	AnimatePacket();
	AnimatePacket(int action, __int64 entityRuntimeID, float unknownValue);

	__int64 entityId;
	int action;
	float unknown;
};

class C_NPCRequestPacket : public Packet {
public:
	C_NPCRequestPacket();
	
	long entityRuntimeId;  //Test
	__int64 Unknown0;      //Test
	std::string Unknown1;  //Test
	__int64 Unknown2;      //Test
};

class PlayerSkinPacket : public Packet {
public:
	PlayerSkinPacket();
	
	__int64 UUID;
	__int64 skin;  //meant to be skin class
	std::string skinName;
	std::string oldSkinName;
};

class NetworkLatencyPacket : public Packet {
public:
	NetworkLatencyPacket();
	
	unsigned long timeStamp;  //Test
	bool sendBack;            //Test
	int pad[0x100];           //0x0
};

class CommandRequestPacket : public Packet {
public:
	CommandRequestPacket();
	CommandRequestPacket(std::string cmd);
	
	enum class Origin {
		PLAYER = 0,
		COMMAND_BLOCK = 1,
		MINECART_COMMAND_BLOCK = 2,
		DEV_CONSOLE = 3,
		AUTOMATION_PLAYER = 4,
		CLIENT_AUTOMATION = 5,
		DEDICATED_SERVER = 6,
		ENTITY = 7,
		VIRTUAL = 8,
		GAME_ARGUMENT = 9,
		INTERNAL = 10
	};

	TextHolder command;
	Origin origin = Origin::PLAYER;
	bool isExternal = 0;
	char shit[0x256];
};

class C_InteractPacket : public Packet {
public:
	C_InteractPacket(/**enum InteractPacket::Action, class ActorRuntimeID, Vec3 const&*/);
	
public:
	int action;
	long target;
};

class SetPlayerGameTypePacket : public Packet {
public:
	SetPlayerGameTypePacket();

public:
	int gameMode;
};

class ActorEventPacket : public Packet {
public:
	ActorEventPacket();
	ActorEventPacket(uint64_t entityRuntimeId, char eventId, int16_t itemId);
	
	uint64_t entityRuntimeId;  //0x0030
	char eventId;              //0x0038
	char pad_0x39[0x5];        //0x0039
	int16_t itemId;            //0x003E
};

class C_MobEquipmentPacket : public Packet {
public:
	C_MobEquipmentPacket();
	C_MobEquipmentPacket(__int64 entityRuntimeId, ItemStack& item, int hotbarSlot, int inventorySlot);
	__int64 eid;
	Item* item;
	char inventorySlot;
	char hotbarSlot;
	char windowId;
};

class InventoryTransactionPacket : public Packet {
public:
	InventoryTransactionPacket();
	InventoryTransactionPacket(ComplexInventoryTransaction* transac) : InventoryTransactionPacket() {
		this->complexTransaction = transac;
	}
	
	int unknown = 0;                                    //0x28
	__int64* unknownStart = 0;                          //0x30
	__int64* unknownEnd = 0;                            //0x38
	__int64 filler = 0;                                 // 0x40
	ComplexInventoryTransaction* complexTransaction;  // 0x48
	unsigned char numTransactions;                      // 0x50
};

class C_TextPacket : public Packet {
public:
	C_TextPacket();

	unsigned __int8 messageType;  // 0x28

	unsigned char gap[7];

	TextHolder sourceName;           // 0x30
	TextHolder message;              // 0x50
	unsigned char field_24222[24];   // 0x70
	bool translationNeeded = false;  // 0x88

	unsigned char ga2p[7];  // 0x89
	TextHolder xboxUserId;
	TextHolder platformChatId;
};

#pragma pack(push, 8)

__declspec(align(8)) class C_MovePlayerPacket : public Packet {
public:
	C_MovePlayerPacket();
	C_MovePlayerPacket(LocalPlayer* player, Vec3 pos);
	
	__int64 entityRuntimeID;  //0x28
	Vec3 Position;          //0x30
	float pitch;              //0x3c
	float yaw;                //0x40
	float headYaw;            //0x44
	uint8_t mode;             //0x48
	bool onGround;
	__int64 ridingEid;
	int int1;
	int int2;
};

#pragma pack(pop)
