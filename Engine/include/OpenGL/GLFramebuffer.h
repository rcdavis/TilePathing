#pragma once

#include "Core.h"

#include <vector>

enum class FramebufferTextureFormat {
	None,
	RGBA8,
	RedInt32,
	Depth24Stencil8,
	Depth = Depth24Stencil8
};

struct FramebufferTextureSpecs {
	constexpr FramebufferTextureSpecs() = default;
	constexpr FramebufferTextureSpecs(const FramebufferTextureFormat format) : textureFormat(format) {}

	FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification {
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecs> &attachments) : attachments(attachments) {}

	std::vector<FramebufferTextureSpecs> attachments;
};

struct FramebufferSpecs {
	FramebufferAttachmentSpecification attachments;
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t samples = 1;
	bool swapChainTarget = false;
};

class GLFramebuffer {
public:
	GLFramebuffer(const FramebufferSpecs &specs);
	~GLFramebuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	void Resize(const uint32_t width, const uint32_t height);
	int32_t ReadPixel(const uint32_t attachmentIndex, const int32_t x, const int32_t y);

	void ClearAttachment(const uint32_t attachmentIndex, const int32_t value);

	uint32_t GetColorAttachment(const uint32_t index = 0) const {
		return mColorAttachments[index];
	}

	const FramebufferSpecs &GetSpecs() const { return mSpecs; }

	static Ref<GLFramebuffer> Create(const FramebufferSpecs &specs);

private:
	std::vector<uint32_t> mColorAttachments;
	std::vector<FramebufferTextureSpecs> mColorAttachmentSpecs;

	FramebufferSpecs mSpecs;

	uint32_t mId = 0;
	uint32_t mDepthAttachment = 0;
	FramebufferTextureSpecs mDepthAttachmentSpecs = FramebufferTextureFormat::None;
};
