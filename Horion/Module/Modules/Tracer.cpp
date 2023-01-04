#include "Tracer.h"

#include "../../../SDK/CCamera.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"

bool old = false;
Tracer::Tracer() : IModule('R', Category::VISUAL, "Draws lines to ESP highlighted entities.") {
	//registerBoolSetting("3D Tracers", &old, old);
}

Tracer::~Tracer() {
}

const char* Tracer::getModuleName() {
	return "Tracer";
}

void Tracer::onLevelRender() {
	if (old) {
		Vec3 origin = g_Data.getLocalPlayer()->level->rayHitVec;
		g_Data.forEachEntity([&](C_Entity* ent, bool valid) {
			if (ent != g_Data.getLocalPlayer() && Target::isValidTarget(ent) && g_Data.canUseMoveKeys()) {
				DrawUtils::setColor(255, 255, 255, 1);
				DrawUtils::drawLine3d(origin, *ent->getPos());
			}
		});
	}
}
std::shared_ptr<glmatrixf> refdef2;
void Tracer::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	glmatrixf* badrefdef = g_Data.getClientInstance()->getRefDef();
	refdef2 = std::shared_ptr<glmatrixf>(badrefdef->correct());

	if (!old) {
		g_Data.forEachEntity([&](C_Entity* ent, bool valid) {
			if (ent != g_Data.getLocalPlayer() && Target::isValidTarget(ent) && g_Data.canUseMoveKeys()) {
				static auto tracerMod = moduleMgr->getModule<Tracer>();
				Vec2 target;
				Vec2 screenSize;
				screenSize.x = g_Data.getGuiData()->widthGame;
				screenSize.y = g_Data.getGuiData()->heightGame;
				refdef2->OWorldToScreen(g_Data.getClientInstance()->levelRenderer->getOrigin(), ent->eyePos0, target, g_Data.getClientInstance()->getFov(), screenSize);
				Vec2 mid(((g_Data.getClientInstance()->getGuiData()->widthGame) / 2), ((g_Data.getClientInstance()->getGuiData()->heightGame) / 2));
				if (target != Vec2(0, 0)) {
					DrawUtils::setColor(255, 255, 255, 1);
					DrawUtils::drawLine(mid, target, 0.2f);
				}
			}
		});
	}
}