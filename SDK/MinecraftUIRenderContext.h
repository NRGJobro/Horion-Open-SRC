#pragma once
#include "../Utils/HMath.h"
#include "TextHolder.h"
#include <vector>

struct MC_Color;

class FilePath {
private:
	char pad_0x0[0x8];    //0x0000
	TextHolder filePath;  //0x0008
public:
	FilePath(std::string filePath) {
		memset(this, 0, sizeof(FilePath));
		this->filePath.setText(filePath);
	};
};

class TexturePtr {
private:
	char pad_0x0[0x18];  //0x0000
public:
	TextHolder filePath;  //0x0018
private:
	char pad_0x0038[0x20];  //0x0038
};

class Font {
private:
	virtual void destructorFont();
	virtual void unk1();
	virtual void unk2();
	virtual void unk3();
	virtual void unk4();
	virtual void unk5();

public:
	virtual float getLineLength(TextHolder* str, float textSize);  //bool unknown
	virtual float getLineHeight();
};

struct TextMeasureData {
	float textSize;
	int idk;
	bool displayShadow;
	bool bool2;
};

class MinecraftUIRenderContext {
private:
	virtual auto Constructor() -> void{};

public:
	virtual float getLineLength(Font* font, TextHolder* str, float textSize);  //bool unknown
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual void drawDebugText(const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(Font* font, const float* pos, TextHolder* text, const float* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush);
	virtual __int64 drawImage(TexturePtr* texturePtr, Vec2& ImagePos, Vec2& ImageDimension, __int64& a4, Vec2& idk);
	virtual void drawNineslice(TexturePtr* texturePtr, void* nineslice);
	virtual __int64 flushImages(MC_Color& color, float alpha, __int64 hashedString);
	virtual void beginSharedMeshBatch(uintptr_t ComponentRenderBatch);
	virtual void endSharedMeshBatch(float timeSinceLastFlush);
	virtual void drawRectangle(const float* pos, const float* color, float alpha, int lineWidth);  // line width is guessed
	virtual void fillRectangle(const float* pos, const float* color, float alpha);
	virtual void increaseStencilRef();
	virtual void decreaseStencilRef();
	virtual void resetStencilRef();
	virtual void fillRectangleStencil(Vec4 position);
	virtual void enableScissorTest(Vec4 position);
	virtual void disableScissorTest();
	virtual void setClippingRectangle(Vec4 position);
	virtual void setFullClippingRectangle();
	virtual void saveCurrentClippingRectangle();
	virtual void restoreSavedClippingRectangle();
	virtual int getFullClippingRectangle();
	virtual void updateCustom(uintptr_t a1);
	virtual void renderCustom(uintptr_t a1, int a2, Vec4 position);
	virtual void cleanup();
	virtual void removePersistentMeshes();
	virtual TexturePtr* getTexture(TexturePtr* ptr, FilePath& path);
	virtual int getZippedTexture(TexturePtr* Path, TexturePtr* ResourceLocation, bool a3);
};
