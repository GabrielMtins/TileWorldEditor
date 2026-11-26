#include "Toolbar.hpp"

Tool Toolbar::getTool(void) const {
	return tool;
}

void Toolbar::show(void) {
	ImGui::Begin("Toolbar");
	ImVec2 size = ImGui::GetContentRegionAvail();
	size.y = size.x;

	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
	#define EXPAND_AS_SELECTABLES(id, func, key, icon) \
		if(ImGui::Selectable(icon, tool == Tool::id, 0, size)) { \
			tool = Tool::id; \
		}
	FOR_TOOL_LIST(EXPAND_AS_SELECTABLES)
	#undef EXPAND_AS_SELECTABLES
	ImGui::PopStyleVar();

	ImGui::End();

	handleShortcuts();
}

void Toolbar::handleShortcuts(void) {
	#define EXPAND_AS_SHORTCUTS(id, func, key, icon) \
		if(ImGui::IsKeyPressed(key)) tool = Tool::id;
	FOR_TOOL_LIST(EXPAND_AS_SHORTCUTS)
	#undef EXPAND_AS_SHORTCUTS
}
