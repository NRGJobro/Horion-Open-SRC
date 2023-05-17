#include "Arraylist.h"
#include "../../../Utils/ColorUtil.h"
#include "../../../Utils/ClientColors.h"

Arraylist::Arraylist() : IModule(0, Category::VISUAL, "Display your enabled modules on the HUD!") {
	registerFloatSetting("Opacity", &alpha, alpha, 0.f, 1.f);
	registerFloatSetting("Color Speed", &cycleSpeed, cycleSpeed, 1.f, 5.f);
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f);
}

Arraylist::~Arraylist() {
}

const char* Arraylist::getModuleName() {
	return ("Arraylist");
}

struct IModuleContainer {
	// Struct used to Sort IModules in a std::set
	std::shared_ptr<IModule> backingModule;
	std::string moduleName;
	bool enabled;
	int keybind;
	float textWidth;
	Vec2* pos;
	bool shouldRender = true;

	IModuleContainer(std::shared_ptr<IModule> mod) {
		static auto arrayMod = moduleMgr->getModule<Arraylist>();
		static auto hudMod = moduleMgr->getModule<HudModule>();
		const char* moduleNameChr = mod->getModuleName();
		this->enabled = mod->isEnabled();
		this->keybind = mod->getKeybind();
		this->backingModule = mod;
		this->pos = mod->getPos();

		if (keybind == 0x0)
			moduleName = moduleNameChr;
		else {
			char text[50];
			sprintf_s(text, 50, "%s%s%s", moduleNameChr, std::string(GRAY).c_str(), hudMod->keybinds ? std::string(" [" + std::string(Utils::getKeybindName(keybind)) + "]").c_str() : "");
			moduleName = text;
		}

		if (!this->enabled && *this->pos == Vec2(0.f, 0.f))
			this->shouldRender = false;
		this->textWidth = DrawUtils::getTextWidth(&moduleName, hudMod->scale);
	}

	bool operator<(const IModuleContainer& other) const {
		if (this->textWidth == other.textWidth)
			return moduleName < other.moduleName;
		return this->textWidth > other.textWidth;
	}
};

void Arraylist::onPostRender(MinecraftUIRenderContext* renderCtx) {
	static auto hudMod = moduleMgr->getModule<HudModule>();

	Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
	Vec2 windowSizeReal = Game.getClientInstance()->getGuiData()->windowSizeReal;
	Vec2 mousePos = *Game.getClientInstance()->getMousePos();

	// Convert mousePos to visual Pos
	{
		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

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
			if (it.get() != hudMod)
				modContainerList.emplace(IModuleContainer(it));
		}
	}

	// Parameters
	float textSize = hudMod->scale;
	float textPadding = 1.0f * textSize;
	float textHeight = 10.0f * textSize;
	int seperation = 50;  // Adjust this to change the seperation of the colors
	int index = 0;
	float yOffset = 0;

	for (auto it = modContainerList.begin(); it != modContainerList.end(); ++it) {
		if (!it->shouldRender) continue;

		auto& container = *it;
		auto textStr = container.moduleName;
		auto textWidth = container.textWidth;

		auto arrayColor = ColorUtil::getRainbowColor(cycleSpeed, saturation, 1, index * (seperation * 2));
		auto xOffsetOri = windowSize.x - textWidth - (textPadding * 2);
		auto xOffset = windowSize.x - container.pos->x;

		container.pos->x = smoothLerp(container.enabled ? windowSize.x - xOffsetOri : -1.f, container.pos->x, 0.04);
		if (xOffset >= windowSize.x && !container.enabled) {
			container.pos->x = 0.f;
			container.pos->y = 0.f;
		}

		auto textPos = Vec2(xOffset + textPadding, yOffset + textPadding);
		auto rectPos = Vec4(xOffset - 2, yOffset, windowSize.x, yOffset + textPadding * 2 + textHeight);
		auto sideRect = Vec4(xOffset - 2, yOffset, xOffset - 1, yOffset + textPadding * 2 + textHeight);

		if (alpha > 0) {
			DrawUtils::fillRectangle(rectPos, ClientColors::arraylistBackgroundColor, alpha);
			DrawUtils::fillRectangle(sideRect, arrayColor, 1.f);
		}
		if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos) && clickToggle) {
			auto selectedRect = rectPos;
			selectedRect.x = sideRect.z;
			if (leftMouseDown) {
				DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f), 0.8f);
				if (executeClick) container.backingModule->toggle();
			} else {
				DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f, 0.8f), 0.3f);
			}
		}
		DrawUtils::drawText(textPos, &textStr, arrayColor, textSize);

		yOffset += ((10.0f * textSize) + (textPadding * 2)) * ((windowSize.x - xOffset) / (windowSize.x - xOffsetOri));
		++index;
	}
	modContainerList.clear();
}
