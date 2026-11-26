#ifndef VIEWER2D_HPP
#define VIEWER2D_HPP

#include "Global.hpp"
#include "World.hpp"
#include "TextureList.hpp"
#include "ToolList.hpp"

class Viewer2d {
	public:
		static std::unique_ptr<Viewer2d> create(SDL_Renderer *renderer, const TextureList& texture_list, ToolList& tool_list);
		void show(World &world);

	private:
		static constexpr int background_width = 800;
		static constexpr int background_height = 800;

		SDL_Renderer *renderer = nullptr;
		SDL_Texture *background_texture = nullptr;

		const vec2s background_pixel_size = {float(background_width), float(background_height)};

		vec2s background_imgui_size;
		vec2s num_tiles_shown = {{16, 12}};
		vec2s half_num_tiles_shown = {0};
		vec2s tile_size = {0};

		vec2s camera = glms_vec2_scale(num_tiles_shown, 0.5f);
		const TextureList& texture_list;
		ToolList& tool_list;

		EditMode edit_mode = EditMode::WALL;
		World::TextureType edit_texture_type = World::TextureType::WALL;

		Viewer2d(SDL_Renderer *renderer, SDL_Texture *background_texture, const TextureList& texture_list, ToolList& tool_list);
		void handleInput(World& world);
		void showComboMenu(void);
		void showImage(void);
		void render(const World &world);
		void renderTiles(const World &world);
		vec2s worldToScreenCoords(vec2s pos) const;
		vec2s worldToScreenCoords(float i, float j) const;
		void renderTile(const World& world, int i, int j);
		void renderCleanTile(int i, int j);

		void renderSelection(void);
};

#endif
