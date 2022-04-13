#include "FastPlace.hpp"

namespace misc {
	void fastplace::fast_place(c_context* ctx)
	{
		static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();
		if (!timer->has_passed(20))
			return;
		timer->reset();
		if (!m_enabled)
			return;

		auto current_item_stack = std::make_unique<c_itemstack>(ctx->local_player->get()->get_current_equiped_item());
		if (current_item_stack->get_object() != nullptr)
		{
			if (auto current_item = std::make_unique<c_item>(current_item_stack->get_item()); current_item->get_object() != nullptr)
			{
				if (current_item->is_block() == true)
				{
					ctx->minecraft->get()->set_r_click_delay(fastplace::ticks);
				}

				else {
					ctx->minecraft->get()->set_r_click_delay(5); //6
				}
			}
		}
	}
}