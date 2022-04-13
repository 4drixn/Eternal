#include "Criticals.hpp"


void hypixelcrit(c_context* ctx)
{
	if (!ctx->local_player->get() || !ctx->world->get())
		return;

	if (ctx->local_player->get()->on_ground())
	{
		int n = 4;
		vec3 pos = ctx->local_player->get()->get_position();
		int n2 = 0;

		if (ctx->local_player->get()->get_sendQueue()->get_object() != nullptr)
		{
			jobject one = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.05101, pos.z, false);
			jobject two = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.01601, pos.z, false);
			jobject three = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.0301, pos.z, false);
			jobject four = packets::C04PacketPlayerPosition::init(pos.x, pos.y + 0.00101, pos.z, false);

			if (one && two && three && four)
			{
				ctx->local_player->get()->get_sendQueue()->addToSendQueue(one);
				ctx->local_player->get()->get_sendQueue()->addToSendQueue(two);
				ctx->local_player->get()->get_sendQueue()->addToSendQueue(three);
				ctx->local_player->get()->get_sendQueue()->addToSendQueue(four);

				ctx->env->DeleteLocalRef(one);
				ctx->env->DeleteLocalRef(two);
				ctx->env->DeleteLocalRef(three);
				ctx->env->DeleteLocalRef(four);
			}
		}
	}
}

bool canblock(std::unique_ptr<c_player>* local_player)
{
	auto item_stack = std::make_shared<c_itemstack>(local_player->get()->get_current_equiped_item());
	if (item_stack->get_object() == nullptr)
		return false;
	auto item = std::make_shared<c_item>(item_stack->get_item());

	return item->is_sword();
}


void Criticals::crits(c_context* ctx)
{
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

	if (!timer->has_passed(50))
		return;

	if (!Criticals::m_enabled)
		return;

	if (GetAsyncKeyState(VK_LBUTTON) ) // || clicker::is_clicking
	{
		std::shared_ptr<c_player> target = nullptr;

		if (!killaura::m_enabled)
		{
			if (ctx->minecraft->get()->get_pointed_entity() == nullptr)
				return;

			target = std::make_shared<c_player>(ctx->minecraft->get()->get_pointed_entity(), global::jenv);
		}

		if (target == nullptr)
			return;

		if (config::crits::selected_item == 0) // Hypixel
		{
			if (target->get_object() == nullptr)
				return;

			if (target->get_hurt_time() > 8 && ctx->local_player->get()->is_collidedvertically())
			{
				hypixelcrit(ctx);
			}

			target->~c_player();
		}

	}

	timer->reset();
}