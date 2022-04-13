#pragma once
#include "c_context.hpp"
#include "FontRenderer.hpp"

namespace arraylist {
	inline bool m_enabled = true;
	extern void draw(c_context* ctx);
	extern void onRender();

	inline std::shared_ptr<c_fontrenderer> fontRenderer = nullptr;
}