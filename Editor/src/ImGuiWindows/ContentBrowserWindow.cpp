#include "ImGuiWindows/ContentBrowserWindow.h"

#include <cstdint>
#include <imgui.h>

#include "OpenGL/GLTexture.h"
#include "TextureIds.h"

/*ContentBrowserWindow::ContentBrowserWindow(const bool isOpen) :
	BaseImGuiWindow("Content Browser", isOpen),
	mCurDir("res"),
	mDirIcon(GLTexture::Load(Res::Textures::GetPath(Res::Textures::Id::DirectoryIcon))),
	mFileIcon(GLTexture::Load(Res::Textures::GetPath(Res::Textures::Id::FileIcon))),
	mPadding(16.0f),
	mThumbnailSize(74.0f)
{}*/

void ContentBrowserWindow::Render() {
	if (!isOpen)
		return;

	if (ImGui::Begin(Title, &isOpen)) {
		if (curDir != "res" && ImGui::Button("<-"))
			curDir = curDir.parent_path();

		const float cellSize = thumbnailSize + padding;
		const float panelWidth = ImGui::GetContentRegionAvail().x;
		const int32_t columnCount = std::max((int32_t)(panelWidth / cellSize), 1);

		ImGui::Columns(columnCount, nullptr, false);

		for (const auto& dirEntry : std::filesystem::directory_iterator(curDir)) {
			const auto& path = dirEntry.path();
			const auto relativePath = std::filesystem::relative(path, "res");
			const auto filenameStr = relativePath.filename().string();
			const Ref<GLTexture>& icon = dirEntry.is_directory() ? dirIcon : fileIcon;

			ImGui::PushID(filenameStr.c_str());
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			ImGui::ImageButton("icon_content_browser", (ImTextureID)icon->GetId(),
				{ thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource()) {
				const char* const itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload(ContentBrowserItemType, itemPath,
					(strlen(itemPath) + 1) * sizeof(char));

				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (dirEntry.is_directory())
					curDir /= path.filename();
			}

			ImGui::TextWrapped("%s", filenameStr.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);
	}

	ImGui::End();
}

void ContentBrowserWindow::RenderMenuItem() {
	ImGui::MenuItem(Title, nullptr, &isOpen);
}
