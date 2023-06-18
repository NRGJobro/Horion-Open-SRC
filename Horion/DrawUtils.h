#pragma once

#include "../Memory/GameData.h"
#include "../SDK/ClientInstance.h"
#include "../SDK/MinecraftUIRenderContext.h"
#include "../SDK/Tessellator.h"
#include "../Utils/HMath.h"
#include "../Utils/Target.h"
#include "../Utils/Utils.h"

enum class Fonts { DEFAULT,
				   MCFONT,
				   UNICOD,
				   SMOOTH,
				   RUNE };

using mce__VertexFormat__disableHalfFloats_t = void(__fastcall*)(__int64, int, int);
using Tessellator__initializeFormat_t = void(__fastcall*)(__int64, __int64);

extern MinecraftUIRenderContext* renderCtx;
extern mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats;
extern Tessellator__initializeFormat_t Tessellator__initializeFormat;

struct MC_Color {
	union {
		struct {
			float r, g, b, a;
		};
		float arr[4];
	};
	bool shouldDelete = true;

	MC_Color() : r(1), g(1), b(1), a(1) {
		arr[0] = r;
		arr[1] = g;
		arr[2] = b;
		arr[3] = a;
	}

	MC_Color(const MC_Color& other) : r(other.r), g(other.g), b(other.b), a(other.a), shouldDelete(other.shouldDelete) {
		arr[0] = r;
		arr[1] = g;
		arr[2] = b;
		arr[3] = a;
	}

	MC_Color(const float* arr) : r(arr[0]), g(arr[1]), b(arr[2]), a(arr[3]) {
		this->arr[0] = r;
		this->arr[1] = g;
		this->arr[2] = b;
		this->arr[3] = a;
	}

	MC_Color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {
		arr[0] = r;
		arr[1] = g;
		arr[2] = b;
		arr[3] = a;
	}

	MC_Color(int r, int g, int b, int a = 255) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {
		arr[0] = this->r;
		arr[1] = this->g;
		arr[2] = this->b;
		arr[3] = this->a;
	}

	MC_Color(float r, float g, float b, float a, bool shouldDelete) : r(r), g(g), b(b), a(a), shouldDelete(shouldDelete) {
		arr[0] = r;
		arr[1] = g;
		arr[2] = b;
		arr[3] = a;
	}

	MC_Color lerp(const MC_Color& o, float t) const;
};

class MatrixStack;

class DrawUtils {
public:
	static bool isLeftClickDown;
	static bool isRightClickDown;
	static bool shouldToggleLeftClick;
	static bool shouldToggleRightClick;
	
	static void setCtx(MinecraftUIRenderContext* ctx, GuiData* guiData);
	static void setGameRenderContext(__int64 ctx);
	static void flush();
	static void setColor(float r, float g, float b, float a);  // rgba, values from 0 to 1
	static inline void tess__begin(Tessellator* tesselator, int vertexFormat = 3, int numVerticesReserved = 0);
	static Font* getFont(Fonts font);
	static Tessellator* get3dTessellator();
	static __int64 getScreenContext();
	static MatrixStack* getMatrixStack();
	static float getTextWidth(std::string* textStr, float textSize = 1, Fonts font = Fonts::SMOOTH);
	static float getFontHeight(float textSize = 1, Fonts font = Fonts::SMOOTH);

	static void drawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3);
	static void drawQuad(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4);
	static void drawLine(const Vec2& start, const Vec2& end, float lineWidth);  // rgba
	static void drawLinestrip3d(const std::vector<Vec3>& points);
	static void drawLine3d(const Vec3& start, const Vec3& end, bool onUi = false);
	static void drawBox3d(const Vec3& lower, const Vec3& upper, float scale = 1.f, bool onUi = false);
	static void drawBox3dFilled(const Vec3& lower, const Vec3& upper, float scale = 1.f, bool outline = false, bool onUi = false);
	static void drawCircle(Vec2 pos, Vec2 radius, MC_Color color, double quality);
	static void drawCircleFilled(Vec2 pos, Vec2 radius, MC_Color color, double quality);
	static void fillRectangle(const Vec4& pos, const MC_Color& col, float alpha);
	static inline void fillRectangle(const Vec2& start, const Vec2& end) {
		DrawUtils::drawQuad({start.x, end.y}, {end.x, end.y}, {end.x, start.y}, {start.x, start.y});
	}
	static inline void drawRectangle(const Vec2& start, const Vec2& end, float lineWidth = 1.0f) {
		lineWidth /= 2;
		fillRectangle({start.x - lineWidth, start.y - lineWidth}, {end.x + lineWidth, start.y + lineWidth});  // TOP
		fillRectangle({start.x - lineWidth, start.y}, {start.x + lineWidth, end.y});                          // LEFT
		fillRectangle({end.x - lineWidth, start.y}, {end.x + lineWidth, end.y});                              //
		fillRectangle({start.x - lineWidth, end.y - lineWidth}, {end.x + lineWidth, end.y + lineWidth});
	}
	static inline void drawRectangle(const Vec4& pos, const MC_Color& col, float alpha, float lineWidth = 1.0f) {
		lineWidth /= 2;
		fillRectangle(Vec4(pos.x - lineWidth, pos.y - lineWidth, pos.z + lineWidth, pos.y + lineWidth), col, alpha);  // TOP
		fillRectangle(Vec4(pos.x - lineWidth, pos.y, pos.x + lineWidth, pos.w), col, alpha);                          // LEFT
		fillRectangle(Vec4(pos.z - lineWidth, pos.y, pos.z + lineWidth, pos.w), col, alpha);                          //
		fillRectangle(Vec4(pos.x - lineWidth, pos.w - lineWidth, pos.z + lineWidth, pos.w + lineWidth), col, alpha);
	}
	static void drawImage(std::string filePath, Vec2& ImagePos, Vec2& ImageDimension, Vec2& idk);

	static void drawText(const Vec2& pos, std::string* text, const MC_Color& color, float textSize = 1, float alpha = 1, Fonts font = Fonts::SMOOTH);
	static void drawBox(const Vec3& lower, const Vec3& upper, float lineWidth, bool fill = false, int mode = 1);
	static void draw2DBox(const Vec3& lower, const Vec3& upper, float lineWidth, bool fill = false, bool corners = false);
	static void drawEntityBox(Entity* ent, float lineWidth = 0.f, bool fill = false);
	static void draw2D(Entity* ent, float lineWidth = 0.f);
	static void drawNameTags(Entity* ent, float textSize, bool drawHealth = false, bool useUnicodeFont = false);
	static void drawItem(ItemStack* item, const Vec2& ItemPos, float opacity, float scale, bool isEnchanted);
	static float getLerpTime();
	static Vec3 getOrigin();
	static void onMouseClickUpdate(int key, bool isDown);
	static Vec2 worldToScreen(const Vec3& world);
};

