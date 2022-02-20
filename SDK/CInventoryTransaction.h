#pragma once
#include "../Utils/HMath.h"
#include "CInventory.h"
#include "CItem.h"

class C_InventoryAction {
public:
	void fixInventoryStuff(ItemDescriptor* a1, C_ItemStack* a2);
	C_InventoryAction() = default;
	C_InventoryAction(int slot, C_ItemStack* sourceItem, C_ItemStack* targetItem, int sourceType = 0, int type = 0) {
		memset(this, 0x0, sizeof(C_InventoryAction));
		this->slot = slot;
		if (sourceItem != nullptr) {
			this->sourceItem = *sourceItem;
		}
		if (targetItem != nullptr) {
			this->targetItem = *targetItem;
		}

		// These seem to fix the inventory stuff
		fixInventoryStuff(reinterpret_cast<ItemDescriptor*>(reinterpret_cast<__int64>(this) + 0x10), reinterpret_cast<C_ItemStack*>(reinterpret_cast<__int64>(this) + 0x110));
		fixInventoryStuff(reinterpret_cast<ItemDescriptor*>(reinterpret_cast<__int64>(this) + 0x90), reinterpret_cast<C_ItemStack*>(reinterpret_cast<__int64>(this) + 0x1A0));

		this->sourceType = sourceType;
		this->type = type;
	}

	C_InventoryAction(int slot, ItemDescriptor* source, ItemDescriptor* target, C_ItemStack* sourceItem, C_ItemStack* targetItem, int count, int sourceType = 0, int type = 0) {
		memset(this, 0x0, sizeof(C_InventoryAction));
		this->slot = slot;

		if (source != nullptr) {
			this->sourceItemDescriptor = *source;
			this->s_count = count;
		}

		if (target != nullptr) {
			this->targetItemDescriptor = *target;
			this->t_count = count;
		}

		if (sourceItem != nullptr)
			this->sourceItem = *sourceItem;
		if (targetItem != nullptr)
			this->targetItem = *targetItem;

		this->sourceType = sourceType;
		this->type = type;
	}

public:
	int type;        //0x0  // named sourceType in nukkit
	int sourceType;  //0x4 // sometimes windowId
private:
	int unknown;  //0x8
public:
	int slot;                             //0xC
	ItemDescriptor sourceItemDescriptor;  //0x10
	int s_count;                          //0x58
private:
	char pad_0x0058[0x34];  //0x005C
public:
	ItemDescriptor targetItemDescriptor;  //0x90
	int t_count;                          //0xD8

private:
	char pad_0x00DC[0x34];  //0x00DC
public:
	C_ItemStack sourceItem;  //0x110
	C_ItemStack targetItem;  //0x1A0
};

class C_InventoryTransaction {
private:
	char pad_0x0[8];

public:
	__int64 ptr;  // 0x008
private:
	char pad_0x10[0x58 - 16];  //0x10
};

class C_InventoryTransactionManager {
public:
	uintptr_t* player;               //0x0
	C_InventoryTransaction transac;  //0x8
private:
	int unknown;  //0x60
				  // Total size: 0x68
public:
	void addInventoryAction(C_InventoryAction const& action, bool idk = false);
};