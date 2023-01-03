﻿#include "Hooks.h"

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>  // perspective, translate, rotate
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>         // mat4
#include <glm/trigonometric.hpp>  // radians

#include "../SDK/Tag.h"
#include "../Horion/Menu/TabGui.h"

Hooks g_Hooks;
bool isTicked = false;
bool overrideStyledReturn = false;
TextHolder styledReturnText;
//#define TEST_DEBUG

void Hooks::Init() {
	logF("Setting up Hooks...");
	// clang-format off
	// Vtables
	{
		// GameMode::vtable //good
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 48 89 51 ? 48 C7 41 ? FF FF FF FF C7 41 ? FF FF FF FF ");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** gameModeVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (gameModeVtable == 0x0 || sigOffset == 0x0)
				logF("C_GameMode signature not working!!!");
			else {
				g_Hooks.GameMode_startDestroyBlockHook = std::make_unique<FuncHook>(gameModeVtable[1], Hooks::GameMode_startDestroyBlock);

				g_Hooks.GameMode_getPickRangeHook = std::make_unique<FuncHook>(gameModeVtable[10], Hooks::GameMode_getPickRange);

				g_Hooks.GameMode_attackHook = std::make_unique<FuncHook>(gameModeVtable[14], Hooks::GameMode_attack);
			}
		}
		
		// BlockLegacy::vtable // Not updated
		{
			intptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 06 EB 03 49 8B F4 4D 89 26");  // BlockLegacy constructor
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
            uintptr_t** blockLegacyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (blockLegacyVtable == 0x0 || sigOffset == 0x0)
				logF("C_BlockLegacy signature not working!!!");
			else {
			}
		}

		// LocalPlayer::vtable // Not updated
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 48 8B 89 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 10");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** localPlayerVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (localPlayerVtable == 0x0 || sigOffset == 0x0)
				logF("C_LocalPlayer signature not working!!!");
			else {
				g_Hooks.Actor_startSwimmingHook = std::make_unique<FuncHook>(localPlayerVtable[201], Hooks::Actor_startSwimming);

				g_Hooks.Actor_ascendLadderHook = std::make_unique<FuncHook>(localPlayerVtable[339], Hooks::Actor_ascendLadder);
				
				g_Hooks.Actor__setRotHook = std::make_unique<FuncHook>(localPlayerVtable[27], Hooks::Actor__setRot);

				g_Hooks.Actor_swingHook = std::make_unique<FuncHook>(localPlayerVtable[219], Hooks::Actor_swing);

				g_Hooks.JumpPowerHook = std::make_unique<FuncHook>(localPlayerVtable[345], Hooks::JumpPower); //jump from ground with movement proxy

				//g_Hooks.setPosHook = std::make_unique<FuncHook>(localPlayerVtable[19], Hooks::setPos);

				g_Hooks.Actor_baseTickHook = std::make_unique<FuncHook>(localPlayerVtable[49], Hooks::Actor_baseTick);

				g_Hooks.Mob__isImmobileHook = std::make_unique<FuncHook>(localPlayerVtable[91], Hooks::Mob__isImmobile);

				g_Hooks.Actor_isInWaterHook = std::make_unique<FuncHook>(localPlayerVtable[71], Hooks::Actor_isInWater);

				g_Hooks.Player_tickWorldHook = std::make_unique<FuncHook>(localPlayerVtable[364], Hooks::Player_tickWorld);

				//g_Hooks.Actor__isInvisibleHook = std::make_unique<FuncHook>(localPlayerVtable[59], Hooks::Actor__isInvisible);
			}
		}

		// MoveInputHandler::vtable //good
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 33 C9 48 89 4B ? 48 89 4B ? 48 89 4B ? 48 89 4B ?");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** moveInputVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (moveInputVtable == 0x0 || sigOffset == 0x0)
				logF("C_MoveInputHandler signature not working!!!");
			else {
				g_Hooks.MoveInputHandler_tickHook = std::make_unique<FuncHook>(moveInputVtable[1], Hooks::MoveInputHandler_tick);
			}
		}

		// PackAccessStrategy vtables for isTrusted
		{
			
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** directoryPackVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset +  7);

			{
				g_Hooks.DirectoryPackAccessStrategy__isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable[6], Hooks::DirectoryPackAccessStrategy__isTrusted);
			}

			uintptr_t sigOffset2 = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 4C 8D B1 ?? ?? ?? ?? 49 8B 46 08"); // Was 48 8D 05 ?? ?? ?? ?? 48 89 03 49 8D 57 in 1.18.2
			int offset2 = *reinterpret_cast<int*>(sigOffset2 + 3);
			uintptr_t** directoryPackVtable2 = reinterpret_cast<uintptr_t**>(sigOffset2 + offset2 +  7);

			{
				g_Hooks.ZipPackAccessStrategy__isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable2[6], Hooks::ReturnTrue);
			}
			g_Hooks.SkinRepository___checkSignatureFileInPack = std::make_unique<FuncHook>(FindSignature("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 79"), Hooks::ReturnTrue);
			
		}
	}

	// Signatures
	{
		// vtables better than sigs

		void* _sendChatMessage = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4C 8B EA 4C 8B F9 48 8B 49"));
		g_Hooks.ClientInstanceScreenModel_sendChatMessageHook = std::make_unique<FuncHook>(_sendChatMessage, Hooks::ClientInstanceScreenModel_sendChatMessage);

		void* _renderCtx = reinterpret_cast<void*>(FindSignature("48 8B ? 48 89 ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 4C 8B ? 48 89 ? ? ? 4C 8B"));
		g_Hooks.RenderTextHook = std::make_unique<FuncHook>(_renderCtx, Hooks::RenderText);
		g_Hooks.RenderTextHook->enableHook();

		void* setupRender = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 33 D2 41 B8"));
		g_Hooks.UIScene_setupAndRenderHook = std::make_unique<FuncHook>(setupRender, Hooks::UIScene_setupAndRender);

		void* render = reinterpret_cast<void*>(FindSignature("48 89 5C 24 18 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 B9 10 ? ? ?"));
		g_Hooks.UIScene_renderHook = std::make_unique<FuncHook>(render, Hooks::UIScene_render);

		void* fogColorFunc = reinterpret_cast<void*>(FindSignature("41 0F 10 08 48 8B C2 0F"));
		g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(fogColorFunc, Hooks::Dimension_getFogColor);

		/*void* testy = reinterpret_cast<void*>(FindSignature("48 8B 05 61 DF CD 02 48 85 C0 75 07"));
		g_Hooks.testyHook = std::make_unique<FuncHook>(testy, Hooks::testy);*/
		void* timeOfDay = reinterpret_cast<void*>(FindSignature("44 8B C2 B8 ? ? ? ? F7 EA"));
		g_Hooks.Dimension_getTimeOfDayHook = std::make_unique<FuncHook>(timeOfDay, Hooks::Dimension_getTimeOfDay);

		void* sunItensity = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC 30 48 8B B9 ? ? ? ? 49 8B D8"));
		g_Hooks.Dimension_getSunIntensityHook = std::make_unique<FuncHook>(sunItensity, Hooks::Dimension_getSunIntensity);

		void* chestTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 56 57 41 56 48 83 EC ? 48 83 79"));
		g_Hooks.ChestBlockActor_tickHook = std::make_unique<FuncHook>(chestTick, Hooks::ChestBlockActor_tick);

		void* lerpFunc = reinterpret_cast<void*>(FindSignature("8B 02 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? C3 CC CC CC CC CC 48 89 5C 24"));
		g_Hooks.Actor_lerpMotionHook = std::make_unique<FuncHook>(lerpFunc, Hooks::Actor_lerpMotion);

		void* getRenderLayer = reinterpret_cast<void*>(FindSignature("8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC F3 0F 10 81"));
		g_Hooks.BlockLegacy_getRenderLayerHook = std::make_unique<FuncHook>(getRenderLayer, Hooks::BlockLegacy_getRenderLayer);

		void* getLightEmission = reinterpret_cast<void*>(FindSignature("0F B6 81 ? ? ? ? 88 02 48 8B C2"));
		g_Hooks.BlockLegacy_getLightEmissionHook = std::make_unique<FuncHook>(getLightEmission, Hooks::BlockLegacy_getLightEmission);

		//void* ascendLadder = reinterpret_cast<void*>(FindSignature("C7 81 ? ? ? ? ? ? ? ? C3 CC CC CC CC CC C7 81 ? ? ? ? ? ? ? ? C3 CC CC CC CC CC C7 81"));
		//g_Hooks.Actor_ascendLadderHook = std::make_unique<FuncHook>(ascendLadder, Hooks::Actor_ascendLadder);

		void* getGameEdition = reinterpret_cast<void*>(FindSignature("8B 91 ?? ?? ?? ?? 85 D2 74 1C 83 EA 01"));
		g_Hooks.AppPlatform_getGameEditionHook = std::make_unique<FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);

		uintptr_t** packetSenderVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)g_Data.getClientInstance()->loopbackPacketSender);
		g_Hooks.LoopbackPacketSender_sendToServerHook = std::make_unique<FuncHook>(packetSenderVtable[2], Hooks::LoopbackPacketSender_sendToServer);

		void* getFov = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC ?? 0F 29 7C 24 ?? 44"));
		g_Hooks.LevelRendererPlayer_getFovHook = std::make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);

		void* tick_entityList = reinterpret_cast<void*>(FindSignature("48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D8 ?? ?? ?? ?? ?? ?? 48 99"));
		g_Hooks.MultiLevelPlayer_tickHook = std::make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);

		void* keyMouseFunc = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F0"));
		g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);

		void* renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 20 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B F8 48 8B DA"));
		g_Hooks.LevelRenderer_renderLevelHook = std::make_unique<FuncHook>(renderLevel, Hooks::LevelRenderer_renderLevel);

		void* playerCallBackHook = reinterpret_cast<void*>(FindSignature("F3 0F ?? ?? ?? ?? 00 00 ?? 0F ?? 00 F3 0F ?? ?? F3 0F ?? ?? 04"));
		g_Hooks.playerCallBack_Hook = std::make_unique<FuncHook>(playerCallBackHook, Hooks::playerCallBack);

		void* clickHook = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
		g_Hooks.ClickFuncHook = std::make_unique<FuncHook>(clickHook, Hooks::ClickFunc);

		void* chestScreenControllerTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B 17"));
		g_Hooks.ChestScreenController_tickHook = std::make_unique<FuncHook>(chestScreenControllerTick, Hooks::ChestScreenController_tick);

		void* autoComplete = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 41 8B D9"));
		g_Hooks.PleaseAutoCompleteHook = std::make_unique<FuncHook>(autoComplete, Hooks::PleaseAutoComplete);

		void* fullbright = reinterpret_cast<void*>(FindSignature("48 83 EC ? 80 B9 ? ? ? ? ? 48 8D 54 24 ? 48 8B 01 74 ? 41 B8 ? ? ? ? FF 50 ? 48 8B 10 48 85 D2 74 ? 48 8B 42 ? 48 8B 88 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 83 C4 ? C3 F3 0F 10 42 ? 48 83 C4 ? C3 41 B8 ? ? ? ? FF 50 ? 48 8B 10 48 85 D2 75 ? E8 ? ? ? ? CC E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC F3 0F 11 4C 24"));
		g_Hooks.GetGammaHook = std::make_unique<FuncHook>(fullbright, Hooks::GetGamma);

		void* RakNetInstance__tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D AC 24 20 FD FF FF 48 81 EC E0 03 00 00"));
		g_Hooks.RakNetInstance_tickHook = std::make_unique<FuncHook>(RakNetInstance__tick, Hooks::RakNetInstance_tick);

		void* ConnectionRequest__create = reinterpret_cast<void*>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D9 48 89 55 E8"));
		g_Hooks.ConnectionRequest_createHook = std::make_unique<FuncHook>(ConnectionRequest__create, Hooks::ConnectionRequest_create);

		void* _getSkinPack = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B EA 48 8B F1"));
		g_Hooks.SkinRepository___loadSkinPackHook = std::make_unique<FuncHook>(_getSkinPack, Hooks::SkinRepository___loadSkinPack);

		//void* _toStyledString = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 49 8B D8 48 8B F9"));
		//g_Hooks.toStyledStringHook = std::make_unique<FuncHook>(_toStyledString, Hooks::toStyledString);

		//void* InGamePlayScreen___renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 20 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 B4 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D8 4C"));
		//g_Hooks.InGamePlayScreen___renderLevelHook = std::make_unique<FuncHook>(InGamePlayScreen___renderLevel, Hooks::InGamePlayScreen___renderLevel);

