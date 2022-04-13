#include "SafeWalk.hpp"
#include "timer.hpp"

void safewalk::safewalk(c_context* ctx)
{
	auto local_player = ctx->local_player->get();
	auto world = ctx->world->get();
	//if (!safewalk::m_enabled)
	//	return;

	vec3 cur_pos = local_player->get_position();
	cur_pos.y -= 1.f;

	jobject block = world->get_blocknigger(cur_pos);

	if (block != NULL && local_player->on_ground())
	{
		bool is_block_air = world->is_air_block(block);

		{
			vec3 _motion_ = local_player->get_motion_vector();
			double increment;

			for (increment = 0.05; _motion_.x != 0.;)
			{
				if (_motion_.x < increment && _motion_.x >= -increment)
				{
					_motion_.x = 0;
				}
				else if (_motion_.x > 0)
				{
					_motion_.x -= increment;
				}
				else {
					_motion_.x += increment;
				}
			}

			for (; _motion_.z != 0.f;)
			{
				if (_motion_.z < increment && _motion_.z >= -increment)
				{
					_motion_.z = 0;
				}
				else if (_motion_.z > 0)
				{
					_motion_.z -= increment;
				}
				else {
					_motion_.z += increment;
				}

			}
			float the_yaw = local_player->getDirection();
			local_player->SetmotionX(_motion_.x);
			local_player->SetmotionZ(_motion_.z);
		}
		
	}
	ctx->env->DeleteLocalRef(block);

}


// sneak mode

//vec3 _motion_ = local_player->get_motion_vector();
//
//
//if (local_player->on_ground())
//{
//	double increment;
//	for (increment = 0.05; _motion_.x != 0.;)
//	{
//		if (_motion_.x < increment && _motion_.x >= -increment)
//		{
//			_motion_.x = 0;
//		}
//		else if (_motion_.x > 0)
//		{
//			_motion_.x -= increment;
//		}
//		else {
//			_motion_.x += increment;
//		}
//	}
//
//	for (; _motion_.z != 0.f;)
//	{
//		if (_motion_.z < increment && _motion_.z >= -increment)
//		{
//			_motion_.z = 0;
//		}
//		else if (_motion_.z > 0)
//		{
//			_motion_.z -= increment;
//		}
//		else {
//			_motion_.z += increment;
//		}
//	}
//
//	for (; _motion_.x != 0 && _motion_.z != 0;)
//	{
//		if (_motion_.x < increment && _motion_.x >= -increment)
//		{
//			_motion_.x = 0;
//		}
//		else if (_motion_.x > 0)
//		{
//			_motion_.x -= increment;
//		}
//		else {
//			_motion_.x += increment;
//		}
//		if (_motion_.z < increment && _motion_.z >= -increment)
//		{
//			_motion_.z = 0;
//		}
//		else if (_motion_.z > 0)
//		{
//			_motion_.z -= increment;
//		}
//		else {
//			_motion_.z += increment;
//		}
//	}
//}
//local_player->SetmotionX(_motion_.x);
//local_player->SetmotionZ(_motion_.z);