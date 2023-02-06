#pragma once

#include "../Utils/HMath.h"

class Tessellator {
private:
	char pad_0000[0x128];
	Vec3 transformOffset; // 0x128

public:
	void addPostTransformOffset(int x, int y, int z) {
		addPostTransformOffset((float)x, (float)y, (float)z);
	}
	void addPostTransformOffset(float x, float y, float z);
};

class Block;

class Mesh;

class BlockTessellator {
public:
	Mesh* getMeshForBlockInWorld(Tessellator * t, Block * block, Vec3i & pos);
};