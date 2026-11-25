#include "World.hpp"

#include <cassert>

World::World(size_t width, size_t height) :
	width(width),
	height(height),
	tiles(std::vector<TW_Tile>(width * height))
{
}

size_t World::getWidth(void) const {
	return width;
}

size_t World::getHeight(void) const {
	return height;
}

bool World::inBounds(int i, int j) const {
	return i >= 0 && j >= 0 && i < int(width) && j < int(height);
}

TW_Tile& World::getTile(size_t i, size_t j) {
	assert(i < width && j < height);

	return tiles[i + j * width];
}

const TW_Tile& World::getTile(size_t i, size_t j) const {
	assert(i < width && j < height);

	return tiles[i + j * width];
}

TW_TextureConfig& World::getTextureConfig(size_t i, size_t j, TextureType type) {
	TW_Tile& tile = getTile(i, j);

	switch(type) {
		case TextureType::FLOOR:
			return tile.textures.floor;
			break;

		case TextureType::CEIL:
			return tile.textures.ceil;
			break;

		default: // falthrough
		case TextureType::WALL:
			return tile.textures.wall;
			break;

		case TextureType::WINDOW_FLOOR:
			return tile.textures.window_floor;
			break;

		case TextureType::WINDOW_CEIL:
			return tile.textures.window_ceil;
			break;
	}
}

const TW_TextureConfig& World::getTextureConfig(size_t i, size_t j, TextureType type) const {
	const TW_Tile& tile = getTile(i, j);

	switch(type) {
		case TextureType::FLOOR:
			return tile.textures.floor;
			break;

		case TextureType::CEIL:
			return tile.textures.ceil;
			break;

		default: // falthrough
		case TextureType::WALL:
			return tile.textures.wall;
			break;

		case TextureType::WINDOW_FLOOR:
			return tile.textures.window_floor;
			break;

		case TextureType::WINDOW_CEIL:
			return tile.textures.window_ceil;
			break;
	}
}
