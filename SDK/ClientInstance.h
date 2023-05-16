#pragma once

#include "Entity.h"
#include "GameSettingsInput.h"
#include "LoopbackPacketSender.h"
#include "MinecraftUIRenderContext.h"
#include "TextHolder.h"

class Minecraft {
private:
	char pad_0x0000[0xD8];  //0x0000
public:
	float* timer;       //0x00D8
	float* otherTimer;  //0x00E0

	void setTimerSpeed(float tps) {
		*this->timer = tps;
		*this->otherTimer = tps;
	}
};

class Tessellator;
class Block;

class BlockTessellator;
class ResourceLocation {
private:
	char pad[0x28];
	__int64 hashCode;  // 0x28
	char pad2[8];
};

class HashedString {
private:
	unsigned __int64 hash;
	TextHolder text;  // 0x0008

public:
	HashedString(const std::string& text) {
		this->text.setText(text);

		this->computeHash();
	}

	void computeHash() {
		hash = 0xCBF29CE484222325i64;
		if (this->text.getTextLength() <= 0)
			return;
		char* textP = this->text.getText();
		auto c = *textP;

		do {
			hash = c ^ 0x100000001B3i64 * hash;
			c = *++textP;
		} while (*textP);
	}

	unsigned __int64 getHash() {
		return this->hash;
	}
};

namespace mce {
class TextureGroup;
class MaterialPtr;
class Mesh {
public:
	void renderMesh(__int64 screenContext, mce::MaterialPtr* material, size_t numTextures, __int64** textureArray);

	template <size_t numTextures>
	void renderMesh(__int64 screenContext, mce::MaterialPtr* material, std::array<__int64*, numTextures> textures) {
		this->renderMesh(screenContext, material, numTextures, &textures[0]);
	}
};
class TexturePtr {
private:
	__int64* clientTexture;
	char pad[0x8];
	ResourceLocation resourceLocation;  // 0x10

public:
	__int64* getClientTexture() {
		return this->clientTexture;
	}
};
class MaterialPtr {
private:
	std::shared_ptr<void> materialPtr;

public:
	MaterialPtr(const std::string& materialName);
};
}  // namespace mce

class LevelRenderer {
private:
	char pad_0x0000[0x58];  //0x0000
public:
	mce::TextureGroup* textureGroup;  // 0x0058
private:
	char pad_0x0060[0xE0];  //0x0060
public:
	mce::TexturePtr atlasTexture;  // 0x140
private:
	char pad_0x0188[0x150];  //0x0188
public:
	BlockTessellator* blockTessellator;  // 0x02D8
private:
	char pad_0x02F0[0x568];  //0x02E0
public:
	Vec3& getOrigin() {
		return *(Vec3*)((char*)this + (0x7A8));
	}

	__int64 getLevelRendererPlayer() {
		return reinterpret_cast<__int64>(this) + 0x310;
	}
};
class HitDetectSystem;

struct FontRepository_FontList_FontEntry {
public:
	Font* font;

private:
	void* sharedFontPtr;
};

struct FontRepository_FontList {
public:
	FontRepository_FontList_FontEntry fontEntries[9];
};

struct FontRepository {
private:
	uintptr_t* font_repository_vtable;  // 0x0000
	__int64 pad;                        // 0x0008
	void* ptrToSelf;                    // 0x0010
	void* ptrToSelfSharedPtr;           // 0x0018
public:
	FontRepository_FontList* fontList;  // 0x0028
										// FontRepository_FontList* fontList1;  //0x0040
};

class MinecraftGame {
	char pad_0000[0x110];  // 0x0000
public:
	FontRepository* fontRepository;  // 0x00110
private:
	char pad_0100[0x8];  // 0x00118
public:
	Font* mcFont;  // 0x00120
private:
	char pad_0128[0x18];  // 0x00128
public:
	FontRepository* fontRepository1;  // 0x0140
private:
	char pad_0148[0x1D0];  // 0x0148
public:
	bool canUseKeys;  // 0x0318
private:
	char pad_0301[799];  // 0x0319
public:
	TextHolder xuid;  // 0x0638

