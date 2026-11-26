#ifndef TEXTUREVIEWER_HPP
#define TEXTUREVIEWER_HPP

#include "Global.hpp"
#include "ToolList.hpp"
#include "TextureList.hpp"

class TextureViewer {
	public:
		TextureViewer(const TextureList& texture_list, const ToolList &tool_list);

		void show(void);

	private:
		const TextureList& texture_list;
		const ToolList& tool_list;
};

#endif

