#include "ToolList.hpp"

#include <utility>
#include <stack>

void ToolList::setEditMode(EditMode mode) {
	this->mode = mode;
}

void ToolList::setTool(Tool tool) {
	this->tool = tool;
}

const Selection& ToolList::getSelection(void) const {
	return selection;
}

void ToolList::use(World& world, int i, int j) {
	#define EXPAND_AS_CASE(id, func) case Tool::id: func(world, i, j); break;
	switch(tool) {
		FOR_TOOL_LIST(EXPAND_AS_CASE)
	}
	#undef EXPAND_AS_CASE
}

void ToolList::editTile(World& world, int i, int j) const {
	if(!world.inBounds(i, j)) return;
	if(!selection.inBounds(i, j)) return;

	TW_Tile& tile = world.getTile(i, j);

	switch(mode) {
		case EditMode::FLOOR:
			tile.textures.floor.id = texture_id;
			break;

		case EditMode::WINDOW_FLOOR:
			tile.textures.window_floor.id = texture_id;
			break;

		case EditMode::WALL:
			tile.textures.wall.id = texture_id;
			break;

		case EditMode::WINDOW_CEIL:
			tile.textures.window_ceil.id = texture_id;
			break;

		case EditMode::CEIL:
			tile.textures.ceil.id = texture_id;
			break;

		default:
			break;
	}
}

void ToolList::useBrush(World& world, int i, int j) {
	if(!world.inBounds(i, j)) return;
	if(!selection.inBounds(i, j)) return;

	editTile(world, i, j);
}

void ToolList::useSelection(World& world, int i, int j) {
	if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		selection.active = false;
	}

	if(!world.inBounds(i, j)) return;

	if(!selection.active) {
		selection.begin = Point2i(i, j);
	}

	selection.end = Point2i(i, j);
	selection.setMinMax();
}

void ToolList::useBucket(World& world, int i, int j) {
	if(!world.inBounds(i, j)) return;
	if(!ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		return;
	}

	const TW_Tile begin_tile = world.getTile(i, j);
	editTile(world, i, j);
	const TW_Tile edited_tile = world.getTile(i, j);

	std::stack<std::pair<int, int>> to_visit;
	to_visit.emplace(i + 1, j);
	to_visit.emplace(i - 1, j);
	to_visit.emplace(i, j + 1);
	to_visit.emplace(i, j - 1);

	while(!to_visit.empty()) {
		auto [x, y] = to_visit.top();
		to_visit.pop();

		if(!world.inBounds(x, y)) {
			continue;
		}

		if(!world.tileEquals(mode, begin_tile, x, y)) {
			continue;
		}

		if(world.tileEquals(mode, edited_tile, x, y)) {
			continue;
		}

		editTile(world, x, y);
		to_visit.emplace(x + 1, y);
		to_visit.emplace(x - 1, y);
		to_visit.emplace(x, y + 1);
		to_visit.emplace(x, y - 1);
	}
}
