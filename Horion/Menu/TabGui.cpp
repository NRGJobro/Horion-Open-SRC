#include "TabGui.h"

#include <Windows.h>

#include "../../Utils/ClientColors.h"

struct SelectedItemInformation {
	int selectedItemId = 0;
	float currentSelectedItemInterpol = 0;
	float rollbackVal = 1;

	void setSelectedItemForce(int item) {
		selectedItemId = item;
		currentSelectedItemInterpol = static_cast<float>(item);
	}

	void interp() {
		currentSelectedItemInterpol += (selectedItemId - currentSelectedItemInterpol) * 0.05f;
	}

	void rollback() {
		rollbackVal *= 0.95f;
	}

	void rollin() {
		rollbackVal = 1 - ((1 - rollbackVal) * 0.95f);
	}
};

// State
int level;
SelectedItemInformation selected[4];
bool toggleCurrentSelection = false;

// Render
static float yOffset;
static float xOffset;
int renderedLevel;

struct LabelContainer {
	const char* text = nullptr;
	bool enabled = false;
	std::shared_ptr<IModule> mod = nullptr;
};

std::vector<LabelContainer> labelList;

void TabGui::renderLabel(const char* text, std::shared_ptr<IModule> mod) {
	LabelContainer label;
	label.text = text;
	if (mod != nullptr) {
		label.enabled = mod->isEnabled();
		label.mod = mod;
	}

	labelList.push_back(label);
}

void TabGui::renderLevel() {
	auto hudModule = moduleMgr->getModule<HudModule>();

	// Parameters
	float textSize = hudModule->scale;
	float textHeight = 10.f * textSize;
	float alphaVal = 1.0f;

	// First loop: Get the maximum text length
	float maxLength = 0.f;
	int labelListLength = 0;
	for (const auto& label : labelList) {
		labelListLength++;
		std::string labelText(label.text);
		maxLength = std::max(maxLength, DrawUtils::getTextWidth(&labelText, textSize));
	}

	if (selected[renderedLevel].selectedItemId < 0)
		selected[renderedLevel].selectedItemId += labelListLength;
	if (selected[renderedLevel].selectedItemId >= labelListLength)
		selected[renderedLevel].selectedItemId -= labelListLength;

	selected[renderedLevel].interp();  // Converge to selected item
	if (renderedLevel < level)
		selected[renderedLevel].rollbackVal = 1;  // Speed up animation when we are in the next menu already

	// Second loop: Render everything
	int i = 0;
	float selectedYOffset = yOffset;
	float startYOffset = yOffset;
	for (const auto& label : labelList) {
		Vec4 rectPos = Vec4(
			xOffset - 0.5f,  // Off screen / Left border not visible
			yOffset,
			xOffset + maxLength + 4.5f,
			yOffset + textHeight);

		MC_Color color(200, 200, 200);

		if (selected[renderedLevel].selectedItemId == i && level >= renderedLevel) {  // We are selected
			if (renderedLevel == level) {                                             // Are we actually in the menu we are drawing right now?
				// We are selected in the current menu
				DrawUtils::fillRectangle(rectPos, ClientColors::tabGuiBackground, 1.f);
				static bool lastVal = toggleCurrentSelection;

				if (toggleCurrentSelection) {
					if (label.mod->isFlashMode()) {
						label.mod->setEnabled(true);
					} else {
						toggleCurrentSelection = false;
						label.mod->toggle();
					}
				} else if (toggleCurrentSelection != lastVal && label.mod->isFlashMode()) {
					label.mod->setEnabled(false);
				}
				lastVal = toggleCurrentSelection;
			} else {  // selected, but not what the user is interacting with
				DrawUtils::fillRectangle(rectPos, ClientColors::tabGuiBackground, 1.f);
			}
		} else {  // We are not selected
			DrawUtils::fillRectangle(rectPos, label.enabled ? ClientColors::tabGuiEnabledItemColor : ClientColors::tabGuiBackground, 1.f);
		}

		std::string tempLabel(label.text);
		DrawUtils::drawText(Vec2(xOffset + 1.5f, yOffset + 0.5f), &tempLabel, label.enabled ? MC_Color() : color, textSize);

		yOffset += textHeight;
		i++;
	}

	// Draw selected item
	{
		selectedYOffset = startYOffset + textHeight * selected[renderedLevel].currentSelectedItemInterpol;
		Vec4 selectedPos = Vec4(
			xOffset - 0.5f,  // Off screen / Left border not visible
			selectedYOffset,
			xOffset + maxLength + 4.5f,
			selectedYOffset + textHeight);

		float diff = selectedPos.z - selectedPos.x;
		selectedPos.z = selectedPos.x + diff * selected[renderedLevel].rollbackVal;

		if (renderedLevel > level) {
			selected[renderedLevel].rollback();
		} else {
			selected[renderedLevel].rollin();
		}

		DrawUtils::fillRectangle(selectedPos, ClientColors::tabGuiSelectedItemColor, alphaVal);
	}

	// Cleanup
	DrawUtils::flush();
	labelList.clear();
	xOffset += maxLength + 4.5f;
	yOffset = selectedYOffset;
	renderedLevel++;
}

void TabGui::render() {
	if (!moduleMgr->isInitialized())
		return;
	if (!GameData::canUseMoveKeys())
		level = -1;
	renderedLevel = 0;
	yOffset = 4;
	xOffset = 3;

	// Render all categories
	renderLabel("Combat");
	renderLabel("Visual");
	renderLabel("Movement");
	renderLabel("Player");
	renderLabel("World");
	renderLabel("Misc");
	renderLabel("Client");
	renderLevel();

	// Render all modules
	if (level >= 0) {
		auto lock = moduleMgr->lockModuleList();

		std::vector<std::shared_ptr<IModule>>* modules = moduleMgr->getModuleList();
		for (const auto& mod : *modules) {
			if (selected[0].selectedItemId == static_cast<int>(mod->getCategory())) {
				auto name = mod->getModuleName();
				renderLabel(name, mod);
			}
		}

		renderLevel();
	}
}

void TabGui::init() {
	level = 0;
	xOffset = 0;
	yOffset = 0;
	renderedLevel = 0;
}

void TabGui::onKeyUpdate(int key, bool isDown) {
	if (!GameData::canUseMoveKeys()) {
		level = -1;
		return;
	}

	if (!isDown) {
		if (key == VK_RIGHT)
			toggleCurrentSelection = false;
		return;
	}

	switch (key) {
	case VK_LEFT:  // Leave menus
		if (level > -1) {
			level--;
		}
		if (level == -1)
			selected[1].rollbackVal = 0;
		return;
	case VK_RIGHT:
		if (level < 1) {
			level++;
			selected[level].setSelectedItemForce(0);
			selected[level].rollbackVal = 0;
		} else {
			toggleCurrentSelection = true;
		}
		return;
	case VK_UP:
		if (level >= 0)
			selected[level].selectedItemId--;
		else
			level = 0;
		break;
	case VK_DOWN:
		if (level >= 0)
			selected[level].selectedItemId++;
		else
			level = 0;
		break;
	};

	if (level < 3)
		selected[level + 1].setSelectedItemForce(0);
}
