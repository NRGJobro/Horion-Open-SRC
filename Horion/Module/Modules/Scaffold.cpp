#include "Scaffold.h"

#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"
#include "../../Module/ModuleManager.h"

Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::WORLD, "Automatically build blocks beneath you") {
	registerBoolSetting("AutoSelect", &autoSelect, autoSelect);
	registerBoolSetting("Down", &down, down);
	registerBoolSetting("Highlight", &highlight, highlight);
	registerBoolSetting("Hive", &hive, hive);
	registerBoolSetting("Rotations", &rotations, rotations);
	registerBoolSetting("Y Lock", &Ylock, Ylock);
	registerIntSetting("Extend", &extend, extend, 0, 8);
}

bool Scaffold::tryScaffold(Vec3 blockBelow) {
	Vec3 vel = Game.getLocalPlayer()->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1
	blockBelow = blockBelow.floor();

	DrawUtils::setColor(1.f, 1.f, 1.f, 1.f);  // white when placing all the time
	if (highlight) DrawUtils::drawBox(blockBelow, Vec3(blockBelow).add(1), 0.4f);  // Draw a box around the block about to be placed

	BlockSource* region = Game.getLocalPlayer()->region;
	Block* block = region->getBlock(Vec3i(blockBelow));
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
			bool Y = ((region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			if (autoSelect) findBlock();
			Game.getGameMode()->buildBlock(&blok, i, false);
			return true;
		}
	}
	return false;
}

bool Scaffold::tryClutchScaffold(Vec3 blockBelow) {
	Vec3 vel = Game.getLocalPlayer()->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1
	blockBelow = blockBelow.floor();

	DrawUtils::setColor(0.f, 0.f, 1.f, 1.f);  // blue when predicting
	if (highlight) DrawUtils::drawBox(blockBelow, Vec3(blockBelow).add(1), 0.4f);  // Draw a box around the block about to be placed

	static std::vector<Vec3i> checkBlocks;
	if (checkBlocks.empty()) {  // Only re sort if its empty
		for (int y = -4; y <= 4; y++) {
			for (int x = -4; x <= 4; x++) {
				for (int z = -4; z <= 4; z++) {
					checkBlocks.push_back(Vec3i(x, y, z));
				}
			}
		}
		// https://www.mathsisfun.com/geometry/pythagoras-3d.html c2 = x2 + y2 + z2 funny
		std::sort(checkBlocks.begin(), checkBlocks.end(), [](Vec3i first, Vec3i last) {
			return sqrtf((float)(first.x * first.x) + (float)(first.y * first.y) + (float)(first.z * first.z)) < sqrtf((float)(last.x * last.x) + (float)(last.y * last.y) + (float)(last.z * last.z));
		});
	}

	for (const Vec3i& blockOffset : checkBlocks) {
		Vec3i currentBlock = Vec3i(blockBelow).add(blockOffset);

		// Normal tryScaffold after it sorts
		BlockSource* region = Game.getLocalPlayer()->region;
		Block* block = region->getBlock(Vec3i(currentBlock));
		BlockLegacy* blockLegacy = (block->blockLegacy);
		if (blockLegacy->material->isReplaceable) {
			Vec3i blok(currentBlock);

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
				bool Y = ((region->getBlock(calc)->blockLegacy))->material->isReplaceable;
				if (!((region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
					// Found a solid block to click
					foundCandidate = true;
					blok = calc;
					break;
				}
				i++;
			}
			if (foundCandidate) {
				if (autoSelect) findBlock();
				Game.getGameMode()->buildBlock(&blok, i, false);
				return true;
			}
		}
	}
	return false;
}

bool Scaffold::findBlock() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	auto prevSlot = supplies->selectedHotbarSlot;
	for (int i = 0; i < 8; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->item != nullptr) {
			if (stack->getItem()->isBlock()) {
				if (prevSlot != i)
					supplies->selectedHotbarSlot = i;
				return true;
			}
		}
	}
	return false;
}

void Scaffold::onPostRender(MinecraftUIRenderContext* ctx) {
	if (Game.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;

	auto player = Game.getLocalPlayer();
	auto selectedItem = player->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()) && !autoSelect)  // Block in hand?
		return;

	// Adjustment by velocity
	float speed = Game.getLocalPlayer()->velocity.magnitudexz();
	Vec3 vel = Game.getLocalPlayer()->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1

	if (down) {
		Vec3 blockBelow = player->eyePos0;  // Block 1 block below the player
		blockBelow.y -= player->height;
		blockBelow.y -= 1.5f;

		Vec3 blockBelowBelow = player->eyePos0;  // Block 2 blocks below the player
		blockBelowBelow.y -= player->height;
		blockBelowBelow.y -= 2.0f;

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				blockBelowBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					blockBelowBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow) && player->isSprinting()) {
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
		Vec3 blockBelowReal = player->eyePos0;  // Block 1 block below the player
		blockBelowReal.y -= player->height;
		blockBelowReal.y -= 0.5f;
		Vec3 blockBelow = blockBelowReal;
		//Lock the Y coordinate
		if (Ylock) {
			blockBelow.y = YCoord;
			if (blockBelowReal.y < YCoord)
				YCoord = blockBelowReal.y;
		}

		//Extend
		blockBelow.x += vel.x * extend;
		blockBelow.z += vel.z * extend;

		Vec3 nextBlock = blockBelow;
		if (abs(vel.x) > abs(vel.z)) nextBlock.x += (vel.x > 0 ? 1 : (vel.x < 0 ? -1 : 0)); else nextBlock.z += (vel.z > 0 ? 1 : (vel.z < 0 ? -1 : 0));

		if (player->region->getBlock(Vec3i(blockBelow.floor()))->blockLegacy->material->isReplaceable) {
			tryClutchScaffold(blockBelow);
			if (hive) tryClutchScaffold(nextBlock);
		} else {
			if (!hive) {
				if (!tryScaffold(blockBelow)) {
					if (speed > 0.05f) {  // Are we actually walking?
						blockBelow.z -= vel.z * 0.4f;
						if (!tryScaffold(blockBelow)) {
							blockBelow.x -= vel.x * 0.4f;
							if (!tryScaffold(blockBelow) && player->isSprinting()) {
								blockBelow.z += vel.z;
								blockBelow.x += vel.x;
								tryScaffold(blockBelow);
							}
						}
					}
				}
			}
		}
	}
}

//Rotations with packets/serversided
void Scaffold::onSendPacket(Packet* packet) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (hive || rotations) {
		float speed = player->velocity.magnitudexz();
		Vec3 blockBelow = player->eyePos0;  // Block 1 block below the player
		blockBelow.y -= player->height;
		blockBelow.y -= 0.5f;

		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			if (speed > 0.05f) {
				auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
				movePacket->pitch = 83;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
		}
	}
}

// Rotations clientsided
void Scaffold::onPlayerTick(Player* player) {
	if (player == nullptr) return;
	if (hive || rotations) {
		float speed = player->velocity.magnitudexz();
		Vec3 blockBelow = player->eyePos0;  // Block 1 block below the player
		blockBelow.y -= player->height;
		blockBelow.y -= 0.5f;

		if (speed > 0.05f) {
			Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			player->pitch = angle.x;
			player->yawUnused1 = angle.y;
			player->bodyYaw = angle.y;
		}
	}
}