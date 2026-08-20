#include "TileMap/TileSet.h"

#include "Core.h"
#include "Utils/Log.h"
#include "OpenGL/GLTexture.h"

#include <pugixml.hpp>

std::array<glm::vec2, 4> TileSet::GetTexCoords(const uint32 tileId)
{
    const uint32 texWidth = texture->GetWidth();
    const uint32 texHeight = texture->GetHeight();
    const f32 texCoordU = (((tileId - firstGid) % columnCount) * tileWidth) / (f32)texWidth;
    const f32 texCoordV = (((tileId - firstGid) / columnCount) * tileHeight) / (f32)texHeight;

    const std::array<glm::vec2, 4> texCoords = {
        glm::vec2 { texCoordU, 1.0f - texCoordV },
        glm::vec2 { texCoordU + (tileWidth / (f32)texWidth), 1.0f - texCoordV },
        glm::vec2 { texCoordU + (tileWidth / (f32)texWidth), 1.0f - (texCoordV + (tileHeight / (f32)texHeight)) },
        glm::vec2 { texCoordU, 1.0f - (texCoordV + (tileHeight / (f32)texHeight)) }
    };

    return texCoords;
}

const TileSet::Terrain& TileSet::GetTerrain(const uint32 tileId)
{
    assert(tileId - firstGid >= 0 && tileId - firstGid < std::size(terrains) && "Invalid tileId");
    return terrains[tileId - firstGid];
}

Ref<TileSet> TileSet::Load(const pugi::xml_node& node)
{
    const std::filesystem::path filepath = std::filesystem::path("res/tilemaps") / node.attribute("source").as_string();

    pugi::xml_document doc;
    if (const auto result = doc.load_file(filepath.string().c_str()); !result)
    {
        LOG_ERROR("Failed to load tile set \"{0}\": {1}", filepath.string(), result.description());
        return nullptr;
    }

    auto tileSet = CreateRef<TileSet>();
    tileSet->firstGid = node.attribute("firstgid").as_uint();

    const auto tilesetNode = doc.child("tileset");
    tileSet->name = tilesetNode.attribute("name").as_string();
    tileSet->tileWidth = tilesetNode.attribute("tilewidth").as_uint();
    tileSet->tileHeight = tilesetNode.attribute("tileheight").as_uint();
    tileSet->tileCount = tilesetNode.attribute("tilecount").as_uint();
    tileSet->columnCount = tilesetNode.attribute("columns").as_uint();
    tileSet->properties = Property::LoadList(tilesetNode.child("properties"));

    const auto imageNode = tilesetNode.child("image");
    const std::filesystem::path imagePath = imageNode.attribute("source").as_string();
    tileSet->texture = GLTexture::Load("res/textures" / imagePath.filename());

    tileSet->terrains.resize(tileSet->tileCount);
    for (int i = 0; i < std::size(tileSet->terrains); ++i)
        tileSet->terrains[i].mTileId = i;

    for (auto tileNode = tilesetNode.child("tile"); tileNode; tileNode = tileNode.next_sibling("tile"))
    {
        const uint32 tileId = tileNode.attribute("id").as_uint();
        tileSet->terrains[tileId].mTileId = tileId;
        tileSet->terrains[tileId].mProperties = Property::LoadList(tileNode.child("properties"));
    }

    return tileSet;
}
