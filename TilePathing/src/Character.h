#pragma once

#include "Core.h"

#include <glm/glm.hpp>

class GLTexture;
class GLVertexArray;

class Character
{
public:
    Character() = default;

    glm::uvec2 GetTileCoords() const { return mTileCoords; }
    void SetTileCoords(glm::uvec2 coords) { mTileCoords = coords; }

    void MoveUp(const uint32 numRows) { mTileCoords.y -= numRows; }
    void MoveDown(const uint32 numRows) { mTileCoords.y += numRows; }
    void MoveRight(const uint32 numCol) { mTileCoords.x += numCol; }
    void MoveLeft(const uint32 numCol) { mTileCoords.x -= numCol; }

    Ref<GLTexture> GetTexture() { return mTexture; }
    void SetTexture(Ref<GLTexture> texture) { mTexture = texture; }

    Ref<GLVertexArray> GetVertexArray() { return mVAO; }
    void SetVertexArray(Ref<GLVertexArray> vao) { mVAO = vao; }

private:
    glm::uvec2 mTileCoords{ 0.0f, 0.0f };
    Ref<GLTexture> mTexture;
    Ref<GLVertexArray> mVAO;
};
