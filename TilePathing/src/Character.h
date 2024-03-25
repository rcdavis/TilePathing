#pragma once

#include "Core.h"

#include <glm/glm.hpp>

#include <vector>

class GLTexture;
class GLVertexArray;
class TimeStep;
class TileMap;

class Character
{
public:
    Character() = default;

    void Update(TimeStep ts);

    glm::vec2 GetPos() const { return mPos; }
    void SetPos(glm::vec2 pos) { mPos = pos; }

    glm::uvec2 GetTileCoords() const { return mTileCoords; }
    void SetTileCoords(glm::uvec2 coords) { mTileCoords = coords; }

    Ref<GLTexture> GetTexture() { return mTexture; }
    void SetTexture(Ref<GLTexture> texture) { mTexture = texture; }

    Ref<GLVertexArray> GetVertexArray() { return mVAO; }
    void SetVertexArray(Ref<GLVertexArray> vao) { mVAO = vao; }

    uint32 GetMovementSteps() const { return mMovementSteps; }
    void SetMovementSteps(const uint32 steps) { mMovementSteps = steps; }

    void SetMovementPath(const std::vector<glm::uvec2>& path) { mMovementPath = path; }
    void SetTileMap(Ref<TileMap> tileMap) { mTileMap = tileMap; }

private:
    std::vector<glm::uvec2> mMovementPath;
    glm::uvec2 mTileCoords{ 0, 0 };
    glm::vec2 mPos{ 0.0f, 0.0f };
    Ref<GLTexture> mTexture;
    Ref<GLVertexArray> mVAO;
    Ref<TileMap> mTileMap;
    uint32 mMovementSteps = 0;
};
