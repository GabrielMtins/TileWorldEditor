#ifndef TOOL_LIST_HPP
#define TOOL_LIST_HPP

#include "Global.hpp"
#include "World.hpp"

#include <algorithm>

#define FOR_TOOL_LIST(DO) \
	DO(BRUSH,     useBrush    ) \
	DO(SELECTION, useSelection) \
	DO(BUCKET,    useBucket   )

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
		if(i > max.first) return false;
		if(j > max.second) return false;

		return true;
	}

	constexpr void setMinMax(void) {
		min.first = std::min(begin.first, end.first);
		min.second = std::min(begin.second, end.second);

		max.first = std::max(begin.first, end.first);
		max.second = std::max(begin.second, end.second);
	}
};

class ToolList {
	public:
		#define EXPAND_AS_ENUM(id, ...) id,
		enum class Tool {
			FOR_TOOL_LIST(EXPAND_AS_ENUM)
		};
		#undef EXPAND_AS_ENUM

		void setEditMode(EditMode mode);
		void setTool(Tool tool);
		void setTexture(uint8_t texture_id);
		const Selection& getSelection(void) const;
		void use(World& world, int i, int j);
	
	private:
		Tool tool = Tool::BRUSH;
		uint8_t texture_id = 1;
		EditMode mode = EditMode::WALL;

		Selection selection;

		void editTile(World& world, int i, int j) const;

		#define EXPAND_AS_DECLARATION(id, func) void func(World& world, int i, int j);
		FOR_TOOL_LIST(EXPAND_AS_DECLARATION)
		#undef EXPAND_AS_DECLARATION
};

#endif
