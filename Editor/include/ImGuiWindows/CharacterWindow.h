#pragma once

#include "Character.h"

#include <glm/glm.hpp>

#include <vector>

class TileMap;

struct CharacterWindow {
	static constexpr const char* Title = "Character";

	std::vector<Character> characters;
	TileMap* tileMap = nullptr;
	uint32_t curSelected = 0;
	bool isOpen = true;

	void Render();

	void RenderMenuItem();

	Character* GetCharacter(glm::uvec2 coords);

	void AddCharacter(Character& character);
};
