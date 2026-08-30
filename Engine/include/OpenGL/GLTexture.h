#pragma once

#include <cstdint>
#include <filesystem>

#include "Core.h"

class GLTexture {
public:
	GLTexture();
	GLTexture(const std::filesystem::path &filepath);
	~GLTexture();

	uint32_t GetId() const { return mId; }
	uint32_t GetWidth() const { return mWidth; }
	uint32_t GetHeight() const { return mHeight; }

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	bool IsLoaded() const { return mIsLoaded; }

	bool operator==(const GLTexture &texture) const;

	static Ref<GLTexture> Load(const std::filesystem::path &filepath);

private:
	uint32_t mId = 0;
	uint32_t mWidth = 0;
	uint32_t mHeight = 0;
	uint32_t mInternalFormat = 0;
	uint32_t mDataFormat = 0;
	bool mIsLoaded = false;
};