#ifdef TEST_DEBUG
		void* addAction = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC 30 45 0F B6 F8 4C 8B F2 48 8B F1 48 8B 01 48 8B 88 ? ? ? ? 48 85 C9"));
		g_Hooks.InventoryTransactionManager__addActionHook = std::make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
#endif

		void* localPlayerUpdateFromCam = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 80"));//Broken
		g_Hooks.LocalPlayer__updateFromCameraHook = std::make_unique<FuncHook>(localPlayerUpdateFromCam, Hooks::LocalPlayer__updateFromCamera);

		//void* renderNameTags = reinterpret_cast<void*>(FindSignature("4C 8B DC 49 89 5B ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 41 0F 29 73 ? 41 0F 29 7B ? 45 0F 29 43 ? 48 8B 05"));//Broken
		//g_Hooks.LevelRendererPlayer__renderNameTagsHook = std::make_unique<FuncHook>(renderNameTags, Hooks::LevelRendererPlayer__renderNameTags);
		
		static constexpr auto counterStart = __COUNTER__ + 1;
		#define lambda_counter (__COUNTER__ - counterStart)

		void* levelRendererBobView = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 20 57 48 ?? ?? ?? ?? 00 00 0F ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ??"));

		static auto bobViewHookF = [](__int64 _this, glm::mat4& matrix, float lerpT){
			static auto origFunc = g_Hooks.lambdaHooks.at(lambda_counter)->GetFastcall<void, __int64, glm::mat4&, float>();
			
			static auto testMod = moduleMgr->getModule<ViewModel>();
			auto p = g_Data.getLocalPlayer();
			float degrees = fmodf(p->getPosOld()->lerp(p->getPos(), lerpT).x, 5) - 2.5f;
			degrees *= 180 / 2.5f;

			auto pos = g_Data.getClientInstance()->levelRenderer->getOrigin();
			
			glm::mat4 View = matrix;
			
			matrix = View;
			if (testMod->isEnabled()) {
				if (testMod->doTranslate)
					matrix = glm::translate<float>(matrix, glm::vec3(testMod->xTrans, testMod->yTrans, testMod->zTrans));

				if (testMod->doScale)
					matrix = glm::scale<float>(matrix, glm::vec3(testMod->xMod, testMod->yMod, testMod->zMod));
			}
			return origFunc(_this, matrix, lerpT);
		};
		
		std::shared_ptr<FuncHook> bobViewHook = std::make_shared<FuncHook>(levelRendererBobView, (decltype(&bobViewHookF.operator()))bobViewHookF);

		g_Hooks.lambdaHooks.push_back(bobViewHook);

		#undef lambda_counter
		
		

		logF("Hooks initialized");
	}

// clang-format on
}

void Hooks::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

void Hooks::Enable() {
	logF("Hooks enabled");
	MH_EnableHook(MH_ALL_HOOKS);
}

bool Hooks::playerCallBack(C_Player* lp, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.playerCallBack_Hook->GetFastcall<bool, C_Player*, __int64, __int64>();
	//if (lp == g_Data.getLocalPlayer())
		//moduleMgr->onPlayerTick(lp);
		if (g_Data.getLocalPlayer() != nullptr && lp == g_Data.getLocalPlayer()) {
			if (!g_Data.getLocalPlayer() || !g_Data.getLocalPlayer()->level || !*(&g_Data.getLocalPlayer()->region + 1))
				g_Hooks.entityList.clear();

			std::vector<EntityListPointerHolder> validEntities;
			for (const auto& ent : g_Hooks.entityList) {
				auto entity = ent.ent;
				MEMORY_BASIC_INFORMATION info;
				VirtualQuery(ent.ent, &info, sizeof(MEMORY_BASIC_INFORMATION));
				if (info.State & MEM_FREE) continue;
				if (info.State & MEM_RESERVE) continue;

				if (entity->isAlive() && entity != nullptr && (__int64)entity != 0xFFFFFFFFFFFFFCD7 && ent.ent != nullptr && *(__int64*)ent.ent != 0xFFFFFFFFFFFFFCD7 && *(__int64*)ent.ent > 0x6FF000000000 && *(__int64*)ent.ent < 0x800000000000 && *((int64_t*)ent.ent + 1) < 0x6FF000000000 && *(__int64*)ent.ent <= Utils::getBase() + 0x10000000)
					validEntities.push_back(ent);
			}
			g_Hooks.entityList.clear();
			g_Hooks.entityList = validEntities;
		}
	return oTick(lp, a2, a3);
}

void* Hooks::Player_tickWorld(C_Player* _this, __int64 unk) {
	static auto oTick = g_Hooks.Player_tickWorldHook->GetFastcall<void*, C_Player*, __int64>();
	auto o = oTick(_this, unk);

	if (_this == g_Data.getLocalPlayer()) {
		// TODO: refactor all modules to not use GameMode
		C_GameMode* gm = *reinterpret_cast<C_GameMode**>(reinterpret_cast<__int64>(_this) + 0x1238);
		GameData::updateGameData(gm);
		moduleMgr->onWorldTick(gm);
	}
	return o;
}

void Hooks::ClientInstanceScreenModel_sendChatMessage(void* _this, TextHolder* text) {
	static auto oSendMessage = g_Hooks.ClientInstanceScreenModel_sendChatMessageHook->GetFastcall<void, void*, TextHolder*>();


	if (text->getTextLength() > 0) {
		char* message = text->getText();

		if (*message == cmdMgr->prefix) {
			cmdMgr->execute(message);

			return;
		} else if (*message == '.') {
			// maybe the user forgot his prefix, give him some helpful advice
			static bool helpedUser = false;
			if (!helpedUser) {
				helpedUser = true;
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYour Horion prefix is: \"%s%c%s\"", RED, YELLOW, cmdMgr->prefix, RED);
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sEnter \"%s%cprefix .%s\" to reset your prefix", RED, YELLOW, cmdMgr->prefix, RED);
			}
		}
	}
	oSendMessage(_this, text);
}

