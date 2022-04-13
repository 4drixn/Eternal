#include "clicker.hpp"
#include "timer.hpp"
#include "Item.hpp"
#include "ItemStack.hpp"
#include "main.hpp"

#include "global.hpp"
bool first_click = true; 
int click_counter = 0;

bool should_click(std::unique_ptr<c_player>* local_player_, std::unique_ptr<c_minecraft>* minecraft_, std::unique_ptr<c_guiscreen>* current_gui_)
{
	auto local_player = local_player_->get();
	auto minecraft = minecraft_->get();
	auto current_gui = current_gui_->get();

	auto current_item_stack = std::make_unique<c_itemstack>(local_player->get_current_equiped_item());

	if (!clicker::m_enabled)
		return false;

	if (c_main::get().b_open)
		return false;

	if (config::clicker::selected[0])
	{
		if (local_player->is_breaking_block()) return false;
	}

	if (config::clicker::selected[2] || config::clicker::selected[3])
	{
		if (current_item_stack->get_object() == nullptr)
		{
			return false;
		}
	}
	if (current_item_stack->get_object() != nullptr)
	{
		auto current_item = std::make_shared<c_item>(current_item_stack->get_item());

		if (config::clicker::selected[2] || config::clicker::selected[3]) {
			if (current_item->get_object() != nullptr)
			{
				if (config::clicker::selected[2] && config::clicker::selected[3])
				{
					if (current_item->is_axe() == false && current_item->is_sword() == false)
						return false;
					if (current_item->is_block() == true && current_item->is_projectile() == true)
						return false;
				}
				else {
					if (config::clicker::selected[2])
					{
						if (current_item->is_sword() == false)
						{
							return false;
						}
					}
					if (config::clicker::selected[3])
					{
						if (current_item->is_axe() == false)
						{
							return false;
						}
					}
				}

			}
		}
	}

	if (config::clicker::selected[1])
	{
		if (current_gui->inventory_open())
			return true;
		if (current_gui->get_object() != nullptr)
			return false;
	}
	else
	{
		if (current_gui->get_object() != nullptr)
		{
			return false;
			//if (current_gui->inventory_open())
			//{
			//	return true;
			//}
		}
	}

	return true;
}

void clicker::clicker(c_context* ctx)
{
	auto timer_ = [](float val) {
		static double start_time = GetTickCount();
		double current_time = GetTickCount() - start_time;

		if (current_time >= val) {
			start_time = GetTickCount();
			return true;
		}
		return false;
	};
	if (!clicker::m_enabled)
		return;

	if (c_main::get().b_open)
		return;

	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

	auto minecraft = ctx->minecraft->get();
	auto current_gui = std::make_unique<c_guiscreen>(minecraft->getCurrentScreen());
	
	int min_val = clicker::cps - utils::_rand_val_int(1, 3)- (click_counter / 50);
	int max_val = clicker::cps + utils::_rand_val_int(1, 3) + (click_counter / 50 );
	int big_val = utils::_rand_val_int(1200, 1280 + click_counter);
	float delay = (big_val / utils::_rand_val_int(min_val, max_val));

	if (!timer->has_passed(delay))
		return;

	static std::shared_ptr<c_timer> timer2 = std::make_shared<c_timer>();
	static std::shared_ptr<c_timer> timer3 = std::make_shared<c_timer>();
	static std::shared_ptr<c_timer> timer__ = std::make_shared<c_timer>();
	static std::shared_ptr<c_timer> timer___ = std::make_shared<c_timer>();

	static std::shared_ptr<c_timer> timer___1 = std::make_shared<c_timer>();
	static std::shared_ptr<c_timer> timer___2 = std::make_shared<c_timer>();
	static std::shared_ptr<c_timer> timer___3 = std::make_shared<c_timer>();

	//if (minecraft->getPlayer() && minecraft->getWorld())
	{
		auto local_player = ctx->local_player->get(); // std::make_unique<c_player>(minecraft->getPlayer(), global::jenv);

		const auto is_sane = [&]() {
			return (local_player->get_object());
		};

		if (is_sane() && should_click(ctx->local_player, ctx->minecraft, &current_gui))
		{
			if (!config::clicker::selected[1])
			{
				if (current_gui->get_object() != NULL)
					return;
			}

			HWND window = FindWindowA("LWJGL", NULL);
			if (GetForegroundWindow() == window)
			{
				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (!should_click(ctx->local_player, ctx->minecraft, &current_gui))
						return;

					clicker::is_clicking = true;
					if (click_counter >= 64)
					{
						click_counter = 0;
						if (!timer___->has_passed(12))
							return;
						timer___->reset();
					}

					if ((click_counter & 1) == 0)
					{
						int sleep_time = utils::_rand_val(10, 20);
						if (!timer__->has_passed(sleep_time))
							return;
						timer__->reset();
					}

					//clicker part
					if (first_click)
					{
						first_click = false;
						POINT pt;
						GetCursorPos(&pt);
						if (!timer___3->has_passed(50))
							return;
						timer___3->reset();
						
						if (should_click(ctx->local_player, ctx->minecraft, &current_gui))
						{
							PostMessage(window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
						}
					}
					else
					{
						if (!should_click(ctx->local_player, ctx->minecraft, &current_gui))
							return;

						POINT pt;
						GetCursorPos(&pt);

						if (click_counter >= 64)
						{
							if (!timer2->has_passed(90))
								return;
							click_counter = 0;
							timer2->reset();
						}
						
						if (click_counter >= 30)
						{
							if (!timer2->has_passed(25))
								return;
							timer2->reset();
						}
						
						int sleep_time = utils::_rand_val(40, 50);
						
						if (!timer___1->has_passed(sleep_time))
							return;
						timer___1->reset();

						if (should_click(ctx->local_player, ctx->minecraft, &current_gui))
						{
							PostMessage(window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
						}
						
						if (!timer___2->has_passed(100))
							return;
						timer___2->reset();
						if (should_click(ctx->local_player, ctx->minecraft, &current_gui))
						{
							PostMessage(window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
						}
		
						click_counter++;

					}
				}
				else if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0x8000)
				{

				}
				else
				{
					first_click = true;
					clicker::is_clicking = false;
				}

			}
		}


	}
	timer->reset();
}
