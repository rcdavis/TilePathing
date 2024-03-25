#include "Property.h"

#include <pugixml.hpp>

Property::Property(const std::string& name) :
    mName(name), mValue(), mType(Type::Undef) {}

Property::Property(const std::string& name, const char* const val) :
    mName(name), mValue(std::string(val)), mType(Type::String) {}

Property::Property(const std::string& name, const std::string& val) :
    mName(name), mValue(val), mType(Type::String) {}

Property::Property(const std::string& name, const int32 val) :
    mName(name), mValue(val), mType(Type::Int32) {}

Property::Property(const std::string& name, const f32 val) :
    mName(name), mValue(val), mType(Type::Float32) {}

Property::Property(const std::string& name, const bool val) :
    mName(name), mValue(val), mType(Type::Bool) {}

Property& Property::operator=(const Property& prop)
{
    if (this == &prop)
        return *this;

    mName = prop.mName;
    mValue = prop.mValue;
    mType = prop.mType;
    return *this;
}

Property& Property::operator=(const char* const val)
{
    mValue = std::string(val);
    mType = Type::String;
    return *this;
}

Property& Property::operator=(const std::string& val)
{
    mValue = val;
    mType = Type::String;
    return *this;
}

Property& Property::operator=(const int32 val)
{
    mValue = val;
    mType = Type::Int32;
    return *this;
}

Property& Property::operator=(const f32 val)
{
    mValue = val;
    mType = Type::Float32;
    return *this;
}

Property& Property::operator=(const bool val)
{
    mValue = val;
    mType = Type::Bool;
    return *this;
}

std::vector<Property> Property::LoadList(const pugi::xml_node& node)
{
    std::vector<Property> properties;
    for (pugi::xml_node curNode = node.child("property"); curNode; curNode = curNode.next_sibling("property"))
        properties.push_back(Load(curNode));
    return properties;
}

Property Property::Load(const pugi::xml_node& node)
{
    const auto name = node.attribute("name").as_string();
    const auto type = node.attribute("type").as_string();
    const auto val = node.attribute("value");
    if (strcmp(type, "int") == 0)
        return Property(name, val.as_int());
    else if (strcmp(type, "float") == 0)
        return Property(name, val.as_float());
    else if (strcmp(type, "bool") == 0)
        return Property(name, val.as_bool());

    return Property(name, val.as_string());
}
