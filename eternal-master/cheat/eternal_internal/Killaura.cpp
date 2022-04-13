#include "Killaura.hpp"
#include "Criticals.hpp"
#include <mutex>
#include "FriendList.hpp"

__forceinline double angle_difference(int a, int b) {
	return ((a - b) % 360 + 540) % 360 - 180;
}


void hypixel_crit(c_context* ctx)
{
	if (ctx->local_player->get()->on_ground())
	{
		vec3 pos = ctx->local_player->get()->get_position();

		auto packet = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.051, pos.z, false);
		auto packet2 = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.011511, pos.z, false);
		auto packet3 = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.001, pos.z, false);
		auto packet4 = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.001, pos.z, false);

		if(auto sendqueue = ctx->local_player->get()->get_sendQueue(); sendqueue->get_object() != nullptr)
		{
			sendqueue->addToSendQueue(packet);
			sendqueue->addToSendQueue(packet2);
			sendqueue->addToSendQueue(packet3);
			sendqueue->addToSendQueue(packet4);
		}

		//ctx->env->DeleteLocalRef(packet);
		//ctx->env->DeleteLocalRef(packet2);
		//ctx->env->DeleteLocalRef(packet3);
		//ctx->env->DeleteLocalRef(packet4);
	}
}

bool can_block(std::unique_ptr<c_player>* local_player)
{
	auto item_stack = std::make_shared<c_itemstack>(local_player->get()->get_current_equiped_item());
	if (item_stack->get_object() == NULL)
		return false;

	auto item = std::make_shared<c_item>(item_stack->get_item());
	if (item->get_object() == NULL)
		return false;

	return item->is_sword();
}

void startAutoBlock(c_context* ctx)
{
	if (*ctx->local_player == NULL || *ctx->world == NULL)
		return;

	auto local_player = ctx->local_player->get();

	if (local_player->get_current_equiped_item() == NULL)
		return;

	if (local_player->sendUseItem(local_player->get_object(), ctx->world->get()->get_object(), local_player->get_current_equiped_item()))
	{
		//auto render_item = std::make_shared<c_itemrenderer>(ctx->minecraft->get()->get_render_item());
		//if (render_item->get_object() != NULL)
		//{
		//	render_item->set_equippedProgress(0.f);
		//	std::cout << "BLOCK\n";
		//}
	}
}

DWORD crit_timer;
static std::once_flag once_ll;

