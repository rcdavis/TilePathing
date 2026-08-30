#include "ImGuiWindows/CharacterWindow.h"

#include "Character.h"
#include "TextureIds.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLVertexArray.h"

#include "TileMap/TileMap.h"

#include "Utils/MeshUtils.h"

#include <cstdint>
#include <imgui.h>

CharacterWindow::CharacterWindow(const bool isOpen, TileMap& tileMap) :
	BaseImGuiWindow("Character", isOpen),
	mCharacters(),
	mTileMap(tileMap),
	mCurSelected(0)
{}

void CharacterWindow::OnRender() {
	if (ImGui::BeginListBox("Characters")) {
		for (uint32_t i = 0; i < std::size(mCharacters); ++i) {
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

	if (mCurSelected < std::size(mCharacters)) {
		glm::ivec2 coords = mCharacters[mCurSelected].tileCoords;
		ImGui::InputInt("Row", &coords.y);
		ImGui::InputInt("Columns", &coords.x);
		mCharacters[mCurSelected].tileCoords = coords;

		int32_t steps = (int32_t)mCharacters[mCurSelected].movementSteps;
		ImGui::InputInt("Movement", &steps);
		mCharacters[mCurSelected].movementSteps = (uint32_t)steps;
	}

	if (ImGui::Button("Add Character")) {
		constexpr auto dirIconPath = Res::Textures::GetPath(Res::Textures::Id::DirectoryIcon);
		Character c;
		c.texture = GLTexture::Load(dirIconPath);
		c.vao = MeshUtils::CreateColoredTileMesh(mTileMap);
		c.movementSteps = 6;
		AddCharacter(c);
	}

	if (mCurSelected < std::size(mCharacters) && ImGui::Button("Remove Character")) {
		mCharacters.erase(std::begin(mCharacters) + mCurSelected);
		mCurSelected = std::max(0, (int32_t)mCurSelected - 1);
	}
}

void CharacterWindow::AddCharacter(Character& character) {
	mCharacters.push_back(character);
	mCurSelected = (uint32_t)std::size(mCharacters) - 1;
}

Character* CharacterWindow::GetCharacter(glm::uvec2 coords) {
	for (auto& c : mCharacters) {
		if (c.tileCoords == coords)
			return &c;
	}

	return nullptr;
}
