#include "AssetBuilder.h"

#include "pugixml.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <regex>

void AssetBuilder::BuildAssets(const std::filesystem::path& inputDir, const std::filesystem::path& outputDir, const std::filesystem::path& generatedDir) {
	std::filesystem::create_directories(outputDir);
	std::filesystem::create_directories(generatedDir);

	BuildTextures(inputDir, generatedDir);

	BuildShaders(inputDir, generatedDir);

	BuildTiles(inputDir, outputDir, generatedDir);

	// TODO: Replace hardcoded tilemap and tileset paths.
	//ConvertTilemap(inputDir / "tilemaps/SMBMap.tmx", inputDir / "tilemaps/SMBTiles.tsx", outputDir / "tilemaps/SMBMap.tmbin");
}

void AssetBuilder::BuildTextures(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir) {
	for (const auto& entry : std::filesystem::recursive_directory_iterator(inputDir / "textures")) {
		if (entry.is_regular_file()) {
			const auto& path = entry.path();
			if (path.extension() == ".png") {
				mTextures.push_back(path);
				std::cout << "Found texture: " << path << std::endl;
			}
		}
	}

	CreateTextureIdHeader(inputDir, generatedDir);
}

void AssetBuilder::BuildShaders(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir) {
	for (const auto& entry : std::filesystem::recursive_directory_iterator(inputDir / "shaders")) {
		if (entry.is_regular_file()) {
			const auto& path = entry.path();
			if (path.extension() == ".vert" || path.extension() == ".frag") {
				mShaders.push_back(path);
				std::cout << "Found shader: " << path << std::endl;
			}
		}
	}

	CreateShaderIdHeader(inputDir, generatedDir);
}

void AssetBuilder::BuildTiles(const std::filesystem::path& inputDir, const std::filesystem::path& outputDir, const std::filesystem::path& generatedDir) {
	for (const auto& entry : std::filesystem::recursive_directory_iterator(inputDir / "tilemaps")) {
		if (entry.is_regular_file()) {
			const auto& path = entry.path();
			if (path.extension() == ".tmx") {
				mTilemaps.push_back(path);
				std::cout << "Found tilemap: " << path << std::endl;
			} else if (path.extension() == ".tsx") {
				mTilesets.push_back(path);
				std::cout << "Found tileset: " << path << std::endl;
			}
		}
	}

	for (const auto& path : mTilemaps) {
		TileMapData tileMapData;
		ParseTiledMap(path, inputDir, tileMapData);

		const auto outputPath = outputDir / "tilemaps" / (path.stem().string() + ".tmbin");
		CreateTileMapBinary(path, outputPath, tileMapData);
	}

	CreateTileIdHeader(inputDir, generatedDir);
}

