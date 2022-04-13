#include "m_timer.hpp"
#include "Timer.h"

void m_timer::timer_(c_context* ctx)
{
	static std::shared_ptr<c_timer> timer_uwu = std::make_shared<c_timer>();
	auto timer_t = std::make_unique<timer>(ctx->minecraft->get()->getTimer());

	if (!timer_uwu->has_passed(50))
		return;

	if (m_timer::enabled)
	{
		if (m_timer::only_while_breaking_blocks)
		{
			if (ctx->local_player->get()->is_breaking_block())
			{
				timer_t->set_timer_speed(m_timer::value);
			}
			else
			{
				timer_t->set_timer_speed(1.f);
			}
		}
		if (m_timer::strafe_only)
		{
			if (ctx->local_player->get()->get_hurt_time() == 0
				&& (ctx->local_player->get()->get_moveforward() != .0f
					&& ctx->local_player->get()->get_movestrafe() != .0f))
			{
				timer_t->set_timer_speed(m_timer::value);
			}
			else
			{
				timer_t->set_timer_speed(1.f);
			}


		}
		if (m_timer::strafe_only == false && m_timer::only_while_breaking_blocks == false)
		{
			//timer_t->set_timer_speed(m_timer::value);
		}
	}
	timer_uwu->reset();
}