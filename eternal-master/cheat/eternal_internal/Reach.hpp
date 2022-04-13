#pragma once

#include "c_context.hpp"
#include "timer.hpp"
#include "vec3.hpp"
#include <limits>

namespace reach {
	extern void reach(c_context* ctx);
	extern void reach_();

	inline auto m_enabled = true;
	inline auto m_reach = 4.f;
	inline auto hitbox_size = 0.0f;

	// Conditions:
	inline bool visible_only = false;
	inline bool only_while_sprinting = false;
	inline bool disable_in_water = false;


	// Disable reach while in water
	// Only while Sprinting
	// Visible only (for hitboxes especially so you wont hit people through walls lol)
    
}