	// 0x2F0

	Font* getTheGoodFontThankYou() {
		return fontRepository->fontList->fontEntries[7].font;
	};

	Font* getTheRandomFontPlease() {
		return fontRepository->fontList->fontEntries[4].font;
	};

	Font* getOldFont() {
		return fontRepository->fontList->fontEntries[7].font;
	};

	Font* getTheBetterFontYes() {
		return fontRepository->fontList->fontEntries[3].font;
	}

	Font* getMCFont() {
		return mcFont;
	}

	const bool canUseKeybinds() {
		return canUseKeys;
	};

	__int64 getServerEntries();
};

class GuiData {
private:
	char pad_0x0000[0x30];  //0x0000
public:
	union {
		struct {
			float widthReal;   //0x0018
			float heightReal;  //0x001C
		};
		Vec2 windowSizeReal;  //0x0018
	};

	float widthReal2;   //0x0020
	float heightReal2;  //0x0024
	union {
		struct {
			float widthGame;   //0x0028
			float heightGame;  //0x002C
		};
		Vec2 windowSize;  //0x0028
	};

	void displayClientMessageVA(const char* fmt, va_list lis, bool sendToInjector = true);

	void displayClientMessageF(const char* fmt, ...);
	void displayClientMessageNoSendF(const char* fmt, ...);
	void displayClientMessage(std::string* a2);
};

struct PtrToGameSettings3 {
private:
	char pad_0x0000[0x18];  //0x0000
public:
	GameSettingsInput* settingsInput;
};

struct PtrToGameSettings2 {
private:
	char pad_0x0000[0x148];  //0x0000
public:
	PtrToGameSettings3* ptr;
};

struct PtrToGameSettings1 {
private:
	char pad_0x0000[0x18];  //0x0000
public:
	PtrToGameSettings2* ptr;
};

class MoveInputHandler;
class CameraManager;

class ClientInstance {
private:
	char pad_0x8[0xA0];  //0x8
public:
	class MinecraftGame* minecraftGame;  //0xA8
private:
	class MinecraftGame* N00000A0C;  //0xB0
	class MinecraftGame* N00000A0D;  //0xB8
public:
	class Minecraft* minecraft;  //0xC0
private:
	char pad_0xC8[0x8];  //0xC8
public:
	class LevelRenderer* levelRenderer;  //0xD0
private:
	char pad_0xD8[0x8];  //0xD8
public:
	LoopbackPacketSender* loopbackPacketSender;  //0xE0
private:
	char pad_0xE8[0x18];  //0xE8
public:
	PtrToGameSettings1* ptr;  //0x100
private:
	char pad_0x108[0x8];  // 0x108
public:
	class HitDetectSystem* hitDetectSystem;  // 0x110
private:
	char pad_0118[0x340];  // 0x0118 + 0x3A0
public:
	Vec2 mousePos;  // 0x458
private:
	char pad_0460[0x78];

public:
	struct {
		char pad[0x238];
		struct {
			__int64 materialPtr;
			size_t refCount;
		} entityLineMaterial;
	} * itemInHandRenderer;  // 0x04D8

