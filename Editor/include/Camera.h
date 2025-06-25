#pragma once

#include <glm/glm.hpp>

#include "Core.h"

class Camera
{
public:
    Camera(f32 left, f32 right, f32 bottom, f32 top);

    void SetProjection(f32 left, f32 right, f32 bottom, f32 top);

    const glm::vec3& GetPosition() const { return mPosition; }
    void SetPosition(const glm::vec3& position) { mPosition = position; RecalculateViewMatrix(); }

    const glm::mat4& GetProjection() const { return mProjection; }
    const glm::mat4& GetView() const { return mView; }
    const glm::mat4& GetViewProjection() const { return mViewProjection; }

private:
    void RecalculateViewMatrix();

private:
    glm::mat4 mProjection;
    glm::mat4 mView;
    glm::mat4 mViewProjection;

    glm::vec3 mPosition;
};
