#include "TestCommand.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"
#include <sstream>
#include "../../../SDK/Tag.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
}

void listEnts() {
	Level* level = Game.getLocalPlayer()->getlevel();
	auto entity = level->getEntity();
	if (entity != nullptr) {
		auto id = entity->getEntityTypeId();
		char* name = entity->getNameTag()->getText();
		Game.getGuiData()->displayClientMessageF("---------------");
		Game.getGuiData()->displayClientMessageF("Entity Name: %s", name);
		Game.getGuiData()->displayClientMessageF("Entity ID: %lld", id);
		Game.getGuiData()->displayClientMessageF("---------------");
	}
}

void itemId() {
	LocalPlayer* player = Game.getLocalPlayer();
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	auto n = supplies->selectedHotbarSlot;
	ItemStack* stack = inv->getItemStack(n);
	if (stack->item != nullptr) {
		auto id = stack->getItem()->itemId;
		char* name = stack->getItem()->name.getText();
		Game.getGuiData()->displayClientMessageF("---------------");
		Game.getGuiData()->displayClientMessageF("Item Name: %s", name);
		Game.getGuiData()->displayClientMessageF("Item ID: %lld", id);
		Game.getGuiData()->displayClientMessageF("---------------");
	}
}

void showAimedBlockInfo() {
	LocalPlayer* player = Game.getLocalPlayer();
	Level* level = Game.getLocalPlayer()->getlevel();
	Block* block = Game.getLocalPlayer()->region->getBlock(level->block);
	auto entity = level->getEntity();
	if (block != nullptr && level != nullptr && entity == nullptr && block->blockLegacy->blockId != 7) {
		char* name = block->toLegacy()->name.getText();
		auto id = block->toLegacy()->blockId;
		Game.getGuiData()->displayClientMessageF("---------------");
		Game.getGuiData()->displayClientMessageF("Block Name: %s", name);
		Game.getGuiData()->displayClientMessageF("Block ID: %lld", id);
		Game.getGuiData()->displayClientMessageF("---------------");
	}
}

bool TestCommand::execute(std::vector<std::string>* args) {
	listEnts();
	itemId();
	showAimedBlockInfo();
	return true;
}
