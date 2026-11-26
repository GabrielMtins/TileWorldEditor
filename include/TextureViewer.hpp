#ifndef TEXTUREVIEWER_HPP
#define TEXTUREVIEWER_HPP

#include "Global.hpp"
#include "ToolList.hpp"
#include "TextureList.hpp"

class TextureViewer {
	public:
		static std::unique_ptr<TextureViewer> create(SDL_Renderer *renderer, const TextureList& texture_list, ToolList &tool_list);
		~TextureViewer(void);

		void show(void);

	private:
		SDL_Renderer *renderer = nullptr;
		SDL_Texture *background_texture = nullptr;

		const TextureList& texture_list;
		ToolList& tool_list;

		TextureViewer(SDL_Renderer *renderer, SDL_Texture *background_texture, const TextureList& texture_list, ToolList &tool_list);
		void render(void);
		void handleInput(void);
};

#endif

