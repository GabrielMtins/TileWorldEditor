#include "Viewer2d.hpp"

#include <imgui.h>

std::unique_ptr<Viewer2d> Viewer2d::create(SDL_Renderer *renderer, const TextureList& texture_list, ToolList& tool_list) {
	SDL_Texture *background_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_TARGET,
			background_width,
			background_height
			);

	if(background_texture == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<Viewer2d>(new Viewer2d(renderer, background_texture, texture_list, tool_list));
}

Viewer2d::~Viewer2d(void) {
	SDL_DestroyTexture(background_texture);
}

void Viewer2d::show(World &world) {
	ImGui::Begin("Editor");
	showComboMenu();
	showImage();
	handleInput(world);
	handleKeyboard(world);
	ImGui::End();

	render(world);
}

Viewer2d::Viewer2d(SDL_Renderer *renderer, SDL_Texture *background_texture, const TextureList& texture_list, ToolList& tool_list) :
	renderer(renderer),
	background_texture(background_texture),
	texture_list(texture_list),
	tool_list(tool_list)
{
}

void Viewer2d::handleInput(World& world) {
	if(!ImGui::IsItemActive()) {
		return;
	}

	if(ImGui::IsKeyDown(ImGuiKey_Space)) {
		vec2s delta = IMVEC2_TO_VEC2S(ImGui::GetIO().MouseDelta);
		delta = glms_vec2_div(delta, background_imgui_size);
		delta = glms_vec2_mul(delta, num_tiles_shown);

		camera = glms_vec2_sub(camera, delta);
		return;
	}

	vec2s cursor_pos = IMVEC2_TO_VEC2S(ImGui::GetMousePos());

	cursor_pos = glms_vec2_sub(cursor_pos, IMVEC2_TO_VEC2S(ImGui::GetItemRectMin()));
	cursor_pos = glms_vec2_div(cursor_pos, background_imgui_size);
	cursor_pos = glms_vec2_mul(cursor_pos, num_tiles_shown);
	cursor_pos = glms_vec2_add(cursor_pos, camera);
	cursor_pos = glms_vec2_sub(cursor_pos, half_num_tiles_shown);

	tool_list.setEditMode(edit_mode);
	tool_list.use(world, cursor_pos);
}

void Viewer2d::handleKeyboard(World& world) {
	(void) world;

	if(ImGui::IsKeyDown(ImGuiKey_Escape)) {
		tool_list.cancelSelection();
	}

	for(int i = 0; i < int(EditMode::NUM); i++) {
		if(ImGui::IsKeyDown(ImGuiKey(ImGuiKey_1 + i))) {
			edit_mode = EditMode(i);
		}
	}
}

void Viewer2d::showComboMenu(void) {
	ImGui::Text("Edit mode:");

	ImGui::SameLine();

	if(ImGui::BeginCombo("##texture_type", EditMode_GetLabel(edit_mode))) {
		for(EditMode i : edit_mode_list) {
			bool res = ImGui::Selectable(
					EditMode_GetLabel(i),
					i == edit_mode
					);

			if(res) {
				edit_mode = i;
			}

		}

		ImGui::EndCombo();
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
}

void Viewer2d::render(const World &world) {
	SDL_SetRenderTarget(renderer, background_texture);
	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xff);
	SDL_RenderClear(renderer);

	renderTiles(world);
	renderSelection();

	SDL_SetRenderTarget(renderer, nullptr);
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
			renderTileByMode(world, i, j);
		}
	}
}

vec2s Viewer2d::worldToScreenCoords(vec2s pos) const {
	pos = glms_vec2_sub(pos, camera);
	pos = glms_vec2_add(pos, half_num_tiles_shown);
	pos = glms_vec2_mul(pos, tile_size);

	return pos;
}

vec2s Viewer2d::worldToScreenCoords(int i, int j) const {
	return worldToScreenCoords(vec2s{float(i), float(j)});
}

SDL_FRect Viewer2d::worldCoordsToScreenTileRect(int i, int j) const {
	vec2s pos = worldToScreenCoords(i, j);

	return SDL_FRect{
		pos.raw[0], pos.raw[1],
		tile_size.raw[0], tile_size.raw[1]
	};
}

void Viewer2d::renderTileByMode(const World& world, int i, int j) {
	if(!world.inBounds(i, j)) return;

	switch(edit_mode) {
		case EditMode::WALL:
			renderTileByType(world, i, j, World::TextureType::FLOOR);
			renderDarkTileIfCellIsNotEmpty(world, i, j, World::TextureType::FLOOR);
			renderTileByType(world, i, j, World::TextureType::WALL);
			break;

		case EditMode::FLOOR:
			renderTileByType(world, i, j, World::TextureType::FLOOR);
			renderTileByType(world, i, j, World::TextureType::WALL);
			renderDarkTileIfCellIsNotEmpty(world, i, j, World::TextureType::WALL);
			break;

		case EditMode::CEIL:
			renderTileByType(world, i, j, World::TextureType::FLOOR);
			renderDarkTileIfCellIsNotEmpty(world, i, j, World::TextureType::FLOOR);
			renderTileByType(world, i, j, World::TextureType::WALL);
			renderDarkTileIfCellIsNotEmpty(world, i, j, World::TextureType::WALL);
			renderTileByType(world, i, j, World::TextureType::CEIL);
			break;

		case EditMode::WINDOW_CEIL:
			renderTileByType(world, i, j, World::TextureType::WINDOW_CEIL);
			break;

		case EditMode::WINDOW_FLOOR:
			renderTileByType(world, i, j, World::TextureType::WINDOW_FLOOR);
			break;

		default:
			break;
	}

	renderGridTile(i, j);
}

void Viewer2d::renderTileByType(const World& world, int i, int j, World::TextureType type) {
	const TW_TextureConfig& cfg = world.getTextureConfig(i, j, type);

	if(cfg.id == 0) return;

	SDL_FRect dst = worldCoordsToScreenTileRect(i, j);
	SDL_FRect src = texture_list.getCellFRect(cfg.id);

	SDL_RenderTexture(
			renderer,
			texture_list.getTexture(),
			&src,
			&dst
			);
}

void Viewer2d::renderGridTile(int i, int j) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

	SDL_FRect dst = worldCoordsToScreenTileRect(i, j);

	SDL_RenderRect(renderer, &dst);
}

void Viewer2d::renderDarkTileIfCellIsNotEmpty(const World& world, int i, int j, World::TextureType type) {
	const TW_TextureConfig& cfg = world.getTextureConfig(i, j, type);

	if(cfg.id == 0) return;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x80);

	SDL_FRect dst = worldCoordsToScreenTileRect(i, j);

	SDL_RenderFillRect(renderer, &dst);
}

void Viewer2d::renderSelection(void) {
	const auto& selection = tool_list.getSelection();

	if(!selection.active) {
		return;
	}

	vec2s min = worldToScreenCoords(POINT2I_TO_VEC2S(selection.min));
	vec2s max = worldToScreenCoords(POINT2I_TO_VEC2S(selection.max));
	vec2s size = glms_vec2_sub(max, min);

	SDL_FRect screen_rect = {
		0, 0, (float) background_width, (float) background_height
	};

	SDL_FRect selection_rect = {
		min.raw[0],  min.raw[1],
		size.raw[0], size.raw[1]
	};

	SDL_FRect intersect;

	SDL_GetRectIntersectionFloat(&screen_rect, &selection_rect, &intersect);
	
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x80);
	SDL_RenderFillRect(renderer, &intersect);
}
