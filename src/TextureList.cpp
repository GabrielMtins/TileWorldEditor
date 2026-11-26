#include "TextureList.hpp"

#include <cstring>
#include <stb_image.h>

std::unique_ptr<TextureList> TextureList::load(const char *filename, int cell_width, int cell_height) {
	if(filename == nullptr || filename[0] == '\0') {
		return nullptr;
	}

	if(cell_width <= 0 || cell_height <= 0) {
		return nullptr;
	}

	int w, h;
	unsigned char *pixels = stbi_load(filename, &w, &h, NULL, 4);

	if(pixels == nullptr) {
		return nullptr;
	}

	SDL_Surface *surface = SDL_CreateSurfaceFrom(
			w, h,
			SDL_PIXELFORMAT_RGBA32,
			pixels,
			w * 4
			);

	if(surface == nullptr) {
		stbi_image_free(pixels);
		return nullptr;
	}

	return std::unique_ptr<TextureList>(new TextureList(surface, pixels, cell_width, cell_height));
}

TextureList::~TextureList(void) {
	void *pixels = surface->pixels;
	SDL_DestroySurface(surface);

	stbi_image_free(pixels);
}

SDL_Surface * TextureList::getSurface(void) const {
	return surface;
}

SDL_Rect TextureList::getCellRect(int cell) const {
	return SDL_Rect{
		((cell % num_cells_x) * cell_width),
		((cell / num_cells_x) * cell_height),
		(cell_width),
		(cell_height)
	};
}

uint32_t TextureList::getPixelNorm(int cell, float x, float y) const {
	int xi = x * cell_width;
	int yi = y * cell_height;

	return getPixel(cell, xi, yi);
}

uint32_t TextureList::getPixel(int cell, int x, int y) const {
	int i = (cell % num_cells_x) * cell_width + x;
	int j = (cell / num_cells_x) * cell_height + y;

	if(i < 0 || j < 0 || i >= surface->w || j >= surface->h) {
		return 0;
	}

	uint32_t *pixels = (uint32_t *) surface->pixels;

	return pixels[i + j * surface->w];
}

TextureList::TextureList(SDL_Surface *surface, void *saved_pixels, int cell_width, int cell_height) :
	surface(surface),
	saved_pixels(saved_pixels),
	cell_width(cell_width),
	cell_height(cell_height),
	num_cells_x(surface->w / cell_width),
	num_cells_y(surface->h / cell_height)
{
}