void Hooks::Actor_baseTick(C_Entity* ent) {
	static auto oFunc = g_Hooks.Actor_baseTickHook->GetFastcall<void, C_Entity*>();
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (!player || !player->getlevel()) return oFunc(ent);

	static int tickCountThen = 0;
	int tickCountNow = *(int*)((__int64)player->getlevel() + 0x690);

	if (tickCountNow != tickCountThen) {
		g_Hooks.entityList.clear();
		tickCountThen = tickCountNow;
	}
	if (ent->isClientSide()) {
		EntityListPointerHolder e;
		e.addedTick = tickCountNow;
		e.ent = ent;

		bool found = false;
		for (const auto& entity : g_Hooks.entityList)
			if (entity.ent == ent && entity.addedTick == tickCountNow) {
				found = true;
				break;
			}

		if (!found)
			g_Hooks.entityList.push_back(e);
	}
	return oFunc(ent);
}

__int64 Hooks::UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext) {
	static auto oSetup = g_Hooks.UIScene_setupAndRenderHook->GetFastcall<__int64, C_UIScene*, __int64>();
	g_Hooks.shouldRender = false;
	
	return oSetup(uiscene, screencontext);
}

__int64 Hooks::UIScene_render(C_UIScene* uiscene, __int64 screencontext) {
	static auto oRender = g_Hooks.UIScene_renderHook->GetFastcall<__int64, C_UIScene*, __int64>();

	g_Hooks.shouldRender = false;

	TextHolder alloc{};
	uiscene->getScreenName(&alloc);

	if (alloc.getTextLength() < 100) {
		strcpy_s(g_Hooks.currentScreenName, alloc.getText());
	}
	
	if (!g_Hooks.shouldRender) {
		g_Hooks.shouldRender = (strcmp(alloc.getText(), "start_screen") == 0 || strcmp(alloc.getText(), "hud_screen") == 0);
	}
	alloc.alignedTextLength = 0;

	return oRender(uiscene, screencontext);
}

