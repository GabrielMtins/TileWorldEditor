#include "Viewer3d.hpp"

#include <math.h>

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

vec3s Viewer3d::getSolidColor(const TW_Vertex &vertex) {
	vec3s normal = glms_vec3_make(vertex.normal);
	float res = 0.0f;

	if(fabsf(normal.raw[0]) > 0.9f) {
		res = 0.4f;
	}

	if(fabsf(normal.raw[2]) > 0.9f) {
		res = 0.7f;
	}

	if(fabsf(normal.raw[1]) > 0.9f) {
		res = 1.0f;
	}

	return vec3s{{res, res, res}};
}

void Viewer3d::putPixel(int x, int y, const TW_Vertex& vertex) {
	uint32_t *pixel = ((uint32_t *) (surface->pixels));
	pixel += x + y * surface->pitch / 4;
	vec3s color = {0};

	switch(view_mode) {
		case ViewMode::SOLID:
			color = getSolidColor(vertex);
			break;

		default:
			break;
	}

	*pixel = SDL_MapSurfaceRGB(
			surface,
			color.raw[0] * 255,
			color.raw[1] * 255,
			color.raw[2] * 255
			);
}
