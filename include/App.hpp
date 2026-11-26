#ifndef APP_HPP
#define APP_HPP

#include <imgui.h>
#include <SDL3/SDL.h>
#include <memory>

#include "World.hpp"
#include "Viewer2d.hpp"
#include "TextureList.hpp"

class App {
	public:
		static std::unique_ptr<App> create(const char *title);
		~App(void);

		void run(void);
		bool loop(void); /* returns false if the loop stops. */

	private:
		SDL_Window *window = NULL;
		SDL_Renderer *renderer = NULL;
		bool quit = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		World world = World(256, 256);
		std::unique_ptr<Viewer2d> viewer2d = nullptr;
		std::unique_ptr<TextureList> texture_list = nullptr;

		void processEvents(void);

		void init(void);
		void step(void);
};

#endif
