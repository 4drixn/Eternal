#pragma once

#include "c_context.hpp"
#include "Packet.hpp"
#include "timer.hpp"
#include "Killaura.hpp"
#include "clicker.hpp"

namespace Criticals {
	extern void crits(c_context* ctx);
	inline bool m_enabled = false;
}