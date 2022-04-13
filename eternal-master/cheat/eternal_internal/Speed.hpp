#pragma once
#include "c_context.hpp"
#include "config.hpp"

namespace speed {
	extern void speed_(c_context* ctx);

	inline bool m_enabled = false;
	inline float speed__ = 0.f;
	inline float m_offset = 0.02f;
	inline float y_motion = 0.31;
	inline float m_speed = 0.70;
}