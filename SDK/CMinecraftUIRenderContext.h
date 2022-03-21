#pragma once
#include "../Utils/HMath.h"
#include "TextHolder.h"
#include <vector>

struct MC_Color;

class C_FilePath {
private:
	char pad_0x0[0x8];    //0x0000
	TextHolder filePath;  //0x0008
public:
	C_FilePath(std::string filePath) {
		memset(this, 0, sizeof(C_FilePath));
		this->filePath.setText(filePath);
	};
};

class C_TexturePtr {
private:
	char pad_0x0[0x18];  //0x0000
public:
	TextHolder filePath;  //0x0018
private:
	char pad_0x0038[0x20];  //0x0038
};

class C_Font {
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

struct ImageInfo_t {
	char Present;
	char ResolutionUnits;
	__int16 XDensity;
	__int16 YDensity;
};

struct ImageInfo {
	char FileName[1025];
	ImageInfo_t JfifHeader;
	char CameraMake[32];
	char CameraModel[40];
	char DateTime[20];
	unsigned int Height;
	unsigned int Width;
	int Orientation;
	int IsColor;
	int Process;
	int FlashUsed;
	float FocalLength;
	float ExposureTime;
	float ApertureFNumber;
	float Distance;
	float CCDWidth;
	float ExposureBias;
	float DigitalZoomRatio;
	int FocalLength35mmEquiv;
	int Whitebalance;
	int MeteringMode;
	int ExposureProgram;
	int ExposureMode;
	int ISOequivalent;
	int LightSource;
	int DistanceRange;
	float xResolution;
	float yResolution;
	int ResolutionUnit;
	char Comments[16000];
	int CommentWidthchars;
	unsigned int ThumbnailOffset;
	unsigned int ThumbnailSize;
	unsigned int LargestExifOffset;
	char ThumbnailAtEnd;
	int ThumbnailSizeOffset;
	int DateTimeOffsets[10];
	int numDateTimeTags;
	int GpsInfoPresent;
	char GpsLat[31];
	char GpsLong[31];
	char GpsAlt[20];
	int QualityGuess;
};

class NinesliceInfo {
public:
	ImageInfo TopLeft;
	ImageInfo TopRight;
	ImageInfo BottomLeft;
	ImageInfo BottomRight;
	vec2_t UVScale;
	std::vector<ImageInfo> Left;
	std::vector<ImageInfo> Top;
	std::vector<ImageInfo> Right;
	std::vector<ImageInfo> Bottom;
	std::vector<ImageInfo> Middle;
};

class C_MinecraftUIRenderContext {
private:
	virtual auto Constructor() -> void{};

public:
	virtual float getLineLength(C_Font* font, TextHolder* str, float textSize);  //bool unknown
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual void drawDebugText(const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(C_Font* font, const float* pos, TextHolder* text, const float* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush);
	virtual __int64 drawImage(C_TexturePtr* texturePtr, vec2_t& ImagePos, vec2_t& ImageDimension, __int64& a4, vec2_t& idk);
	virtual void drawNineslice(C_TexturePtr* texturePtr, void* nineslice);
	virtual __int64 flushImages(MC_Color& color, float alpha, __int64 hashedString);
	virtual void beginSharedMeshBatch(uintptr_t ComponentRenderBatch);
	virtual void endSharedMeshBatch(float timeSinceLastFlush);
	virtual void drawRectangle(const float* pos, const float* color, float alpha, int lineWidth);  // line width is guessed
	virtual void fillRectangle(const float* pos, const float* color, float alpha);
	virtual void increaseStencilRef();
	virtual void decreaseStencilRef();
	virtual void resetStencilRef();
	virtual void fillRectangleStencil(vec4_t position);
	virtual void enableScissorTest(vec4_t position);
	virtual void disableScissorTest();
	virtual void setClippingRectangle(vec4_t position);
	virtual void setFullClippingRectangle();
	virtual void saveCurrentClippingRectangle();
	virtual void restoreSavedClippingRectangle();
	virtual int getFullClippingRectangle();
	virtual void updateCustom(uintptr_t a1);
	virtual void renderCustom(uintptr_t a1, int a2, vec4_t position);
	virtual void cleanup();
	virtual void removePersistentMeshes();
	virtual C_TexturePtr* getTexture(C_TexturePtr* ptr, C_FilePath& path);
	virtual int getZippedTexture(C_TexturePtr* Path, C_TexturePtr* ResourceLocation, bool a3);
};
