#include "OpenGL/GLIndexBuffer.h"

#include <cstdint>
#include <glad/glad.h>

GLIndexBuffer::GLIndexBuffer(const uint16_t *const indices, const uint32_t count) : mCount(count) {
	glCreateBuffers(1, &mId);
	glNamedBufferData(mId, sizeof(uint16_t) * count, indices, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() {
	glDeleteBuffers(1, &mId);
}

void GLIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
}

void GLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Ref<GLIndexBuffer> GLIndexBuffer::Create(const uint16_t *const indices, const uint32_t count) {
	return CreateRef<GLIndexBuffer>(indices, count);
}
