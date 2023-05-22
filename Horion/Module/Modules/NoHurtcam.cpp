#include "NoHurtcam.h"

NoHurtcam::~NoHurtcam() {
}
NoHurtcam::NoHurtcam() : IModule(0x0, Category::VISUAL, "Disables rotation of the camera when hit.") {
}
const char* NoHurtcam::getModuleName() {
	return "NoHurtcam";
}