__int64 Hooks::RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx) {
	static auto oText = g_Hooks.RenderTextHook->GetFastcall<__int64, __int64, C_MinecraftUIRenderContext*>();

	C_GuiData* dat = g_Data.getClientInstance()->getGuiData();

	DrawUtils::setCtx(renderCtx, dat);
	{
		if (GameData::shouldHide() || !moduleMgr->isInitialized())
			return oText(a1, renderCtx);

		static auto hudModule = moduleMgr->getModule<HudModule>();
		static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

		HImGui.startFrame();

		g_Data.frameCount++;

		auto wid = g_Data.getClientInstance()->getGuiData()->windowSize;

		// Call PreRender() functions
		moduleMgr->onPreRender(renderCtx);
		DrawUtils::flush();

		__int64 retval = oText(a1, renderCtx);

		bool shouldPostRender = true;
		bool shouldRenderArrayList = true;
		bool shouldRenderTabGui = true;
		bool shouldRenderWatermark = true;

		static float rcolors[4];          // Rainbow color array RGBA
		static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules
		static float currColor[4];        // ArrayList colors

		// Rainbow color updates
		{
			Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
			disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
			disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
			disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
			disabledRcolors[3] = 1;
		}

		{
			// Main Menu
			std::string screenName(g_Hooks.currentScreenName);
			if (strcmp(screenName.c_str(), "start_screen") == 0) {
				// Draw BIG epic horion watermark
				/*{
					std::string text = "H O R I O N";
					vec2_t textPos = vec2_t(wid.x / 2.f - DrawUtils::getTextWidth(&text, 8.f) / 2.f, wid.y / 9.5f);
					vec4_t rectPos = vec4_t(textPos.x - 55.f, textPos.y - 15.f, textPos.x + DrawUtils::getTextWidth(&text, 8.f) + 55.f, textPos.y + 75.f);
					DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48, 1), 1.f);
					DrawUtils::drawRectangle(rectPos, rcolors, 1.f, 2.f);
					DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255, 1), 8.f);
				}*/

				// Draw Custom Geo Button
				if (g_Data.allowWIPFeatures() && g_Data.isInjectorConnectionActive()) {
					if (HImGui.Button("Load Script Folder", vec2_t(wid.x * (0.765f - 0.5f), wid.y * 0.92f), true)) {
						HorionDataPacket packet;
						packet.cmd = CMD_FOLDERCHOOSER;
						auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
						packet.data.swap(tmp);
						memset(packet.data.get(), 0, 300);
						strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a Script Folder\", \"filter\":\".js\"}");
						packet.dataArraySize = (int)strlen((char*)packet.data.get());
						packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
							if (pk->params[0] != 1) {  // Dialog Canceled, reset geo
								auto box = g_Data.addInfoBox("Scripting", "Invalid Folder");
								box->closeTimer = 1;
								return;
							}

							wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
							std::wstring jsonDataStr(jsonData);

							json parsed = json::parse(jsonDataStr);
							if (parsed["path"].is_string()) {
								auto box = g_Data.addInfoBox("Importing Script", "Please wait...");
								std::thread gamer([parsed, box]() {
									auto result = scriptMgr.importScriptFolder(parsed["path"].get<std::string>());
									if (result)
										box->fadeTarget = 0;
									else {
										box->message = "Script import error, \ncheck the console";
										box->closeTimer = 2;
									}
								});
								gamer.detach();
							}
						});

						g_Data.sendPacketToInjector(packet);
					}
					if (HImGui.Button("Custom Geometry", vec2_t(wid.x * 0.765f, wid.y * 0.92f), true)) {
						HorionDataPacket packet;
						packet.cmd = CMD_FILECHOOSER;
						auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
						packet.data.swap(tmp);
						memset(packet.data.get(), 0, 300);
						strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a 3d object\", \"filter\":\"Object Files (*.obj)|*.obj\"}");
						packet.dataArraySize = (int)strlen((char*)packet.data.get());
						packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
							if (pk->params[0] != 1 && std::get<0>(g_Data.getCustomGeoOverride())) {  // Dialog Canceled, reset geo
								auto box = g_Data.addInfoBox("Geometry reset", "Geometry override removed");
								box->closeTimer = 1;
								return;
							}

							wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
							std::wstring jsonDataStr(jsonData);

							json parsed = json::parse(jsonDataStr);
							if (parsed["path"].is_string()) {
								auto box = g_Data.addInfoBox("Importing Skin", "Please wait...");
								std::thread gamer([parsed, box]() {
									SkinUtil::importGeo(Utils::stringToWstring(parsed["path"].get<std::string>()));
									box->fadeTarget = 0;
								});
								gamer.detach();
							}
						});

						g_Data.sendPacketToInjector(packet);
					}
					if (HImGui.Button("Custom Texture", vec2_t(wid.x * 0.5f, wid.y * 0.92f), true)) {
						HorionDataPacket packet;
						packet.cmd = CMD_FILECHOOSER;
						auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[500]);
						packet.data.swap(tmp);
						memset(packet.data.get(), 0, 500);
						strcpy_s((char*)packet.data.get(), 400, "{\"title\": \"Select a raw image file\", \"filter\":\"Raw image files (*.data, *.raw)|*.data;*.raw\"}");
						packet.dataArraySize = (int)strlen((char*)packet.data.get());
						packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
							if (pk->params[0] != 1 && std::get<0>(g_Data.getCustomTextureOverride())) {  // Dialog Canceled, reset texture
								auto box = g_Data.addInfoBox("Texture reset", "Texture override removed");
								box->closeTimer = 1;
								return;
							}

							wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
							std::wstring jsonDataStr(jsonData);

							json parsed = json::parse(jsonDataStr);
							if (parsed["path"].is_string()) {
								auto box = g_Data.addInfoBox("Importing texture...", "");
								std::thread gamer([parsed, box]() {
									auto contents = Utils::readFileContents(Utils::stringToWstring(parsed["path"].get<std::string>()));
									if (contents.size() > 0) {
										auto texturePtr = std::shared_ptr<unsigned char[]>(new unsigned char[contents.size() + 1]);
										memcpy(texturePtr.get(), contents.c_str(), contents.size());
										texturePtr.get()[contents.size()] = 0;
										g_Data.setCustomTextureOverride(true, std::make_shared<std::tuple<std::shared_ptr<unsigned char[]>, size_t>>(texturePtr, contents.size()));
										box->title = "Success";
										box->closeTimer = 0.3f;
									} else {
										box->title = "Error!";
										box->message = "Could not read texture file (empty?)";
										box->closeTimer = 2.f;
									}
								});
								gamer.detach();
							}
						});

						g_Data.sendPacketToInjector(packet);
					}
				}
			} else {
				shouldRenderTabGui = hudModule->tabgui && hudModule->isEnabled();
				shouldRenderArrayList = hudModule->arraylist && hudModule->isEnabled();
				shouldRenderWatermark = hudModule->watermark && hudModule->isEnabled();
				static auto ClientThemes = moduleMgr->getModule<ClientTheme>();

				if (clickGuiModule->isEnabled()) {
					ClickGui::render();
					shouldPostRender = false;
					shouldRenderArrayList = false;
					shouldRenderTabGui = false;
					shouldRenderWatermark = false;
				}

				if (shouldRenderTabGui) TabGui::render();

				{
					// Display ArrayList on the Right?
					static constexpr bool isOnRightSide = true;

					float yOffset = 0;  // Offset of next Text
					vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
					vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
					vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();

					// Convert mousePos to visual Pos
					{
						mousePos = mousePos.div(windowSizeReal);
						mousePos = mousePos.mul(windowSize);
					}

					// Draw Horion logo
					if (shouldRenderWatermark) {
						constexpr float nameTextSize = 1.5f;
						constexpr float versionTextSize = 0.7f;
						static const float textHeight = (nameTextSize + versionTextSize * 0.7f /* We don't quite want the version string in its own line, just a bit below the name */) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
						constexpr float borderPadding = 1;
						constexpr float margin = 5;

						static std::string name = "Horion";
#ifdef _DEBUG
						static std::string version = "dev";
#elif defined _BETA
						static std::string version = "beta";
#else
						static std::string version = "public";
#endif

						float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
						float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, versionTextSize);
						vec4_t rect = vec4_t(
							windowSize.x - margin - fullTextLength - borderPadding * 2,
							windowSize.y - margin - textHeight,
							windowSize.x - margin + borderPadding,
							windowSize.y - margin);

						DrawUtils::drawRectangle(rect, MC_Color(rcolors), 1.f, 2.f);
						if (ClientThemes->Theme.selected == 1) {
							DrawUtils::fillRectangle(rect, MC_Color(13, 29, 48), 1.f);
						} else {
							DrawUtils::fillRectangle(rect, MC_Color(12, 12, 12), 1.f);
						}
						DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(rcolors), nameTextSize);
						DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.w - 7), &version, MC_Color(rcolors), versionTextSize);
					}

					// Draw ArrayList
					if (moduleMgr->isInitialized() && shouldRenderArrayList) {
						// Parameters
						float textSize = hudModule->scale;
						float textPadding = 1.0f * textSize;
						float textHeight = 10.0f * textSize;
						float smoothness = 2;

						struct IModuleContainer {
							// Struct used to Sort IModules in a std::set
							std::shared_ptr<IModule> backingModule;
							std::string moduleName;
							bool enabled;
							int keybind;
							float textWidth;
							vec2_t* pos;
							bool shouldRender = true;

							IModuleContainer(std::shared_ptr<IModule> mod) {
								const char* moduleNameChr = mod->getModuleName();
								this->enabled = mod->isEnabled();
								this->keybind = mod->getKeybind();
								this->backingModule = mod;
								this->pos = mod->getPos();

								if (keybind == 0x0)
									moduleName = moduleNameChr;
								else {
									char text[50];
									sprintf_s(text, 50, "%s%s", moduleNameChr, hudModule->keybinds ? std::string(" [" + std::string(Utils::getKeybindName(keybind)) + "]").c_str() : "");
									moduleName = text;
								}

								if (!this->enabled && *this->pos == vec2_t(0.f, 0.f))
									this->shouldRender = false;
								this->textWidth = DrawUtils::getTextWidth(&moduleName, hudModule->scale);
							}

							bool operator<(const IModuleContainer& other) const {
								/*if (enabled) {
							if (!other.enabled)  // We are enabled
								return true;
						} else if (other.enabled)  // They are enabled
							return false;*/

								if (this->textWidth == other.textWidth)
									return moduleName < other.moduleName;
								return this->textWidth > other.textWidth;
							}
						};

						// Mouse click detector
						static bool wasLeftMouseDown = GameData::isLeftClickDown();  // Last isDown value
						bool leftMouseDown = GameData::isLeftClickDown();            // current isDown value

						bool executeClick = leftMouseDown && leftMouseDown != wasLeftMouseDown;  // isDown == true AND (current state IS NOT last state)
						wasLeftMouseDown = leftMouseDown;                                        // Set last isDown value

						std::set<IModuleContainer> modContainerList;
						// Fill modContainerList with Modules
						{
							auto lock = moduleMgr->lockModuleList();
							std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();
							for (auto it : *moduleList) {
								if (it.get() != hudModule)
									modContainerList.emplace(IModuleContainer(it));
							}
						}

						int a = 0;
						int b = 0;
						int c = 0;

						// Loop through mods to display Labels
						for (std::set<IModuleContainer>::iterator it = modContainerList.begin(); it != modContainerList.end(); ++it) {
							if (!it->shouldRender)
								continue;

							std::string textStr = it->moduleName;
							float textWidth = it->textWidth;

							float xOffsetOri = windowSize.x - textWidth - (textPadding * 2);

							float xOffset = windowSize.x - it->pos->x;

							it->pos->x += smoothness;

							if (xOffset < xOffsetOri) {
								xOffset = xOffsetOri;
							}
							if (!it->enabled) {
								xOffset += it->pos->y;
								it->pos->y += smoothness;
							}
							if (xOffset >= windowSize.x && !it->enabled) {
								it->pos->x = 0.f;
								it->pos->y = 0.f;
							}

							vec2_t textPos = vec2_t(
								xOffset + textPadding,
								yOffset + textPadding);
							vec4_t rectPos = vec4_t(
								xOffset - 2,
								yOffset,
								isOnRightSide ? windowSize.x : textWidth + (textPadding * 2),
								yOffset + textPadding * 2 + textHeight);
							vec4_t leftRect = vec4_t(
								xOffset - 2,
								yOffset,
								xOffset - 1,
								yOffset + textPadding * 2 + textHeight);
							c++;
							b++;
							if (b < 20)
								a = moduleMgr->getEnabledModuleCount() * 2;
							else
								b = 0;
							currColor[3] = rcolors[3];
							Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
							currColor[0] += 1.f / a * c;
							Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);

							if (ClientThemes->Theme.selected == 1) {
								DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 1.f);
							} else {
								DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 1.f);
							}
							DrawUtils::fillRectangle(leftRect, MC_Color(currColor), 1.f);
							if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos) && hudModule->clickToggle) {
								vec4_t selectedRect = rectPos;
								selectedRect.x = leftRect.z;
								if (leftMouseDown) {
									DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f), 0.8f);
									if (executeClick)
										it->backingModule->toggle();
								} else
									DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f, 0.8f), 0.3f);
							}
							DrawUtils::drawText(textPos, &textStr, MC_Color(currColor), textSize);

							yOffset += textHeight + (textPadding * 2);
						}
						c = 0;
						modContainerList.clear();
					}
				}
			}
		}

		if (shouldPostRender) moduleMgr->onPostRender(renderCtx);
		HImGui.endFrame();
		DrawUtils::flush();

		// Draw Info / Alert Boxes
		{
			auto box = g_Data.getFreshInfoBox();
			if (box) {
				box->fade();
				if (box->fadeTarget == 1 && box->closeTimer <= 0 && box->closeTimer > -1)
					box->fadeTarget = 0;
				else if (box->closeTimer > 0 && box->fadeVal > 0.9f)
					box->closeTimer -= 1.f / 60;
				const float paddingHoriz = 40 * box->fadeVal;
				const float paddingVert = 10 * box->fadeVal;
				const float titleTextSize = box->fadeVal * 2;
				const float messageTextSize = box->fadeVal * 1;
				const float titleTextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * titleTextSize;

				int lines = 1;
				std::string substring = box->message;
				while (lines < 5) {
					auto brea = substring.find("\n");
					if (brea == std::string::npos || brea + 1 >= substring.size())
						break;
					substring = substring.substr(brea + 1);
					lines++;
				}
				if (box->message.size() == 0)
					lines = 0;

				const float messageHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * messageTextSize * lines;

				float titleWidth = DrawUtils::getTextWidth(&box->title, titleTextSize);
				float msgWidth = DrawUtils::getTextWidth(&box->message, messageTextSize);
				vec2_t centerPos(wid.x / 2.f, wid.y / 9.f);
				vec2_t textPos = vec2_t(wid.x / 2.f - titleWidth / 2.f, wid.y / 9.f);
				vec2_t msgPos = vec2_t(wid.x / 2.f - msgWidth / 2.f, textPos.y + titleTextHeight + paddingVert);
				vec4_t rectPos = vec4_t(
					centerPos.x - paddingHoriz - std::max(titleWidth, msgWidth) / 2,
					centerPos.y - paddingVert,
					centerPos.x + paddingHoriz + std::max(titleWidth, msgWidth) / 2,
					centerPos.y + paddingVert * 2 + titleTextHeight + messageHeight * lines);
				DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), box->fadeVal);
				DrawUtils::drawRectangle(rectPos, rcolors, box->fadeVal, 2.f);
				DrawUtils::drawText(textPos, &box->title, MC_Color(), titleTextSize, box->fadeVal);
				DrawUtils::drawText(msgPos, &box->message, MC_Color(), messageTextSize, box->fadeVal);
			}
		}
		DrawUtils::flush();

		return retval;
	}
}

