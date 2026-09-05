#include "ImGuiWindows/TileMapPathsWindow.h"

#include <cstdint>
#include <cstdio>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

void TileMapPathsWindow::Render() {
	if (!isOpen)
		return;

	if (ImGui::Begin(Title, &isOpen)) {
		if (ImGui::BeginListBox("Paths")) {
			for (uint32_t i = 0; i < std::size(paths); ++i) {
				char buffer[16] = {};
				snprintf(buffer, sizeof(buffer), "Path %u", i);
				bool isSelected = (curSelected == i);
				if (ImGui::Selectable(buffer, &isSelected))
					curSelected = i;

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndListBox();
		}

		if (curSelected < std::size(paths)) {
			int32_t startRow = paths[curSelected].start.y;
			if (ImGui::InputInt("Start Row", &startRow))
				paths[curSelected].start.y = startRow;

			int32_t startCol = paths[curSelected].start.x;
			if (ImGui::InputInt("Start Column", &startCol))
				paths[curSelected].start.x = startCol;

			int32_t endRow = paths[curSelected].end.y;
			if (ImGui::InputInt("End Row", &endRow))
				paths[curSelected].end.y = endRow;

			int32_t endCol = paths[curSelected].end.x;
			if (ImGui::InputInt("End Column", &endCol))
				paths[curSelected].end.x = endCol;
		}

		if (ImGui::Button("Add Path")) {
			paths.push_back({ {0, 0}, {0, 5} });
			curSelected = (uint32_t)std::size(paths) - 1;
		}

		if (curSelected < std::size(paths) && ImGui::Button("Remove Path")) {
			paths.erase(std::begin(paths) + curSelected);
			curSelected = std::max(0, (int32_t)curSelected - 1);
		}
	}

	ImGui::End();
}

void TileMapPathsWindow::RenderMenuItem() {
	ImGui::MenuItem(Title, nullptr, &isOpen);
}
