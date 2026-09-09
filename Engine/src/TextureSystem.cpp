#include "TextureSystem.h"

#include "OpenGL/GLTexture.h"

#include "Utils/Log.h"

#include <cassert>

namespace TextureSystem {
	static_assert((uint32_t)Res::Textures::Id::Count > 0, "Texture count must be greater than 0");

	static constexpr uint32_t TextureCount = (uint32_t)Res::Textures::Id::Count;
	static GLTexture s_Textures[TextureCount];

	bool Init() {
		for (uint32_t i = 0; i < TextureCount; ++i) {
			const char* const path = Res::Textures::GetPath((Res::Textures::Id)i);
			if (!s_Textures[i].LoadFromFile(path)) {
				LOG_ERROR("Failed to load texture: {}", path);
				return false;
			}
		}

		return true;
	}

	void Shutdown() {
		for (uint32_t i = 0; i < TextureCount; ++i)
			s_Textures[i].Destroy();
	}

	uint32_t GetTextureId(Res::Textures::Id id) {
		assert((uint32_t)id < TextureCount && "Texture ID out of range");
		return s_Textures[(uint32_t)id].GetId();
	}

	void Bind(Res::Textures::Id id, uint32_t slot) {
		assert((uint32_t)id < TextureCount && "Texture ID out of range");
		s_Textures[(uint32_t)id].Bind(slot);
	}

	void Unbind(Res::Textures::Id id) {
		assert((uint32_t)id < TextureCount && "Texture ID out of range");
		s_Textures[(uint32_t)id].Unbind();
	}
}
