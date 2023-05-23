#pragma once

class HIDController {
private:
	char pad_0x0000[0x54];  // 0x0000
public:
	union {
		struct {
			bool leftClickDown;   // 0x0054
			bool rightClickDown;  // 0x0055
			bool wheelDown;       // 0x0056
			bool mouse4Down;      // 0x0057
			bool mouse5Down;      // 0x0058
		};
		bool clickMap[5];
	};
};