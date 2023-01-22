#include "InventoryTransaction.h"
#include "../Utils/Utils.h"
void InventoryTransactionManager::addInventoryAction(const InventoryAction& action, bool idk) {
	static auto InventoryTransactionManager__addAction = Utils::FindSignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC 30 45 0F B6 F8 4C 8B F2 48 8B F1 48 8B 01 48 8B 88 ? ? ? ? 48 85 C9");
	if (InventoryTransactionManager__addAction)
		reinterpret_cast<void(__fastcall*)(InventoryTransactionManager*, const InventoryAction&, bool)>(InventoryTransactionManager__addAction)(this, action, idk);
}

void InventoryAction::fixInventoryStuff(ItemDescriptor* a1, ItemStack* a2) {
	static auto fixInvStuff = Utils::FindSignature("48 89 5C 24 18 48 89 6C 24 20 56 57 41 56 48 81 EC ?? 00 00 00 48 8B ?? ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? 00 00 00 48 8B F2 48 8B F9 48 89 4C");
	if (fixInvStuff)
		reinterpret_cast<void(__fastcall*)(ItemDescriptor*, ItemStack*)>(fixInvStuff)(a1, a2);
}
