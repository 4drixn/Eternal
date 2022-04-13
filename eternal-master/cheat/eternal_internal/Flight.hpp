#pragma once
#include "c_context.hpp"

namespace flight {
	inline bool m_enabled = false;
	inline float m_speed = 10.f;
	extern void flight(c_context* ctx);
}