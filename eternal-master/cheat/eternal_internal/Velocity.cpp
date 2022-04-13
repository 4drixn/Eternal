#include "Velocity.hpp"
#include "timer.hpp"
#include "Packet.hpp"
#include "config.hpp"

namespace velocity {

	void velocity(c_context* context)
	{
		static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();
		static bool motion_set = false;

		if (!velocity::m_enabled) return;

		if (!timer->has_passed(5))
			return;

		timer->reset();


		auto local_player = context->local_player->get();
		vec3 motion = local_player->get_motion_vector();

		vec3 _motion_ = { 0.0, 0.0, 0.0 };

		if (config::velocity::selected_item == 0)
		{
			if (local_player->get_hurt_time() > 0 && (local_player->get_hurt_time() + velocity::delay_ticks) == local_player->get_max_hurt_time()) //&& local_player->get_hurt_time() == local_player->get_max_hurt_time()))
			{
				float x = (motion.x) * (velocity::velocity_h / 100.f);
				float z = (motion.z) * (velocity::velocity_h / 100.f);
				float y = (motion.y) * (velocity::velocity_v / 100.f);
				local_player->SetmotionX(x);
				local_player->SetmotionZ(z);
				motion_set = true;
				//if (motion.y >= 9. || motion.y <= -9) return;
				if(velocity::velocity_v != 100.)
					local_player->SetmotionY(y);

			}
			//if (local_player->get_hurt_time() == 0)
			//	motion_set = false;
		}
		else if (config::velocity::selected_item == 1)
		{
			if (!motion_set && local_player->get_hurt_time() > 0 && (local_player->get_hurt_time() + velocity::delay_ticks) == local_player->get_max_hurt_time())
			{
				float speed = sqrt(motion.x * motion.x + motion.z * motion.z);

				float x = (motion.x) * (speed / 100.) * (velocity_h / 100.);
				float z = (motion.z) * (speed / 100.) * (velocity_h / 100.);
				float y = (motion.y) * (velocity::velocity_v / 100.);

				float the_yaw = local_player->getDirection();

				float x_ = -sin(the_yaw) * speed * (velocity_h / 100.); ;
				float z_ = cos(the_yaw) * speed * (velocity_h / 100.);
				
				local_player->SetmotionX(x_);
				local_player->SetmotionZ(z_);
				local_player->SetmotionY(y);
			}
			if (local_player->get_hurt_time() == 0)
				motion_set = false;
		}
		else if (config::velocity::selected_item == 2)
		{
			//Recode this
			if (local_player->get_hurt_time() == 7 && !motion_set)
			{
				if (local_player->on_ground())
				{
					local_player->set_noclip(true);
				}
				else
				{
					local_player->set_noclip(false);
				}
				local_player->SetmotionY(0.4);
			}
			else if (motion_set && local_player->get_hurt_time() == 0)
				motion_set = false;

		}
		else if (config::velocity::selected_item == 3)
		{

			if ((local_player->get_hurt_time() > 0) ) //local_player->get_hurt_time() == local_player->get_max_hurt_time()
			{
				vec3 motion = local_player->get_motion_vector();
				if (motion.x != 0.f && motion.z != 0.f)
				{
					local_player->set_on_ground(true);
					local_player->get_sendQueue()->addToSendQueue(packets::C03PacketPlayer::init(true));
				}

				if (local_player->get_hurtResistantTime() > 0 )
				{
					float y_ = (motion.y) - 0.014999993f;
					local_player->SetmotionY(y_);
				}

				if (local_player->get_hurtResistantTime() >= 19)
				{
					float x_ = (motion.x) / 1.3;
					float z_ = (motion.z) / 1.3;
					local_player->SetmotionX(x_);
					local_player->SetmotionZ(z_);
				}
			}

		}
		else if (config::velocity::selected_item == 4)
		{
			if (!motion_set && (local_player->get_hurt_time() > 0) && local_player->on_ground() && motion.x > 0 && motion.z > 0)
			{
				local_player->SetmotionY(0.42f);
				float yaw = local_player->get_yaw() * 0.017453292f;

				float speed = 0.2f;//sqrt(motion.x * motion.x + motion.z * motion.z);
				float _x_ = motion.x - sin(yaw) * ((velocity::velocity_h / 100.) * speed);
				float _y_ = motion.z + cos(yaw) * ((velocity::velocity_h / 100.) * speed);

				local_player->SetmotionX(_x_);
				local_player->SetmotionZ(_y_);
			}
			else if (motion_set && local_player->get_hurt_time() == 0)
				motion_set = false;
		}
		

	}
}