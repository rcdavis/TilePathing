#include "OpenGL/GLTexture.h"

#include <cstdint>
#include <glad/glad.h>
#include <memory>
#include <stb_image.h>

#include "Utils/Log.h"

GLTexture::GLTexture() {
	glCreateTextures(GL_TEXTURE_2D, 1, &mId);
}

GLTexture::GLTexture(const std::filesystem::path &filepath) {
	const auto stbiDeleter = [&filepath](stbi_uc* data) {
		LOG_INFO("Freeing image data for {0}", filepath.filename().string());
		stbi_image_free(data);
	};

	int32_t width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	std::unique_ptr<stbi_uc, decltype(stbiDeleter)> data(
		stbi_load(filepath.string().c_str(), &width, &height, &channels, 0),
		stbiDeleter
	);
	if (!data) {
		LOG_ERROR("Failed to load image {0}", filepath.filename().string());
		return;
	}

	mIsLoaded = true;

	mWidth = width;
	mHeight = height;

	if (channels == 4) {
		mInternalFormat = GL_RGBA8;
		mDataFormat = GL_RGBA;
	} else if (channels == 3) {
		mInternalFormat = GL_RGB8;
		mDataFormat = GL_RGB;
	}

	assert(mInternalFormat & mDataFormat && "Format not supported");

	glCreateTextures(GL_TEXTURE_2D, 1, &mId);
	glTextureStorage2D(mId, 1, mInternalFormat, mWidth, mHeight);
	glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data.get());
	glGenerateTextureMipmap(mId);

	glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLTexture::~GLTexture() {
	glDeleteTextures(1, &mId);
}

void GLTexture::Bind(uint32_t slot) const {
	glBindTextureUnit(slot, mId);
}

void GLTexture::Unbind() const {
	glBindTextureUnit(0, 0);
}

bool GLTexture::operator==(const GLTexture &texture) const {
	if (this == &texture)
		return true;

	return mId == texture.mId;
}

Ref<GLTexture> GLTexture::Load(const std::filesystem::path &filepath) {
	return CreateRef<GLTexture>(filepath);
}
