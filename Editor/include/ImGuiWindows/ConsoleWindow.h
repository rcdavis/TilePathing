#pragma once

#include <deque>
#include <string>

#include <glm/glm.hpp>

struct ConsoleWindow {
	static constexpr const char* Title = "Console";

	struct ConsoleItem {
		std::string tag;
		std::string text;
		glm::vec3 color{ 1.0f };
	};

	std::deque<ConsoleItem> items;
	uint32_t limit = 100;
	bool isOpen = false;

	void Render();

	void RenderMenuItem();

	void AddLine(const ConsoleItem& item);

	void Clear() { items.clear(); }
};