	virtual __int64 destructorClientInstance();
	// Duplicate destructor
	virtual __int64 onInitMinecraftGame(void);
	virtual __int64 onDestroyMinecraftGame(void);
	virtual __int64 init(__int64*, __int64&, __int64&, __int64&, __int64&, int);
	virtual __int64 initSceneFactory(__int64*);
	virtual __int64 initGraphics(void);
	virtual __int64 stop(void);
	virtual __int64 teardown(void);
	virtual __int64 preShutDown(void);
	virtual __int64 requestLeaveGame(bool, bool);
	virtual __int64 stopPlayScreen(void);

public:
	virtual bool isLeaveGameDone(void) const;
	virtual void setupPlayScreenForLeaveGame(void);

private:
	virtual __int64 resetPrimaryClient(void);
	virtual __int64 resetGameSession(void);
	virtual __int64 tick(void);
	virtual __int64 frameUpdate(__int64&);
	virtual __int64 update(bool);
	virtual __int64 endFrame(void);
	virtual __int64 startSubClientLateJoin(bool);

public:
	virtual void setupClientGame(__int64&&, bool);

private:
	virtual __int64 sub_1400C5D00(void) const;
	virtual __int64 getLocalPlayer(void);
	virtual __int64 getLocalPlayer(void) const;
	virtual __int64 getCameraEntity(void) const;

public:
	virtual void setCameraEntity(Entity*);

private:
	virtual __int64 getCameraTargetEntity(void) const;

public:
	virtual void setCameraTargetEntity(Entity*);
	virtual bool isLeavingGame(void) const;
	virtual bool isDestroyingGame(void) const;

private:
	virtual __int64 isShuttingDown(void) const;
	virtual __int64 useLowFrequencyUIRender(void) const;

public:
	virtual bool isSplitScreenActive(void) const;
	virtual bool isVRClient(void) const;
	virtual bool isARClient(void) const;
	virtual bool isARVRClient(void) const;
	virtual bool isHoloviewerMode(void) const;
	virtual bool isHoloscreenMode(void) const;
	virtual bool isLivingroomMode(void) const;
	virtual bool isExitingLevel(void) const;
	virtual bool isNotVLRMode(void) const;
	virtual bool isHoloRealityMode(void) const;
	virtual bool isRealityFullVRMode(void) const;
	virtual bool isInBedScreen(void) const;
	virtual bool isKeyboardEnabled(void) const;
	virtual bool isVRTransitioning(void) const;
	virtual bool hasCommands(void) const;
	virtual bool isScriptingEnabled(void) const;

private:
	virtual __int64 getHoloviewerScale(void) const;
	virtual __int64 getSplitScreenCount(void) const;

public:
	virtual bool isStereoRendering(void) const;
	virtual bool isPlatformNX(void) const;
	virtual bool isLocalSplitscreenWith(__int64 const&) const;
	virtual bool isValidCrossPlatformSkin(void) const;
	virtual __int64 isSelectedSkinInitialized(void);

private:
	virtual __int64 getSplitScreenInfo(void) const;
	virtual __int64 getCurrentMaxGUIScaleIndex(void) const;
	virtual __int64 getRealityModeFrameFactor(void) const;

public:
	virtual void setRealityModeFrameFactor(float const&);

private:
	virtual __int64 getRealityModeToggleTriggered(void) const;

public:
	virtual void setRealityModeToggleTriggered(bool);

private:
	virtual bool sub_1400C9C50(void);

public:
	virtual void setOpenControllerDisconnectScreen(bool);


	virtual __int64 getClientPlayMode(void) const;

public:
	virtual void setClientPlayMode(__int64 const&);


	virtual __int64 getLivingRoomForCredits(void) const;

public:
	virtual void setLivingRoomForCredits(bool);


	virtual __int64 getCreditsCallback(void);

public:
	virtual void setCreditsCallback(__int64);
	virtual void setupTransitionForCredits(__int64);


	virtual __int64 refreshScreenSizeData(void);
	virtual __int64 onScreenSizeChanged(int, int, float);
	virtual __int64 onGuiScaleOffsetChanged(void);
	virtual __int64 onSafeZoneChanged(void);
	virtual __int64 quit(std::string const&, std::string const&);

public:
	virtual __int64 getMinecraftGame(void) const;


