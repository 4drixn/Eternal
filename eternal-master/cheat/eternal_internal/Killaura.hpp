#pragma once

#include <limits>
#include "c_context.hpp"
#include "timer.hpp"
#include "vec3.hpp"
#include "config.hpp"
#include "Packet.hpp"
#include "ItemRenderer.hpp"
#include "player_utils.hpp"


namespace killaura {
	inline std::shared_ptr<c_player> target = nullptr;
	inline std::vector<std::shared_ptr<c_player>> targetlist;
	inline std::vector<std::string> targetlistnames;
	extern void aura(c_context* context);

	inline bool m_enabled = false;
	inline float m_silent = true;

	inline float m_fov = 120.f;
	inline float m_dist = 15.f;
	inline float m_cps = 13.f;
}