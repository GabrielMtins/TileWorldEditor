#include "TextureList.hpp"

#include <cstring>
#include <stb_image.h>

std::unique_ptr<TextureList> TextureList::load(SDL_Renderer *renderer, const char *filename, int cell_width, int cell_height) {
	if(filename == nullptr || filename[0] == '\0') {
		return nullptr;
	}

	if(cell_width <= 0 || cell_height <= 0) {
		return nullptr;
	}

	int w, h;
	uint32_t *pixels = (uint32_t *) stbi_load(filename, &w, &h, NULL, 4);

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

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	if(texture == nullptr) {
		SDL_DestroySurface(surface);
		stbi_image_free(pixels);
		return nullptr;
	}

	SDL_DestroySurface(surface);

	return std::unique_ptr<TextureList>(new TextureList(texture, pixels, cell_width, cell_height));
}

TextureList::~TextureList(void) {
	SDL_DestroyTexture(texture);
	stbi_image_free(pixels);
}

SDL_Texture * TextureList::getTexture(void) const {
	return texture;
}

SDL_Rect TextureList::getCellRect(int cell) const {
	return SDL_Rect{
		((cell % num_cells_x) * cell_width),
		((cell / num_cells_x) * cell_height),
		(cell_width),
		(cell_height)
	};
}

SDL_FRect TextureList::getCellFRect(int cell) const {
	return SDL_FRect{
		(float) ((cell % num_cells_x) * cell_width),
		(float) ((cell / num_cells_x) * cell_height),
		(float) (cell_width),
		(float) (cell_height)
	};
}

Point2i TextureList::getCellSize(void) const {
	return Point2i(cell_width, cell_height);
}

Point2i TextureList::getNumCells(void) const {
	return Point2i(num_cells_x, num_cells_y);
}

uint32_t TextureList::getPixelNorm(int cell, float x, float y) const {
	int xi = x * cell_width;
	int yi = y * cell_height;

	return getPixel(cell, xi, yi);
}

uint32_t TextureList::getPixel(int cell, int x, int y) const {
	int i = (cell % num_cells_x) * cell_width + x;
	int j = (cell / num_cells_x) * cell_height + y;

	if(i < 0 || j < 0 || i >= texture->w || j >= texture->h) {
		return 0;
	}

	return pixels[i + j * texture->w];
}

TextureList::TextureList(SDL_Texture *texture, uint32_t *pixels, int cell_width, int cell_height) :
	texture(texture),
	pixels(pixels),
	cell_width(cell_width),
	cell_height(cell_height),
	num_cells_x(texture->w / cell_width),
	num_cells_y(texture->h / cell_height)
{
}