	virtual __int64 getAutomationClient(void) const;
	virtual __int64 getScreenshotStreamer(void) const;
	virtual __int64 getEventing(void) const;
	virtual __int64 sub_1400CA2E0(__int64 a1);
	virtual __int64 sub_1400CA2E8(__int64 a1);
	virtual __int64 sub_1400CA2F0(__int64 a1);
	virtual Font* getFontDONTUSE(void) const;
	virtual Font* getRuneFontDONTUSE(void) const;
	virtual Font* getUnicodeFontDONTUSE(void) const;
	virtual __int64 getGeometryGroup(void) const;
	virtual __int64 getMultiplayerServiceManager(void) const;
	virtual __int64 getLocalServerLevel(void);
	virtual __int64 getResourcePackRepository(void) const;
	virtual __int64 getResourcePackManager(void) const;
	virtual __int64 getSkinRepository(void) const;
	virtual __int64 getSkinRepositoryClientInterface(void) const;
	virtual __int64 getPersonaPieceCollectionModel(void);
	virtual __int64 getPersonaRepository(void);
	virtual __int64 NullFunc_19();
	virtual __int64 getTextures(void);
	virtual __int64 NullFunc_20();
	virtual __int64 getStoreCacheTextures(void) const;
	virtual __int64 getMinecraftGraphics(void) const;
	virtual __int64 getUIRepository(void) const;
	virtual __int64 getUserManager(void) const;
	virtual __int64 wantToQuit(void) const;

public:
	virtual bool isPrimaryLevelCrossPlatformMultiplayer(void) const;
	virtual bool isPrimaryLevelMultiplayer(void) const;
	virtual bool isAdhocEnabled(void) const;


	virtual char sub_1406A1620(void) const;
	virtual __int64 linkToOffer(std::string const&, bool, std::string const&, __int64);
	virtual __int64 linkToAllOffers(std::string const&, std::string const&, bool, std::string const&, bool);
	virtual __int64 navigateToBrazeScreen(std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, std::string const&);
	virtual __int64 navigateToStoreHomeScreen(void);
	virtual __int64 navigateToCoinPurchaseScreen(int, __int64);
	virtual __int64 navigateToPurchaseOfferScreen(__int64, bool);
	virtual __int64 navigateToOfferCollectionScreen(__int64&);
	virtual __int64 navigateToStoreSeeAllByCreatorScreen(std::string const&, bool);
	virtual __int64 navigateToServersScreen(bool);
	virtual __int64 navigateToHowToPlayScreen(std::string const&);
	virtual __int64 tryPushLeaveGameScreen(void);
	virtual __int64 tryStartDayOneExperience(void);

public:
	virtual bool isReadyToRender(void) const;


	virtual __int64 onDimensionChanged(void);
	virtual __int64 onGameEventNotification(__int64);

public:
	virtual TextHolder getTopScreenName(void) const;
	virtual void setLeaveGameInProgressAsReadyToContinue(void);


	virtual __int64 stopDestroying(void);
	virtual __int64 onClientCreatedLevel(__int64);
	virtual __int64 getClientRandomId(void) const;
	virtual __int64 getUserAuthentication(void);
	virtual __int64 getWeakUserAuthentication(void);
	virtual __int64 registerToUserManager(__int64&, int);
	virtual __int64 resumeWithUserManager(__int64&, int);
	virtual __int64 createUserAuthentication(std::string);
	virtual __int64 createUserAuthentication(unsigned long long, std::string const&);
	virtual __int64 getPlatformId(void) const;
	virtual __int64 getPlatformOnlineId(void) const;

public:
	virtual bool isHoloCursorNeeded(void) const;


