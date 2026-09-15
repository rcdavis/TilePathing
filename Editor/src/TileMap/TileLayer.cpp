#include "TileMap/TileLayer.h"

TileLayer::~TileLayer() {
	Destroy();
}

void TileLayer::Destroy() {
	delete[] tiles;
	tiles = nullptr;

	width = 0;
	height = 0;
}
