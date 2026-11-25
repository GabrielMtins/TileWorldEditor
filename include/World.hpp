#ifndef WORLD_HPP
#define WORLD_HPP

#include "Global.hpp"

#include <TileWorld.h>

class World {
	public:
		enum class TextureType {
			FLOOR,
			CEIL,
			WINDOW_FLOOR,
			WINDOW_CEIL,
			WALL
		};

		enum class SurfaceType {
			FLOOR,
			CEIL,
		};

		World(size_t width = 0, size_t height = 0);

		size_t getWidth(void) const;
		size_t getHeight(void) const;
		bool inBounds(int i, int j) const;

		TW_Tile& getTile(size_t i, size_t j);
		const TW_Tile& getTile(size_t i, size_t j) const;

		TW_TextureConfig& getTextureConfig(size_t i, size_t j, TextureType type);
		const TW_TextureConfig& getTextureConfig(size_t i, size_t j, TextureType type) const;


	private:
		size_t width, height;
		std::vector<TW_Tile> tiles;
};

#endif
