#include "Viewer2d.hpp"

#include <imgui.h>

std::unique_ptr<Viewer2d> Viewer2d::create(SDL_Renderer *renderer, const TextureList& texture_list) {
	SDL_Texture *background_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_STREAMING,
			background_width,
			background_height
			);

	if(background_texture == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<Viewer2d>(new Viewer2d(background_texture, texture_list));
}

void Viewer2d::show(World &world) {
	ImGui::Begin("Editor");
	showImage();
	ImGui::End();

	render(world);
}

Viewer2d::Viewer2d(SDL_Texture *background_texture, const TextureList& texture_list) :
	background_texture(background_texture),
	texture_list(texture_list)
{
}

void Viewer2d::handleInput(void) {
	if(!ImGui::IsItemActive()) {
		return;
	}

	if(ImGui::IsKeyDown(ImGuiKey_Space)) {
		vec2s delta = IMVEC2_TO_VEC2S(ImGui::GetIO().MouseDelta);
		delta = glms_vec2_div(delta, background_imgui_size);
		delta = glms_vec2_mul(delta, num_tiles_shown);

		camera = glms_vec2_sub(camera, delta);
	}
}

void Viewer2d::showImage(void) {
	ImVec2 avail_size = ImGui::GetContentRegionAvail();
	ImVec2 cursor = ImGui::GetCursorPos();

	background_imgui_size = IMVEC2_TO_VEC2S(avail_size);
	num_tiles_shown.raw[0] = num_tiles_shown.raw[1] * avail_size.x / avail_size.y;
	half_num_tiles_shown = glms_vec2_scale(num_tiles_shown, 0.5f);
	tile_size = glms_vec2_div(background_pixel_size, num_tiles_shown);

	ImGui::Image(background_texture, avail_size);
	ImGui::SetCursorPos(cursor);
	ImGui::InvisibleButton("#invisible_button", avail_size);

	handleInput();
}

void Viewer2d::render(const World &world) {
	SDL_LockTextureToSurface(background_texture, nullptr, &background_surface);

	renderTiles(world);

	SDL_UnlockTexture(background_texture);
}

void Viewer2d::renderTiles(const World &world) {
	const vec2s min = glms_vec2_sub(
			glms_vec2_floor(glms_vec2_sub(camera, half_num_tiles_shown)),
			vec2s{1.0f, 1.0f}
			);

	const vec2s max = glms_vec2_add(
			glms_vec2_floor(glms_vec2_add(camera, half_num_tiles_shown)),
			vec2s{1.0f, 1.0f}
			);

	for(int j = min.raw[1]; j <= max.raw[1]; j++) {
		for(int i = min.raw[0]; i <= max.raw[0]; i++) {
			renderTile(world, i, j);
		}
	}
}

vec2s Viewer2d::worldToScreenCoords(float i, float j) const {
	vec2s pos = {float(i), float(j)};

	pos = glms_vec2_sub(pos, camera);
	pos = glms_vec2_add(pos, half_num_tiles_shown);
	pos = glms_vec2_mul(pos, tile_size);

	return pos;
}

void Viewer2d::renderTile(const World& world, int i, int j) {
	renderCleanTile(i, j);

	if(!world.inBounds(i, j)) return;
	const TW_TextureConfig& cfg = world.getTextureConfig(i, j, edit_texture_type);
	if(cfg.id == 0) return;

	vec2s pos = worldToScreenCoords(i, j);

	SDL_Rect dst = {
		(int) pos.raw[0], (int) pos.raw[1],
		(int) tile_size.raw[0], (int) tile_size.raw[1]
	};

	SDL_Rect src = texture_list.getCellRect(cfg.id - 1);

	SDL_BlitSurfaceScaled(
			texture_list.getSurface(),
			&src,
			background_surface,
			&dst,
			SDL_SCALEMODE_NEAREST
			);
}

void Viewer2d::renderCleanTile(int i, int j) {
	vec2s pos = worldToScreenCoords(i, j);

	SDL_Rect dst = {
		(int) pos.raw[0], (int) pos.raw[1],
		(int) tile_size.raw[0], (int) tile_size.raw[1]
	};

	SDL_FillSurfaceRect(
			background_surface,
			&dst,
			SDL_MapSurfaceRGB(background_surface, 0x77, 0x77, 0x77)
			);

	dst.x++; dst.y++; dst.w--; dst.h--;
	dst.x++; dst.y++; dst.w--; dst.h--;

	SDL_FillSurfaceRect(
			background_surface,
			&dst,
			SDL_MapSurfaceRGB(background_surface, 0x00, 0x00, 0x00)
			);
}
