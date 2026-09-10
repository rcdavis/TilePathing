#pragma once

#include "ShaderIds.h"

namespace ShaderSystem {
	bool Init();

	void Shutdown();

	void Bind(Res::Shaders::Id id);
}
