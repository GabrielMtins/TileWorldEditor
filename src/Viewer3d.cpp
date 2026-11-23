#include "Viewer3d.hpp"

std::unique_ptr<Viewer3d> Viewer3d::create(SDL_Renderer *renderer) {
	SDL_Texture *texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_STREAMING,
			width,
			height
			);

	if(texture == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<Viewer3d>(new Viewer3d(texture));
}

void Viewer3d::addVertex(void *userdata, const TW_Vertex *vertex) {
	Viewer3d *ptr = (Viewer3d *) userdata;

	ptr->vertices.push_back(*vertex);
}

void Viewer3d::updateVertices(const TW_World& world) {
	TW_VertexContext ctx;

	ctx.world = &world;
	ctx.addVertexToUserdata = addVertex;
	ctx.userdata = this;

	vertices.clear();
	TW_ComputeStaticVertices(&ctx);
}

Viewer3d::Viewer3d(SDL_Texture *texture) :
	texture(texture),
	cg(TCG_BaseData(TCG_TRIANGLE, sizeof(TW_Vertex) / sizeof(float), 0))
{
}

void Viewer3d::putPixel(int x, int y, const TW_Vertex& vertex) {
	uint8_t r, g, b;
	uint32_t *pixel = ((uint32_t *) (surface->pixels));
	pixel += x + y * surface->pitch / 4;

	switch(view_mode) {
		case ViewMode::SOLID:
			break;

		default:
			break;
	}
}
