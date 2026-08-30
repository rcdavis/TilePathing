#pragma once

#include "Core.h"

#include <cstdint>
#include <glm/glm.hpp>

class GLTexture;
class GLVertexArray;

class Character {
public:
	Character() = default;

	void MoveUp(const uint32_t numRows) { tileCoords.y -= numRows; }
	void MoveDown(const uint32_t numRows) { tileCoords.y += numRows; }
	void MoveRight(const uint32_t numCol) { tileCoords.x += numCol; }
	void MoveLeft(const uint32_t numCol) { tileCoords.x -= numCol; }

public:
	glm::uvec2 tileCoords{ 0, 0 };
	Ref<GLTexture> texture;
	Ref<GLVertexArray> vao;
	uint32_t movementSteps = 0;
};