	virtual __int64 useController(void) const;
	virtual __int64 useTouchscreen(void) const;
	virtual void subFunction1(void) const;
	virtual void subFunction2(void) const;
	virtual void subFunction3(void) const;
	virtual void subFunction4(void) const;
	virtual void subFunction5(void) const;
	virtual void subFunction6(void) const;
	virtual void subFunction7(void) const;

public:
	virtual bool getMouseGrabbed(void) const;

private:
	virtual __int64 currentInputModeIsGamePadOrMotionController(void) const;
	virtual __int64 currentInputModeIsMouseAndKeyboard(void) const;
	virtual __int64 getForceMonoscopic(void) const;
	virtual __int64 allowPicking(void) const;

public:
	virtual bool isShowingMenu(void) const;
	virtual bool isShowingProgressScreen(void) const;
	virtual bool isShowingWorldProgressScreen(void) const;
	virtual bool isShowingDeathScreen(void) const;
	virtual bool isScreenReplaceable(void) const;
	virtual __int64 NullFunc_27();
	virtual bool isInGame(void);
	virtual bool isInRealm(void);

private:
	virtual __int64 readyForShutdown(void) const;

public:
	virtual bool isPrimaryClient(void) const;
	virtual bool isEduMode(void) const;
	virtual bool isGamepadCursorEnabled(void) const;

private:
	virtual Minecraft* getServerData(void);
	virtual Minecraft* getServerData(void) const;

public:
	virtual Level* getLevel(void);
	virtual Level* getLevel(void) const;
	virtual bool isPreGame(void) const;
	virtual bool isInMultiplayerGame(void) const;
	virtual bool isMultiPlayerClient(void) const;
	virtual __int64 sub_14012F590() const;
	virtual __int64 sub_14012F4D0() const;
	virtual __int64 getOptions(void);
	virtual __int64 getOptions(void) const;
	//virtual __int64 getOptionsPtr(void);
	virtual __int64 getUser(void);
	virtual __int64 getUser(void) const;
	virtual __int64 getGameRenderer(void) const;
	virtual __int64 getHolosceneRenderer(void) const;
	virtual LevelRenderer* getLevelRenderer(void) const;
	virtual __int64 getLevelRendererCameraProxy(void) const;

public:
	virtual CameraManager* getCameraManager(void) const;

private:
	virtual __int64 sub_1400CCC08(void) const;

public:
	virtual __int64 getLightTexture(void);

public:
	virtual void setupLevelRendering(__int64&, Entity&);

private:
	virtual __int64 getViewportInfo(void) const;

public:
	virtual void setViewportInfo(__int64 const&);

private:
	virtual __int64 getNormalizedViewportSize(void) const;
	virtual __int64 updateChunkRadius(void);

public:
	virtual void setUITexture(__int64*);

private:
	virtual __int64 getUITexture(void);

public:
	virtual void setLevelTexture(__int64*);

private:
	virtual __int64 getLevelTexture(void);

public:
	virtual void setUICursorTexture(__int64);

