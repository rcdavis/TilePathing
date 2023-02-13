#include "ContentBrowserWindow.h"

#include <imgui.h>

#include "OpenGL/GLTexture.h"

ContentBrowserWindow::ContentBrowserWindow(const bool isOpen) :
    ImGuiWindow("Content Browser", isOpen),
    mCurDir("assets"),
    mDirIcon(GLTexture::Load("assets/textures/DirectoryIcon.png")),
    mFileIcon(GLTexture::Load("assets/textures/FileIcon.png")),
    mPadding(16.0f),
    mThumbnailSize(74.0f)
{}

void ContentBrowserWindow::OnRender()
{
    if (mCurDir != "assets" && ImGui::Button("<-"))
        mCurDir = mCurDir.parent_path();

    const f32 cellSize = mThumbnailSize + mPadding;
    const f32 panelWidth = ImGui::GetContentRegionAvail().x;
    const int32 columnCount = std::max((int32)(panelWidth / cellSize), 1);

    ImGui::Columns(columnCount, nullptr, false);

    for (const auto& dirEntry : std::filesystem::directory_iterator(mCurDir))
    {
        const auto& path = dirEntry.path();
        const auto relativePath = std::filesystem::relative(path, "assets");
        const auto filenameStr = relativePath.filename().string();
        const Ref<GLTexture>& icon = dirEntry.is_directory() ? mDirIcon : mFileIcon;

        ImGui::PushID(filenameStr.c_str());
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        ImGui::ImageButton((ImTextureID)icon->GetId(),
            { mThumbnailSize, mThumbnailSize }, { 0, 1 }, { 1, 0 });

        if (ImGui::BeginDragDropSource())
        {
            const wchar_t* const itemPath = relativePath.c_str();
            ImGui::SetDragDropPayload(ContentBrowserItemType, itemPath,
                (wcslen(itemPath) + 1) * sizeof(wchar_t));

            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (dirEntry.is_directory())
                mCurDir /= path.filename();
        }

        ImGui::TextWrapped(filenameStr.c_str());

        ImGui::NextColumn();

        ImGui::PopID();
    }

    ImGui::Columns(1);

    ImGui::SliderFloat("Thumbnail Size", &mThumbnailSize, 16, 512);
    ImGui::SliderFloat("Padding", &mPadding, 0, 32);
}
