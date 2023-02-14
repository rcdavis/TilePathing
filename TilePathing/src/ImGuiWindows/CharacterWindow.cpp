#include "CharacterWindow.h"

#include "Character.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLVertexArray.h"

#include "Utils/MeshUtils.h"

#include <imgui.h>

CharacterWindow::CharacterWindow(const bool isOpen, Ref<TileMap> tileMap) :
    ImGuiWindow("Character", isOpen),
    mCharacters(),
    mTileMap(tileMap),
    mCurSelected(0)
{}

void CharacterWindow::OnRender()
{
    if (ImGui::BeginListBox("Characters"))
    {
        for (uint32 i = 0; i < std::size(mCharacters); ++i)
        {
            char buffer[16] = {};
            snprintf(buffer, sizeof(buffer), "Character %u", i);
            bool isSelected = (mCurSelected == i);
            if (ImGui::Selectable(buffer, &isSelected))
                mCurSelected = i;

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndListBox();
    }

    if (mCurSelected < std::size(mCharacters))
    {
        glm::ivec2 coords = mCharacters[mCurSelected]->GetTileCoords();
        ImGui::InputInt("Row", &coords.y);
        ImGui::InputInt("Columns", &coords.x);
        mCharacters[mCurSelected]->SetTileCoords(coords);

        int32 steps = (int32)mCharacters[mCurSelected]->GetMovementSteps();
        ImGui::InputInt("Movement", &steps);
        mCharacters[mCurSelected]->SetMovementSteps((uint32)steps);
    }

    if (ImGui::Button("Add Character"))
    {
        Ref<Character> c = CreateRef<Character>();
        c->SetTexture(GLTexture::Load("assets/textures/DirectoryIcon.png"));
        c->SetVertexArray(MeshUtils::CreateColoredTileMesh(mTileMap));
        c->SetMovementSteps(6);
        AddCharacter(c);
    }

    if (mCurSelected < std::size(mCharacters) && ImGui::Button("Remove Character"))
    {
        mCharacters.erase(std::begin(mCharacters) + mCurSelected);
        mCurSelected = std::max(0, (int32)mCurSelected - 1);
    }
}

void CharacterWindow::AddCharacter(Ref<Character> character)
{
    mCharacters.push_back(character);
    mCurSelected = (uint32)std::size(mCharacters) - 1;
}

Ref<Character> CharacterWindow::GetCharacter(glm::uvec2 coords)
{
    for (const auto& c : mCharacters)
    {
        if (c->GetTileCoords() == coords)
            return c;
    }

    return nullptr;
}
