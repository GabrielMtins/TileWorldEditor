#include "App.hpp"
#include "font_awesome.h"

#include <cstdlib>

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

std::unique_ptr<App> App::create(const char *title) {
	auto app = std::make_unique<App>();

	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

	app->window = SDL_CreateWindow(
			title, main_scale * 1280.0f, main_scale * 720.0f,
			SDL_WINDOW_RESIZABLE
			);

	if(app->window == NULL) {
		return nullptr;
	}

	app->renderer = SDL_CreateRenderer(
			app->window,
			NULL
			);

	if(app->renderer == NULL) {
		return nullptr;
	}

	SDL_SetRenderVSync(app->renderer, 1);
	SDL_SetWindowPosition(app->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark();

    style.FontSizeBase = 18.0f;

	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImFontConfig icon_config{};

	io.Fonts->AddFontDefault();
	icon_config.MergeMode = true;
	static const ImWchar icon_ranges[] = {0xf000, 0xf3ff, 0};

	io.Fonts->AddFontFromMemoryTTF(
			(void *) font_awesome_otf,
			font_awesome_otf_len,
			0.0f,
			&icon_config,
			icon_ranges
			);

	ImGui_ImplSDL3_InitForSDLRenderer(app->window, app->renderer);
	ImGui_ImplSDLRenderer3_Init(app->renderer);

	app->init();

	return app;
}

App::~App(void) {
	if(window != NULL) {
		SDL_DestroyWindow(window);
	}

	if(renderer != NULL) {
		SDL_DestroyRenderer(renderer);
	}
}

void App::run(void) {
	while(!quit) {
		loop();
	}
}

bool App::loop(void) {
	processEvents();

	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	step();

	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
	SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_RenderPresent(renderer);

	return !quit;
}

void App::processEvents(void) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);
		if(event.type == SDL_EVENT_QUIT) quit = true;
	}
}

/* Insert your code here. */
void App::init(void) {
	SDL_SetWindowFullscreen(window, true);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	texture_list = TextureList::load(renderer, "tilemap.png", 64, 64);
	assert(texture_list);

	viewer2d = Viewer2d::create(renderer, *texture_list, tool_list);
	assert(viewer2d);

	texture_viewer = TextureViewer::create(renderer, *texture_list, tool_list);
	assert(texture_viewer);
}

void App::step(void) {
	viewer2d->show(world);
	texture_viewer->show();
	toolbar.show();
	tool_list.setTool(toolbar.getTool());
}
