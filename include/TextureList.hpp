#ifndef TEXTURE_LIST_HPP
#define TEXTURE_LIST_HPP

#include "Global.hpp"

class TextureList {
	public:
		static std::unique_ptr<TextureList> load(const char *filename, int cell_width, int cell_height);
		~TextureList(void);

		SDL_Surface * getSurface(void) const;
		SDL_Rect getCellRect(int cell) const;
		uint32_t getPixelNorm(int cell, float x, float y) const;
		uint32_t getPixel(int cell, int x, int y) const;

	private:
		SDL_Surface *surface = nullptr;
		void *saved_pixels;

		int cell_width = 0, cell_height = 0;
		int num_cells_x, num_cells_y;

		TextureList(SDL_Surface *surface, void *saved_pixels, int cell_width, int cell_height);
};

#endif
