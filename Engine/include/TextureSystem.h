#pragma once

#include "TextureIds.h"

namespace TextureSystem {
	bool Init();

	void Shutdown();

	uint32_t GetTextureId(Res::Textures::Id id);

	void Bind(Res::Textures::Id id, uint32_t slot = 0);

	void Unbind(Res::Textures::Id id);
}