float* Hooks::Dimension_getFogColor(__int64 _this, float* color, __int64 a3, float a4) {
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetFastcall<float*, __int64, float*, __int64, float>();

	static float rcolors[4];

	static auto nightMod = moduleMgr->getModule<NightMode>();
	if (nightMod->isEnabled()) {
		color[0] = 0.f;
		color[1] = 0.f;
		color[2] = 0.2f;
		color[3] = 1;
		return color;
	}

	static auto rainbowSkyMod = moduleMgr->getModule<RainbowSky>();
	if (rainbowSkyMod->isEnabled()) {
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.2f;
			rcolors[2] = 0.2f;
			rcolors[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);  // perfect code, dont question this

		rcolors[0] += 0.001f;
		if (rcolors[0] >= 1)
			rcolors[0] = 0;

		Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

		return rcolors;
	}
	return oGetFogColor(_this, color, a3, a4);
}

float Hooks::Dimension_getTimeOfDay(__int64 _this, int a2, float a3) {
	static auto oGetTimeOfDay = g_Hooks.Dimension_getTimeOfDayHook->GetFastcall<float, __int64, int, float>();

	static auto timeChange = moduleMgr->getModule<TimeChanger>();
	if (timeChange->isEnabled()) {
		return timeChange->modifier;
	}

	return oGetTimeOfDay(_this, a2, a3);
}

float Hooks::Dimension_getSunIntensity(__int64 a1, float a2, vec3_t* a3, float a4) {
	static auto oGetSunIntensity = g_Hooks.Dimension_getSunIntensityHook->GetFastcall<float, __int64, float, vec3_t*, float>();

	static auto nightMod = moduleMgr->getModule<NightMode>();
	if (nightMod->isEnabled()) {
		return -0.5f;
	}

	return oGetSunIntensity(a1, a2, a3, a4);
}

void Hooks::ChestBlockActor_tick(C_ChestBlockActor* _this, void* a) {
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetFastcall<void, C_ChestBlockActor*, void*>();
	oTick(_this, a);
	static auto* storageEspMod = moduleMgr->getModule<StorageESP>();
	if (_this != nullptr && storageEspMod->isEnabled())
		GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(C_Entity* _this, vec3_t motVec) {
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetFastcall<void, C_Entity*, vec3_t>();

	if (g_Data.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static auto noKnockbackmod = moduleMgr->getModule<Velocity>();
	if (noKnockbackmod->isEnabled()) {
		static void* networkSender = nullptr;
		if (!networkSender)
			networkSender = reinterpret_cast<void*>(9 + FindSignature("48 8B CB FF ?? ?? ?? ?? 00 C6 47 ?? 01 48 8B 5C 24"));

		if (networkSender == _ReturnAddress()) {
			motVec = _this->velocity.lerp(motVec, noKnockbackmod->xModifier, noKnockbackmod->yModifier, noKnockbackmod->xModifier);
		}
	}

	oLerp(_this, motVec);
}

int Hooks::AppPlatform_getGameEdition(__int64 _this) {
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetFastcall<signed int, __int64>();

	static auto mod = moduleMgr->getModule<EditionFaker>();
	if (mod->isEnabled()) {
		return mod->getFakedEditon();
	}

	return oGetEditon(_this);
}

void Hooks::PleaseAutoComplete(__int64 a1, __int64 a2, TextHolder* text, int a4) {
	static auto oAutoComplete = g_Hooks.PleaseAutoCompleteHook->GetFastcall<void, __int64, __int64, TextHolder*, int>();
	char* tx = text->getText();
	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		std::string search = tx + 1;                                              // Dont include the '.'
		std::transform(search.begin(), search.end(), search.begin(), ::tolower);  // make the search text lowercase

		struct LilPlump {
			std::string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;  // Should replace the current text in the box (autocomplete)

			bool operator<(const LilPlump& o) const {
				return cmdAlias < o.cmdAlias;
			}
		};  // This is needed so the std::set sorts it alphabetically

		std::set<LilPlump> searchResults;

		std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
		for (auto it = commandList->begin(); it != commandList->end(); ++it) {  // Loop through commands
			IMCCommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (auto it = aliasList->begin(); it != aliasList->end(); ++it) {  // Loop through aliases
				std::string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) {  // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					} else if (i >= cmd.size())
						goto outerContinue;

					if (car != cmd.at(i))  // and compare
						goto outerContinue;
				}
				// Not at outerContinue? Then we got a good result!
				{
					cmd.insert(0, 1, '.');  // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			outerContinue:
				continue;
			}
		}

		if (!searchResults.empty()) {
			LilPlump firstResult = *searchResults.begin();

			size_t maxReplaceLength = firstResult.cmdAlias.size();
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin()++; it != searchResults.end(); it++) {
					auto alias = it->cmdAlias;
					maxReplaceLength = std::min(maxReplaceLength, alias.size());

					for (int i = 0; i < maxReplaceLength; i++) {
						if (alias[i] != firstResult.cmdAlias[i]) {
							maxReplaceLength = i;
							break;
						}
					}
				}
			} else
				maxReplaceLength = firstResult.cmdAlias.size();

			g_Data.getGuiData()->displayClientMessageF("==========");
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			if (firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1)[0] == 0)
				g_Data.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
			else
				g_Data.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1 /*exclude prefix*/), GRAY, firstResult.command->getDescription());

			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1) {
					maxReplaceLength++;
					firstResult.cmdAlias.append(" ");
				}
				text->setText(firstResult.cmdAlias.substr(0, maxReplaceLength));  // Set text
				using syncShit = void(__fastcall*)(TextHolder*, TextHolder*);
				static syncShit sync = reinterpret_cast<syncShit>(Utils::FindSignature("40 53 48 83 EC ? 48 8B DA 48 8D 4C 24 ? E8 ? ? ? ? 90 48 8B 40 ? 48 8B 08 48 8B 01 48 8B D3 FF 90 ? ? ? ? 90 F0 48 FF 0D ? ? ? ? 48 8B 44 24 ? 48 85 C0 74 ? 48 83 38 ? 74 ? 80 7C 24 ? ? 74 ? F0 48 FF 0D ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? 80 7C 24 ? ? 74 ? E8 ? ? ? ? C6 44 24 ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 80 7C 24 ? ? 74 ? 48 8B 4C 24 ? E8 ? ? ? ? 90 48 83 C4 ? 5B C3 B9 ? ? ? ? E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53"));
				sync(text, text);
			}
		}
		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet) {
	static auto oFunc = g_Hooks.LoopbackPacketSender_sendToServerHook->GetFastcall<void, C_LoopbackPacketSender*, C_Packet*>();

	static auto autoSneakMod = moduleMgr->getModule<AutoSneak>();
	static auto freecamMod = moduleMgr->getModule<Freecam>();
	static auto blinkMod = moduleMgr->getModule<Blink>();
	static auto noPacketMod = moduleMgr->getModule<NoPacket>();

	if (noPacketMod->isEnabled() && g_Data.isInGame())
		return;

	if (freecamMod->isEnabled() || blinkMod->isEnabled()) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
			if (blinkMod->isEnabled()) {
				if (packet->isInstanceOf<C_MovePlayerPacket>()) {
					C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
					meme->onGround = true;                                                            //Don't take Fall Damages when turned off
					blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*meme));  // Saving the packets
				} else {
					blinkMod->getPlayerAuthInputPacketHolder()->push_back(new PlayerAuthInputPacket(*reinterpret_cast<PlayerAuthInputPacket*>(packet)));
				}
			}
			return;  // Dont call LoopbackPacketSender_sendToServer
		}
	} else if (!blinkMod->isEnabled()) {
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getPlayerAuthInputPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}
	}

	if (autoSneakMod->isEnabled() && g_Data.getLocalPlayer() != nullptr && autoSneakMod->doSilent && packet->isInstanceOf<C_PlayerActionPacket>()) {
		auto* pp = reinterpret_cast<C_PlayerActionPacket*>(packet);

		if (pp->action == 12 && pp->entityRuntimeId == g_Data.getLocalPlayer()->entityRuntimeId)
			return;  //dont send uncrouch
	}

	moduleMgr->onSendPacket(packet);

	/*if (strcmp(packet->getName()->getText(), "EmotePacket") == 0) {
		auto varInt = reinterpret_cast<__int64*>(reinterpret_cast<__int64>(packet) + 0x28);
		auto text = reinterpret_cast<TextHolder*>(reinterpret_cast<__int64>(packet) + 0x30);
		auto bet = reinterpret_cast<unsigned char*>(reinterpret_cast<__int64>(packet) + 0x50);
		logF("emote %llX %s %i", *varInt, text->getText(), *bet);
	} fix emote crashing*/ 

	oFunc(a, packet);
}

float Hooks::LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3) {
	static auto oGetFov = g_Hooks.LevelRendererPlayer_getFovHook->GetFastcall<float, __int64, float, bool>();
	return oGetFov(_this, a2, a3);
}

void Hooks::MultiLevelPlayer_tick(C_EntityList* _this) {
	static auto oTick = g_Hooks.MultiLevelPlayer_tickHook->GetFastcall<void, C_EntityList*>();
	C_GameMode* gm = g_Data.getCGameMode();
	if (gm != nullptr) moduleMgr->onTick(gm);
	oTick(_this);
	GameData::EntityList_tick(_this);
}

