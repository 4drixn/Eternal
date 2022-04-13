#include "Speed.hpp"
#include "Timer.h"
#include "timer.hpp"
#include "vec3.hpp"
#include "Potion.hpp"

int jumps = 0;

float calc_speed(c_context* ctx)
{
	auto local_player = ctx->local_player->get();

	if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), local_player->get_bounding_box()->offset(0, -0.02, 0)->get_object()) == 0)
	{
		local_player->SetmotionX(0.f);
		local_player->SetmotionZ(0.f);
		return 0.67f;
	}

	else if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), local_player->get_bounding_box()->offset(0, -0.03, 0)->get_object()) == 0)
	{
		float lol = local_player->get_motion_vector().y - 0.11f;
		local_player->SetmotionY(lol);
		return 0.78f;
	}

	else if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), local_player->get_bounding_box()->offset(0, -0.01, 0)->get_object()) == 0)
	{
		return 0.98f;
	}
	else if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), local_player->get_bounding_box()->offset(0, -0.00, 0)->get_object()) == 0)
	{
		return 1.12f;
	}

	return 0.33f;
}


// 0 = Hypixel, 1 = VerusHop, 2 = YPort, 3 = Cubecraft
float ypos = 0.f;
bool prevOnGround;
float Speed = 0;

void speed::speed_(c_context* ctx)
{

	float speed = 0.50f;
	int stage = 0;

	if (config::speed::selected_mode == 2)
	{
		static std::shared_ptr<c_timer> timer_uwu = std::make_shared<c_timer>();
		if (timer_uwu->has_passed(320))
		{
			timer_uwu->reset();
		}
		else {
			return;
		}
	}




	//if (!timer_uwu->has_passed(10))
	//if (!timer_uwu->has_passed(5))
	//	return;
	//if (!timer_uwu->has_passed(320))
	//	return;
	//timer_uwu->reset();


	auto local_player = ctx->local_player->get();
	auto timer_t = std::make_unique<timer>(ctx->minecraft->get()->getTimer());

	if (!speed::m_enabled)
	{
		//timer_t->set_timer_speed(1);
		return;
	}

	
	//timer_uwu->reset();

	if (config::speed::selected_mode == 0) // Hypixel
	{
		if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
		{
			//static auto potion = std::make_shared<c_potion>();
			static jobject pot = get_speed_potion();

			float moveSpeed = local_player->get_move_speed(pot) * 2.1492287832;//+ 0.0000000023841858f;

			if (local_player->get_ticksExisted() % 2 == 0)
			{
				Speed += 1.73E-11;
			}

			vec3 mot = local_player->get_motion_vector();

			float speed_ = sqrt(mot.x * mot.x + mot.z * mot.z);

			auto bb = local_player->get_bounding_box();

			timer_t->set_timer_speed(1.f);

			if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), bb->offset(0, -0.02, 0)->get_object()) > 0)
			{
				Speed = local_player->get_move_speed(pot) * 1.6892287832; // FUN = * 5.f;
				timer_t->set_timer_speed(1.25f);
				local_player->jump();
				//local_player->SetmotionY(0.399175222222555f);
				//local_player->set_movestrafe(local_player->get_movestrafe() * 2.);

				prevOnGround = true;
			}

			// vclip = offsetAndUpdate
			else {
				if (prevOnGround)
				{
					Speed -= (float)0.66333332222 * (float)((float)Speed - (float)local_player->get_move_speed(pot));
					prevOnGround = false;
				}
				else {
					Speed -= Speed / 160 - 1.0E-9;
				}
			}

			local_player->set_jump_mov_fac(0.0265f);


			float the_yaw = local_player->getDirection();

			float x_ = -sin(the_yaw) * Speed;
			float z_ = cos(the_yaw) * Speed;
			local_player->SetmotionX(x_);
			local_player->SetmotionZ(z_);

		}
	}
	else if (config::speed::selected_mode == 1) // verus
	{
		if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
		{
			//static auto potion = std::make_shared<c_potion>();
			static jobject pot = get_speed_potion();

			if (local_player->get_ticksExisted() % 2 == 0)
			{
				Speed += 1.73E-11;
			}

			auto bb = local_player->get_bounding_box();

			if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), bb->offset(0, -0.03, 0)->get_object()) != 0)
			{
				Speed = local_player->get_move_speed(pot) * 1.589228764;
				local_player->jump();

				prevOnGround = true;
			}

			else {
				if (prevOnGround)
				{
					Speed -= (float)0.66333332222 * (float)((float)Speed - (float)local_player->get_move_speed(pot));
					prevOnGround = false;
				}
				else {
					Speed -= Speed / 160 - 1.0E-9;
				}
			}

			local_player->set_jump_mov_fac(0.0265f);

			vec3 mot = local_player->get_motion_vector();
			float the_yaw = local_player->getDirection();

			float x_ = -sin(the_yaw) * Speed;
			float z_ = cos(the_yaw) * Speed;
			local_player->SetmotionX(x_);
			local_player->SetmotionZ(z_);

		}

	}
	else if (config::speed::selected_mode == 3)
	{
		static std::shared_ptr<c_timer> y_timer = std::make_shared<c_timer>();
		static std::shared_ptr<c_timer> y_timer2 = std::make_shared<c_timer>();

		if (!y_timer2->has_passed(10))
			return;
		y_timer2->reset();

		//if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
		//{
		//	static auto potion = std::make_shared<c_potion>();
		//	jobject pot = potion->get_speed_potion();

		//	float moveSpeed = local_player->get_move_speed(pot);

		//	auto bb = local_player->get_bounding_box();
		//	if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), bb->offset(0, -0.01, 0)->get_object()) == 0)
		//	{
		//		vec3 motion_ = local_player->get_motion_vector();
		//		float speed = sqrt(motion_.x * motion_.x + motion_.z * motion_.z);

		//		float the_yaw = local_player->getDirection();

		//		float x_ = -sin(the_yaw) * speed;
		//		float z_ = cos(the_yaw) * speed;
		//		local_player->SetmotionX(x_);
		//		local_player->SetmotionZ(z_);
		//	}
		//
		//}

		if (!y_timer->has_passed(90))
			return;

		y_timer->reset();

		if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
		{
		//	static auto potion = std::make_shared<c_potion>();
			//static jobject pot = get_speed_potion();
			float moveSpeed = 0.2873f;
			//float moveSpeed = local_player->get_move_speed(pot);
			auto bb = local_player->get_bounding_box();

			if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), bb->offset(0, -0.01, 0)->get_object()) > 0)
			{
			//	if (local_player->on_ground())
				{
					ypos = local_player->get_position().y;

					float the_val = local_player->getJumpMotion();
					local_player->SetmotionY(the_val);

					float the_yaw = local_player->getDirection();

					float x_ = -sin(the_yaw) * moveSpeed * 1.3737 * 1.022521442084547;
					float z_ = cos(the_yaw) * moveSpeed * 1.3737 * 1.022521442084547;  // 1.4337 karhu
					local_player->SetmotionX(x_);
					local_player->SetmotionZ(z_);

				}

			}
			else {
				vec3 pos = local_player->get_position();
				vec3 mot = local_player->get_motion_vector();
				local_player->set_pos(vec3({ pos.x, ypos, pos.z }), false);
				local_player->SetmotionY(-utils::_rand_val(0.40, 0.41));
			}

		}

	}
	else if (config::speed::selected_mode == 2)
	{
		float mov_speed = 0.f;
		auto bb = local_player->get_bounding_box();
		//static auto potion = std::make_shared<c_potion>();
		jobject pot = get_speed_potion();

		if (local_player->get_moveforward() == .0f || local_player->is_collidedhorizontally())
		{
			jumps = 0;
		}

		if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
		{
			if (local_player->is_collidedvertically() && (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), bb->offset(0, -0.01, 0)->get_object()) > 0))
			{
				local_player->SetmotionY(utils::_rand_val(0.40, 0.41));
				double more = jumps <= 0 ? 0 : (double)jumps / 10;
				mov_speed = 0.40f + local_player->getSpeedEffect(pot) * 0.1;
				stage = 0;

				if (jumps < 2)
					jumps++;
			}
			else {
				stage++;
				double more = jumps <= 0 ? 0 : (double)jumps / 10;
				speed = 0.30 - (double)stage / 200 + local_player->getSpeedEffect(pot) * 0.1;

			}

			speed = std::max(speed, 0.1f);

			float the_yaw = local_player->getDirection();

			float x_ = -sin(the_yaw) * speed;
			float z_ = cos(the_yaw) * speed;
			local_player->SetmotionX(x_);
			local_player->SetmotionZ(z_);
		}
	}
	else if (config::speed::selected_mode == 4) // custom
	{
		if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
		{
		//	static auto potion = std::make_shared<c_potion>();
			//static jobject pot = get_speed_potion();
			float moveSpeed = 0.2873f * (speed::m_speed / 1.5);
			//float moveSpeed = local_player->get_move_speed(pot) * (speed::m_speed / 1.5);

			const auto& bb = local_player->get_bounding_box();

			if (ctx->world->get()->getCollidingBoundingBoxes(local_player->get_object(), bb->offset(0, -speed::m_offset, 0)->get_object()) > 0)
			{
			//	Speed = local_player->get_move_speed(pot) * 1.3492287832; // FUN = * 5.f;
				local_player->SetmotionY(speed::y_motion);
				//local_player->set_movestrafe(local_player->get_movestrafe() * 2.);
			}

			vec3 mot = local_player->get_motion_vector();
			float the_yaw = local_player->getDirection();

			float x_ = -sin(the_yaw) * moveSpeed;
			float z_ = cos(the_yaw) * moveSpeed;
			local_player->SetmotionX(x_);
			local_player->SetmotionZ(z_);

		}
		
	}
	//else if (config::speed::selected_mode == 3)
	//{
	//	auto bb = local_player->get_bounding_box();
	//	static auto potion = std::make_shared<c_potion>();
	//	static jobject pot = potion->get_speed_potion();

	//	float mov_speed = local_player->get_move_speed(pot);

	//	if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
	//	{
	//		if (local_player->on_ground())
	//		{
	//			float length = 0.5f;

	//			float the_yaw = local_player->getDirection();
	//			float x_ = -sin(the_yaw) * length;
	//			float z_ = cos(the_yaw) * length;
	//			local_player->SetmotionX(x_);
	//			local_player->SetmotionZ(z_);

	//		}


	//	}
	//	else {
	//		local_player->SetmotionX(0);
	//		local_player->SetmotionZ(0);
	//	}
	//}
	//else if (mode == 3)
	//{
	//	float mov_speed = 0.f;
	//	auto bb = local_player->get_bounding_box();
	//	static auto potion = std::make_shared<c_potion>();
	//	jobject pot = potion->get_speed_potion();

	//	if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
	//	{
	//		if (local_player->get_ticksExisted() % 2 == 0)
	//		{
	//			float the_yaw = local_player->getDirection();

	//			float x_ = -sin(the_yaw) * 0.23f;
	//			float z_ = cos(the_yaw) * 0.23f;
	//			local_player->SetmotionX(x_);
	//			local_player->SetmotionZ(z_);
	//		}
	//		if (local_player->on_ground())
	//		{
	//			local_player->jump();
	//		}
	//		else {

	//		}

	//	}
	//}

}
























