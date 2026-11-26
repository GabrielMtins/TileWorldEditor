#ifndef WORLD_HPP
#define WORLD_HPP

#include "Global.hpp"

#include <TileWorld.h>

#include <unordered_map>

class World {
	public:
		enum class TextureType {
			FLOOR = 0,
			WINDOW_FLOOR,
			WALL,
			WINDOW_CEIL,
			CEIL,
			NUM
		};

		static constexpr int NUM_TEXTURE_TYPE = int(TextureType::NUM);

		enum class SurfaceType {
			FLOOR,
			CEIL,
		};

		World(size_t width = 0, size_t height = 0);

		size_t getWidth(void) const;
		size_t getHeight(void) const;
		bool inBounds(vec2s point) const;
		bool inBounds(int i, int j) const;

		bool tileEquals(EditMode mode, const TW_Tile& tile, size_t i, size_t j) const;

		TW_Tile& getTile(size_t i, size_t j);
		const TW_Tile& getTile(size_t i, size_t j) const;

		TW_TextureConfig& getTextureConfig(size_t i, size_t j, TextureType type);
		const TW_TextureConfig& getTextureConfig(size_t i, size_t j, TextureType type) const;

		static const char * getTextureTypeLabel(int type);
		static const char * getTextureTypeLabel(TextureType type);


	private:
		size_t width, height;
		std::vector<TW_Tile> tiles;
};

#endif