	virtual __int64 getUICursorTexture(void) const;
	virtual __int64 getCamera(void);
	virtual __int64 getShaderColor(void);
	virtual __int64 getDarkShaderColor(void);
	virtual __int64 clearGraphicsCache(void);
	virtual __int64 getNormalizedUICursorTransform(__int64&, float);
	virtual __int64 shouldRenderUICursor(void) const;
	virtual bool sub_1400CD3B0(void);
	virtual bool sub_1400CD3B1(void);
	virtual __int64 getGuiScale(void) const;
	virtual __int64 getGuiScaleOption(void) const;
	virtual __int64 getGuiScaleOffset(void) const;

public:
	virtual void setGuiScaleOffset(int);

private:
	virtual __int64 renderEditorGui(__int64&, bool);

public:
	virtual GuiData* getGuiDataDOESNTWORK(void);
	virtual GuiData* getGuiDataDOESNTWORK(void) const;

private:
	virtual __int64 getGuidedFlowManager(void);
	virtual __int64 getDpadScale(void) const;
	virtual __int64 getDateManager(void) const;
	virtual __int64 addOverrideHoursToDateTime(unsigned int);
	virtual __int64 getToastManager(void);
	virtual __int64 getTopScene(void);
	virtual __int64 getTopScene(void) const;
	virtual __int64 getActiveScene(void);
	virtual __int64 getActiveScene(void) const;
	virtual __int64 getSceneFactory(void) const;
	virtual __int64 getClientSceneStack(void) const;
	virtual __int64 getClientSceneStack(void);
	virtual __int64 sub_140634B30(void) const;
	virtual __int64 getMainSceneStack(void);
	virtual __int64 getMainSceneStack(void) const;
	virtual __int64 getCurrentSceneStack(void) const;
	virtual __int64 getCurrentSceneStack(void);
	virtual __int64 getCurrentUIRouter(void);
	virtual __int64 getCurrentUIRouter(void) const;
	virtual __int64 sub_14012BAB0(void) const;
	virtual __int64 prepareSceneFor(__int64);
	virtual __int64 getCachedScenes(void);

public:
	virtual TextHolder getScreenName(void) const;

private:
	virtual __int64 getScreenTelemetry(void) const;
	virtual __int64 getTopSceneType(void) const;
	virtual __int64 getMobEffectsLayout(void);
	virtual __int64 newFunc2(__int64 a2, __int64 a3, char a4);
	virtual __int64 newFunc3(__int64 a2, __int64 a3);
	virtual __int64 onMobEffectsChange(void);

public:
	virtual void setUISizeAndScale(int, int, float);

private:
	virtual __int64 forEachVisibleScreen(__int64, bool);
	virtual __int64 forEachScreen(__int64, bool);
	virtual __int64 forEachScreenConst(__int64, bool) const;
	virtual __int64 updateSceneStack(void);
	virtual __int64 forEachAlwaysAcceptInputScreen(__int64);
	virtual __int64 forEachAlwaysAcceptInputScreenWithTop(__int64);
	virtual __int64 showPlayerProfile(std::string const&, __int64);
	virtual __int64 getCurrentInputMode(void) const;

public:
	virtual bool isTouchGameplayAllowed(void) const;

private:
	virtual __int64 getMinecraftInput(void) const;

public:
	virtual void setHoloInput(__int64);

private:
	virtual __int64 getHoloInput(void) const;
	virtual __int64 getVoiceSystem(void) const;
	virtual __int64 getKeyboardManager(void);

public:
	virtual void setVoiceSystem(__int64);
	virtual void setLastPointerLocation(float, float, float);

private:
	virtual __int64 getLastPointerLocation(void);
	virtual __int64 shouldUseLastPointerLocationOnFocusChange(void);
	virtual __int64 adjustGazeCursorByMouse(float, float);
	virtual __int64 currentScreenShouldStealMouse(void);
	virtual __int64 getInProgressBAI(void) const;
	virtual __int64 newDictationDataAvailable(void) const;
	virtual __int64 clearDictationDataAvailable(void);
	virtual __int64 getDictationText(void) const;
	virtual __int64 getPacketSender(void);
	virtual __int64 getClientNetworkSystem(void);
	virtual __int64 getClientNetworkSystem(void) const;
	virtual void fake_function_to_make_grabmouse_work(void) const;

public:
	virtual void setMoveTurnInput(__int64);
	virtual MoveInputHandler* getMoveTurnInput(void) const;
	virtual MoveInputHandler* getMoveTurnInput(void);

public:
	virtual void setupPersistentControls(__int64);

private:
	virtual __int64 resetPlayerMovement(void);
	virtual __int64 onClientInputInitComplete(void);

public:
	virtual void setClientInputHandler(__int64);

private:
	virtual __int64 getInput(void) const;
	virtual __int64 getControllerId(void) const;

public:
	virtual bool hasConnectedController(void) const;

private:
	virtual __int64 getClientSubId(void) const;

public:
	virtual void setSuspendInput(bool);
	virtual void setDisableInput(bool);

private:
	virtual void subFunction(void) const;

public:
	virtual void grabMouse(void);
	virtual void releaseMouse(void);
	virtual void refocusMouse(void);

private:
	virtual __int64 resetBai(int);
	virtual __int64 clearInProgressBAI(void);
	virtual __int64 tickBuildAction(void);
	virtual __int64 getSoundEngine(void) const;
	virtual __int64 play(std::string const&, Vec3 const&, float, float);
	virtual __int64 playUI(std::string const&, float, float);
	virtual __int64 muteAudio(void);
	virtual __int64 unMuteAudio(void);
	virtual __int64 fadeOutMusic(void) const;
	virtual __int64 getTaskGroup(void);
	virtual __int64 onFullVanillaPackOnStack(void);

public:
	virtual bool isFullVanillaPackOnStack(void) const;

private:
	virtual __int64 onPlayerLoaded(Player&);

public:
	virtual void setClientGameMode(__int64);

private:
	virtual __int64 resetToDefaultGameMode(void);
	virtual __int64 connectToThirdParyServer(std::string const&, std::string const&, int);
	virtual __int64 startExternalNetworkWorld(__int64, std::string const&, bool);
	virtual __int64 checkForPiracy(void);
	virtual __int64 updateChatFilterStatus(void);
	virtual void sub_1400D2A20(void);

public:
	virtual void setBehaviorCommandCallback(__int64);
	virtual void setBehaviorCommandStatus(std::string, __int64);

private:
	virtual __int64 requestScreenshot(__int64&);
	virtual void sub_1400BE0A0(char a2);
	virtual __int64 getDevConsoleLogger(void) const;
	virtual __int64 requestImageFromUrl(std::string const&, __int64);
	virtual __int64 initializeRenderResources(void);
	virtual __int64 postInitRenderResources(void);
	virtual __int64 onAppSuspended(void);
	virtual __int64 onActiveResourcePacksChanged(__int64 const&);
	virtual __int64 reloadEntityRenderers(__int64 const&);
	virtual __int64 getBlockTessellator(void);
	virtual __int64 getBlockEntityRenderDispatcher(void);
	virtual __int64 getEntityRenderDispatcher(void);
	virtual __int64 getEntityBlockRenderer(void);
	virtual __int64 getItemInHandRenderer(void);
	virtual __int64 getItemRenderer(void);
	virtual __int64 getSentMessageHistory(void);
	virtual __int64 getUIProfanityContext(void) const;
	virtual __int64 initTTSClient(__int64&);
	virtual __int64 getTTSClient(void);
	virtual __int64 getTTSClient(void) const;
	virtual __int64 getTTSEventManager(void);
	virtual __int64 addTTSMessage(std::string const&, __int64, bool, bool, bool);
	virtual __int64 initCommands(void);
	virtual __int64 getUserId(void) const;
	virtual __int64 getServerConnectionTime(void) const;
	virtual __int64 getHMDState(void);
	virtual __int64 getHMDState(void) const;

public:
	virtual void setServerPingTime(unsigned int);

private:
	virtual __int64 getServerPingTime(void) const;

public:
	virtual void setDefaultPlayscreenTab(__int64);
	virtual void setClientInstanceState(__int64 const&);
	virtual void setUIEventCoordinator(__int64&&);

private:
	virtual __int64 getUIEventCoordinator(void);
	virtual __int64 getEventCoordinator(void);
	virtual __int64 computeScreenCoordsFromScreenNormCoords(float, float, short&, short&);
	virtual __int64 getNoBlockBreakUntil(void);

public:
	virtual void setNoBlockBreakUntil(__int64);
	virtual void setDictation(std::string const&);
	virtual void setNewDictationString(bool);
	virtual void setGameModule(__int64);

private:
	virtual __int64 getGameModule(void);
	virtual __int64 getGameCallbacks(void);
	virtual __int64 getStorageAreaStateListener(void);
	virtual __int64 getScriptEngine(void);
	virtual __int64 sendClientEnteredLevel(void);
	virtual __int64 getHitDetectSystem(void);

public:
	virtual bool isPlaying(void) const;

private:
	virtual __int64 getLatencyGraphDisplay(void) const;
	virtual __int64 createSkin(void);
	virtual __int64 onExtendDiskSpace(bool, __int64&, unsigned long long const&, __int64);
	virtual __int64 onLowDiskSpace(bool);
	virtual __int64 onOutOfDiskSpace(bool);
	virtual __int64 onCriticalDiskError(bool, __int64 const&);
	virtual __int64 onLevelCorrupt(void);
	virtual __int64 onGameModeChanged(void);
	virtual __int64 onTick(int, int);
	virtual __int64 onInternetUpdate(void);
	virtual __int64 onGameSessionReset(void);
	virtual __int64 onLevelExit(void);
	virtual __int64 updateScreens(void);

public:

