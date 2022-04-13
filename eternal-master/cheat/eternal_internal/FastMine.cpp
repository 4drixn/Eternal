#include "FastMine.hpp"
#include "timer.hpp"

void fastmine::fastmine(c_context* ctx)
{
	float perTickBoost = 0.04f;

	static std::shared_ptr<c_timer> timer_uwu = std::make_shared<c_timer>();

	if (!fastmine::m_enabled)
		return;

	if (!timer_uwu->has_passed(40))
		return;

	auto local_player = ctx->local_player->get();

	local_player->set_block_hit_delay(0);

	float damage = local_player->get_current_block_dmg();

	if (damage >= (float)((float)fastmine::val / 100))
	{
		damage = 1.0f;
	}
	//damage += perTickBoost;

	local_player->set_current_block_dmg(damage);

	timer_uwu->reset();
}