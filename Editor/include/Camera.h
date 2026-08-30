#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera(float left, float right, float bottom, float top);

	void SetProjection(float left, float right, float bottom, float top);

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
