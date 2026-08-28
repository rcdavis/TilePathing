#include "Input/Input.h"

#include <GLFW/glfw3.h>

namespace Input {
	static GLFWwindow* s_Window = nullptr;
	static TimeStep s_CurTime = 0.0f;

	void Init(GLFWwindow* window) {
		s_Window = window;
	}

	void Poll(TimeStep ts) {
		s_CurTime += ts;
	}

	bool IsKeyPressed(const KeyCode keycode) {
		auto state = glfwGetKey(s_Window, keycode);
		return state == GLFW_PRESS;
	}

	bool IsKeyReleased(const KeyCode keycode) {
		auto state = glfwGetKey(s_Window, keycode);
		return state == GLFW_RELEASE;
	}

	bool IsKeyDown(const KeyCode keycode, const TimeStep timeDiff) {
		if (s_CurTime <= timeDiff)
			return false;

		auto state = glfwGetKey(s_Window, keycode);
		if (state == GLFW_PRESS || state == GLFW_REPEAT) {
			s_CurTime = 0.0f;
			return true;
		}
		return false;
	}

	bool IsMouseButtonPressed(const MouseCode button) {
		auto state = glfwGetMouseButton(s_Window, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 GetMousePosition() {
		double x, y;
		glfwGetCursorPos(s_Window, &x, &y);
		return { (float)x, (float)y };
	}

	f32 GetMouseX() {
		return GetMousePosition().x;
	}

	f32 GetMouseY() {
		return GetMousePosition().y;
	}
}