void Hooks::GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5) {
	static auto oFunc = g_Hooks.GameMode_startDestroyBlockHook->GetFastcall<void, C_GameMode*, vec3_ti*, uint8_t, void*, void*>();

	static auto nukerModule = moduleMgr->getModule<Nuker>();
	static auto instaBreakModule = moduleMgr->getModule<InstaBreak>();

	if (nukerModule->isEnabled()) {
		vec3_ti tempPos;

		int range = nukerModule->getNukerRadius();
		const bool isVeinMiner = nukerModule->isVeinMiner();
		const bool isAutoMode = nukerModule->isAutoMode();

		C_BlockSource* region = g_Data.getLocalPlayer()->region;
		auto selectedBlockId = ((region->getBlock(*a2)->blockLegacy))->blockId;
		uint8_t selectedBlockData = region->getBlock(*a2)->data;

		if (!isAutoMode) {
			for (int x = -range; x < range; x++) {
				for (int y = -range; y < range; y++) {
					for (int z = -range; z < range; z++) {
						tempPos.x = a2->x + x;
						tempPos.y = a2->y + y;
						tempPos.z = a2->z + z;
						if (tempPos.y > 0) {
							C_Block* blok = region->getBlock(tempPos);
							uint8_t data = blok->data;
							auto id = blok->blockLegacy->blockId;
							if (blok->blockLegacy->material->isSolid == true && (!isVeinMiner || (id == selectedBlockId && data == selectedBlockData)))
								_this->destroyBlock(&tempPos, face);
						}
					}
				}
			}
		}
		return;
	}
	if (instaBreakModule->isEnabled()) {
		_this->destroyBlock(a2, face);
		return;
	}

	oFunc(_this, a2, face, a4, a5);
}

void Hooks::HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3) {
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, C_HIDController*, void*, void*>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

int Hooks::BlockLegacy_getRenderLayer(C_BlockLegacy* a1) {
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetFastcall<int, C_BlockLegacy*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		char* text = a1->name.getText();
		if (strstr(text, "ore") == NULL)
			if (strcmp(text, "lava") != NULL)
				if (strcmp(text, "water") != NULL)
					if (strcmp(text, "portal") != NULL)
						if (strcmp(text, "amethyst_block") != NULL)
							if (strcmp(text, "ancient_debris") != NULL)
								if (strcmp(text, "command_block") != NULL)
									if (strcmp(text, "repeating_command_block") != NULL)
										if (strcmp(text, "chain_command_block") != NULL)
											if (strcmp(text, "structure_block") != NULL)
												if (strcmp(text, "deny") != NULL)
													if (strcmp(text, "allow") != NULL)
														if (strcmp(text, "bedrock") != NULL)
															if (strcmp(text, "border_block") != NULL)
																return 10;
	}
	return oFunc(a1);
}

__int8* Hooks::BlockLegacy_getLightEmission(C_BlockLegacy* a1, __int8* a2) {
	static auto oFunc = g_Hooks.BlockLegacy_getLightEmissionHook->GetFastcall<__int8*, C_BlockLegacy*, __int8*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		*a2 = 15;
		return a2;
	}
	return oFunc(a1, a2);
}

__int64 Hooks::LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3) {
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();

	using reloadShit_t = void(__fastcall*)(__int64);
	static reloadShit_t reloadChunk = nullptr;

	if (!reloadChunk) {
		// RenderChunkCoordinator::rebuildAllRenderChunkGeometry
		reloadChunk = reinterpret_cast<reloadShit_t>(FindSignature("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 48 8D A9"));
	}

	static auto xrayMod = moduleMgr->getModule<Xray>();

	static bool lastState = false;
	if (lastState != xrayMod->isEnabled()) {
		// LevelRenderer::rebuildAllRenderChunkGeometry
		lastState = xrayMod->isEnabled();
		unsigned long long* v5;  // rdi
		unsigned long long* i;   // rbx

		v5 = *(unsigned long long**)(_this + 32);
		for (i = (unsigned long long*)*v5; i != v5; i = (unsigned long long*)*i)
			reloadChunk(i[3]);
	}

	auto ret = oFunc(_this, a2, a3);

	DrawUtils::setGameRenderContext(a2);
	moduleMgr->onLevelRender();
	DrawUtils::setGameRenderContext(0);

	return ret;
}

void Hooks::ClickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	static auto oFunc = g_Hooks.ClickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	//MouseButtons
	//0 = mouse move
	//1 = left click
	//2 = right click
	//3 = middle click
	//4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	ClickGui::onMouseClickUpdate((int)mouseButton, isDown);
	HImGui.onMouseClickUpdate((int)mouseButton, isDown);

	if (isDown)
		if (mouseButton == 1)
			g_Data.leftclickCount++;
		else if (mouseButton == 2)
			g_Data.rightclickCount++;

	if (clickGuiModule->isEnabled()) {
		if (mouseButton == 4) {
			ClickGui::onWheelScroll(isDown > 0);
		}
		if (mouseButton != 0)  // Mouse click event
			return;
	}
	return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

__int64 Hooks::MoveInputHandler_tick(C_MoveInputHandler* a1, C_Entity* a2) {
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetFastcall<__int64, C_MoveInputHandler*, C_Entity*>();

	auto ret = oTick(a1, a2);
	moduleMgr->onMove(a1);

	return 0;
}

__int64 Hooks::ChestScreenController_tick(C_ChestScreenController* a1) {
	static auto oFunc = g_Hooks.ChestScreenController_tickHook->GetFastcall<__int64, C_ChestScreenController*>();

	static auto chestStealerMod = moduleMgr->getModule<ChestStealer>();
	if (chestStealerMod->isEnabled()) chestStealerMod->chestScreenController_tick(a1);

	return oFunc(a1);
}

float Hooks::GetGamma(uintptr_t* a1) {
	static auto fullbright = moduleMgr->getModule<FullBright>();
	static auto xrayMod = moduleMgr->getModule<Xray>();
	static auto nametagmod = moduleMgr->getModule<NameTags>();
	static auto zoomMod = moduleMgr->getModule<Zoom>();

	uintptr_t** list = (uintptr_t**)a1;

	char obtainedSettings = 0;
	bool hadIt = false;
	for (uint16_t i = 3; i < 450; i++) {
		if (list[i] == nullptr) continue;
		uintptr_t* info = *(uintptr_t**)((uintptr_t)list[i] + 8);
		if (info == nullptr) continue;

		TextHolder* translateName = (TextHolder*)((uintptr_t)info + 0x1D8);
		TextHolder* settingname = (TextHolder*)((uintptr_t)info + 0x188);

		if (!strcmp(translateName->getText(), "options.smoothlighting")) {
			if (hadIt) continue;
			bool* smoothlightning = (bool*)((uintptr_t)list[i] + 16);
			xrayMod->smoothLightningSetting = smoothlightning;
			obtainedSettings++;
			hadIt = true;
		} else if (!strcmp(settingname->getText(), "gfx_ingame_player_names")) {
			bool* ingamePlayerName = (bool*)((uintptr_t)list[i] + 16);
			nametagmod->ingameNametagSetting = ingamePlayerName;
			obtainedSettings++;
		} else if (!strcmp(settingname->getText(), "gfx_field_of_view")) {
			float* FieldOfView = (float*)((uintptr_t)list[i] + 24);
			if (zoomMod->isEnabled())
				zoomMod->OGFov = *FieldOfView;
			// Zoom calc
			{
				static auto zoomModule = moduleMgr->getModule<Zoom>();
				if (zoomModule->isEnabled()) zoomModule->target = zoomModule->strength;
				zoomModule->modifier = zoomModule->target - ((zoomModule->target - zoomModule->modifier) * 0.8f);
				if (abs(zoomModule->modifier - zoomModule->target) < 0.1f && !zoomModule->isEnabled())
					zoomModule->zooming = false;
			}
			obtainedSettings++;
		}
		if (obtainedSettings == 3) break;
	}

	if (xrayMod->isEnabled())
		return 25.f;

	if (fullbright->isEnabled())
		return fullbright->intensity;

	static auto ofunc = g_Hooks.GetGammaHook->GetFastcall<float, uintptr_t*>();
	return ofunc(a1);
}

bool Hooks::Actor_isInWater(C_Entity* _this) {
	static auto oFunc = g_Hooks.Actor_isInWaterHook->GetFastcall<bool, C_Entity*>();

	if (g_Data.getLocalPlayer() != _this)
		return oFunc(_this);

	static auto airSwimModule = moduleMgr->getModule<AirSwim>();
	if (airSwimModule->isEnabled())
		return true;

	return oFunc(_this);
}

void Hooks::JumpPower(C_Entity* a1, float a2) {
	static auto oFunc = g_Hooks.JumpPowerHook->GetFastcall<void, C_Entity*, float>();
	static auto highJumpMod = moduleMgr->getModule<HighJump>();
	if (highJumpMod->isEnabled() && g_Data.getLocalPlayer() == a1) {
		a1->velocity.y = highJumpMod->jumpPower;
		return;
	}
	oFunc(a1, a2);
}


void Hooks::Actor_ascendLadder(C_Entity* _this) {
	static auto oFunc = g_Hooks.Actor_ascendLadderHook->GetFastcall<void, C_Entity*>();

	static auto fastLadderModule = moduleMgr->getModule<FastLadder>();
	if (fastLadderModule->isEnabled() && g_Data.getLocalPlayer() == _this) {
		_this->velocity.y = fastLadderModule->speed;
		return;
	}
	return oFunc(_this);
}

