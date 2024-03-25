#pragma once

#include "Core.h"

#include <vector>
#include <string>
#include <variant>

namespace pugi
{
    class xml_node;
}

class Property
{
public:
    enum class Type
    {
        String,
        Int32,
        Float32,
        Bool,
        Undef
    };

public:
    Property() = default;
    Property(const std::string& name);
    Property(const std::string& name, const char* const val);
    Property(const std::string& name, const std::string& val);
    Property(const std::string& name, const int32 val);
    Property(const std::string& name, const f32 val);
    Property(const std::string& name, const bool val);

    Property(const Property& prop) = default;
    Property& operator=(const Property& prop);
    Property& operator=(const char* const val);
    Property& operator=(const std::string& val);
    Property& operator=(const int32 val);
    Property& operator=(const f32 val);
    Property& operator=(const bool val);

    std::string AsString() const { return std::get<std::string>(mValue); }
    int32 AsInt32() const { return std::get<int32>(mValue); }
    f32 AsFloat32() const { return std::get<f32>(mValue); }
    bool AsBool() const { return std::get<bool>(mValue); }

    const std::string& GetName() const { return mName; }
    Type GetType() const { return mType; }

    static std::vector<Property> LoadList(const pugi::xml_node& node);
    static Property Load(const pugi::xml_node& node);

private:
    std::string mName;
    std::variant<std::string, int32, f32, bool> mValue;
    Type mType = Type::Undef;
};
