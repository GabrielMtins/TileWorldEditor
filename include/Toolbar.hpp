#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "Global.hpp"
#include "ToolList.hpp"

class Toolbar {
	public:
		Tool getTool(void) const;
		void show(void);

	private:
		Tool tool = Tool::BRUSH;

		void handleShortcuts(void);
};

#endif
