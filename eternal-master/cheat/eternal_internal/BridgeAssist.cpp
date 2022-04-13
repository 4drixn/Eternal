#include "BridgeAssist.hpp"
#include "GameSettings.hpp"
#include "timer.hpp"

bool eagle = false;

void bridgeassist::bridge(c_context* ctx)
{
    //static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();
    //if (!timer->has_passed(5))
    //    return;

    if (!m_enabled)
        return;

    auto local_player = ctx->local_player->get();
    auto world = ctx->world->get();
    auto game_settings = std::make_unique<c_gamesettings>(
	    ctx->minecraft->get()->getGameSettings());
	
    if (game_settings->get_object() == nullptr) return;

    vec3 my_pos = local_player->get_position();
    my_pos.y = my_pos.y - 1;

    bool yes = true;
    if (bridgeassist::block_only)
    {
        jobject j_current_item = ctx->local_player->get()->get_current_equiped_item();
        auto current_item_stack = std::make_unique<c_itemstack>(j_current_item);

        if (current_item_stack->get_object() != nullptr)
        {
            if (auto current_item = std::make_unique<c_item>(current_item_stack->get_item()); current_item->get_object() != nullptr)
            {
                if (current_item->is_block() == false)
                {
                    ctx->env->DeleteLocalRef(j_current_item);
                    yes = false;
                }
            }
        }
        else
            yes = false;

        if(j_current_item != nullptr)
            ctx->env->DeleteLocalRef(j_current_item);

        if (current_item_stack == nullptr) {
            yes = false;
        }
    }

    // Unsneak in air

    static auto _timer_ = std::make_shared<c_timer>();
    if (bridgeassist::crouch_first)
    {
        if (yes) {
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
            {
                jobject block = world->get_blocknigger(my_pos);
                if (world->is_air_block(block) && local_player->on_ground())
                {
                    game_settings->keybindSneak()->set_pressed(true);
                    eagle = true;

                }
                else if (eagle && local_player->on_ground())
                {
                    if (!_timer_->has_passed(m_delay)) { return; }
                    game_settings->keybindSneak()->set_pressed(false);
                }
                ctx->env->DeleteLocalRef(block);
            }
        }
    	
    }
    else
    {
        if (yes) {
            jobject block = world->get_blocknigger(my_pos);
            if (world->is_air_block(block) && local_player->on_ground())
            {
                game_settings->keybindSneak()->set_pressed(true);
                eagle = true;

            }
            else if (eagle && local_player->on_ground())
            {
                if (!_timer_->has_passed(m_delay)) { return; }
                game_settings->keybindSneak()->set_pressed(false);
            }
            ctx->env->DeleteLocalRef(block);
        }
    }

    _timer_->reset();
	
}