#include "GameData.h"

#include <Windows.h>

#include <set>

#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "Hooks.h"

GameData Game;
bool GameData::keys[0x256];

size_t AABBHasher::operator()(const AABB& i) const {
	return Utils::posToHash(i.lower);
}
void GameData::retrieveClientInstance() {
	static uintptr_t clientInstanceOffset = 0x0;
	uintptr_t sigOffset = 0x0;
	if (clientInstanceOffset == 0x0) {
		sigOffset = FindSignature("48 8B 15 ? ? ? ? 4C 8B 02 4C 89 06 40 84 FF 74 ? 48 8B CD E8 ? ? ? ? 48 8B C6 48 8B 4C 24 ? 48 33 CC E8 ? ? ? ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 ? 5F C3 B9 ? ? ? ? E8 ? ? ? ? CC E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 6C 24 ? 56");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3));                                                 // Get Offset from code
			clientInstanceOffset = sigOffset - Game.gameModule->ptrBase + offset + /*length of instruction*/ 7;  // Offset is relative
			logF("Client: %llX", clientInstanceOffset);
		}
	}
	// clientInstanceOffset = 0x03CD5058;  // pointer scanned, can't find good signatures so it'll stay
	Game.clientInstance = reinterpret_cast<ClientInstance*>(Game.slimMem->ReadPtr<uintptr_t*>(Game.gameModule->ptrBase + clientInstanceOffset, {0x0, 0x0, 0x58}));
#ifdef _DEBUG
	if (Game.clientInstance == 0)
		throw std::exception("Client Instance is 0");
#endif
}

bool GameData::canUseMoveKeys() {
	MinecraftGame* mc = Game.clientInstance->minecraftGame;
	if (mc == nullptr) {
		return false;
	}

	return mc->canUseKeybinds();
}

bool GameData::isKeyDown(int key) {
	return keys[(int)key];
}

bool GameData::isKeyPressed(int key) {
	if (isKeyDown(key)) {
		while (isKeyDown(key))
			Sleep(1);
		return true;
	}
	return false;
}

bool GameData::isRightClickDown() {
	if (Game.hidController == 0)
		return false;
	return Game.hidController->rightClickDown;
}

bool GameData::isLeftClickDown() {
	if (Game.hidController == 0)
		return false;
	return Game.hidController->leftClickDown;
}

bool GameData::isWheelDown() {
	if (Game.hidController == 0)
		return false;
	return Game.hidController->wheelDown;
}

bool GameData::shouldTerminate() {
	return Game.shouldTerminateB;
}

void GameData::terminate() {
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	Game.shouldTerminateB = true;
}

bool GameData::shouldHide() {
	return Game.shouldHideB;
}

void GameData::hide() {
	Game.shouldHideB = !(Game.shouldHideB);
}

void GameData::updateGameData(GameMode* gameMode) {
	retrieveClientInstance();
	Game.localPlayer = Game.getLocalPlayer();

	if (Game.localPlayer != nullptr && gameMode->player == Game.localPlayer) {  // GameMode::tick might also be run on the local server
		Game.gameMode = gameMode;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&Game.lastUpdate));

		if (Game.localPlayer != nullptr) {
			GuiData* guiData = Game.clientInstance->getGuiData();

			if (guiData != nullptr) {
				{
					auto vecLock = Logger::GetTextToPrintLock();
					auto* stringPrintVector = Logger::GetTextToPrint();
#ifdef _DEBUG
					int numPrinted = 0;
					std::vector<TextForPrint>::iterator it;
					for (it = stringPrintVector->begin(); it != stringPrintVector->end(); ++it) {
						numPrinted++;
						if (numPrinted > 20) {
							break;
						}

						guiData->displayClientMessageNoSendF("%s%s%s%s", GOLD, it->time, RESET, it->text);
					}
					stringPrintVector->erase(stringPrintVector->begin(), it);
#else
					stringPrintVector->clear();
#endif
				}
				{
					auto lock = std::lock_guard<std::mutex>(Game.textPrintLock);

					auto& stringPrintVector = Game.textPrintList;
					int numPrinted = 0;
					std::vector<std::string>::iterator it;
					for (it = stringPrintVector.begin(); it != stringPrintVector.end(); ++it) {
						numPrinted++;
						if (numPrinted > 20) {
							break;
						}

						guiData->displayClientMessageNoSendF(it->c_str());
					}
					stringPrintVector.erase(stringPrintVector.begin(), it);
				}
			}
		}
	}
}

void GameData::EntityList_tick(EntityList* list) {
	Game.entityList = list;
}

void GameData::setHIDController(HIDController* Hid) {
	Game.hidController = Hid;
}

void GameData::setRakNetInstance(RakNetInstance* raknet) {
	Game.raknetInstance = raknet;
}

void GameData::forEachEntity(std::function<void(Entity*, bool)> callback) {
	/*//Player EntityList
	EntityList* entityList = (EntityList*)Game.getLocalPlayer()->level;
	uintptr_t start = ((uintptr_t)entityList + 0x70);
	uintptr_t stop = ((uintptr_t)entityList + 0x78);
	start = *(uintptr_t*)start;
	stop = *(uintptr_t*)stop;
	//logF("size: %i", (stop - start) / sizeof(uintptr_t*));
	while (start < stop) {
		Entity* ent = *(Entity**)start;
		if (ent != nullptr)
			callback(ent, false);
		start += 8;
	}
	// New EntityList
	{
		// MultiplayerLevel::directTickEntities
		__int64 region = reinterpret_cast<__int64>(Game.getLocalPlayer()->region);
		__int64* entityIdMap = *(__int64**)(*(__int64*)(region + 0x20) + 0x138i64);
		for (__int64* i = (__int64*)*entityIdMap; i != entityIdMap; i = (__int64*)*i) {
			__int64 actor = i[3];
			// !isRemoved() && !isGlobal()
			if (actor && !*(char*)(actor + 993) && !*(char*)(actor + 994)) {
				Entity* ent = reinterpret_cast<Entity*>(actor);
				callback(ent, false);
			}
		}
	}*/

	if (localPlayer && localPlayer->level) {
		for (const auto& ent : g_Hooks.entityList)
			if (ent.ent != nullptr) callback(ent.ent, false);
	}
}

void GameData::addChestToList(ChestBlockActor* chest) {
	if (chest == nullptr || !chest->isMainSubchest())
		return;
	AABB chestAabb = chest->getFullAABB();
	std::lock_guard<std::mutex> listGuard(Game.chestListMutex);
	if (Game.chestList.count(chestAabb) > 0)
		return;

	Game.chestList.insert(chestAabb);
}

void GameData::initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem, void* hDllInst) {
	Game.gameModule = gameModule;
	Game.slimMem = slimMem;
	Game.hDllInst = hDllInst;
	retrieveClientInstance();
#ifdef _DEBUG
	logF("Base: %llX", Game.getModule()->ptrBase);
	if (Game.clientInstance != nullptr) {
		logF("ClientInstance: %llX", Game.clientInstance);
		logF("LocalPlayer: %llX", Game.getLocalPlayer());
		logF("MinecraftGame: %llX", Game.clientInstance->minecraftGame);
		logF("LevelRenderer: %llX", Game.clientInstance->levelRenderer);
	}

#endif
}
void GameData::log(const char* fmt, ...) {
	auto lock = std::lock_guard<std::mutex>(Game.textPrintLock);
	va_list arg;
	va_start(arg, fmt);
	char message[300];
	vsprintf_s(message, 300, fmt, arg);
	std::string msg(message);
	Game.textPrintList.push_back(msg);
	va_end(arg);
}