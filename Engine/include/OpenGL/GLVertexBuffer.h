#pragma once

#include "Core.h"

#include "OpenGL/GLBufferLayout.h"
#include <cstdint>

class GLVertexBuffer {
public:
	GLVertexBuffer(uint32_t size);
	GLVertexBuffer(const float *const vertices, const uint32_t count);
	~GLVertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void *data, const uint32_t size);

	const GLBufferLayout &GetLayout() const { return mLayout; }
	void SetLayout(const GLBufferLayout &layout) { mLayout = layout; }

	static Ref<GLVertexBuffer> Create(uint32_t size);
	static Ref<GLVertexBuffer> Create(const float *const vertices, uint32_t count);

private:
	uint32_t mId = 0;
	GLBufferLayout mLayout;
};
