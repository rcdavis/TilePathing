#pragma once

#include "Core.h"
#include "ImGuiWindows/BaseImGuiWindow.h"

#include <filesystem>

class GLTexture;

class ContentBrowserWindow : public BaseImGuiWindow
{
public:
    static constexpr auto ContentBrowserItemType = "ContentBrowserItemType";

public:
    ContentBrowserWindow(const bool isOpen = false);

    void OnRender() override;

private:
    std::filesystem::path mCurDir;

    Ref<GLTexture> mDirIcon;
    Ref<GLTexture> mFileIcon;

    f32 mPadding = 16.0f;
    f32 mThumbnailSize = 128.0f;
};
