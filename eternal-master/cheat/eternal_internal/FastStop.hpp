#pragma once
#include "c_context.hpp"

namespace faststop {
	inline bool m_enabled = false;
	inline float m_delay = 50;
	extern void faststop(c_context* ctx);
}