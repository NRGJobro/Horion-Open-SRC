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
	Level* level = g_Data.getLocalPlayer()->getlevel();
	auto entity = level->getEntity();
	if (entity != nullptr) {
		auto id = entity->getEntityTypeId();
		char* name = entity->getNameTag()->getText();
		g_Data.getGuiData()->displayClientMessageF("---------------");
		g_Data.getGuiData()->displayClientMessageF("Entity Name: %s", name);
		g_Data.getGuiData()->displayClientMessageF("Entity ID: %lld", id);
		g_Data.getGuiData()->displayClientMessageF("---------------");
	}
}

void itemId() {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	auto n = supplies->selectedHotbarSlot;
	C_ItemStack* stack = inv->getItemStack(n);
	if (stack->item != nullptr) {
		auto id = stack->getItem()->itemId;
		char* name = stack->getItem()->name.getText();
		g_Data.getGuiData()->displayClientMessageF("---------------");
		g_Data.getGuiData()->displayClientMessageF("Item Name: %s", name);
		g_Data.getGuiData()->displayClientMessageF("Item ID: %lld", id);
		g_Data.getGuiData()->displayClientMessageF("---------------");
	}
}

void showAimedBlockInfo() {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	Level* level = g_Data.getLocalPlayer()->getlevel();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(level->block);
	auto entity = level->getEntity();
	if (block != nullptr && level != nullptr && entity == nullptr && block->blockLegacy->blockId != 7) {
		char* name = block->toLegacy()->name.getText();
		auto id = block->toLegacy()->blockId;
		g_Data.getGuiData()->displayClientMessageF("---------------");
		g_Data.getGuiData()->displayClientMessageF("Block Name: %s", name);
		g_Data.getGuiData()->displayClientMessageF("Block ID: %lld", id);
		g_Data.getGuiData()->displayClientMessageF("---------------");
	}
}

bool TestCommand::execute(std::vector<std::string>* args) {
	listEnts();
	itemId();
	showAimedBlockInfo();
	return true;
}
