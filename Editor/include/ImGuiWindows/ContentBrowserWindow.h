#pragma once

#include "Core.h"

#include <filesystem>

class GLTexture;

struct ContentBrowserWindow {
	static constexpr const char* Title = "Content Browser";
	static constexpr const char* ContentBrowserItemType = "ContentBrowserItemType";

	std::filesystem::path curDir = "res";

	Ref<GLTexture> dirIcon;
	Ref<GLTexture> fileIcon;

	float padding = 16.0f;
	float thumbnailSize = 74.0f;

	bool isOpen = true;

	void Render();

	void RenderMenuItem();
};
