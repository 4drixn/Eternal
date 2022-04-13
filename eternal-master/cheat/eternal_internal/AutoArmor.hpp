#pragma once

#include "c_context.hpp"
#include "timer.hpp"
#include "GuiScreen.hpp"

namespace autoarmor {
	inline bool m_enabled = false;
	inline bool m_delay = 50;

	inline bool m_inventory = false;
	inline bool m_standing_still = true;

	extern void autoarmor_();
}