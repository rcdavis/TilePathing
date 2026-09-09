#pragma once

#include <filesystem>

struct ContentBrowserWindow {
	static constexpr const char* Title = "Content Browser";
	static constexpr const char* ContentBrowserItemType = "ContentBrowserItemType";

	std::filesystem::path curDir = "res";

	float padding = 16.0f;
	float thumbnailSize = 74.0f;

	bool isOpen = true;

	void Render();

	void RenderMenuItem();
};