void AssetBuilder::ParseTiledMap(const std::filesystem::path& tilemapPath, const std::filesystem::path& inputDir, TileMapData& tileMapData) {
	pugi::xml_document doc;
	if (const auto result = doc.load_file(tilemapPath.c_str()); !result) {
		std::cerr << "Failed to load tile map " << tilemapPath << ": " << result.description() << std::endl;
		return;
	}

	const pugi::xml_node mapNode = doc.child("map");
	tileMapData.width = mapNode.attribute("width").as_uint();
	tileMapData.height = mapNode.attribute("height").as_uint();
	tileMapData.tileWidth = mapNode.attribute("tilewidth").as_uint();
	tileMapData.tileHeight = mapNode.attribute("tileheight").as_uint();

	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode; layerNode = layerNode.next_sibling("layer")) {
		TileLayerData layerData;
		layerData.width = layerNode.attribute("width").as_uint();
		layerData.height = layerNode.attribute("height").as_uint();

		const pugi::xml_node dataNode = layerNode.child("data");
		if (strcmp(dataNode.attribute("encoding").as_string(), "csv") == 0) {
			const std::string csvText = dataNode.text().as_string();
			std::vector<std::string> splitValues;
			const std::regex reg("[, \n]");
			std::copy(
				std::sregex_token_iterator(std::cbegin(csvText), std::cend(csvText), reg, -1),
				std::sregex_token_iterator(),
				std::back_inserter(splitValues)
			);

			for (const auto& tileId : splitValues) {
				if (!tileId.empty()) {
					layerData.tiles.emplace_back((uint8_t)atoi(tileId.c_str()));
				}
			}
		}

		tileMapData.layers.emplace_back(layerData);
	}

	const auto tilesetChildNode = mapNode.child("tileset");
	if (!tilesetChildNode) {
		std::cerr << "No tileset found in tile map " << tilemapPath << std::endl;
		return;
	}

	const auto tilesetPath = inputDir / "tilemaps" / tilesetChildNode.attribute("source").as_string();
	if (const auto result = doc.load_file(tilesetPath.c_str()); !result) {
		std::cerr << "Failed to load tile set " << tilesetPath << ": " << result.description() << std::endl;
		return;
	}

	TileSetData tilesetData;
	const pugi::xml_node tilesetNode = doc.child("tileset");
	tilesetData.name = tilesetNode.attribute("name").as_string();
	tilesetData.tileWidth = tilesetNode.attribute("tilewidth").as_uint();
	tilesetData.tileHeight = tilesetNode.attribute("tileheight").as_uint();
	tilesetData.tileCount = tilesetNode.attribute("tilecount").as_uint();
	tilesetData.columnCount = tilesetNode.attribute("columns").as_uint();
	tilesetData.movementCosts.resize(tilesetData.tileCount, 1);

	for (pugi::xml_node tileNode = tilesetNode.child("tile"); tileNode; tileNode = tileNode.next_sibling("tile")) {
		const uint8_t tileId = (uint8_t)tileNode.attribute("id").as_uint();
		const pugi::xml_node propertiesNode = tileNode.child("properties");
		for (pugi::xml_node propNode = propertiesNode.child("property"); propNode; propNode = propNode.next_sibling("property")) {
			const auto name = propNode.attribute("name").as_string();
			const auto val = propNode.attribute("value");
			if (strcmp(name, "movementCost") == 0) {
				tilesetData.movementCosts[tileId] = (uint8_t)val.as_uint(1);
			}
		}
	}

	tileMapData.tilesets.emplace_back(tilesetData);
}

void AssetBuilder::CreateTileMapBinary(const std::filesystem::path& tilemapPath, const std::filesystem::path& outputPath, const TileMapData& tileMapData) {
	// Implementation of CreateTileMapBinary goes here
}

