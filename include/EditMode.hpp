#ifndef EDITMODE_HPP
#define EDITMODE_HPP

#include <array>

#define FOR_EDITMODE_LIST(DO) \
	DO(FLOOR,        "Floor"       ) \
	DO(WINDOW_FLOOR, "Window floor") \
	DO(WALL,         "Wall"        ) \
	DO(WINDOW_CEIL,  "Window ceil" ) \
	DO(CEIL,         "Ceil"        ) \
	DO(COLOR,        "Color"       )

#define EXPAND_AS_ENUM(id, ...) id,
enum class EditMode {
	FOR_EDITMODE_LIST(EXPAND_AS_ENUM)
	NUM
};
#undef EXPAND_AS_ENUM

#define EXPAND_AS_ARRAY(id, ...) EditMode::id,
constexpr std::array<EditMode, int(EditMode::NUM)> edit_mode_list = {
	FOR_EDITMODE_LIST(EXPAND_AS_ARRAY)
};
#undef EXPAND_AS_ARRAY

constexpr const char * EditMode_GetLabel(int mode) {
	#define EXPAND_AS_STR(id, str) str,
	constexpr std::array<const char *, int(EditMode::NUM)> edit_mode_labels = {
		FOR_EDITMODE_LIST(EXPAND_AS_STR)
	};
	#undef EXPAND_AS_STR

	return edit_mode_labels[mode];
}

constexpr const char * EditMode_GetLabel(EditMode mode) {
	return EditMode_GetLabel(int(mode));
}

#endif
