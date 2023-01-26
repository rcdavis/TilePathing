#include "GLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Log.h"

GLTexture::GLTexture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
}

GLTexture::GLTexture(const std::filesystem::path& filepath)
{
    int32 width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* const data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        LOG_ERROR("Failed to load image {0}", filepath.filename().string());
        return;
    }

    mIsLoaded = true;

    mWidth = width;
    mHeight = height;

    if (channels == 4)
    {
        mInternalFormat = GL_RGBA8;
        mDataFormat = GL_RGBA;
    }
    else if (channels == 3)
    {
        mInternalFormat = GL_RGB8;
        mDataFormat = GL_RGB;
    }

    assert(mInternalFormat & mDataFormat && "Format not supported");

    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
    glTextureStorage2D(mId, 1, mInternalFormat, mWidth, mHeight);

    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &mId);
}

void GLTexture::Bind(uint32 slot) const
{
    glBindTexture(GL_TEXTURE_2D, mId);
}

void GLTexture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Ref<GLTexture> GLTexture::Load(const std::filesystem::path& filepath)
{
    return CreateRef<GLTexture>(filepath);
}
