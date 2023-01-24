#include "TopCommand.h"

TopCommand::TopCommand() : IMCCommand("top", "Teleports to the nearest pocket of air above you - highly recommend against using this inside the Nether.", "") {
}

TopCommand::~TopCommand() {
}

bool TopCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);

	LocalPlayer* player = Game.getLocalPlayer();
	Vec3 playerPos = player->eyePos0;
	bool groundAbove = false;  // Checking if ground above us.
	Vec3i blockPos;

	// Start the loop from the player's current y position instead of 0
	for (int y = (int)playerPos.y; y < 256; ++y) {
		if (player->region->getBlock({(int)playerPos.x, y, (int)playerPos.z})->toLegacy()->blockId != 0) {
			groundAbove = true;
			blockPos = {(int)playerPos.x, y, (int)playerPos.z};
			break;
		}
	}

	if (groundAbove) {
		for (int y(blockPos.y); y < 256; ++y) {  // This time we're going through loop again, but continuing where we left off to find open air pocket.
			if ((player->region->getBlock({blockPos.x, y, blockPos.z})->toLegacy()->blockId == 0) && (player->region->getBlock({blockPos.x, y + 1, blockPos.z})->toLegacy()->blockId == 0)) {
				player->setPos({(float)blockPos.x, y + 1.f, (float)blockPos.z});
				clientMessageF("Whoosh!");
				return true;
			}
		}
		clientMessageF("There is no open space above you!");
		return true;
	} else {
		clientMessageF("There are no blocks above you!");
		return true;
	}
}