#include "AntiBot.hpp"
#include "timer.hpp"
#include "main.hpp"
#include "c_notification.hpp"

void antibot::antibot(c_context* ctx)
{
	if (!m_enabled)
		return;
	static std::shared_ptr<c_timer> timer_uwu = std::make_shared<c_timer>();


	if (!timer_uwu->has_passed(2500))
		return;

	timer_uwu->reset();

	auto local_player = ctx->local_player->get();
	auto world = ctx->world->get();

	vec3 local_player_pos = local_player->get_position();

	for (const auto& player : world->get_players())
	{
		if (!player->is_invisible())
			continue;
		{
			vec3 player_pos = player->get_position();

			float diffX = abs(player_pos.x - local_player_pos.x);
			float diffY = abs(player_pos.y - local_player_pos.y);
			float diffZ = abs(player_pos.z - local_player_pos.z);
			float diffH = sqrt(diffX * diffX + diffZ * diffZ);

			if (diffY < 13 && diffY > 10 && diffH < 3)
			{
				world->remove_entity(player);

				c_notification noti = c_notification({ GetTickCount(), ImVec2 {200, 75}, "Removed a bot from ur game." });
				notification::notifications.push_back(noti);

				printf("[DEBUG] Removed a bot from ur game. (%s)\n", player->get_name().c_str());
			}

		}
		//if (player->is_invisible() && !utils::startsWith(formatted_name, "§c") && (formatted_name.find("§r") != std::string::npos) && nametag == player->get_name())
		//{
		//	vec3 local_player_pos = local_player->get_position();
		//	vec3 player_pos = player->get_position();
		//	float diffX = abs(player_pos.x - local_player_pos.x);
		//	float diffY = abs(player_pos.y - local_player_pos.y);
		//	float diffZ = abs(player_pos.z - local_player_pos.z);
		//	float diffH = sqrt(diffX * diffX + diffZ * diffZ);
		//	std::cout << "BOT: " << player->get_name() << std::endl;
		//	if (diffY < 13 && diffY > 10 && diffH < 3)
		//	{
		//		// add one check (if entity is not in tab menu)
		//		printf("%s Is a bot.\n", player->get_name().c_str());
	
		//	}
		//}
	}

}