void AssetBuilder::ConvertTilemap(const std::filesystem::path& tilemapPath, const std::filesystem::path& tilesetPath, const std::filesystem::path& outputPath) {
	pugi::xml_document doc;
	if (const auto result = doc.load_file(tilemapPath.c_str()); !result) {
		std::cerr << "Failed to load tile map " << tilemapPath << ": " << result.description() << std::endl;
		return;
	}

	TileMapData tileMapData;
	const pugi::xml_node mapNode = doc.child("map");
	tileMapData.width = mapNode.attribute("width").as_uint();
	tileMapData.height = mapNode.attribute("height").as_uint();
	tileMapData.tileWidth = mapNode.attribute("tilewidth").as_uint();
	tileMapData.tileHeight = mapNode.attribute("tileheight").as_uint();

	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode; layerNode = layerNode.next_sibling("layer")) {
		TileLayerData layerData;
		layerData.width = layerNode.attribute("width").as_uint();
		layerData.height = layerNode.attribute("height").as_uint();

		const pugi::xml_node dataNode = layerNode.child("data");
		if (strcmp(dataNode.attribute("encoding").as_string(), "csv") == 0) {
			const std::string csvText = dataNode.text().as_string();
			std::vector<std::string> splitValues;
			const std::regex reg("[, \n]");
			std::copy(
				std::sregex_token_iterator(std::cbegin(csvText), std::cend(csvText), reg, -1),
				std::sregex_token_iterator(),
				std::back_inserter(splitValues)
			);

			for (const auto& tileId : splitValues) {
				layerData.tiles.emplace_back((uint8_t)atoi(tileId.c_str()));
			}
		}

		tileMapData.layers.emplace_back(layerData);
	}

	if (const auto result = doc.load_file(tilesetPath.c_str()); !result) {
		std::cerr << "Failed to load tile set " << tilesetPath << ": " << result.description() << std::endl;
		return;
	}

	TileSetData tilesetData;
	const pugi::xml_node tilesetNode = doc.child("tileset");
	tilesetData.name = tilesetNode.attribute("name").as_string();
	tilesetData.tileWidth = tilesetNode.attribute("tilewidth").as_uint();
	tilesetData.tileHeight = tilesetNode.attribute("tileheight").as_uint();
	tilesetData.tileCount = tilesetNode.attribute("tilecount").as_uint();
	tilesetData.columnCount = tilesetNode.attribute("columns").as_uint();
	tilesetData.movementCosts.resize(tilesetData.tileCount, 1);

	for (pugi::xml_node tileNode = tilesetNode.child("tile"); tileNode; tileNode = tileNode.next_sibling("tile")) {
		const uint8_t tileId = (uint8_t)tileNode.attribute("id").as_uint();
		const pugi::xml_node propertiesNode = tileNode.child("properties");
		for (pugi::xml_node propNode = propertiesNode.child("property"); propNode; propNode = propNode.next_sibling("property")) {
			const auto name = propNode.attribute("name").as_string();
			const auto val = propNode.attribute("value");
			if (strcmp(name, "movementCost") == 0) {
				tilesetData.movementCosts[tileId] = (uint8_t)val.as_uint(1);
			}
		}
	}

	tileMapData.tilesets.emplace_back(tilesetData);

	std::ofstream file(outputPath, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to create file " << outputPath << std::endl;
		return;
	}

	file.write((const char*)&tileMapData.width, sizeof(uint32_t));
	file.write((const char*)&tileMapData.height, sizeof(uint32_t));
	file.write((const char*)&tileMapData.tileWidth, sizeof(uint32_t));
	file.write((const char*)&tileMapData.tileHeight, sizeof(uint32_t));

	const uint32_t tilesetCount = (uint32_t)tileMapData.tilesets.size();
	file.write((const char*)&tilesetCount, sizeof(uint32_t));
	const uint32_t layerCount = (uint32_t)tileMapData.layers.size();
	file.write((const char*)&layerCount, sizeof(uint32_t));

	for (const TileSetData& tileSet : tileMapData.tilesets) {
		file.write((const char*)&tileSet.tileWidth, sizeof(uint32_t));
		file.write((const char*)&tileSet.tileHeight, sizeof(uint32_t));
		file.write((const char*)&tileSet.tileCount, sizeof(uint32_t));
		file.write((const char*)&tileSet.columnCount, sizeof(uint32_t));

		file.write((const char*)tileSet.movementCosts.data(), tileSet.movementCosts.size() * sizeof(uint8_t));
	}

	for (const TileLayerData& layerData : tileMapData.layers) {
		file.write((const char*)&layerData.width, sizeof(uint32_t));
		file.write((const char*)&layerData.height, sizeof(uint32_t));

		file.write((const char*)layerData.tiles.data(), layerData.tiles.size());
	}

	std::cout << "Converted tile map " << tilemapPath << " to " << outputPath << std::endl;
}