void Hooks::Actor_swing(C_Entity* _this) {
	static auto oFunc = g_Hooks.Actor_swingHook->GetFastcall<void, C_Entity*>();
	static auto noSwingMod = moduleMgr->getModule<NoSwing>();
	if(!noSwingMod->isEnabled()) return oFunc(_this);
}

void Hooks::Actor_startSwimming(C_Entity* _this) {
	static auto oFunc = g_Hooks.Actor_startSwimmingHook->GetFastcall<void, C_Entity*>();

	static auto jesusModule = moduleMgr->getModule<Jesus>();
	if (jesusModule->isEnabled() && g_Data.getLocalPlayer() == _this)
		return;

	oFunc(_this);
}

void Hooks::RakNetInstance_tick(C_RakNetInstance* _this, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.RakNetInstance_tickHook->GetFastcall<void, C_RakNetInstance*, __int64, __int64>();
	GameData::setRakNetInstance(_this);
	oTick(_this, a2, a3);
}

float Hooks::GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3) {
	static auto oFunc = g_Hooks.GameMode_getPickRangeHook->GetFastcall<float, C_GameMode*, __int64, char>();

	if (g_Data.getLocalPlayer() != nullptr) {
		static auto BlockReachModule = moduleMgr->getModule<BlockReach>();
		if (BlockReachModule->isEnabled())
			return BlockReachModule->getBlockReach();

		static auto teleportModule = moduleMgr->getModule<Teleport>();
		if (teleportModule->isEnabled())
			return 255;
	}

	return oFunc(_this, a2, a3);
}

__int64 Hooks::ConnectionRequest_create(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId) {
	static auto oFunc = g_Hooks.ConnectionRequest_createHook->GetFastcall<__int64, __int64, __int64, void*, TextHolder*, TextHolder*, __int64, TextHolder*, SkinData*, __int64, CoolSkinData*, TextHolder*, int, int, int, TextHolder*, bool, TextHolder*, __int64, TextHolder*, TextHolder*, bool, TextHolder*, TextHolder*, TextHolder*>();

	auto geoOverride = g_Data.getCustomGeoOverride();

	if (g_Data.allowWIPFeatures()) {
		logF("Connection Request: InputMode: %i UiProfile: %i GuiScale: %i", inputMode, uiProfile, guiScale);

		//Logger::WriteBigLogFileF(skinGeometryData->getTextLength() + 20, "Geometry: %s", skinGeometryData->getText());
		auto hResourceGeometry = FindResourceA((HMODULE)g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
		auto hMemoryGeometry = LoadResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);

		auto sizeGeometry = SizeofResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);
		auto ptrGeometry = LockResource(hMemoryGeometry);

		auto hResourceSteve = FindResourceA((HMODULE)g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_STEVE), (char*)RT_RCDATA);
		auto hMemorySteve = LoadResource((HMODULE)g_Data.getDllModule(), hResourceSteve);

		auto sizeSteve = SizeofResource((HMODULE)g_Data.getDllModule(), hResourceSteve);
		auto ptrSteve = LockResource(hMemorySteve);

		//std::unique_ptr<TextHolder> newGeometryData(new TextHolder(ptrGeometry, sizeGeometry));
		TextHolder* newGeometryData = nullptr;

		if (std::get<0>(geoOverride)) {  // Is overriding geometry
			auto overrideGeo = std::get<1>(geoOverride);
			newGeometryData = new TextHolder(*overrideGeo.get());
		} else {  // Default Skin
				  /*char* str;  // Obj text
			{
				auto hResourceObj = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_OBJ), "TEXT");
				auto hMemoryObj = LoadResource(g_Data.getDllModule(), hResourceObj);

				auto sizeObj = SizeofResource(g_Data.getDllModule(), hResourceObj);
				auto ptrObj = LockResource(hMemoryObj);

				str = new char[sizeObj + 1];
				memset(str, 0, sizeObj + 1);
				memcpy(str, ptrObj, sizeObj);
			}

			newGeometryData = new TextHolder(SkinUtil::modGeometry(reinterpret_cast<char*>(ptrGeometry), SkinUtil::objToMesh(str)));*/
		}

		SkinData* newSkinData = new SkinData();
		newSkinData->SkinWidth = 128;
		newSkinData->SkinHeight = 128;
		newSkinData->skinData = ptrSteve;
		newSkinData->skinSize = sizeSteve;

		auto texOverride = g_Data.getCustomTextureOverride();
		auto texture = std::get<1>(texOverride);  // Put it here so it won't go out of scope until after it has been used
		if (std::get<0>(texOverride)) {           // Enabled
			newSkinData->skinData = std::get<0>(*texture.get()).get();
			newSkinData->skinSize = std::get<1>(*texture.get());
		}

		//Logger::WriteBigLogFileF(newGeometryData->getTextLength() + 20, "Geometry: %s", newGeometryData->getText());
		TextHolder* newSkinResourcePatch = new TextHolder(Utils::base64_decode("ewogICAiZ2VvbWV0cnkiIDogewogICAgICAiYW5pbWF0ZWRfZmFjZSIgOiAiZ2VvbWV0cnkuYW5pbWF0ZWRfZmFjZV9wZXJzb25hXzRjZGJiZmFjYTI0YTk2OGVfMF8wIiwKICAgICAgImRlZmF1bHQiIDogImdlb21ldHJ5LnBlcnNvbmFfNGNkYmJmYWNhMjRhOTY4ZV8wXzAiCiAgIH0KfQo="));

		TextHolder* fakeName = g_Data.getFakeName();
		TextHolder resourcePatchBackup;

		if (newGeometryData != nullptr) {
			memcpy(&resourcePatchBackup, &coolSkinStuff->skinResourcePatch, sizeof(TextHolder));
			memcpy(&coolSkinStuff->skinResourcePatch, newSkinResourcePatch, sizeof(TextHolder));
			styledReturnText = *newGeometryData;
			overrideStyledReturn = true;
		}

		//  newGeometryData == nullptr ? skinResourcePatch : newSkinResourcePatch, newGeometryData == nullptr ? skinGeometryData : newGeometryData, skinAnimationData, isPremiumSkin, isPersonaSkin,
		__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, (newGeometryData == nullptr && !std::get<0>(texOverride)) ? skinData : newSkinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);

		if (newGeometryData != nullptr) {
			memcpy(&coolSkinStuff->skinResourcePatch, &resourcePatchBackup, sizeof(TextHolder));
			resourcePatchBackup.resetWithoutDelete();
		}
		overrideStyledReturn = false;

		styledReturnText = TextHolder();

		if (hMemoryGeometry)
			FreeResource(hMemoryGeometry);
		if (hMemorySteve)
			FreeResource(hMemorySteve);

		if (newGeometryData)
			delete newGeometryData;
		delete newSkinData;
		delete newSkinResourcePatch;
		return res;
	} else {
		TextHolder* fakeName = g_Data.getFakeName();
		__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);
		return res;
	}
}

void Hooks::InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2) {
	static auto Func = g_Hooks.InventoryTransactionManager_addActionHook->GetFastcall<void, C_InventoryTransactionManager*, C_InventoryAction*>();
	Func(a1, a2);
}

bool Hooks::DirectoryPackAccessStrategy__isTrusted(__int64 _this) {
	static auto func = g_Hooks.DirectoryPackAccessStrategy__isTrustedHook->GetFastcall<bool, __int64>();

	static uintptr_t** directoryPackAccessStrategyVtable = 0;

	if (!directoryPackAccessStrategyVtable) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		directoryPackAccessStrategyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
	}

	if (*reinterpret_cast<uintptr_t*>(_this) == (uintptr_t)directoryPackAccessStrategyVtable)
		return true;

	return func(_this);
}

bool Hooks::ReturnTrue(__int64 _this) {
	return true;
}

__int64 Hooks::SkinRepository___loadSkinPack(__int64 _this, __int64 pack, __int64 a3) {
	static auto func = g_Hooks.SkinRepository___loadSkinPackHook->GetFastcall<__int64, __int64, __int64, __int64>();

	//auto res = (*(unsigned __int8 (**)(void))(**(__int64**)(pack + 8) + 48i64))();
	//logF("SkinRepository___loadSkinPack: origin %i, is Trusted: %i", *(int*)((*(__int64*)pack) + 888i64), res);
	*(int*)((*(__int64*)pack) + 888i64) = 2;  // Set pack origin to "2"

	return func(_this, pack, a3);
}

GamerTextHolder* Hooks::toStyledString(__int64 strIn, GamerTextHolder* strOut) {
	static auto func = g_Hooks.toStyledStringHook->GetFastcall<GamerTextHolder*, __int64, GamerTextHolder*>();

	static auto conRequest = reinterpret_cast<__int64>(g_Hooks.ConnectionRequest_createHook->funcPtr);
	if (reinterpret_cast<__int64>(_ReturnAddress()) > conRequest && reinterpret_cast<__int64>(_ReturnAddress()) < conRequest + 10000 && overrideStyledReturn) {
		// Return address is within boundaries of connection request function

		strOut->copyFrom(&styledReturnText);

		return strOut;
	}

	return func(strIn, strOut);
}

