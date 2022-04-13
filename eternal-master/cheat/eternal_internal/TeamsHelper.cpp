#include "TeamsHelper.hpp"

bool teams::IsOnSameTeam(std::unique_ptr<c_player>* local, std::shared_ptr<c_player> other_player)
{
	int mode = config::global::team_mode;
	auto local_player = local->get();

	if (local_player == nullptr || other_player == nullptr)
		return false;

	if (local_player->get_object() == NULL || other_player->get_object() == NULL)
		return false;


	if (mode == 0)
	{
		return local_player->is_on_same_team(other_player->get_object());
	}
	else if (mode == 2)
	{
		return local_player->is_hypixel_teammate(&other_player);
	}
	else if (mode == 3)
	{
		std::string player_name = other_player->getDisplayName()->getUnformattedText();

		if (player_name.find("§a") != std::string::npos)
			return true;

		if (player_name.find("§2") != std::string::npos)
			return true;

		if (player_name.find("§4") != std::string::npos)
			return false;
	}
}