void killaura::aura(c_context* ctx)
{
	std::call_once(once_ll, [&]() {
		crit_timer = GetTickCount64();
	});
	
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();
	static std::shared_ptr<c_timer> timer2 = std::make_shared<c_timer>();

	const auto ingame = [&]() {
		return (ctx->local_player->get()->get_object() && ctx->world->get()->get_object());
	};

	auto timer_ = [](float val) {
		static double start_time = GetTickCount();
		double current_time = GetTickCount() - start_time;

		if (current_time >= val) {
			start_time = GetTickCount();
			return true;
		}
		return false;
	};

	auto sendqueue = ctx->local_player->get()->get_sendQueue();
	
	std::vector<std::string> updated;
	const auto get_targets = [&]() {

		std::vector<std::shared_ptr<c_player>> list;

		for (const auto& player : ctx->world->get()->get_players())
		{
			if (player == nullptr)
				continue;
			if (player->get_object() == nullptr)
				continue;
			
			if (list.size() > 15) //20
				break;

			if (ctx->env->IsSameObject(ctx->local_player->get()->get_object(), player->get_object()))
				continue;

			if (ctx->local_player->get()->get_distance_to(player) > killaura::m_dist)
				continue;

			auto name = player->get_name();
			auto itr = std::find(targetinfo::FriendList.begin(), targetinfo::FriendList.end(), name);
			if (itr != targetinfo::FriendList.end())
				continue;

			list.push_back(player);
			//updated.push_back(name);

		}
		//killaura::targetlistnames = updated;
		return list;
	};

	const auto get_target = [&]() {

		double dist = (std::numeric_limits<double>::max)();
		double best = 360;
		float best_health = 999.f;

		//int dist = (std::numeric_limits<int>::max)();

		std::shared_ptr<c_player> target = nullptr;

		for (const auto& player : ctx->world->get()->get_players())
		{
			if (player == nullptr)
				continue;
			if (player->get_object() == nullptr)
				continue;
			
			if (ctx->env->IsSameObject(ctx->local_player->get()->get_object(), player->get_object()))
				continue;

			if (player->get_health() <= 0.0f)
				continue;

			// team check

			if (ctx->local_player->get()->get_distance_to(player) > 50)
				continue;

			//auto itr = std::find(targetinfo::FriendList.begin(), targetinfo::FriendList.end(), player->get_name());
			//if (itr != targetinfo::FriendList.end())
			//	continue;

			auto angles = util::get_angles(ctx->local_player->get()->get_position(), player->get_position());
			auto difference = util::wrap_to_180(-(ctx->local_player->get()->get_yaw() - angles.first));

			if (config::killaura::selected_item == 0)
			{
				if ((ctx->local_player->get()->get_distance_to(player) <= (killaura::m_dist))) // Distance check
				{
					double diffX = player->get_position().x - ctx->local_player->get()->get_position().x;
					double diffZ = player->get_position().z - ctx->local_player->get()->get_position().z;
					float newYaw = (float)(atan2(diffZ, diffX) * 180.0 / 3.141592653589793 - 90);

					double difference = abs(angle_difference(newYaw, ctx->local_player->get()->get_yaw()));

					if (difference < best)
					{
						target = player;
						best = difference;
					}
				}
			}
			else if (config::killaura::selected_item == 1)
			{
				if (ctx->local_player->get()->get_distance_to(player) <= dist) // closest dist
				{
					if ((ctx->local_player->get()->get_distance_to(player) <= (killaura::m_dist))) // Distance check
					{
						if (difference < best)
						{
							target = player;
							dist = ctx->local_player->get()->get_distance_to(player);
						}
					}
				}
			}
			else if (config::killaura::selected_item == 2)
			{
				if ((ctx->local_player->get()->get_distance_to(player) <= (killaura::m_dist)))
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


	if (!killaura::m_enabled)
		return;

	float delay = (1000 / killaura::m_cps);
	
	if (!timer->has_passed(delay))
		return;
	
	timer->reset();
	
	if (config::killaura::selected_mode == 0)
	{
		killaura::target = get_target();

		if (!killaura::target)
			return;
	}
	else if (config::killaura::selected_mode == 1){
		killaura::targetlist = get_targets();
	}


	if (killaura::m_enabled)
	{
		if (config::killaura::selected_mode == 0)
		{
			
			auto local_player = ctx->local_player->get();
		
			if (killaura::target->get_object() != nullptr)
			{
				if (fabs(ctx->local_player->get()->get_distance_to(killaura::target) <= (killaura::m_dist)))
				{
					auto angles = util::get_angles(ctx->local_player->get()->get_position(), killaura::target->get_position());
					auto difference = util::wrap_to_180(-(ctx->local_player->get()->get_yaw() - angles.first));
					auto difference2 = util::wrap_to_180(-(ctx->local_player->get()->get_pitch() - angles.second));
		
					if ((difference) <= killaura::m_fov) {

						if (!killaura::m_silent)
						{
							if ((difference) > 5)
							{
								auto current_yaw = ctx->local_player->get()->get_yaw();
								current_yaw += difference;
								ctx->local_player->get()->set_yaw(current_yaw);
								//ctx->local_player->get()->set_pre_yaw(current_yaw);
							}
						}

						if ((GetTickCount64() - crit_timer) >= 50)
						{
							if (target->get_hurt_time() > 8 && ctx->local_player->get()->is_collidedvertically())
							{
								hypixel_crit(ctx);
							}
							crit_timer = GetTickCount64();

						}
						//ctx->local_player->get()->swing_item();

						if (ctx->local_player == nullptr || ctx->world == nullptr)
							return;

						if (ctx->local_player->get()->get_object() == nullptr || ctx->local_player->get()->get_object() == nullptr)
							return;

						ctx->local_player->get()->attack_entity(killaura::target->get_object());

						if (config::killaura::blockhit_selected == 1)
						{
							if (can_block(ctx->local_player))
							{
								startAutoBlock(ctx);
							}

						}

					}
				}
			}
		}
		else if (config::killaura::selected_mode == 1) // multi
		{
			if (ctx->local_player == nullptr || ctx->world == nullptr)
				return;

			if (ctx->local_player->get()->get_object() == nullptr || ctx->local_player->get()->get_object() == nullptr)
				return;

			
			static bool has_blocked = false;
			for (const auto& entity : targetlist)
			{
				if (entity->get_object() == nullptr)
					continue;

				float dis = ctx->local_player->get()->get_distance_to(entity);
				if (dis > killaura::m_dist)
					continue;

				if (Criticals::m_enabled)
					hypixel_crit(ctx);

				ctx->local_player->get()->swing_item();

				//auto pos = ctx->local_player->get()->get_position();
				//if(dis < 4)
				//{
				//	ctx->local_player->get()->attack_entity(entity->get_object());
				//}
				//else {

				//	if (!ctx->local_player->get()->is_visible(entity->get_object()))
				//		continue;
				//	auto pos = entity->get_position();
				//	auto onground = ctx->local_player->get()->on_ground();
				//	auto funnypacket = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.001f, pos.z, true);

				//	if (sendqueue->get_object() != nullptr)
				//	{
				//		if(funnypacket != nullptr)
				//			sendqueue->addToSendQueue(funnypacket);
				//	}
				//}
				
			    ctx->local_player->get()->attack_entity(entity->get_object());

				if (config::killaura::blockhit_selected == 1 && !has_blocked)
				{
					if (can_block(ctx->local_player))
					{
						startAutoBlock(ctx);
						has_blocked = true;
					}
				}

			}
		}
	}
}
