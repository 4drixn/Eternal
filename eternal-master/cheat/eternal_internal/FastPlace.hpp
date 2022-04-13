#pragma once
#include "c_context.hpp"
#include "timer.hpp"
#include <limits>
namespace misc{
	namespace fastplace {
		extern void fast_place(c_context* ctx);
		inline int ticks = 0;
		inline bool m_enabled = true;
	}
}