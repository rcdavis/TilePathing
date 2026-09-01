#include "OpenGL/GLVertexBuffer.h"

#include <glad/glad.h>

GLVertexBuffer::GLVertexBuffer(uint32_t size) {
	glCreateBuffers(1, &mId);
	glNamedBufferData(mId, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::GLVertexBuffer(const float *const vertices, const uint32_t count) {
	glCreateBuffers(1, &mId);
	glNamedBufferData(mId, sizeof(float) * count, vertices, GL_STATIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer() {
	glDeleteBuffers(1, &mId);
}

void GLVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void GLVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::SetData(const void *data, const uint32_t size) {
	glNamedBufferSubData(mId, 0, size, data);
}

Ref<GLVertexBuffer> GLVertexBuffer::Create(uint32_t size) {
	return CreateRef<GLVertexBuffer>(size);
}

Ref<GLVertexBuffer> GLVertexBuffer::Create(const float *const vertices, uint32_t count) {
	return CreateRef<GLVertexBuffer>(vertices, count);
}
