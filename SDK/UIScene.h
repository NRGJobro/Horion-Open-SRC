#pragma once

#include "TextHolder.h"

class UIScene {
public:
	bool isPlayScreen() {
		return Utils::CallVFunc<55, bool>(this);
	}

	void getScreenName(TextHolder* txt) {
		Utils::CallVFunc<66, void, TextHolder*>(this, txt);
	}
};
