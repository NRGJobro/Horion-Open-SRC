#include "StackableItem.h"

StackableItem::StackableItem() : IModule(0, Category::PLAYER, "Stack items you normally wouldn't be able to stack (e. g. swords).") {
	registerIntSetting("MaxItemAmount", &maxAmount, maxAmount, 1, 255);
}

StackableItem::~StackableItem() {
}

const char* StackableItem::getModuleName() {
	return ("StackableItem");
}

void StackableItem::onTick(GameMode* gm) {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* a = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = a->getItemStack(i);
		if (stack->item != NULL) {
			Item* item = *stack->item;
			item->setStackedByData(true);
			item->setMaxStackSize(maxAmount);
		}
	}
}