void AssetBuilder::CreateTextureIdHeader(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir) {
	std::ofstream file(generatedDir / "TextureIds.h");
	if (!file) {
		std::cerr << "Failed to create header file " << generatedDir / "TextureIds.h" << std::endl;
		return;
	}

	file << "////////////////////////////////////////////////////////////////////////\n";
	file << "// This file is auto-generated. Do not modify directly.\n";
	file << "////////////////////////////////////////////////////////////////////////\n";
	file << "#pragma once\n\n";
	file << "#include <cstdint>\n\n";

	file << "namespace Res::Textures {\n";

	file << "\tenum class Id : uint8_t {\n";
	for (const auto& path : mTextures) {
		file << "\t\t" << path.stem().string() << ",\n";
	}
	file << "\t\tCount\n";
	file << "\t};\n\n";

	file << "\tinline constexpr const char* ToString(Id id) {\n";
	file << "\t\tswitch (id) {\n";
	for (const auto& path : mTextures) {
		file << "\t\t\tcase Id::" << path.stem().string() << ": return \"" << path.stem().string() << "\";\n";
	}
	file << "\t\t\tdefault: return \"Unknown\";\n";
	file << "\t\t}\n";
	file << "\t}\n";

	const auto resParentDir = inputDir / "..";
	file << "\n\tinline constexpr const char* GetPath(Id id) {\n";
	file << "\t\tswitch (id) {\n";
	for (const auto& path : mTextures) {
		const auto relativePath = std::filesystem::relative(path, resParentDir);
		file << "\t\t\tcase Id::" << path.stem().string() << ": return \"" << relativePath.generic_string() << "\";\n";
	}
	file << "\t\t\tdefault: return nullptr;\n";
	file << "\t\t}\n";
	file << "\t}\n";

	file << "}\n";

	std::cout << "Generated texture ID header at " << generatedDir / "TextureIds.h" << std::endl;
}

void AssetBuilder::CreateShaderIdHeader(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir) {
	std::ofstream file(generatedDir / "ShaderIds.h");
	if (!file) {
		std::cerr << "Failed to create header file " << generatedDir / "ShaderIds.h" << std::endl;
		return;
	}

	file << "////////////////////////////////////////////////////////////////////////\n";
	file << "// This file is auto-generated. Do not modify directly.\n";
	file << "////////////////////////////////////////////////////////////////////////\n";
	file << "#pragma once\n\n";
	file << "#include <cstdint>\n\n";

	file << "namespace Res::Shaders {\n";

	file << "\tenum class Id : uint8_t {\n";
	for (const auto& path : mShaders) {
		const auto extension = path.extension().string();
		if (extension == ".vert") {
			file << "\t\t" << path.stem().string() << "VS,\n";
		} else if (extension == ".frag") {
			file << "\t\t" << path.stem().string() << "FS,\n";
		} else {
			file << "\t\t" << path.stem().string() << ",\n";
		}
	}
	file << "\t\tCount\n";
	file << "\t};\n\n";

	file << "\tinline constexpr const char* ToString(Id id) {\n";
	file << "\t\tswitch (id) {\n";
	for (const auto& path : mShaders) {
		const auto extension = path.extension().string();
		if (extension == ".vert") {
			file << "\t\t\tcase Id::" << path.stem().string() << "VS: return \"" << path.stem().string() << "VS\";\n";
		} else if (extension == ".frag") {
			file << "\t\t\tcase Id::" << path.stem().string() << "FS: return \"" << path.stem().string() << "FS\";\n";
		} else {
			file << "\t\t\tcase Id::" << path.stem().string() << ": return \"" << path.stem().string() << "\";\n";
		}
	}
	file << "\t\t\tdefault: return \"Unknown\";\n";
	file << "\t\t}\n";
	file << "\t}\n";

	const auto resParentDir = inputDir / "..";
	file << "\n\tinline constexpr const char* GetPath(Id id) {\n";
	file << "\t\tswitch (id) {\n";
	for (const auto& path : mShaders) {
		const auto relativePath = std::filesystem::relative(path, resParentDir);
		const auto extension = path.extension().string();
		if (extension == ".vert") {
			file << "\t\t\tcase Id::" << path.stem().string() << "VS: return \"" << relativePath.generic_string() << "\";\n";
		} else if (extension == ".frag") {
			file << "\t\t\tcase Id::" << path.stem().string() << "FS: return \"" << relativePath.generic_string() << "\";\n";
		} else {
			file << "\t\t\tcase Id::" << path.stem().string() << ": return \"" << relativePath.generic_string() << "\";\n";
		}
	}
	file << "\t\t\tdefault: return nullptr;\n";
	file << "\t\t}\n";
	file << "\t}\n";

	file << "}\n";

	std::cout << "Generated shader ID header at " << generatedDir / "ShaderIds.h" << std::endl;
}

