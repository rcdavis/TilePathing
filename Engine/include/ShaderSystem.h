#pragma once

#include "ShaderIds.h"

struct GLShader;

namespace ShaderSystem {
	bool Init();

	void Shutdown();

	void Bind(Res::Shaders::Id id);

	GLShader* Get(Res::Shaders::Id id);
}
