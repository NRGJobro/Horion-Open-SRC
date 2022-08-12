#include "EditionFaker.h"

EditionFaker::EditionFaker() : IModule(0, Category::MISC, "Fakes your device.") {
	edition = SettingEnum(this)
		.addEntry(EnumEntry("Unknown", 0))
		.addEntry(EnumEntry("Android", 1))
		.addEntry(EnumEntry("iOS", 2))
		.addEntry(EnumEntry("macOS", 3)) // OS_X
		.addEntry(EnumEntry("FireOS", 4))
		.addEntry(EnumEntry("GearVR", 5))
		.addEntry(EnumEntry("Hololens", 6))
		.addEntry(EnumEntry("Win10", 7)) // Windows 10 (PC)
		.addEntry(EnumEntry("Win32", 8)) // Windows
		.addEntry(EnumEntry("Linux", 9)) // Dedicated
		.addEntry(EnumEntry("TvOS", 10))
		.addEntry(EnumEntry("Playstation", 11)) // Orbis
		.addEntry(EnumEntry("Nintendo Switch", 12)) // NX								
		.addEntry(EnumEntry("Xbox", 13))
		.addEntry(EnumEntry("Windows Phone", 14));
	registerEnumSetting("Edition", &edition, 1);
	inputMode = SettingEnum(this)
		.addEntry(EnumEntry("Unknown", 0))
		.addEntry(EnumEntry("Mouse", 1))
		.addEntry(EnumEntry("Touch", 2))
		.addEntry(EnumEntry("Gamepad", 3))
		.addEntry(EnumEntry("Motion controller", 4));
	registerEnumSetting("InputMode", &inputMode, 2);
}

EditionFaker::~EditionFaker() {
}

int EditionFaker::getFakedEditon() {
	// static const char* gameEditons[] = { "Unknown", "Android", "iOS", "macOS", "FireOS", "GearVR", "HoloLens", "Windows 10 (PC)", "Windows", "Dedicated", "Orbis", "NX" };

	return edition.GetSelectedEntry().GetValue();
}

int EditionFaker::getFakedInputMode() {
	return inputMode.selected;
}

const char* EditionFaker::getModuleName() {
	return ("EditionFaker");
}
