#pragma once
#include "../Utils/HMath.h"
#include "Inventory.h"
#include "Item.h"

class InventoryAction {
public:
	void fixInventoryStuff(ItemDescriptor* a1, ItemStack* a2);
	InventoryAction() = default;
	InventoryAction(int slot, ItemStack* sourceItem, ItemStack* targetItem, int sourceType = 0, int type = 0, int flag = 0) {
		memset(this, 0x0, sizeof(InventoryAction));
		this->slot = slot;
		if (sourceItem != nullptr) {
			this->sourceItem = *sourceItem;
		}
		if (targetItem != nullptr) {
			this->targetItem = *targetItem;
		}

		// These seem to fix the inventory stuff
		fixInventoryStuff(reinterpret_cast<ItemDescriptor*>(reinterpret_cast<__int64>(this) + 0x10), reinterpret_cast<ItemStack*>(reinterpret_cast<__int64>(this) + 0x140));
		fixInventoryStuff(reinterpret_cast<ItemDescriptor*>(reinterpret_cast<__int64>(this) + 0xA8), reinterpret_cast<ItemStack*>(reinterpret_cast<__int64>(this) + 0x1E0));

		this->sourceType = sourceType;
		this->type = type;
		this->flags = flag;
	}

	InventoryAction(int slot, ItemDescriptor* source, ItemDescriptor* target, ItemStack* sourceItem, ItemStack* targetItem, int count, int sourceType = 0, int type = 0, int flag = 0) {
		memset(this, 0x0, sizeof(InventoryAction));
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
		this->flags = flag;
	}

public:
	int type;        //0x0  // named sourceType in nukkit
	int sourceType;  //0x4 // sometimes windowId
private:
	int flags;  //0x8
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
	ItemStack sourceItem;  //0x110
	ItemStack targetItem;  //0x1A0
};

class InventoryTransaction {
private:
	char pad_0x0[8];

public:
	__int64 ptr;  // 0x008
private:
	char pad_0x10[0x58 - 16];  //0x10
};

class InventoryTransactionManager {
public:
	uintptr_t* player;               //0x0
	InventoryTransaction transac;  //0x8
private:
	int unknown;  //0x60
				  // Total size: 0x68
public:
	void addInventoryAction(InventoryAction const& action, bool idk = false);
};