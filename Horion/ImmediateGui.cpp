#include "ImmediateGui.h"

ImmediateGui HImGui;

ComponentInfo::ComponentInfo(int id) : id(id) {
}

ButtonInfo::ButtonInfo(int id, Vec2 pos, bool centered) : ComponentInfo(id), pos(pos), centered(centered) {
}

void ButtonInfo::calculateSize(const char* txt) {
	std::string str(txt);
	size = {DrawUtils::getTextWidth(&str), DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight()};
}

bool ButtonInfo::isInSelectableSurface(Vec2 mouse) {
	Vec4 surface = getSelectableSurface();
	return surface.contains(&mouse);
}

Vec4 ButtonInfo::getSelectableSurface() {
	if (centered) {
		return {pos.x - padding - size.x / 2,
				pos.y - padding - size.y / 2,
				pos.x + padding + size.x / 2,
				pos.y + padding + size.y / 2};
	} else {
		return {pos.x - padding,
				pos.y - padding,
				pos.x + padding + size.x,
				pos.y + padding + size.y};
	}
}

void ButtonInfo::draw(Vec2 mousePos, const char* label) {
	calculateSize(label);
	auto surface = getSelectableSurface();
	Vec2 textPos = pos;
	std::string str(label);
	if (centered) {
		textPos.x -= DrawUtils::getTextWidth(&str) / 2;
		textPos.y -= DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2;
	}
		
	DrawUtils::drawText(textPos, &str, MC_Color());
	if (isInSelectableSurface(mousePos)) {  // Mouse hovering over us
		DrawUtils::fillRectangle(surface, MC_Color(85, 85, 85), 1);
		canClickB = true;
	} else {
		DrawUtils::fillRectangle(surface, MC_Color(12, 12, 12), 1);
		canClickB = false;
	}		
}

void ImmediateGui::onMouseClickUpdate(int key, bool isDown) {
	switch (key) {
	case 1:  // Left Click
		leftMb.nextIsDown = true;
		break;
	case 2:  // Right Click
		rightMb.nextIsDown = true;
		break;
	}
}

void ImmediateGui::startFrame() {
	Vec2 windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	Vec2 windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	mousePos = *g_Data.getClientInstance()->getMousePos();
	mousePos = mousePos.div(windowSizeReal);
	mousePos = mousePos.mul(windowSize);

	leftMb.update();
	rightMb.update();

	if (g_Data.getClientInstance()->getMouseGrabbed()) {
		leftMb.isClicked = false;
		rightMb.isClicked = false;

		mousePos = {-1000, 1000};
	}
}

void ImmediateGui::endFrame() {
	leftMb.isClicked = false;
	rightMb.isClicked = false;
}

bool ImmediateGui::Button(const char* label, Vec2 pos, bool centered) {
	auto id = Utils::getCrcHash(label);
	if (components.find(id) == components.end()) {  // Create new button
		components[id] = std::make_shared<ButtonInfo>(id, pos, centered);
	}
	auto comp = components[id];
	auto button = dynamic_cast<ButtonInfo*>(comp.get());

	button->updatePos(pos);
	button->draw(mousePos, label);
	if (button->canClick() && leftMb.trySteal()) {  // Click
		return true;
	}

	return false;
}
