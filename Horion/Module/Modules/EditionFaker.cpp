#include "EditionFaker.h"

EditionFaker::EditionFaker() : IModule(0, Category::MISC, "Fakes your device") {
		edition = SettingEnum(this)
				.addEntry(EnumEntry("Unknown", 0))
				.addEntry(EnumEntry("Android", 1))
				.addEntry(EnumEntry("iOS", 2))
				.addEntry(EnumEntry("MacOS", 3))
				.addEntry(EnumEntry("Win10", 7))
				.addEntry(EnumEntry("Windows", 8))
				.addEntry(EnumEntry("Linux", 9))
				.addEntry(EnumEntry("Xbox", 13))
				.addEntry(EnumEntry("WindowsPhone", 14));
		registerEnumSetting("Edition", &edition, 1);
}

EditionFaker::~EditionFaker() {
}

int EditionFaker::getFakedEditon() {
	// static const char* gameEditons[] = { "Unknown", "Android", "iOS", "macOS", "FireOS", "GearVR", "HoloLens", "Windows 10 (PC)", "Windows", "Dedicated", "Orbis", "NX" };

	return edition.GetSelectedEntry().GetValue();
}

const char* EditionFaker::getModuleName() {
	return ("EditionFaker");
}