void prepCoolBean() {
	if (g_Data.getClientInstance() && g_Data.getClientInstance()->minecraftGame->getServerEntries() && *reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50)) {
		auto serverEntries = g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x48;

		struct ThirdPartyServer {
			TextHolder serverName;
			TextHolder uuid;
			TextHolder masterPlayerAccount;
			TextHolder serverName2;
			TextHolder lobbyDescription;
			TextHolder domain;            // contains the last two parts of the domain .hivebedrock.network .mineplex.com
			TextHolder pathToServerIcon;  // C:\Users\user\AppData\Local\Packages\Microsoft.MinecraftUWP_8wekyb3d8bbwe\LocalCache\minecraftpe\ContentCache\ThirdPartyServer\\<file hash>.jpg
			TextHolder serverAddress;
			int coolBoye;

			ThirdPartyServer() {
				memset(this, 0, sizeof(ThirdPartyServer));
			}
		};

		struct BeansEntry {
			BeansEntry* nextEntry;
			BeansEntry* prevEntry;
			TextHolder masterPlayer;
			float unk;
			char filler[0x3c];
			TextHolder masterPlayer2;
			TextHolder serverName;
			char filler2[8];
			std::shared_ptr<ThirdPartyServer>* start;  // array
			std::shared_ptr<ThirdPartyServer>* end;    // end of array
		};

		auto listEnd = *reinterpret_cast<BeansEntry**>(serverEntries);

		auto current = listEnd;
		int count = 0;
		while (listEnd != current->nextEntry) {
			current = current->nextEntry;
			count++;
		}
		if (count > 5)  // we already added a server
			goto end;

		// make new one
		BeansEntry* epic = new BeansEntry();
		epic->nextEntry = listEnd;
		epic->prevEntry = current;
		epic->masterPlayer.setText("");
		epic->unk = current->unk;
		memcpy(epic->filler, current->filler, sizeof(BeansEntry::filler));
		epic->masterPlayer2.setText("");
		epic->serverName.setText("Epic");
		memcpy(epic->filler2, current->filler2, sizeof(BeansEntry::filler2));

		auto cT = current->start[0].get();

		std::shared_ptr<ThirdPartyServer>* start = new std::shared_ptr<ThirdPartyServer>[1];

		{
			ThirdPartyServer* t = new ThirdPartyServer();

			t->coolBoye = cT->coolBoye;
			t->uuid.setText("");
			t->masterPlayerAccount = cT->masterPlayerAccount;
			t->lobbyDescription = cT->lobbyDescription;
			t->pathToServerIcon.setText("");
			t->serverName.setText("Horion Server");
			t->serverName2.setText("Horion Server");  // This is the one actually displayed
			t->domain.setText(".horionbeta.club");
			t->serverAddress.setText("mc.horionbeta.club");
			start[0] = std::shared_ptr<ThirdPartyServer>(t);
		}

		epic->start = start;
		epic->end = &start[1];

		current->nextEntry = epic;

		// increase count
		*reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50) += 1;
	end:;
	}
}

__int64 Hooks::prepFeaturedServers(__int64 a1) {
	static auto func = g_Hooks.prepFeaturedServersHook->GetFastcall<__int64, __int64>();
	auto ret = func(a1);

	if (g_Data.getClientInstance() == nullptr)
		return ret;

	if (g_Data.allowWIPFeatures())
		prepCoolBean();

	return ret;
}

__int64 Hooks::prepFeaturedServersFirstTime(__int64 a1, __int64 a2) {
	static auto func = g_Hooks.prepFeaturedServersFirstTimeHook->GetFastcall<__int64, __int64, __int64>();

	if (g_Data.allowWIPFeatures())
		prepCoolBean();

	auto ret = func(a1, a2);

	return ret;
}


__int64 Hooks::Cube__compile(__int64 a1, __int64 a2) {
	auto func = g_Hooks.cube__compileHook->GetFastcall<__int64, __int64, __int64>();

	auto ret = func(a1, a2);

	auto end = *reinterpret_cast<__int64*>(a1 + 0x38);
	auto it = *reinterpret_cast<__int64*>(a1 + 0x30);
	auto boi = it + 0x1C;
	while (it != end) {  // loop through PolygonQuad
		if (it != boi + 0x34) {
			auto iter2 = boi - 0xC;
			do {
				// PolygonQuad::compile
				float* floatyBoi = reinterpret_cast<float*>(iter2 - 16);
				logF("%.1f %.1f %.1f", floatyBoi[0], floatyBoi[1], floatyBoi[2]);
				iter2 += 0x14;
			} while (iter2 - 0x10 != boi + 0x34);
		}
		boi += 0x50;
		it += 0x50;
	}

	return ret;
}

__int64 Hooks::InGamePlayScreen___renderLevel(__int64 playScreen, __int64 a2, __int64 a3) {
	auto func = g_Hooks.InGamePlayScreen___renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();
	return func(playScreen, a2, a3);
}
__int64 Hooks::GameMode_attack(C_GameMode* _this, C_Entity* ent) {
	auto func = g_Hooks.GameMode_attackHook->GetFastcall<__int64, C_GameMode*, C_Entity*>();
	moduleMgr->onAttack(ent);
	return func(_this, ent);
}
void Hooks::LocalPlayer__updateFromCamera(__int64 a1, C_Camera* camera) {
	auto func = g_Hooks.LocalPlayer__updateFromCameraHook->GetFastcall<__int64, __int64, C_Camera*>();
	/* auto noHurtcamMod = moduleMgr->getModule<NoHurtcam>();
	if (noHurtcamMod->isEnabled() && g_Data.isInGame() && g_Data.getLocalPlayer()->isAlive()) {
		vec2_t rot;
		camera->getPlayerRotation(&rot);
		if (camera->facesPlayerFront) {
			rot.x *= -1;  // rotate back
			rot.y += 180;
			rot = rot.normAngles();
		}
		camera->setOrientationDeg(rot.x, rot.y, 0);
	}
	*/
	func(a1, camera);
}
bool Hooks::Mob__isImmobile(C_Entity* ent) {
	auto func = g_Hooks.Mob__isImmobileHook->GetFastcall<bool, C_Entity*>();

	static auto antiImmobileMod = moduleMgr->getModule<AntiImmobile>();
	if (antiImmobileMod->isEnabled() && ent == g_Data.getLocalPlayer())
		return false;

	return func(ent);
}

void Hooks::Actor__setRot(C_Entity* _this, vec2_t& angle) {
	auto func = g_Hooks.Actor__setRotHook->GetFastcall<void, C_Entity*, vec2_t&>();
	auto killauraMod = moduleMgr->getModule<Killaura>();
	auto freelookMod = moduleMgr->getModule<Freelook>();
	if (killauraMod->isEnabled() && !killauraMod->targetListEmpty && killauraMod->rotations && _this == g_Data.getLocalPlayer()) {
		func(_this, angle = killauraMod->angle);
	}
	if (freelookMod->isEnabled() && g_Data.getLocalPlayer() == _this) {
		func(_this, angle = freelookMod->oldPos);
	}
	func(_this, angle);
}

void Hooks::test(void* _this) {
	auto func = g_Hooks.testHook->GetFastcall<void, void*>();
	func(_this);
}

void Hooks::InventoryTransactionManager__addAction(C_InventoryTransactionManager* _this, C_InventoryAction& action) {
	auto func = g_Hooks.InventoryTransactionManager__addActionHook->GetFastcall<void, C_InventoryTransactionManager*, C_InventoryAction&>();

#ifdef TEST_DEBUG
	char* srcName = "none";
	if (action.sourceItem.item && *action.sourceItem.item)
		srcName = (*action.sourceItem.item)->name.getText();
	char* targetName = "none";
	if (action.targetItem.item && *action.targetItem.item)
		targetName = (*action.targetItem.item)->name.getText();
	logF("%i %i %i %s %s", action.type, action.slot, action.sourceType, srcName, targetName, action.sourceType);

	/*if(/*action.slot == 14 && action.sourceType == 124 && strcmp(targetName, "none") == 0 && *strcmp(srcName, "stone_shovel") == 0){
		std::string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.sourceItem.setUserData(std::move(Mojangson::parseTag(tag)));
	}
	if(/*action.slot == 2 && action.sourceType == 256 && strcmp(srcName, "none") == 0 &&* strcmp(targetName, "stone_shovel") == 0){
		std::string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.targetItem.setUserData(std::move(Mojangson::parseTag(tag)));
	}*/
#endif

	func(_this, action);
}

void Hooks::LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, TextHolder* a3, __int64 a4) {
	static auto func = g_Hooks.LevelRendererPlayer__renderNameTagsHook->GetFastcall<void, __int64, __int64, TextHolder*, __int64>();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (nameTagsMod->isEnabled() && nameTagsMod->nameTags.size() > 0) {
		std::string text = Utils::sanitize(a3->getText());
		std::size_t found = text.find('\n');

		if (found != std::string::npos)
			text = text.substr(0, found);

		if (nameTagsMod->nameTags.find(text) != nameTagsMod->nameTags.end())
			return;
	}

	return func(a1, a2, a3, a4);
}
