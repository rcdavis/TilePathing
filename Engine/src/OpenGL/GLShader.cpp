#include "OpenGL/GLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"
#include "Utils/FileUtils.h"

namespace Utils {
	static GLenum ShaderTypeFromString(const std::string &type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		assert(false && "Unknown shader type");
		return 0;
	}
}

GLShader::GLShader(const std::string &name, const std::filesystem::path &vs, const std::filesystem::path &fs) :
	mName(name)
{
	std::unordered_map<uint32, std::filesystem::path> shaders;
	shaders[GL_VERTEX_SHADER] = vs;
	shaders[GL_FRAGMENT_SHADER] = fs;
	CompileProgram(shaders);
}

GLShader::~GLShader() {
	glDeleteProgram(mId);
}

void GLShader::Bind() const {
	glUseProgram(mId);
}

void GLShader::Unbind() const {
	glUseProgram(0);
}

void GLShader::SetInt(const std::string &name, const int32 value) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniform1i(location, value);
}

void GLShader::SetIntArray(const std::string &name, const int32 *value, const uint32 count) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniform1iv(location, count, value);
}

void GLShader::SetFloat(const std::string &name, const f32 value) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniform1f(location, value);
}

void GLShader::SetFloat2(const std::string &name, const glm::vec2 &value) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniform2f(location, value.x, value.y);
}

void GLShader::SetFloat3(const std::string &name, const glm::vec3 &value) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniform3f(location, value.x, value.y, value.z);
}

void GLShader::SetFloat4(const std::string &name, const glm::vec4 &value) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void GLShader::SetMat3(const std::string &name, const glm::mat3 &value) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void GLShader::SetMat4(const std::string &name, const glm::mat4 &value) {
	const GLint location = glGetUniformLocation(mId, std::data(name));
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void GLShader::CompileFromSources(const std::unordered_map<uint32, std::string> &shaderSources) {
	assert(std::size(shaderSources) <= 2 && "Only 2 shaders supported (vertex and fragment)");
	const GLuint program = glCreateProgram();
	std::vector<uint32> shaderIds;
	shaderIds.reserve(std::size(shaderSources));
	for (const auto &[shaderId, source] : shaderSources) {
		auto shader = CompileShaderFromSource(shaderId, source);
		if (shader) {
			glAttachShader(program, shader);
			shaderIds.push_back(shader);
		}
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, std::data(infoLog));

		glDeleteProgram(program);

		for (auto id : shaderIds)
			glDeleteShader(id);

		LOG_ERROR("Failed to link program {0}: {1}", mName, std::data(infoLog));
		assert(false && "Shader link failure");
	}

	for (auto id : shaderIds) {
		glDetachShader(program, id);
		glDeleteShader(id);
	}

	mId = program;
}

uint32 GLShader::CompileShaderFromSource(const uint32 type, const std::string &src) {
	const GLuint shader = glCreateShader(type);

	const GLchar *const source = std::data(src);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, std::data(infoLog));

		glDeleteShader(shader);

		LOG_ERROR("{0}", std::data(infoLog));
		return 0;
	}

	return shader;
}

void GLShader::CompileProgram(const std::unordered_map<uint32, std::filesystem::path> &shaderFiles) {
	assert(std::size(shaderFiles) <= 2 && "Only 2 shaders supported (vertex and fragment)");
	const GLuint program = glCreateProgram();
	std::vector<uint32> shaderIds;
	shaderIds.reserve(std::size(shaderFiles));
	for (const auto &[shaderId, file] : shaderFiles) {
		auto shader = CompileShader(shaderId, file);
		if (shader) {
			glAttachShader(program, shader);
			shaderIds.push_back(shader);
		}
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, std::data(infoLog));

		glDeleteProgram(program);

		for (auto id : shaderIds)
			glDeleteShader(id);

		LOG_ERROR("Failed to link program {0}: {1]}", mName, std::data(infoLog));
		assert(false && "Shader link failure");
	}

	for (auto id : shaderIds) {
		glDetachShader(program, id);
		glDeleteShader(id);
	}

	mId = program;
}

uint32 GLShader::CompileShader(const uint32 type, const std::filesystem::path& shaderFile) {
	const std::string src = FileUtils::ReadText(shaderFile);
	const GLuint shader = glCreateShader(type);

	const GLchar *const source = std::data(src);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, std::data(infoLog));

		glDeleteShader(shader);

		LOG_ERROR("Failed to compile {0}: {1}", shaderFile.string(), std::data(infoLog));
		return 0;
	}

	return shader;
}

Ref<GLShader> GLShader::Create(const std::string &name, const std::filesystem::path &vs, const std::filesystem::path &fs) {
	return CreateRef<GLShader>(name, vs, fs);
}
