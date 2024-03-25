#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Input/Input.h"

Camera::Camera(f32 left, f32 right, f32 bottom, f32 top) :
    mProjection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
    mView(1.0f),
    mPosition(0.0f)
{
    mViewProjection = mProjection * mView;
}

void Camera::SetProjection(f32 left, f32 right, f32 bottom, f32 top)
{
    mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    mViewProjection = mProjection * mView;
}

void Camera::RecalculateViewMatrix()
{
    mView = glm::inverse(glm::translate(glm::mat4(1.0f), mPosition));
    mViewProjection = mProjection * mView;
}