	glmatrixf* getRefDef() {
		return reinterpret_cast<glmatrixf*>((uintptr_t)(this) + 0x2F0);
	};

	Vec2* getMousePos() {
		return reinterpret_cast<Vec2*>((uintptr_t)(this) + 0x458);
	}

	Vec2 getFov() {
		Vec2 fov;
		fov.x = *reinterpret_cast<float*>((uintptr_t)(this) + 0x678);
		fov.y = *reinterpret_cast<float*>((uintptr_t)(this) + 0x68C);
		return fov;
	}

	Font* getFont() {
		using getFont_t = Font*(__fastcall*)(ClientInstance*);
		static getFont_t getFontFunc = reinterpret_cast<getFont_t>(FindSignature("40 53 48 83 EC 30 48 8B DA 48 8B 89 ? ? ? ? 48 8B 01 48 8D 54 24 ? FF 90 ? ? ? ? 90 48 8B C8 E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 90 48 8D 4C 24 ? E8 ? ? ? ? 48 8B C3 48 83 C4 30 5B C3 CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ?"));
		return getFontFunc(this);
	}

	Font* getRuneFont() {
		using getRuneFont_t = Font*(__fastcall*)(ClientInstance*);
		static getRuneFont_t getRuneFontFunc = reinterpret_cast<getRuneFont_t>(FindSignature("40 53 48 83 EC 30 48 8B DA 48 8B 89 ? ? ? ? 48 8B 01 48 8D 54 24 ? FF 90 ? ? ? ? 90 48 8B C8 E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 90 48 8D 4C 24 ? E8 ? ? ? ? 48 8B C3 48 83 C4 30 5B C3 CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 30"));
		return getRuneFontFunc(this);
	}

