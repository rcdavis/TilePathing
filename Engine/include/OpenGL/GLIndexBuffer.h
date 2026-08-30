#pragma once

#include "Core.h"
#include <cstdint>

class GLIndexBuffer {
public:
	GLIndexBuffer(const uint16_t *const indices, const uint32_t count);
	~GLIndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() const { return mCount; }

	static Ref<GLIndexBuffer> Create(const uint16_t *const indices, const uint32_t count);

private:
	uint32_t mId = 0;
	uint32_t mCount = 0;
};
