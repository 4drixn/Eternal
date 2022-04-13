#pragma once
#include "AntiBotEnum.hpp"
#include "c_context.hpp"

namespace antibot {

	inline bool m_enabled = true;
	inline MODE mode = HYPIXEL;

	inline bool remove_entity = true;
	extern void antibot(c_context* ctx);
}