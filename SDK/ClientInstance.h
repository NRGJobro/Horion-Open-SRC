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
	float* simTimer;   // 0x00D8
	float* realTimer;  // 0x00E0

	void setTimerSpeed(float tps) {
		*this->simTimer = tps;
		*this->realTimer = tps;
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
	MaterialPtr(const HashedString& materialName);
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

	Vec3& getOrigin() {
		return *(Vec3*)((char*)this + (0x79C));
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
	FontRepository_FontList* fontList;  // 0x0020
	//FontRepository_FontList* fontList1;  //0x0028
};

class MinecraftGame {
private:
	char pad_0000[312];                     // 0x0000
public:
	bool canUseKeys;                        // 0x0138
private:
	char pad_0139[3319];                    // 0x0139
public:
	FontRepository** fontRepository;  // 0x0E30
private:
	char pad_0E38[8];                       // 0x0E38
public:
	Font* mcFont;                     // 0x0E40

	Font* getTheGoodFontThankYou() {
		return (*fontRepository)->fontList->fontEntries[7].font;
	};

	Font* getTheRandomFontPlease() {
		return (*fontRepository)->fontList->fontEntries[4].font;
	};

	Font* getOldFont() {
		return (*fontRepository)->fontList->fontEntries[7].font;
	};

	Font* getTheBetterFontYes() {
		return (*fontRepository)->fontList->fontEntries[3].font;
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
	char pad_0x0[0xB8];  // 0x0
public:
	MinecraftGame* minecraftGame;  // 0xB8
	Minecraft* minecraft;          // 0xC0
private:
	char pad_0xC8[0x8];  // 0xC8
public:
	LevelRenderer* levelRenderer;  // 0xD0
private:
	char pad_0xD8[0x8];  // 0xD8
public:
	LoopbackPacketSender* loopbackPacketSender;  // 0xE0
private:
	char pad_0xE8[0x18];  // 0xE8
public:
	PtrToGameSettings1* ptr;  // 0x100
private:
	char pad_0x108[0x8];  // 0x108
public:
	HitDetectSystem* hitDetectSystem;  // 0x110
private:
	char pad_0x118[0x3D8];  // 0x118
public:
	class ItemInHandRenderer* itemInHandRenderer;  // 0x4F0

	glmatrixf* getRefDef() {
		return reinterpret_cast<glmatrixf*>((uintptr_t)(this) + 0x2F0);
	};

	Vec2* getMousePos() {
		return reinterpret_cast<Vec2*>((uintptr_t)(this) + 0x458);
	}

	Vec2 getFov() {
		Vec2 fov;
		fov.x = *reinterpret_cast<float*>((uintptr_t)(this) + 0x690);
		fov.y = *reinterpret_cast<float*>((uintptr_t)(this) + 0x6A4);
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

	MoveInputHandler* getMoveTurnInput() {
		using GetMoveInputHandlerFunc = MoveInputHandler*(__fastcall*)(ClientInstance*);
		static GetMoveInputHandlerFunc getMoveInputHandlerFunc = reinterpret_cast<GetMoveInputHandlerFunc>(FindSignature("48 83 EC 28 8D 42 FE"));
		return getMoveInputHandlerFunc(this);
	}

	void grabMouse() {
		return Utils::CallVFunc<284, void>(this);
	}

	void releaseMouse() {
		return Utils::CallVFunc<285, void>(this);
	}

	GuiData* getGuiData() {
		return (GuiData*)*reinterpret_cast<__int64*>(reinterpret_cast<GuiData*>(reinterpret_cast<__int64>(this) + 0x500));
	};

	BlockTessellator* getBlockTesselator() {
		return (BlockTessellator*)*reinterpret_cast<__int64*>(reinterpret_cast<BlockTessellator*>(reinterpret_cast<__int64>(this) + 0x4C8));
	};

	class LocalPlayer* getCILocalPlayer() {
		return Utils::CallVFunc<24, LocalPlayer*>(this);
	}

	inline GameSettingsInput* getGameSettingsInput() { return this->ptr->ptr->ptr->settingsInput; };
};