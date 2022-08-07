#include "GameData.h"

#include <Windows.h>

#include <set>

#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "Hooks.h"

GameData g_Data;

size_t AABBHasher::operator()(const AABB& i) const {
	return Utils::posToHash(i.lower);
}
void GameData::retrieveClientInstance() {
	static uintptr_t clientInstanceOffset = 0x0;
	uintptr_t sigOffset = 0x0;
	if (clientInstanceOffset == 0x0) {
		sigOffset = FindSignature("48 8B 15 ? ? ? ? 4C 8B 02 4C 89 06 40 84 FF 74 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B C6 48 8B 4C 24 ? 48 33 CC E8 ? ? ? ? 48 8B 5C 24 ? 48 8B 74 24 ? 48 83 C4 40 5F C3 E8 ? ? ? ? ? CC CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 6C 24 ? 56");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3));                                                 // Get Offset from code
			clientInstanceOffset = sigOffset - g_Data.gameModule->ptrBase + offset + /*length of instruction*/ 7;  // Offset is relative
			logF("client: %llX", clientInstanceOffset);
		}
	}
	// clientInstanceOffset = 0x03CD5058;  // pointer scanned, can't find good signatures so it'll stay
	g_Data.clientInstance = reinterpret_cast<C_ClientInstance*>(g_Data.slimMem->ReadPtr<uintptr_t*>(g_Data.gameModule->ptrBase + clientInstanceOffset, {0x0, 0x0, 0x58}));
#ifdef _DEBUG
	if (g_Data.clientInstance == 0)
		throw std::exception("Client Instance is 0");
#endif
}

bool GameData::canUseMoveKeys() {
	MinecraftGame* mc = g_Data.clientInstance->minecraftGame;
	if (mc == nullptr) {
		return false;
	}

	return mc->canUseKeybinds();
}

bool GameData::isKeyDown(int key) {
	static uintptr_t keyMapOffset = 0x0;
	if (keyMapOffset == 0x0) {
		uintptr_t sigOffset = FindSignature("4C 8D 05 ? ? ? ? 89 54 24 ? 88 4C 24");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3));                                         // Get Offset from code
			keyMapOffset = sigOffset - g_Data.gameModule->ptrBase + offset + /*length of instruction*/ 7;  // Offset is relative
			logF("KeyMap: %llX", keyMapOffset + g_Data.gameModule->ptrBase);
		}
	}
	// All keys are mapped as bools, though aligned as ints (4 byte)
	// key0 00 00 00 key1 00 00 00 key2 00 00 00 ...
	return *reinterpret_cast<bool*>(g_Data.gameModule->ptrBase + keyMapOffset + ((uintptr_t)key * 0x4));
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
	if (g_Data.hidController == 0)
		return false;
	return g_Data.hidController->rightClickDown;
}

bool GameData::isLeftClickDown() {
	if (g_Data.hidController == 0)
		return false;
	return g_Data.hidController->leftClickDown;
}

bool GameData::isWheelDown() {
	if (g_Data.hidController == 0)
		return false;
	return g_Data.hidController->wheelDown;
}

bool GameData::shouldTerminate() {
	return g_Data.shouldTerminateB;
}

void GameData::terminate() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
	g_Data.shouldTerminateB = true;
}

bool GameData::shouldHide() {
	return g_Data.shouldHideB;
}

void GameData::hide() {
	g_Data.shouldHideB = !(g_Data.shouldHideB);
}

