#include "TextureViewer.hpp"

TextureViewer::TextureViewer(const TextureList& texture_list, const ToolList &tool_list) :
	texture_list(texture_list),
	tool_list(tool_list)
{
}

void TextureViewer::show(void) {
	ImGui::Begin("Textures");

	ImVec2 cursor = ImGui::GetCursorPos();
	ImVec2 size = ImGui::GetContentRegionAvail();
	size.y = size.x;

	ImGui::Image(texture_list.getTexture(), size);

	ImGui::SetCursorPos(cursor);
	ImGui::InvisibleButton("##texture_viewer_button", size);

	ImGui::End();
}
