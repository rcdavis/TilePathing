#include "TileLayer.h"

#include "Utils/StringUtils.h"

#include <pugixml.hpp>

Ref<TileLayer> TileLayer::Load(const pugi::xml_node& node)
{
    auto tileLayer = CreateRef<TileLayer>();

    tileLayer->mName = node.attribute("name").as_string();
    tileLayer->mIsVisible = node.attribute("visible").as_bool(true);
    tileLayer->mWidth = node.attribute("width").as_uint();
    tileLayer->mHeight = node.attribute("height").as_uint();
    tileLayer->mProperties = Property::LoadList(node.child("properties"));

    const auto dataNode = node.child("data");
    if (strcmp(dataNode.attribute("encoding").as_string(), "csv") == 0)
    {
        const auto strs = StringUtils::Split(dataNode.text().as_string(), ", \n");
        for (const auto& s : strs)
            if (!std::empty(s))
                tileLayer->mTiles.push_back(TileLayer::Tile{ (uint32)atoi(s.c_str()) });
    }

    return tileLayer;
}
