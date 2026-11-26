#include "World.hpp"

#include <array>
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

bool World::tileEquals(EditMode mode, const TW_Tile& tile, size_t i, size_t j) const {
	const TW_Tile& other = getTile(i, j);

	switch(mode) {
		case EditMode::FLOOR:
			return tile.textures.floor.id == other.textures.floor.id;
			break;

		case EditMode::WINDOW_FLOOR:
			return tile.textures.window_floor.id == other.textures.window_floor.id;
			break;

		case EditMode::WALL:
			return tile.textures.wall.id == other.textures.wall.id;
			break;

		case EditMode::WINDOW_CEIL:
			return tile.textures.window_ceil.id == other.textures.window_ceil.id;
			break;

		case EditMode::CEIL:
			return tile.textures.ceil.id == other.textures.ceil.id;
			break;

		default:
			break;
	}

	return false;
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

const char * World::getTextureTypeLabel(int type) {
	assert(type >= 0 && type < NUM_TEXTURE_TYPE);
	static constexpr std::array<const char *, NUM_TEXTURE_TYPE> labels = {
		"Floor", "Window floor", "Wall", "Window ceil", "Ceil",
	};

	return labels[type];
}

const char * World::getTextureTypeLabel(TextureType type) {
	return getTextureTypeLabel((int) type);
}
