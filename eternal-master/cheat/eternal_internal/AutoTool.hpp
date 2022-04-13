#pragma once
#include "c_context.hpp"

namespace autotool {
	inline bool m_enabled = false;
	inline float m_delay = 50;
	extern void autotool(c_context* ctx);
}