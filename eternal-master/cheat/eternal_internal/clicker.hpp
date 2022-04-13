#pragma once

#include "c_context.hpp"
#include "GuiScreen.hpp"
#include "global.hpp"
#include <limits>
#include "utils.hpp"
#include "config.hpp"

namespace clicker {
	extern void clicker(c_context* ctx);

	inline int cps = 13;

	inline bool m_enabled = false;

	inline bool break_blocks = true;
	inline bool sword_only = false;
	inline bool axe_only = false;
	inline bool inventory_click = false;

	inline bool is_clicking = false;
}