//local_player->forward(1.3337, 0); spam this phase

//	local_player->set_jump_mov_fac(0);
//if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
//{
//	if (local_player->on_ground())
//	{
//		local_player->set_noclip(true);
//
//		if (local_player->get_ticksExisted() % 2 == 0)
//		{
//			switch (stage)
//			{
//			case 0:
//				timer_t->set_ticksPerSecond(23);
//				stage++;
//				break;
//			case 1:
//				timer_t->set_ticksPerSecond(24);
//				stage++;
//				break;
//			case 2: timer_t->set_ticksPerSecond(25);
//				stage = 0;
//				break;
//
//			}
//		}
//		else {
//			timer_t->set_ticksPerSecond(20);
//		}
//
//		vec3 motion = local_player->get_motion_vector();
//		float the_yaw = local_player->getDirection();
//		float uwu = sqrt(motion.x * motion.x + motion.z * motion.z);
//
//		float x_ = -sin(the_yaw) * uwu * 1.03;
//		float z_ = cos(the_yaw) * uwu * 1.03;
//		local_player->SetmotionX(local_player->get_moveforward() * x_);
//		local_player->SetmotionZ(local_player->get_moveforward() * z_);
//
//		if (local_player->get_moveforward() != .0f || local_player->get_movestrafe() != .0f)
//		{
//
//			local_player->jump();
//			if (local_player->get_hurt_time() > 0)
//			{
//				float the_yaw = local_player->getDirection();
//
//				float x_ = -sin(the_yaw) * 0.25;
//				float z_ = cos(the_yaw) * 0.25;
//				local_player->SetmotionX(local_player->get_motion_vector().x - x_);
//				local_player->SetmotionZ(local_player->get_motion_vector().x + z_);
//			}
//
//			vec3 motion = local_player->get_motion_vector();
//			local_player->SetmotionX(motion.x * 1.03);
//			local_player->SetmotionZ(motion.z * 1.03);
//			local_player->set_movestrafe(local_player->get_movestrafe() * 2.3);
//			//timer_t->set_timer_speed(1.5f);
//
//		}
//	}
//	else {
//		local_player->set_jump_mov_fac(0.0265f);
//		local_player->set_land_mov_fac(0.0265f);
//		local_player->set_movestrafe(local_player->get_movestrafe() * 2.3);
//		//timer_t->set_timer_speed(1.05f);
//	}
//}
