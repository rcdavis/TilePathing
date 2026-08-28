#pragma once

#include <string>
#include <cstdint>

class TileMapLayer {
public:
	enum class Type : uint8_t {
		Tile,
		Object
	};

public:
	virtual ~TileMapLayer() = default;

	Type GetType() const { return mType; }
	const std::string& GetName() const { return mName; }

protected:
	TileMapLayer(Type type) : mType(type) {}

	std::string mName;

	bool mIsVisible = true;

private:
	Type mType;
};
