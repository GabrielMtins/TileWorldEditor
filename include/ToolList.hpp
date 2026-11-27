#ifndef TOOL_LIST_HPP
#define TOOL_LIST_HPP

#include "Global.hpp"
#include "World.hpp"

#include <algorithm>

#define FOR_TOOL_LIST(DO) \
	DO(SELECTION, useSelection, ImGuiKey_V, "\uf245") \
	DO(BRUSH,     useBrush,     ImGuiKey_B, "\uf304") \
	DO(ERASER,    useEraser,    ImGuiKey_X, "\uf12d") \
	DO(BUCKET,    useBucket,    ImGuiKey_F, "\uf575") \
	DO(PICKER,    usePicker,    ImGuiKey_I, "\uf1fb")

#define EXPAND_AS_ENUM(id, ...) id,
enum class Tool {
	FOR_TOOL_LIST(EXPAND_AS_ENUM)
};
#undef EXPAND_AS_ENUM

struct Selection {
	bool active = false;
	Point2i begin = Point2i(0, 0);
	Point2i end = Point2i(0, 0);

	Point2i min = Point2i(0, 0);
	Point2i max = Point2i(0, 0);

	constexpr bool inBounds(int i, int j) const {
		if(!active) return true;

		if(i < min.first) return false;
		if(j < min.second) return false;
		if(i >= max.first) return false;
		if(j >= max.second) return false;

		return true;
	}

	constexpr bool inBounds(vec2s pos) const {
		if(!active) return true;

		if(pos.raw[0] < min.first) return false;
		if(pos.raw[1] < min.second) return false;

		if(pos.raw[0] >= max.first) return false;
		if(pos.raw[1] >= max.second) return false;

		return true;
	}

	constexpr void setMinMax(void) {
		min.first = std::min(begin.first, end.first);
		min.second = std::min(begin.second, end.second);

		max.first = std::max(begin.first, end.first) + 1;
		max.second = std::max(begin.second, end.second) + 1;
	}
};

class ToolList {
	public:
		void setEditMode(EditMode mode);
		void setTool(Tool tool);
		void setTextureId(uint8_t texture_id);
		uint8_t getTextureId(void) const;
		const Selection& getSelection(void) const;
		void cancelSelection(void);
		void use(World& world, vec2s pos);
	
	private:
		Tool tool = Tool::SELECTION;
		uint8_t texture_id = 1;
		EditMode mode = EditMode::WALL;

		Selection selection;

		void editTile(World& world, int i, int j) const;
		void editTile(World& world, vec2s pos) const;

		#define EXPAND_AS_DECLARATION(id, func, ...) void func(World& world, vec2s pos);
		FOR_TOOL_LIST(EXPAND_AS_DECLARATION)
		#undef EXPAND_AS_DECLARATION
};

#endif
