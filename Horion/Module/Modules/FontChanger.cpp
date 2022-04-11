#include "FontChanger.h"

FontChanger::FontChanger() : IModule(0, Category::CLIENT, "Font Changer.") {
	registerEnumSetting("Fonts", &Fonts, 0);
	Fonts = SettingEnum(this)
				.addEntry(EnumEntry("Notosans", 0))
				.addEntry(EnumEntry("Mojangles", 1));
}

FontChanger::~FontChanger() {
}

const char* FontChanger::getModuleName() {
	return ("CustomFont");
}