#ifndef VIEWER3D_HPP
#define VIEWER3D_HPP

#include "Global.hpp"

#include <TinyCG.h>
#include <TileWorld.h>
#include <array>

class Viewer3d {
	public:
		static std::unique_ptr<Viewer3d> create(SDL_Renderer *renderer);
		static void addVertex(void *userdata, const TW_Vertex *vertex);
		void updateVertices(const TW_World& world);
		void show(void); /* TODO */

	private:
		static constexpr int width = 640;
		static constexpr int height = 480;

		std::array<float, width * height> depth_buffer;

		enum class ViewMode {
			SOLID = 0,
		};

		ViewMode view_mode = ViewMode::SOLID;

		SDL_Texture *texture;
		SDL_Surface *surface = nullptr;
		std::vector<TW_Vertex> vertices;
		TCG_Data cg;

		Viewer3d(SDL_Texture *texture);
		static vec3s getSolidColor(const TW_Vertex &vertex);
		void putPixel(int x, int y, const TW_Vertex& vertex);
};

#endif
