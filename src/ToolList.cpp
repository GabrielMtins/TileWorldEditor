#include "ToolList.hpp"

#include <utility>
#include <stack>

void ToolList::setEditMode(EditMode mode) {
	this->mode = mode;
}

void ToolList::setTool(Tool tool) {
	this->tool = tool;
}

void ToolList::setTextureId(uint8_t texture_id) {
	this->texture_id = texture_id;
}

uint8_t ToolList::getTextureId(void) const {
	return texture_id;
}

const Selection& ToolList::getSelection(void) const {
	return selection;
}

void ToolList::cancelSelection(void) {
	selection.active = false;
}

void ToolList::deleteSelectionContent(World& world) {
	if(!selection.active) {
		return;
	}

	for(int j = selection.min.second; j < selection.max.second; j++) {
		for(int i = selection.min.first; i < selection.max.first; i++) {
			useEraser(world, POINT2I_TO_VEC2S(Point2i(i, j)));
		}
	}

	cancelSelection();
}

void ToolList::use(World& world, vec2s pos) {
	#define EXPAND_AS_CASE(id, func, ...) case Tool::id: func(world, pos); break;
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

void ToolList::editTile(World& world, vec2s pos) const {
	editTile(
			world,
			int(pos.raw[0]),
			int(pos.raw[1])
			);
}

void ToolList::useBrush(World& world, vec2s pos) {
	if(!world.inBounds(pos)) return;
	if(!selection.inBounds(pos)) return;

	Point2i current_point = VEC2S_TO_POINT2I(pos);

	if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		brush_data.waiting_for_second_point = true;
		brush_data.first_point = current_point;
	}

	if(brush_data.waiting_for_second_point) {
		brush_data.second_point = VEC2S_TO_POINT2I(pos);

		if(brush_data.second_point != brush_data.first_point) {
			brush_data.waiting_for_second_point = false;

			if(brush_data.second_point.first == brush_data.first_point.first) {
				brush_data.axis = Axis::Y;
			} else {
				brush_data.axis = Axis::X;
			}
		}
	} else if(ImGui::IsKeyDown(ImGuiKey_LeftShift)){
		if(brush_data.axis == Axis::X) {
			current_point.second = brush_data.first_point.second;
		} else {
			current_point.first = brush_data.first_point.first;
		}
	}

	editTile(world, current_point.first, current_point.second);
}

void ToolList::useEraser(World& world, vec2s pos) {
	if(!world.inBounds(pos)) return;
	if(!selection.inBounds(pos)) return;

	uint8_t old_texture_id = texture_id;
	texture_id = 0;

	editTile(world, pos);

	texture_id = old_texture_id;
}

void ToolList::useSelection(World& world, vec2s pos) {
	if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		selection.active = false;
	}

	if(!world.inBounds(pos)) return;

	if(!selection.active) {
		selection.begin = VEC2S_TO_POINT2I(pos);
		selection.active = true;
	}

	selection.end = VEC2S_TO_POINT2I(pos);

	selection.setMinMax();
}

void ToolList::useBucket(World& world, vec2s pos) {
	if(!world.inBounds(pos)) return;
	if(!ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		return;
	}

	auto [i, j] = VEC2S_TO_POINT2I(pos);

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

		if(!selection.inBounds(x, y)) {
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

// TODO
void ToolList::usePicker(World& world, vec2s pos) {
	if(!world.inBounds(pos)) {
		return;
	}
}
