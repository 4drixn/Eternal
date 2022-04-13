#include "FastStop.hpp"
#include "timer.hpp"

void faststop::faststop(c_context* ctx)
{

	static std::shared_ptr<c_timer> timer_uwu = std::make_shared<c_timer>();

	if (!timer_uwu->has_passed(40))
		return;

	auto local_player = ctx->local_player->get();

	if (local_player->get_moveforward() == 0.2f && local_player->get_movestrafe() == 0)
	{
		local_player->SetmotionX(0);
		local_player->SetmotionZ(0);
	}
		
	timer_uwu->reset();
}