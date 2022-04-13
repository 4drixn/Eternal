#include "MCF.hpp"
#include "timer.hpp"
#include "FriendList.hpp"
#include "main.hpp"
#include "c_notification.hpp"

void MCF::onUpdate(c_context* ctx)
{
    static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();
    if (!timer->has_passed(20))
        return;
    timer->reset();


    if (GetAsyncKeyState(VK_MBUTTON) & 1)
    {
        if (GetForegroundWindow() != FindWindowA("LWJGL", nullptr))
            return;
    	
        std::shared_ptr<c_player> target = std::make_shared<c_player>(ctx->minecraft->get()->get_pointed_entity(), ctx->env);
        if (target->get_object() != nullptr)
        {
            std::string player_name = target->get_name();

            auto itr = std::find(targetinfo::FriendList.begin(), targetinfo::FriendList.end(), player_name);
            if (itr != targetinfo::FriendList.end()) {

                c_notification noti = c_notification({ GetTickCount(), ImVec2 {200, 75}, "Removed \"" + player_name + "\" as friend." });
                notification::notifications.push_back(noti);
                targetinfo::FriendList.erase(itr);
            }
            else
            {

                c_notification noti = c_notification({ GetTickCount(), ImVec2 {200, 75}, "Added \"" + player_name + "\" as friend." });
                notification::notifications.push_back(noti);
                targetinfo::FriendList.push_back(player_name);

            }

            //target->~c_player();
        }
    }
}