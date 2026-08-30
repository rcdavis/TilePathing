#include "OpenGL/GLVertexBuffer.h"

#include <glad/glad.h>

GLVertexBuffer::GLVertexBuffer(uint32_t size) {
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::GLVertexBuffer(const float *const vertices, const uint32_t count) {
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertices, GL_STATIC_DRAW);
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
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

Ref<GLVertexBuffer> GLVertexBuffer::Create(uint32_t size) {
	return CreateRef<GLVertexBuffer>(size);
}

Ref<GLVertexBuffer> GLVertexBuffer::Create(const float *const vertices, uint32_t count) {
	return CreateRef<GLVertexBuffer>(vertices, count);
}
