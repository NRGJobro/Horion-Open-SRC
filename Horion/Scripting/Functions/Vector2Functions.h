#pragma once

#include "../../../Utils/HMath.h"
#include "../ScriptManager.h"
#include <optional>

class Vector2Functions {
public:
	static std::optional<Vec2> getVec2FromValue(JsValueRef);
	static std::optional<Vec2> getVec2FromArguments(JsValueRef*, int argCount, int* nextArg = nullptr);

	DECL_FUN(isValid);
	DECL_FUN(getX);
	DECL_FUN(getY);
	DECL_FUN(toString);
	DECL_FUN(constructor);

	DECL_FUN(add);
	DECL_FUN(sub);
	DECL_FUN(div);
	DECL_FUN(mul);
};
