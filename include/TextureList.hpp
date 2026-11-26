#ifndef TEXTURE_LIST_HPP
#define TEXTURE_LIST_HPP

#include "Global.hpp"

class TextureList {
	public:
		static std::unique_ptr<TextureList> load(SDL_Renderer *renderer, const char *filename, int cell_width, int cell_height);
		~TextureList(void);

		SDL_Texture * getTexture(void) const;
		SDL_Rect getCellRect(int cell) const;
		SDL_FRect getCellFRect(int cell) const;
		Point2i getCellSize(void) const;
		Point2i getNumCells(void) const;
		uint32_t getPixelNorm(int cell, float x, float y) const;
		uint32_t getPixel(int cell, int x, int y) const;

	private:
		SDL_Texture *texture = nullptr;
		void *saved_pixels;
		uint32_t *pixels;

		int cell_width = 0, cell_height = 0;
		int num_cells_x, num_cells_y;

		TextureList(SDL_Texture *texture, uint32_t *pixels, int cell_width, int cell_height);
};

#endif
