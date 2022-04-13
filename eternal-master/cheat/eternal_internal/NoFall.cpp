#include "NoFall.hpp"
#include "timer.hpp"
#include "player_utils.hpp"


bool nofall_set = false;
float lastFall = 0.f;

bool isBlockUnder(c_context* ctx) {
    vec3 my_pos = ctx->local_player->get()->get_position();

    for (int i = (int)(my_pos.y - 1); i > 0; --i) {
        
        jobject b = ctx->world->get()->get_blocknigger(my_pos);
        if (ctx->world->get()->is_air_block(b) == false)
        {
            ctx->env->DeleteLocalRef(b);
            return true;
        }
        ctx->env->DeleteLocalRef(b);
    }
    return false;
}

int delay = 0;
void nofall::nofall(c_context* ctx)
{

    static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

    if (!timer->has_passed(75 + delay))
        return;

    timer->reset();

    if (!nofall::m_enabled)
        return;

    delay = 0;

    auto local_player = ctx->local_player->get();

    if (local_player->get_falldistance() >= 2.5f && blockutils::isBlockUnder(ctx->local_player, ctx->world))
    {
        jobject b = packets::C03PacketPlayer::init(true);
        local_player->get_sendQueue()->addToSendQueue(b);
        local_player->set_falldistance(0);
        ctx->env->DeleteLocalRef(b);
        delay = 100;
    }


    //if (vel >= 3.f && !nofall_set && !local_player->on_ground() && isBlockUnder(ctx))
    //{
    //    lastFall = local_player->get_falldistance();
    //    local_player->get_sendQueue()->addToSendQueue(packets::C03PacketPlayer::init(true));
    //    std::cout << "[DEBUG] Nofall packet sent!\n";
    //    nofall_set = true;
    //}
    //else if (local_player->is_collidedvertically())
    //{
    //    lastFall = 0.0f;
    //}
    //if (nofall_set && local_player->get_falldistance() == 0)
    //    nofall_set = false;

}


