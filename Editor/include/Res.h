#pragma once

namespace Res::Paths {
	namespace Shaders {
		constexpr const char* ColoredTileVS = "res/shaders/ColoredTile.vert";
		constexpr const char* ColoredTileFS = "res/shaders/ColoredTile.frag";
		constexpr const char* TileMapVS = "res/shaders/TileMap.vert";
		constexpr const char* TileMapFS = "res/shaders/TileMap.frag";
	}

	namespace Textures {
		constexpr const char* DirectoryIcon = "res/textures/DirectoryIcon.png";
		constexpr const char* FileIcon = "res/textures/FileIcon.png";
		constexpr const char* SelectionRing = "res/textures/SelectionRing.png";
		constexpr const char* SMB_BlockTiles = "res/textures/SMB_BlockTiles.png";
	}

	namespace TileMap {
		constexpr const char* SMBMap = "res/tilemaps/SMBMap.tmx";
		constexpr const char* SMBMapTileset = "res/tilemaps/SMBMapTileset.png";
	}
}
