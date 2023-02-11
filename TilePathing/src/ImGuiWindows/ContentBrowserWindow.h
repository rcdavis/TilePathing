#pragma once

#include "Core.h"
#include "ImGuiWindow.h"

#include <filesystem>

class GLTexture;

class ContentBrowserWindow : public ImGuiWindow
{
public:
    ContentBrowserWindow(const bool isOpen = false);

    constexpr const char* GetContentBrowserItemType() const { return "ContentBrowserItemType"; }

    void OnRender() override;

private:
    std::filesystem::path mCurDir;

    Ref<GLTexture> mDirIcon;
    Ref<GLTexture> mFileIcon;

    f32 mPadding = 16.0f;
    f32 mThumbnailSize = 128.0f;
};
