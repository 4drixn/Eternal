#pragma once
#include "c_context.hpp"

namespace cheststealer {

	inline bool m_enabled = false;
	inline int m_delay = 100; // to bypass anticheats
	extern void stealer(c_context* ctx);
}