void GameData::updateGameData(C_GameMode* gameMode) {
	retrieveClientInstance();
	g_Data.localPlayer = g_Data.getLocalPlayer();

	if (g_Data.localPlayer != nullptr && gameMode->player == g_Data.localPlayer) {  // GameMode::tick might also be run on the local server
		g_Data.gameMode = gameMode;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&g_Data.lastUpdate));

		if (g_Data.localPlayer != nullptr) {
			C_GuiData* guiData = g_Data.clientInstance->getGuiData();

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
					auto lock = std::lock_guard<std::mutex>(g_Data.textPrintLock);

					auto& stringPrintVector = g_Data.textPrintList;
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

void GameData::EntityList_tick(C_EntityList* list) {
	g_Data.entityList = list;
}

void GameData::setHIDController(C_HIDController* Hid) {
	g_Data.hidController = Hid;
}

void GameData::setRakNetInstance(C_RakNetInstance* raknet) {
	g_Data.raknetInstance = raknet;
}

void GameData::forEachEntity(std::function<void(C_Entity*, bool)> callback) {
	/*//Player EntityList
	C_EntityList* entityList = (C_EntityList*)g_Data.getLocalPlayer()->level;
	uintptr_t start = ((uintptr_t)entityList + 0x70);
	uintptr_t stop = ((uintptr_t)entityList + 0x78);
	start = *(uintptr_t*)start;
	stop = *(uintptr_t*)stop;
	//logF("size: %i", (stop - start) / sizeof(uintptr_t*));
	while (start < stop) {
		C_Entity* ent = *(C_Entity**)start;
		if (ent != nullptr)
			callback(ent, false);
		start += 8;
	}
	// New EntityList
	{
		// MultiplayerLevel::directTickEntities
		__int64 region = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->region);
		__int64* entityIdMap = *(__int64**)(*(__int64*)(region + 0x20) + 0x138i64);
		for (__int64* i = (__int64*)*entityIdMap; i != entityIdMap; i = (__int64*)*i) {
			__int64 actor = i[3];
			// !isRemoved() && !isGlobal()
			if (actor && !*(char*)(actor + 993) && !*(char*)(actor + 994)) {
				C_Entity* ent = reinterpret_cast<C_Entity*>(actor);
				callback(ent, false);
			}
		}
	}*/

	if (localPlayer && localPlayer->level) {
		for (const auto& ent : g_Hooks.entityList)
			if (ent.ent != nullptr) callback(ent.ent, false);
	}
}

void GameData::addChestToList(C_ChestBlockActor* chest) {
	if (chest == nullptr || !chest->isMainSubchest())
		return;
	AABB chestAabb = chest->getFullAABB();
	std::lock_guard<std::mutex> listGuard(g_Data.chestListMutex);
	if (g_Data.chestList.count(chestAabb) > 0)
		return;

	g_Data.chestList.insert(chestAabb);
}

void GameData::initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem, void* hDllInst) {
	g_Data.gameModule = gameModule;
	g_Data.slimMem = slimMem;
	g_Data.hDllInst = hDllInst;
	g_Data.networkedData.xorKey = rand() % 0xFFFF | ((rand() % 0xFFFF) << 16);
	retrieveClientInstance();
#ifdef _DEBUG
	logF("base: %llX", g_Data.getModule()->ptrBase);
	logF("clientInstance %llX", g_Data.clientInstance);
	logF("localPlayer %llX", g_Data.getLocalPlayer());
	if (g_Data.clientInstance != nullptr) {
		logF("minecraftGame: %llX", g_Data.clientInstance->minecraftGame);
		logF("levelRenderer: %llX", g_Data.clientInstance->levelRenderer);
	}

#endif
}
void GameData::sendPacketToInjector(HorionDataPacket horionDataPack) {
	if (!isInjectorConnectionActive())
		throw std::exception("Horion injector connection not active");
	if (horionDataPack.dataArraySize >= 3000) {
		logF("Tried to send data packet with array size: %i %llX", horionDataPack.dataArraySize, horionDataPack.data.get());
		throw std::exception("Data packet data too big");
	}

	horionToInjectorQueue.push(horionDataPack);
}
void GameData::callInjectorResponseCallback(int id, std::shared_ptr<HorionDataPacket> packet) {
	if (injectorToHorionResponseCallbacks.find(id) == injectorToHorionResponseCallbacks.end()) {
		logF("No response callback for request with id=%i!", id);
		return;
	}
	injectorToHorionResponseCallbacks[id](packet);
	injectorToHorionResponseCallbacks.erase(id);
}
void GameData::log(const char* fmt, ...) {
	auto lock = std::lock_guard<std::mutex>(g_Data.textPrintLock);
	va_list arg;
	va_start(arg, fmt);
	char message[300];
	vsprintf_s(message, 300, fmt, arg);
	std::string msg(message);
	g_Data.textPrintList.push_back(msg);
	va_end(arg);
}
