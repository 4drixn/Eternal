#pragma once
#include "c_context.hpp"

namespace bridgeassist {
	inline bool m_enabled = true;
	inline bool crouch_first = true;
	inline bool block_only = true;

	inline int m_delay = 0;
	extern void bridge(c_context* ctx);
}