#include "Viewer2d.hpp"

#include <imgui.h>

std::unique_ptr<Viewer2d> Viewer2d::create(SDL_Renderer *renderer) {
	SDL_Texture *background = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_TARGET,
			background_width,
			background_height
			);

	if(background == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<Viewer2d>(new Viewer2d(renderer, background));
}

void Viewer2d::show(World &world) {
	render(world);

	ImGui::Begin("Editor");
	showImage();
	ImGui::End();
}

Viewer2d::Viewer2d(SDL_Renderer *renderer, SDL_Texture *background) :
	renderer(renderer),
	background(background)
{
}

void Viewer2d::showImage(void) {
	ImVec2 avail_size = ImGui::GetContentRegionAvail();

	num_tiles_shown.raw[0] = num_tiles_shown.raw[1] * avail_size.x / avail_size.y;

	ImGui::Image(background, avail_size);
}

void Viewer2d::render(const World &world) {
	SDL_SetRenderTarget(renderer, background);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);

	renderTiles(world);

	SDL_SetRenderTarget(renderer, nullptr);
}

void Viewer2d::renderTiles(const World &world) {
	const vec2s half_size = glms_vec2_scale(num_tiles_shown, 0.5f);
	const vec2s min = glms_vec2_sub(glms_vec2_floor(glms_vec2_sub(camera, half_size)), vec2s{1.0f, 1.0f});
	const vec2s max = glms_vec2_add(glms_vec2_floor(glms_vec2_add(camera, half_size)), vec2s{1.0f, 1.0f});

	for(int j = min.raw[1]; j <= max.raw[1]; j++) {
		for(int i = min.raw[0]; i <= max.raw[0]; i++) {
			renderTile(world, i, j);
		}
	}
}

void Viewer2d::renderTile(const World& world, int i, int j) {
	if(!world.inBounds(i, j)) return;

	const vec2s half_size = glms_vec2_scale(num_tiles_shown, 0.5f);
	const vec2s tile_size = glms_vec2_div(background_pixel_size, num_tiles_shown);

	vec2s pos = {float(i), float(j)};

	pos = glms_vec2_sub(pos, camera);
	pos = glms_vec2_add(pos, half_size);
	pos = glms_vec2_mul(pos, tile_size);

	SDL_FRect dst = {
		pos.raw[0], pos.raw[1],
		tile_size.raw[0], tile_size.raw[1]
	};

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

	const TW_TextureConfig& cfg = world.getTextureConfig(i, j, edit_texture_type);

	if(cfg.id == 0) {
		SDL_RenderRect(renderer, &dst);
	} else {
		SDL_RenderFillRect(renderer, &dst);
	}
}
