#include "Scaffold.h"

#include "../../../Utils/Logger.h"

Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::WORLD, "Automatically build blocks beneath you.") {
	registerBoolSetting("Spoof", &spoof, spoof);
	registerBoolSetting("Staircase Mode", &staircaseMode, staircaseMode);
}

Scaffold::~Scaffold() {
}

const char* Scaffold::getModuleName() {
	return "Scaffold";
}

bool Scaffold::tryScaffold(Vec3 blockBelow) {
	blockBelow = blockBelow.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(blockBelow));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(blockBelow);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
			int p = supplies->selectedHotbarSlot;
			if (spoof) findBlock();
			bool idk = true;
			__int64 id = *Game.getLocalPlayer()->getUniqueId();
			Game.getGameMode()->buildBlock(&blok, i, idk);
			if (spoof) {
				C_MobEquipmentPacket a;
				a.actorRuntimeId = id;
				a.inventorySlot = p;
				a.selectedSlot = p;
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				supplies->selectedHotbarSlot = p;
			}
			return true;
		}
	}
	return false;
}

bool Scaffold::findBlock() {
	__int64 id = *Game.getLocalPlayer()->getUniqueId();
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->isBlock() && (*stack->item)->itemId != 0) {
				C_MobEquipmentPacket a;
				a.actorRuntimeId = id;
				a.inventorySlot = n;
				a.selectedSlot = n;
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				supplies->selectedHotbarSlot = n;
				return true;
			}
		}
	}
	return false;
}

void Scaffold::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;
	
	auto selectedItem = Game.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()) && !spoof)  // Block in hand?
		return;

	// Adjustment by velocity
	float speed = Game.getLocalPlayer()->velocity.magnitudexz();
	Vec3 vel = Game.getLocalPlayer()->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1

	if (staircaseMode) {
		Vec3 blockBelow = Game.getLocalPlayer()->eyePos0;  // Block 1 block below the player
		blockBelow.y -= Game.getLocalPlayer()->height;
		blockBelow.y -= 1.5f;

		Vec3 blockBelowBelow = Game.getLocalPlayer()->eyePos0;  // Block 2 blocks below the player
		blockBelowBelow.y -= Game.getLocalPlayer()->height;
		blockBelowBelow.y -= 2.0f;

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				blockBelowBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					blockBelowBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow) && Game.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						blockBelowBelow.z += vel.z;
						blockBelowBelow.x += vel.x;
						tryScaffold(blockBelow);
						tryScaffold(blockBelowBelow);
					}
				}
			}
		}
	} else {
		Vec3 blockBelow = Game.getLocalPlayer()->eyePos0;  // Block below the player
		blockBelow.y -= Game.getLocalPlayer()->height;
		blockBelow.y -= 0.5f;

		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && Game.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						tryScaffold(blockBelow);
					}
				}
			}
		}
	}
}

void Scaffold::onDisable() {
	if (Game.getLocalPlayer() == nullptr)
		return;
}
