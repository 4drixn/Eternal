#pragma once

#include "c_context.hpp"
#include "timer.hpp"
#include "vec3.hpp"
#include <limits>

namespace m_timer {

	extern void timer_(c_context* ctx);

	inline bool enabled = false;
	inline float value = 2.f;

	// Conditions
	inline bool strafe_only = false; 
	inline bool only_while_breaking_blocks = true;
}