#pragma once

#include "Core.h"
#include "ImGuiWindows/BaseImGuiWindow.h"

#include <glm/glm.hpp>

#include <vector>

class Character;
class TileMap;

class CharacterWindow : public BaseImGuiWindow {
public:
	CharacterWindow(const bool isOpen, TileMap& tileMap);

	const std::vector<Ref<Character>>& GetCharacters() const { return mCharacters; }
	Ref<Character> GetCharacter(glm::uvec2 coords);

	void OnRender() override;

	void AddCharacter(Ref<Character> character);

private:
	std::vector<Ref<Character>> mCharacters;
	TileMap& mTileMap;
	uint32 mCurSelected = 0;
};
