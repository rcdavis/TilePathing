#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float left, float right, float bottom, float top) :
	mProjection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
	mView(1.0f),
	mPosition(0.0f)
{
	mViewProjection = mProjection * mView;
}

void Camera::SetProjection(float left, float right, float bottom, float top) {
	mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	mViewProjection = mProjection * mView;
}

void Camera::RecalculateViewMatrix() {
	mView = glm::inverse(glm::translate(glm::mat4(1.0f), mPosition));
	mViewProjection = mProjection * mView;
}
