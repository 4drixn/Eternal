#pragma once

#include "c_context.hpp"
#include "timer.hpp"
#include "TargetEnum.hpp"
#include "config.hpp"

namespace aimassist {
	extern void aim(c_context* ctx);

	inline auto m_enabled = true;
	inline auto m_speed = 3.f;
	inline auto m_speed_body = 5.0f;
	inline auto m_fov = 90.f;
	inline auto m_dis = 4.0f;

	inline bool vertical_aim = false; // Kinda detected atm

	// Conditions
	inline bool teams = false; // dont aim at team mates
	inline bool break_blocks = true; // dont aim while break blocks
	inline bool npcs = true; // dotn aim at npcs
	inline bool invisibles = false; // target invisibles
	inline bool visible_only = true;
	// to add: Swords, Axe only

	inline target::target_mode target_mode = target::CROSSHAIR;

	inline DWORD TIMER;
	inline std::shared_ptr<c_player> target = nullptr;

}