void AssetBuilder::CreateTileIdHeader(const std::filesystem::path& inputDir, const std::filesystem::path& generatedDir) {
	std::ofstream file(generatedDir / "TileIds.h");
	if (!file) {
		std::cerr << "Failed to create header file " << generatedDir / "TileIds.h" << std::endl;
		return;
	}

	file << "////////////////////////////////////////////////////////////////////////\n";
	file << "// This file is auto-generated. Do not modify directly.\n";
	file << "////////////////////////////////////////////////////////////////////////\n";
	file << "#pragma once\n\n";
	file << "#include <cstdint>\n\n";

	file << "namespace Res::Tiles {\n";

	file << "\tnamespace Maps {\n";

	file << "\t\tenum class Id : uint8_t {\n";
	for (const auto& path : mTilemaps) {
		file << "\t\t\t" << path.stem().string() << ",\n";
	}
	file << "\t\t\tCount\n";
	file << "\t\t};\n\n";

	file << "\t\tinline constexpr const char* ToString(Id id) {\n";
	file << "\t\t\tswitch (id) {\n";
	for (const auto& path : mTilemaps) {
		file << "\t\t\t\tcase Id::" << path.stem().string() << ": return \"" << path.stem().string() << "\";\n";
	}
	file << "\t\t\t\tdefault: return \"Unknown\";\n";
	file << "\t\t\t}\n";
	file << "\t\t}\n";

	const auto resParentDir = inputDir / "..";
	file << "\n\t\tinline constexpr const char* GetPath(Id id) {\n";
	file << "\t\t\tswitch (id) {\n";
	for (const auto& path : mTilemaps) {
		const auto relativePath = std::filesystem::relative(path, resParentDir);
		file << "\t\t\t\tcase Id::" << path.stem().string() << ": return \"" << relativePath.generic_string() << "\";\n";
	}
	file << "\t\t\t\tdefault: return nullptr;\n";
	file << "\t\t\t}\n";
	file << "\t\t}\n";

	file << "\t}\n\n";

	file << "\tnamespace Sets {\n";

	file << "\t\tenum class Id : uint8_t {\n";
	for (const auto& path : mTilesets) {
		file << "\t\t\t" << path.stem().string() << ",\n";
	}
	file << "\t\t\tCount\n";
	file << "\t\t};\n\n";

	file << "\t\tinline constexpr const char* ToString(Id id) {\n";
	file << "\t\t\tswitch (id) {\n";
	for (const auto& path : mTilesets) {
		file << "\t\t\t\tcase Id::" << path.stem().string() << ": return \"" << path.stem().string() << "\";\n";
	}
	file << "\t\t\t\tdefault: return \"Unknown\";\n";
	file << "\t\t\t}\n";
	file << "\t\t}\n";

	file << "\n\t\tinline constexpr const char* GetPath(Id id) {\n";
	file << "\t\t\tswitch (id) {\n";
	for (const auto& path : mTilesets) {
		const auto relativePath = std::filesystem::relative(path, resParentDir);
		file << "\t\t\t\tcase Id::" << path.stem().string() << ": return \"" << relativePath.generic_string() << "\";\n";
	}
	file << "\t\t\t\tdefault: return nullptr;\n";
	file << "\t\t\t}\n";
	file << "\t\t}\n";

	file << "\t}\n";

	file << "}\n";

	std::cout << "Generated tile ID header at " << generatedDir / "TileIds.h" << std::endl;
}
