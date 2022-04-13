#include "ChestStealer.hpp"
#include "timer.hpp"
#include "IInventory.hpp"

void cheststealer::stealer(c_context* ctx)
{
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

	if (!cheststealer::m_enabled) return;

	if (!timer->has_passed(40))
		return;

	auto local_player = ctx->local_player->get();


	auto timer_ = []() {
		static double start_time = GetTickCount();
		double current_time = GetTickCount() - start_time;

		if (current_time >= m_delay) {
			start_time = GetTickCount();
			return true;
		}
		return false;
	};

	static std::shared_ptr<c_timer> timer__ = std::make_shared<c_timer>();
	
	if (const auto& openContainer = local_player->get_openContainer(); openContainer->get_object() != nullptr && openContainer->is_chest())
	{
		for (int i = 0; i < openContainer->getLowerChestInventory()->getSizeInventory(); i++)
		{
			if (openContainer->getLowerChestInventory()->getStackInSlot(i)->get_object() == nullptr)
			{
				continue;
			}
			if(!timer__->has_passed(m_delay)) { continue; }
				
			local_player->window_click(openContainer->get_windowId(), i, 0, 1);
			timer__->reset();
		}
	}
	timer->reset();
}

//static std::shared_ptr<c_timer> timer__ = std::make_shared<c_timer>();
//
//if (const auto& openContainer = local_player->get_openContainer(); openContainer->get_object() != nullptr)
//{
//	if (openContainer->is_chest())
//	{
//		for (int i = 0; i < openContainer->getLowerChestInventory()->getSizeInventory(); i++)
//		{
//			if (openContainer->getLowerChestInventory()->getStackInSlot(i)->get_object() == nullptr)
//			{
//				continue;
//			}
//			if (!timer__->has_passed(m_delay)) { continue; }
//
//			local_player->window_click(openContainer->get_windowId(), i, 0, 1);
//			timer__->reset();
//		}
//	}
//
//}
//timer->reset();