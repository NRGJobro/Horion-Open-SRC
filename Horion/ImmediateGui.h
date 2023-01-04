#pragma once

#include <map>

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "DrawUtils.h"

class ComponentInfo {
public:
	ComponentInfo(int id);
	virtual ~ComponentInfo(){};

protected:
	unsigned int id;
};

class ButtonInfo : public ComponentInfo {
private:
	Vec2 pos;
	Vec2 size;
	bool centered;
	float padding = 3.f;
	bool canClickB = false;

public:
	ButtonInfo(int id, Vec2 pos, bool centered = false);
	virtual ~ButtonInfo(){};

	void calculateSize(const char*);
	bool isInSelectableSurface(Vec2 mouse);
	Vec4 getSelectableSurface();
	void draw(Vec2 mousePos, const char* label);
	bool canClick() { return canClickB; };
	void updatePos(Vec2 pos) { pos = pos; }
	
};


struct KeyInfo {
	bool isDown;     // Held down right now
	bool isClicked;  // Did it go down this frame
	bool nextIsDown;

	void update() {
		if (!isDown && nextIsDown)
			isClicked = true;
		isDown = nextIsDown;
	}

	bool trySteal() {
		if (isClicked) {
			isClicked = false;
			return true;
		}
		return false;
	}
};

class ImmediateGui {
private:
	Vec2 mousePos;
	KeyInfo leftMb;
	KeyInfo rightMb;
	std::map<unsigned int, std::shared_ptr<ComponentInfo>> components;

public:
	void onMouseClickUpdate(int key, bool isDown);
	void startFrame();
	void endFrame();
	bool Button(const char* label, Vec2 pos, bool centered = false);
};

extern ImmediateGui HImGui;