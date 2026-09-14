#pragma once

#include "ShaderIds.h"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"

namespace ShaderSystem {
	bool Init();

	void Shutdown();

	void Bind(Res::Shaders::Id id);

	void SetMat4(Res::Shaders::Id id, const char* name, const glm::mat4& matrix);
	void SetFloat4(Res::Shaders::Id id, const char* name, const glm::vec4& value);
}
