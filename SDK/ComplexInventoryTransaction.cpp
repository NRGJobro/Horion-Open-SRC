#include "ComplexInventoryTransaction.h"
#include "../Utils/Utils.h"
ComplexInventoryTransaction::ComplexInventoryTransaction(InventoryTransaction& transac) {
	static uintptr_t** ComplexInventoryTransactionVtable = 0x0;
	if (ComplexInventoryTransactionVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 89 73 08 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ComplexInventoryTransactionVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (ComplexInventoryTransactionVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	memset(this, 0, sizeof(ComplexInventoryTransaction));  // Avoid overwriting vtable
	vTable = ComplexInventoryTransactionVtable;
	uintptr_t boi = reinterpret_cast<uintptr_t>(this);
	using constructor_t = void(__fastcall*)(uintptr_t, InventoryTransaction&);
	static constructor_t constructor = reinterpret_cast<constructor_t>(FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B DA 48 8B F9 48 89 4C 24 ?? 8B 02 89 01 33 F6 48 89 71 ??"));
	if (constructor != 0)
		constructor(boi + 0x10, transac);
	this->actionType = 0;
}
ComplexInventoryTransaction::ComplexInventoryTransaction() {
	memset(this, 0, sizeof(ComplexInventoryTransaction));
	static uintptr_t** ComplexInventoryTransactionVtable = 0x0;
	if (ComplexInventoryTransactionVtable == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 89 73 08 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		ComplexInventoryTransactionVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
#ifdef _DEBUG
		if (ComplexInventoryTransactionVtable == 0x0 || sigOffset == 0x0)
			__debugbreak();
#endif
	}
	vTable = ComplexInventoryTransactionVtable;
}
ItemUseInventoryTransaction::ItemUseInventoryTransaction() {
	memset(this, 0x0, sizeof(ItemUseInventoryTransaction));
	using ItemUseInventoryTransactionContructor = void(__fastcall*)(ItemUseInventoryTransaction*);
	static ItemUseInventoryTransactionContructor constructor = reinterpret_cast<ItemUseInventoryTransactionContructor>(FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 50 48 89 74 24 58 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB E8 ?? ?? ?? ?? 33 F6 48 89 73 ?? 48 89 73 ?? 48 89 73 ?? 48 8D 05 ?? ?? ?? ?? 48 89 07 48 89 77 ?? 48 89 77 ?? 89 77 ?? 40 88 77 ?? 89"));
	if (constructor != 0)
		constructor(this);
}
ItemUseInventoryTransaction::ItemUseInventoryTransaction(int slot, const ItemStack* item, const Vec3 pos, int blockSide, int runtimeBlockId) {
	memset(this, 0x0, sizeof(ItemUseInventoryTransaction));
	using ItemUseInventoryTransactionContructor = void(__fastcall*)(ItemUseInventoryTransaction*);
	static ItemUseInventoryTransactionContructor constructor = reinterpret_cast<ItemUseInventoryTransactionContructor>(FindSignature("48 89 4C 24 08 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 50 48 89 74 24 58 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB E8 ?? ?? ?? ?? 33 F6 48 89 73 ?? 48 89 73 ?? 48 89 73 ?? 48 8D 05 ?? ?? ?? ?? 48 89 07 48 89 77 ?? 48 89 77 ?? 89 77 ?? 40 88 77 ?? 89"));
	if (constructor != 0)
		constructor(this);
	this->slot = slot;
	this->item = *item;
	this->pos = pos;
	this->runtimeBlockId = runtimeBlockId;
	this->blockSide = blockSide;
}
ItemReleaseInventoryTransaction::ItemReleaseInventoryTransaction() {
	memset(this, 0x0, sizeof(ItemReleaseInventoryTransaction));
	using ItemReleaseInventoryTransactionContructor = void(__fastcall*)(ItemReleaseInventoryTransaction*);
	static ItemReleaseInventoryTransactionContructor constructor = reinterpret_cast<ItemReleaseInventoryTransactionContructor>(FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB"));
	if (constructor != 0)
		constructor(this);
}
ItemReleaseInventoryTransaction::ItemReleaseInventoryTransaction(int slot, const ItemStack* item, const Vec3 pos) {
	memset(this, 0x0, sizeof(ItemReleaseInventoryTransaction));
	using ItemReleaseInventoryTransactionContructor = void(__fastcall*)(ItemReleaseInventoryTransaction*);
	static ItemReleaseInventoryTransactionContructor constructor = reinterpret_cast<ItemReleaseInventoryTransactionContructor>(FindSignature("48 89 4C 24 ?? 57 48 83 EC 30 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 5C 24 ?? 48 89 74 24 ?? 48 8B F9 48 8D 05 ?? ?? ?? ?? 48 89 01 C7 41 ?? ?? ?? ?? ?? 48 8D 59 ?? 48 89 5C 24 ?? 48 8B CB"));
	if (constructor != 0)
		constructor(this);
	this->slot = slot;
	this->item = *item;
	this->pos = pos;
}
