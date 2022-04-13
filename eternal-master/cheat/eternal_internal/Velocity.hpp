#pragma once
#include "c_context.hpp"
#include "global.hpp"
#include <limits>
#include "utils.hpp"
#include "VelocityEnum.hpp"

namespace velocity {
	void velocity(c_context* context);

	inline auto m_enabled = true;
	inline float velocity_h = 70.;
	inline float velocity_v = 100.;

	inline MODE m_type = NORMAL;

	inline int delay_ticks = 0;
}