#include "TileMap/TileSet.h"

#include "Core.h"
#include "Log.h"
#include "OpenGL/GLTexture.h"

#include <pugixml.hpp>

std::array<glm::vec2, 4> TileSet::GetTexCoords(const uint32 tileId)
{
    const uint32 texWidth = mTexture->GetWidth();
    const uint32 texHeight = mTexture->GetHeight();
    const f32 texCoordU = (((tileId - mFirstGid) % mColumnCount) * mTileWidth) / (f32)texWidth;
    const f32 texCoordV = (((tileId - mFirstGid) / mColumnCount) * mTileHeight) / (f32)texHeight;

    const std::array<glm::vec2, 4> texCoords = {
        glm::vec2 { texCoordU, 1.0f - texCoordV },
        glm::vec2 { texCoordU + (mTileWidth / (f32)texWidth), 1.0f - texCoordV },
        glm::vec2 { texCoordU + (mTileWidth / (f32)texWidth), 1.0f - (texCoordV + (mTileHeight / (f32)texHeight)) },
        glm::vec2 { texCoordU, 1.0f - (texCoordV + (mTileHeight / (f32)texHeight)) }
    };

    return texCoords;
}

const TileSet::Terrain& TileSet::GetTerrain(const uint32 tileId)
{
    assert(tileId - mFirstGid >= 0 && tileId - mFirstGid < std::size(mTerrains) && "Invalid tileId");
    return mTerrains[tileId - mFirstGid];
}

Ref<TileSet> TileSet::Load(const pugi::xml_node& node)
{
    const std::filesystem::path filepath = std::filesystem::path("assets/tilemaps") / node.attribute("source").as_string();

    pugi::xml_document doc;
    if (const auto result = doc.load_file(filepath.string().c_str()); !result)
    {
        LOG_ERROR("Failed to load tile set \"{0}\": {1}", filepath.string(), result.description());
        return nullptr;
    }

    auto tileSet = CreateRef<TileSet>();
    tileSet->mFirstGid = node.attribute("firstgid").as_uint();

    const auto tilesetNode = doc.child("tileset");
    tileSet->mName = tilesetNode.attribute("name").as_string();
    tileSet->mTileWidth = tilesetNode.attribute("tilewidth").as_uint();
    tileSet->mTileHeight = tilesetNode.attribute("tileheight").as_uint();
    tileSet->mTileCount = tilesetNode.attribute("tilecount").as_uint();
    tileSet->mColumnCount = tilesetNode.attribute("columns").as_uint();
    tileSet->mProperties = Property::LoadList(tilesetNode.child("properties"));

    const auto imageNode = tilesetNode.child("image");
    const std::filesystem::path imagePath = imageNode.attribute("source").as_string();
    tileSet->mTexture = GLTexture::Load("assets/textures" / imagePath.filename());

    tileSet->mTerrains.resize(tileSet->mTileCount);
    for (int i = 0; i < std::size(tileSet->mTerrains); ++i)
        tileSet->mTerrains[i].mTileId = i;

    for (auto tileNode = tilesetNode.child("tile"); tileNode; tileNode = tileNode.next_sibling("tile"))
    {
        const uint32 tileId = tileNode.attribute("id").as_uint();
        tileSet->mTerrains[tileId].mTileId = tileId;
        tileSet->mTerrains[tileId].mProperties = Property::LoadList(tileNode.child("properties"));
    }

    return tileSet;
}
