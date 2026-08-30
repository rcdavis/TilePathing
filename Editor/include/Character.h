#pragma once

#include "Core.h"

#include <glm/glm.hpp>

class GLTexture;
class GLVertexArray;

class Character {
public:
	Character() = default;

	void MoveUp(const uint32 numRows) { tileCoords.y -= numRows; }
	void MoveDown(const uint32 numRows) { tileCoords.y += numRows; }
	void MoveRight(const uint32 numCol) { tileCoords.x += numCol; }
	void MoveLeft(const uint32 numCol) { tileCoords.x -= numCol; }

public:
	glm::uvec2 tileCoords{ 0.0f, 0.0f };
	Ref<GLTexture> texture;
	Ref<GLVertexArray> vao;
	uint32 movementSteps = 0;
};
