#include "AutoTotem.h"

AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Automatically puts totems into your offhand.") {
}

AutoTotem::~AutoTotem() {
}

const char* AutoTotem::getModuleName() {
	return ("AutoTotem");
}

void AutoTotem::onTick(GameMode* gm) {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	InventoryTransactionManager* manager = Game.getLocalPlayer()->getTransactionManager();
	ItemStack* current = Game.getLocalPlayer()->getEquippedTotem();
	if (current->item == NULL) {
		for (int i = 0; i < 36; i++) {
			ItemStack* totem = inv->getItemStack(i);
			if (totem->item != NULL && (*totem->item)->itemId == 568) {
				ItemDescriptor* desc = nullptr;
				desc = new ItemDescriptor((*totem->item)->itemId, 0);
				InventoryAction first(i, desc, nullptr, totem, nullptr, 1);
				InventoryAction second(37, nullptr, desc, nullptr, totem, 1);
				Game.getLocalPlayer()->setOffhandSlot(totem);
				manager->addInventoryAction(first);
				manager->addInventoryAction(second);
			}
		}
	}	
}
