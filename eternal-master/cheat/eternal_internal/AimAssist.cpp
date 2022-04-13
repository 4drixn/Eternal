#include "AimAssist.hpp"
#include "player_utils.hpp"
#include "GameSettings.hpp"
#include "clicker.hpp"
#include "main.hpp"
#include <mutex>
#include "FriendList.hpp"
#include "TeamsHelper.hpp"


__forceinline double angleDifference(int a, int b) {
	return ((a - b) % 360 + 540) % 360 - 180;
}

static std::once_flag once_ll;

void aimassist::aim(c_context* ctx)
{
	try {
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

	std::call_once(once_ll, [&]() {
		TIMER = GetTickCount64();
	});

	
	const auto ingame = [&]() {
		return (ctx->local_player->get()->get_object()); //&& ctx->world->get()->get_object());
	};

	if (!timer->has_passed(10))
		return;

	timer->reset();

	if (!aimassist::m_enabled )
		return;

	if (c_main::get().b_open)
		return;

	HWND window = FindWindowA("LWJGL", NULL);
	if (GetForegroundWindow() != window)
		return;

	auto timer_ = [](float val) {
		static double start_time = GetTickCount();
		double current_time = GetTickCount() - start_time;

		if (current_time >= val) {
			start_time = GetTickCount();
			return true;
		}
		return false;
	};

	const auto get_target = [&]() {

		double dist = (std::numeric_limits<double>::max)();
		double best = 360;
		float best_health = 999.f;

		//int dist = (std::numeric_limits<int>::max)();

		std::shared_ptr<c_player> target = nullptr;

		for (auto player : ctx->world->get()->get_players())
		{
			if (ctx->env->IsSameObject(ctx->local_player->get()->get_object(), player->get_object()))
				continue;

			if (!config::aimassist::selected[4])
			{
				if (player->is_invisible())
					continue;
			}

			if (!player->get_health() > 0.0f)
				continue;

			if (ctx->local_player->get()->get_distance_to(player) > (aimassist::m_dis))
				continue;

			if (config::aimassist::selected[6])
			{
				if (is_npc(&player))
					continue;
				//if (is_hypixel_npc(&player))
				//	continue;
			}

			if (config::aimassist::selected[3] == true)
			{
				//if (ctx->local_player->get()->is_on_same_team(player->get_object()))
				//	continue;
				if (teams::IsOnSameTeam(ctx->local_player, target))
					continue;

				//std::string _name_ = player->getDisplayName()->getUnformattedText();

				//if (_name_.find("§a") != std::string::npos)
				//	continue;
			}
			if (config::aimassist::selected[5] == true)
			{
				if (!ctx->local_player->get()->is_visible(player->get_object()))
					continue;
			}

			auto itr = std::find(targetinfo::FriendList.begin(), targetinfo::FriendList.end(), player->get_name());

			if (itr != targetinfo::FriendList.end())
				continue;

			auto angles = util::get_angles(ctx->local_player->get()->get_position(), player->get_position());
			auto difference = util::wrap_to_180(-(ctx->local_player->get()->get_yaw() - angles.first));
		
			if (config::aimassist::selected_item == 0)
			{
				if ((ctx->local_player->get()->get_distance_to(player) <= (aimassist::m_dis))) // Distance check
				{
					double diffX = player->get_position().x - ctx->local_player->get()->get_position().x;
					double diffZ = player->get_position().z - ctx->local_player->get()->get_position().z;
					float newYaw = (float)(atan2(diffZ, diffX) * 180.0 / M_PI - 90);//3.141592653589793

					double difference = abs(angleDifference(newYaw, ctx->local_player->get()->get_yaw()));

					if (difference < best)
					{
						target = player;
						best = difference;
					}
				}
			}
			else if (config::aimassist::selected_item == 1)
			{
				if (ctx->local_player->get()->get_distance_to(player) <= dist) // closest dist
				{
					if ((ctx->local_player->get()->get_distance_to(player) <= (aimassist::m_dis))) // Distance check
					{
						if (difference < best)
						{
							target = player;
							dist = ctx->local_player->get()->get_distance_to(player);
						}
					}
				}
			}
			else if (config::aimassist::selected_item == 2)
			{
				if ((ctx->local_player->get()->get_distance_to(player) <= (aimassist::m_dis)))
				{
					if (player->get_health() < best_health)
					{
						best_health = player->get_health();
						target = player;
					}
				}
			}
		}
		return target;
	};

	if (config::aimassist::selected[2] == true)
	{
		if (ctx->local_player->get()->is_breaking_block()) return;
	}

	if ((GetTickCount() - TIMER) >= 20) {
		target = get_target();
	}

	if ((GetTickCount() - TIMER) >= 20)
		TIMER = GetTickCount64();



	if (target)
		{
			HWND window = FindWindowA("LWJGL", nullptr);
			if (GetForegroundWindow() != window)
				return;

			if (ctx->minecraft->get()->getCurrentScreen() != nullptr)
				return;


			if (GetAsyncKeyState(VK_LBUTTON))
			{


				vec3 my_pos = ctx->local_player->get()->get_position();
				vec3 cp_pos = target->get_position(), crnr_1, crnr_2, crnr_3, crnr_4;

				//crnr_1 = { cp_pos.x - 0.30f, cp_pos.y, cp_pos.z + 0.30f };
				//crnr_2 = { cp_pos.x - 0.30f, cp_pos.y, cp_pos.z - 0.30f };
				//crnr_3 = { cp_pos.x + 0.30f, cp_pos.y, cp_pos.z - 0.30f };
				//crnr_4 = { cp_pos.x + 0.30f, cp_pos.y, cp_pos.z + 0.30f };

				crnr_1 = { cp_pos.x - 0.25f, cp_pos.y, cp_pos.z + 0.25f };
				crnr_2 = { cp_pos.x - 0.25f, cp_pos.y, cp_pos.z - 0.25f };
				crnr_3 = { cp_pos.x + 0.25f, cp_pos.y, cp_pos.z - 0.25f };
				crnr_4 = { cp_pos.x + 0.25f, cp_pos.y, cp_pos.z + 0.25f };

				double distance = sqrt(pow(cp_pos.x - my_pos.x, 2) + pow(cp_pos.y - my_pos.y, 2) + pow(cp_pos.z - my_pos.z, 2));

				double distance_c_1 = sqrt(pow(crnr_1.x - my_pos.x, 2) + pow(crnr_1.y - my_pos.y, 2) + pow(crnr_1.z - my_pos.z, 2));
				double distance_c_2 = sqrt(pow(crnr_2.x - my_pos.x, 2) + pow(crnr_2.y - my_pos.y, 2) + pow(crnr_2.z - my_pos.z, 2));
				double distance_c_3 = sqrt(pow(crnr_3.x - my_pos.x, 2) + pow(crnr_3.y - my_pos.y, 2) + pow(crnr_3.z - my_pos.z, 2));
				double distance_c_4 = sqrt(pow(crnr_4.x - my_pos.x, 2) + pow(crnr_4.y - my_pos.y, 2) + pow(crnr_4.z - my_pos.z, 2));

				std::vector<double> closest_crn;
				closest_crn.push_back(distance_c_1);
				closest_crn.push_back(distance_c_2);
				closest_crn.push_back(distance_c_3);
				closest_crn.push_back(distance_c_4), utils::_vec_sort(closest_crn);

				vec3 closest_crnr_1, closest_crnr_2{};

				for (size_t x = 0; x < 2; ++x) {

					if (closest_crn[x] == distance_c_1) if (x == 0) closest_crnr_1 = crnr_1; else if (x == 1) closest_crnr_2 = crnr_1;
					if (closest_crn[x] == distance_c_2) if (x == 0) closest_crnr_1 = crnr_2; else if (x == 1) closest_crnr_2 = crnr_2;
					if (closest_crn[x] == distance_c_3) if (x == 0) closest_crnr_1 = crnr_3; else if (x == 1) closest_crnr_2 = crnr_3;
					if (closest_crn[x] == distance_c_4) if (x == 0) closest_crnr_1 = crnr_4; else if (x == 1) closest_crnr_2 = crnr_4;

				}

				vec3 closest_point = closest_crnr_1;

				if (closest_crnr_1.x == closest_crnr_2.x) {
					closest_point.z = my_pos.z;

					if (!utils::_in_range(closest_point.z, closest_crnr_1.z, closest_crnr_2.z)) closest_point = closest_crnr_1;

				}

				else
					if (closest_crnr_2.z == closest_crnr_2.z) {
						closest_point.x = my_pos.x;
						if (!utils::_in_range(closest_point.x, closest_crnr_1.x, closest_crnr_2.x)) closest_point = closest_crnr_1;
					}
				

				float diffY = (target->get_bounding_box()->get_native_boundingbox().minY + target->get_bounding_box()->get_native_boundingbox().maxY) / 2.0 - (target->get_position().y + target->get_eye_height());
				float dist = ctx->local_player->get()->get_distance_to(target);
				//float pitch = util::wrap_to_180((-(atan2(diffY, dist) * 180.0 / 3.141592653589793)));


				auto angles = util::get_angles(ctx->local_player->get()->get_position(), closest_point);

				auto angles_2 = util::get_angles(ctx->local_player->get()->get_position(), target->get_position());

				auto difference_2 = util::wrap_to_180(-(ctx->local_player->get()->get_yaw() - angles_2.first));

				auto difference = util::wrap_to_180(-(ctx->local_player->get()->get_yaw() - angles.first));
				auto difference2 = util::wrap_to_180(-(ctx->local_player->get()->get_pitch() - angles.second));

				n_glrender::AimPoint = closest_point;
				
				if (utils::is_between(difference_2, -(aimassist::m_fov), aimassist::m_fov))
				{
					if (fabs(difference_2) < 5) {
						difference = utils::_rand_val(0.1f, difference);
					}

					if (fabs(difference) < 2.3) return;

					auto settings = std::make_unique<c_gamesettings>(ctx->minecraft->get()->getGameSettings());
					float mouse_sens = settings->get_sens();

					float current_yaw = ctx->local_player->get()->get_yaw();

					if (ctx->local_player->get()->get_movestrafe() != 0 && config::aimassist::selected[1])
					{
						difference *= utils::_rand_val(0.5, 0.8) * (mouse_sens * (aimassist::m_speed / 2) + 0.5);
					}

					current_yaw += mouse_sens * (difference / (10 / aimassist::m_speed)) / utils::_rand_val(2, 3.2); // / utils::_rand_val(0.5, 1.2);


					if (config::aimassist::selected[2] == true)
					{
						if (ctx->local_player->get()->is_breaking_block()) {
							return;
						}
					}

					ctx->local_player->get()->set_yaw(current_yaw);

					if (config::aimassist::selected[0] == true)
					{
						if (utils::_rand_val_int(1, 4) == 2)
						{
							//if (fabs(difference2) < 2.0f) difference2 = utils::_rand_val(1.f, 2.f) * (fabs(difference2) / difference2);

							auto current_pitch = ctx->local_player->get()->get_pitch();
							current_pitch += mouse_sens * (difference2 / (10 / aimassist::m_speed) / utils::_rand_val(2, 3.2));// // (difference2 / aimassist::m_speed);

							if (config::aimassist::selected[2] == true)
							{
								if (ctx->local_player->get()->is_breaking_block())
								{
									return;
								}
							}
							if (ctx->local_player->get()->on_ground())
							{
								ctx->local_player->get()->set_pitch(current_pitch);
							}
						}
					}
				}
			}
		}
	}
	catch (std::exception err) { }
}