	Font* getUnicodeFont() {
		using getUnicodeFont_t = Font*(__fastcall*)(ClientInstance*);
		static getUnicodeFont_t getUnicodeFontFunc = reinterpret_cast<getUnicodeFont_t>(FindSignature("40 53 48 83 EC 30 48 8B DA 48 8B 89 ? ? ? ? 48 8B 01 48 8D 54 24 ? FF 90 ? ? ? ? 90 48 8B C8 E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 90 48 8D 4C 24 ? E8 ? ? ? ? 48 8B C3 48 83 C4 30 5B C3 CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 20"));
		return getUnicodeFontFunc(this);
	}

	Font* _getFont() {
		return this->getFont();
	}

	Font* _getRuneFont() {
		return this->getRuneFont();
	}

	Font* _getUnicodeFont() {
		return this->getUnicodeFont();
	}

	GuiData* getGuiData() {
		return (GuiData*)*reinterpret_cast<__int64*>(reinterpret_cast<GuiData*>(reinterpret_cast<__int64>(this) + 0x500));
	};

	class LocalPlayer* getCILocalPlayer() {
		return Utils::CallVFunc<24, LocalPlayer*>(this);
	}

	inline GameSettingsInput* getGameSettingsInput() { return this->ptr->ptr->ptr->settingsInput; };
};