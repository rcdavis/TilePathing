#include "ImGuiWindows/CharacterWindow.h"

#include "Character.h"
#include "TextureIds.h"

#include "OpenGL/GLTexture.h"
#include "OpenGL/GLVertexArray.h"

#include "TileMap/TileMap.h"

#include "Utils/MeshUtils.h"

#include <cstdint>
#include <imgui.h>

void CharacterWindow::Render() {
	if (!isOpen)
		return;

	if (ImGui::Begin(Title, &isOpen)) {
		if (ImGui::BeginListBox("Characters")) {
			for (uint32_t i = 0; i < std::size(characters); ++i) {
				char buffer[16] = {};
				snprintf(buffer, sizeof(buffer), "Character %u", i);
				bool isSelected = (curSelected == i);
				if (ImGui::Selectable(buffer, &isSelected))
					curSelected = i;

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndListBox();
		}

		if (curSelected < std::size(characters)) {
			glm::ivec2 coords = characters[curSelected].tileCoords;
			ImGui::InputInt("Row", &coords.y);
			ImGui::InputInt("Columns", &coords.x);
			characters[curSelected].tileCoords = coords;

			int32_t steps = (int32_t)characters[curSelected].movementSteps;
			ImGui::InputInt("Movement", &steps);
			characters[curSelected].movementSteps = (uint32_t)steps;
		}

		if (ImGui::Button("Add Character") && tileMap) {
			constexpr auto dirIconPath = Res::Textures::GetPath(Res::Textures::Id::DirectoryIcon);
			Character c;
			c.texture = GLTexture::Load(dirIconPath);
			c.vao = MeshUtils::CreateColoredTileMesh(*tileMap);
			c.movementSteps = 6;
			AddCharacter(c);
		}

		if (curSelected < std::size(characters) && ImGui::Button("Remove Character")) {
			characters.erase(std::begin(characters) + curSelected);
			curSelected = std::max(0, (int32_t)curSelected - 1);
		}
	}

	ImGui::End();
}

void CharacterWindow::RenderMenuItem() {
	ImGui::MenuItem(Title, nullptr, &isOpen);
}

void CharacterWindow::AddCharacter(Character& character) {
	characters.push_back(character);
	curSelected = (uint32_t)std::size(characters) - 1;
}

Character* CharacterWindow::GetCharacter(glm::uvec2 coords) {
	for (auto& c : characters) {
		if (c.tileCoords == coords)
			return &c;
	}

	return nullptr;
}
