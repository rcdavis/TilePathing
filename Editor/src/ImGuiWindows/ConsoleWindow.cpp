#include "ImGuiWindows/ConsoleWindow.h"

#include <imgui.h>

void ConsoleWindow::AddLine(const ConsoleItem& item) {
	if ((uint32_t)std::size(items) == limit)
		items.pop_front();

	items.push_back(item);
}

void ConsoleWindow::Render() {
	if (!isOpen)
		return;

	if (ImGui::Begin(Title, &isOpen)) {
		if (ImGui::BeginChild("ConsoleLines")) {
			for (const ConsoleItem& item : items) {
				const ImVec4 color(item.color.r, item.color.g, item.color.b, 1.0f);
				ImGui::TextColored(color, "[%s] %s", std::data(item.tag), std::data(item.text));
			}
		}

		ImGui::EndChild();
	}

	ImGui::End();
}
