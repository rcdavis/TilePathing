#include "ShaderSystem.h"

#include "OpenGL/GLShader.h"
#include "ShaderIds.h"
#include "Utils/Log.h"

#include <cassert>

namespace ShaderSystem {
	static GLShader s_Shaders[(uint32_t)Res::Shaders::Id::Count];

	bool Init() {
		for (uint8_t i = 0; i < (uint8_t)Res::Shaders::Id::Count; ++i) {
			const char* const vs = Res::Shaders::GetVertexPath((Res::Shaders::Id)i);
			const char* const fs = Res::Shaders::GetFragmentPath((Res::Shaders::Id)i);
			if (!s_Shaders[i].Create(vs, fs)) {
				LOG_ERROR("Failed to create shader: {}", Res::Shaders::ToString((Res::Shaders::Id)i));
				return false;
			}
		}

		return true;
	}

	void Shutdown() {
		for (uint8_t i = 0; i < (uint8_t)Res::Shaders::Id::Count; ++i)
			s_Shaders[i].Destroy();
	}

	void Bind(Res::Shaders::Id id) {
		assert(id < Res::Shaders::Id::Count && "Invalid shader ID");
		s_Shaders[(uint8_t)id].Bind();
	}
}
