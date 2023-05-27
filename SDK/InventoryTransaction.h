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
		}
		if (target != nullptr) {
			this->targetItemDescriptor = *target;
		}

		if (sourceItem != nullptr) {
			this->sourceItem = *sourceItem;
			this->sourceItem.count = count;
		}
		if (targetItem != nullptr) {
			this->targetItem = *targetItem;
			this->targetItem.count = count;
		}

		this->sourceType = sourceType;
		this->type = type;
		this->flags = flag;
	}

public:
	int type;        //0x0  // named sourceType in nukkit
	int sourceType;  //0x4 // sometimes windowId
	int flags;  //0x8
	int slot;                             //0xC
	ItemDescriptor sourceItemDescriptor;  //0x10
	ItemDescriptor targetItemDescriptor;  //0x90
	ItemStack sourceItem;  //0x110
	ItemStack targetItem;  //0x1A0
};

class InventoryTransaction {};

class InventoryTransactionManager {
public:
	uintptr_t* player;               //0x0
	InventoryTransaction transac;  //0x8
private:
	int unknown;  //0x60
				  // Total size: 0x68
public:
	void addInventoryAction(InventoryAction const& action, bool forceBalanced = false);
};