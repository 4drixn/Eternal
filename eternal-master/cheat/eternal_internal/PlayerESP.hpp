#pragma once

#include "c_context.hpp"
#include "Timer.h"
#include "timer.hpp"

namespace playeresp {
	inline bool m_enabled = false;
	extern void esp(c_context* ctx);

	extern std::vector<vec4> vecs;
	extern GLfloat* viewport;

	inline bool skeleton = false;
}