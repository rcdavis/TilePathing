#pragma once

#include "Input/KeyCodes.h"
#include "Input/MouseButtonCodes.h"

#include "TimeStep.h"

#include <glm/glm.hpp>

struct GLFWwindow;

namespace Input {
	void Init(GLFWwindow* window);

	void Poll(TimeStep ts);

	bool IsKeyPressed(const KeyCode keycode);
	bool IsKeyReleased(const KeyCode keycode);
	bool IsKeyDown(const KeyCode keycode, const TimeStep timeDiff = 0.0f);
	bool IsMouseButtonPressed(const MouseCode button);

	glm::vec2 GetMousePosition();
	float GetMouseX();
	float GetMouseY();
}
