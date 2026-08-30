#pragma once

#include "Core.h"
#include "ImGuiWindows/BaseImGuiWindow.h"

#include "Character.h"

#include <glm/glm.hpp>

#include <vector>

class TileMap;

class CharacterWindow : public BaseImGuiWindow {
public:
	CharacterWindow(const bool isOpen, TileMap& tileMap);

	const std::vector<Character>& GetCharacters() const { return mCharacters; }
	Character* GetCharacter(glm::uvec2 coords);

	void OnRender() override;

	void AddCharacter(Character& character);

private:
	std::vector<Character> mCharacters;
	TileMap& mTileMap;
	uint32 mCurSelected = 0;
};
