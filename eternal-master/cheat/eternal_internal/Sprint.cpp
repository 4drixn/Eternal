#include "Sprint.hpp"
#include "timer.hpp"

void sprint::sprint(c_context* ctx)
{
	static std::shared_ptr<c_timer> timer_uwu = std::make_shared<c_timer>();

	if (!timer_uwu->has_passed(40))
		return;

	auto local_player = ctx->local_player->get();

	if (local_player->is_sneaking()) return;
	//if (local_player->is_inwater()) return;

	if (local_player->get_moveforward() > 0.f)
		if (!local_player->is_sprinting())
			local_player->set_sprinting(true);
}