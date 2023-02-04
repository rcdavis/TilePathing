#pragma once

#include "Property.h"

#include <string>

class TileMapLayer
{
public:
    enum class Type
    {
        Tile,
        Object
    };

public:
    virtual ~TileMapLayer() = default;

    Type GetType() const { return mType; }
    const std::string& GetName() const { return mName; }
    const std::vector<Property>& GetProperties() const { return mProperties; }

protected:
    TileMapLayer(Type type) : mType(type) {}

    std::string mName;
    std::vector<Property> mProperties;

    bool mIsVisible = true;

private:
    Type mType;
};
