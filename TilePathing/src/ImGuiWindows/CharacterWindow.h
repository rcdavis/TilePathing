#pragma once

#include "Core.h"
#include "ImGuiWindow.h"

#include <vector>

class Character;
class TileMap;

class CharacterWindow : public ImGuiWindow
{
public:
    CharacterWindow(const bool isOpen, Ref<TileMap> tileMap);

    const std::vector<Ref<Character>>& GetCharacters() const { return mCharacters; }

    void OnRender() override;

    void AddCharacter(Ref<Character> character);

private:
    std::vector<Ref<Character>> mCharacters;
    Ref<TileMap> mTileMap;
    uint32 mCurSelected = 0;
};
