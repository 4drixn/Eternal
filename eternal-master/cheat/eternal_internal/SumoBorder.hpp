#pragma once

#include "c_context.hpp"
#include "timer.hpp"
#include "vec3.hpp"
#include <limits>

namespace misc {
	
	namespace sumoborder {
		extern void onUpdate (c_context* ctx);
		extern void onDisable(c_context* ctx);

		inline bool enabled = false;
		inline bool should_disable = false;
	}
}