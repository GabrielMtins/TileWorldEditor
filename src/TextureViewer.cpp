#include "TextureViewer.hpp"

std::unique_ptr<TextureViewer> TextureViewer::create(SDL_Renderer *renderer, const TextureList& texture_list, ToolList &tool_list) {
	SDL_Texture *background_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA32,
			SDL_TEXTUREACCESS_TARGET,
			texture_list.getTexture()->w,
			texture_list.getTexture()->h
			);

	if(background_texture == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<TextureViewer>(new TextureViewer(
				renderer,
				background_texture,
				texture_list,
				tool_list
				));
}

TextureViewer::~TextureViewer(void) {
	SDL_DestroyTexture(background_texture);
}

void TextureViewer::show(void) {
	render();

	ImGui::Begin("Texture Viewer");

	ImVec2 cursor = ImGui::GetCursorPos();
	ImVec2 size = ImGui::GetContentRegionAvail();
	size.y = size.x;

	ImGui::Image(background_texture, size);

	ImGui::SetCursorPos(cursor);
	ImGui::InvisibleButton("##texture_viewer_button", size);

	handleInput();

	ImGui::End();
}

TextureViewer::TextureViewer(SDL_Renderer *renderer, SDL_Texture *background_texture, const TextureList& texture_list, ToolList &tool_list) :
	renderer(renderer),
	background_texture(background_texture),
	texture_list(texture_list),
	tool_list(tool_list)
{
}

void TextureViewer::render(void) {
	SDL_SetRenderTarget(renderer, background_texture);

	SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xff);
	SDL_RenderClear(renderer);

	SDL_FRect rect = texture_list.getCellFRect(tool_list.getTextureId());

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

	SDL_RenderFillRect(renderer, &rect);

	SDL_SetTextureBlendMode(texture_list.getTexture(), SDL_BLENDMODE_MUL);
	SDL_RenderTexture(renderer, texture_list.getTexture(), nullptr, nullptr);
	SDL_SetTextureBlendMode(texture_list.getTexture(), SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderRect(renderer, &rect);

	SDL_SetRenderTarget(renderer, nullptr);
}

void TextureViewer::handleInput(void) {
	if(!ImGui::IsItemActivated()) {
		return;
	}

	vec2s pos = IMVEC2_TO_VEC2S(ImGui::GetMousePos());
	pos = glms_vec2_sub(pos, IMVEC2_TO_VEC2S(ImGui::GetItemRectMin()));
	pos = glms_vec2_div(pos, IMVEC2_TO_VEC2S(ImGui::GetItemRectSize()));
	pos = glms_vec2_mul(pos, POINT2I_TO_VEC2S(texture_list.getNumCells()));

	Point2i pos_i = VEC2S_TO_POINT2I(pos);
	uint8_t texture_id = pos_i.first + pos_i.second * texture_list.getNumCells().first;

	tool_list.setTextureId(texture_id);
}
