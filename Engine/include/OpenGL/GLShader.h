#pragma once

#include "Core.h"

#include <glm/glm.hpp>

#include <string>
#include <filesystem>
#include <unordered_map>

class GLShader {
public:
	GLShader(const std::string &name, const std::filesystem::path &vs, const std::filesystem::path &fs);
	~GLShader();

	void Bind() const;
	void Unbind() const;

	void SetInt(const std::string &name, const int32_t value);
	void SetIntArray(const std::string &name, const int32_t *value, const uint32_t count);
	void SetFloat(const std::string &name, const float value);
	void SetFloat2(const std::string &name, const glm::vec2 &value);
	void SetFloat3(const std::string &name, const glm::vec3 &value);
	void SetFloat4(const std::string &name, const glm::vec4 &value);
	void SetMat3(const std::string &name, const glm::mat3 &value);
	void SetMat4(const std::string &name, const glm::mat4 &value);

	const std::string &GetName() const { return mName; }

	static Ref<GLShader> Create(const std::string &name, const std::filesystem::path &vs, const std::filesystem::path &fs);

private:
	void CompileFromSources(const std::unordered_map<uint32_t, std::string> &shaderSources);
	uint32_t CompileShaderFromSource(const uint32_t type, const std::string &src);

	void CompileProgram(const std::unordered_map<uint32_t, std::filesystem::path> &shaderFiles);
	uint32_t CompileShader(const uint32_t type, const std::filesystem::path& shaderFile);

private:
	std::string mName;
	uint32_t mId = 0;
};
