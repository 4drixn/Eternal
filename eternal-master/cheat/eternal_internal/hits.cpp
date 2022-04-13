#include "hits.hpp"

namespace misc {
	void fix_hit_delay(c_context* ctx)
	{
		static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();
		if (!timer->has_passed(100))
		return;

		ctx->minecraft->get()->set_left_click_counter(0);

		timer->reset();
	}
}