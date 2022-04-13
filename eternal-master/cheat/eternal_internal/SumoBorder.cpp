#include "SumoBorder.hpp"

namespace misc {

	namespace sumoborder {

		std::vector<s_blockpos> block_list = 
		{ 
			s_blockpos({9, 65, -2}),  s_blockpos({9, 65, -1}), s_blockpos({9, 65, 0}), s_blockpos({9, 65, 1}), s_blockpos({9, 65, 2}), s_blockpos({9, 65, 3}), s_blockpos({8, 65, 3}), s_blockpos({8, 65, 4}), s_blockpos({8, 65, 5}), s_blockpos({7, 65, 5}),
			s_blockpos({7, 65, 6}), s_blockpos({7, 65, 7}), s_blockpos({6, 65, 7}), s_blockpos({5, 65, 7}), s_blockpos({5, 65, 8}), s_blockpos({4, 65, 8}), s_blockpos({3, 65, 8}), s_blockpos({3, 65, 9}), s_blockpos({2, 65, 9}), s_blockpos({1, 65, 9}),
			s_blockpos({0, 65, 9}), s_blockpos({-1, 65, 9}), s_blockpos({-2, 65, 9}), s_blockpos({-3, 65, 9}), s_blockpos({-3, 65, 8}), s_blockpos({-4, 65, 8}), s_blockpos({-5, 65, 8}), s_blockpos({-5, 65, 7}), s_blockpos({-6, 65, 7}), s_blockpos({-7, 65, 7}),
			s_blockpos({-7, 65, 6}), s_blockpos({-7, 65, 5}), s_blockpos({-8, 65, 5}), s_blockpos({-8, 65, 4}), s_blockpos({-8, 65, 3}), s_blockpos({-9, 65, 3}), s_blockpos({-9, 65, 2}), s_blockpos({-9, 65, 1}), s_blockpos({-9, 65, 0}), s_blockpos({-9, 65, -1}),
			s_blockpos({-9, 65, -2}), s_blockpos({-9, 65, -3}), s_blockpos({-8, 65, -3}), s_blockpos({-8, 65, -4}), s_blockpos({-8, 65, -5}), s_blockpos({-7, 65, -5}), s_blockpos({-7, 65, -6}), s_blockpos({-7, 65, -7}), s_blockpos({-6, 65, -7}), s_blockpos({-5, 65, -7}),
			s_blockpos({-5, 65, -8}), s_blockpos({-4, 65, -8}), s_blockpos({-3, 65, -8}), s_blockpos({-3, 65, -9}), s_blockpos({-2, 65, -9}), s_blockpos({-1, 65, -9}), s_blockpos({0, 65, -9}), s_blockpos({1, 65, -9}), s_blockpos({2, 65, -9}), s_blockpos({3, 65, -9}),
			s_blockpos({3, 65, -8}), s_blockpos({4, 65, -8}), s_blockpos({5, 65, -8}), s_blockpos({5, 65, -7}), s_blockpos({6, 65, -7}), s_blockpos({7, 65, -7}), s_blockpos({7, 65, -6}), s_blockpos({7, 65, -5}), s_blockpos({8, 65, -5}), s_blockpos({8, 65, -4}),
			s_blockpos({8, 65, -3}), s_blockpos({9, 65, -3})
		};


		void onUpdate(c_context* ctx)
		{
			static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();

			if (!timer->has_passed(1000))
				return;

			timer->reset();

			auto world = ctx->world->get();

			if (sumoborder::enabled && !sumoborder::should_disable)
			{
				for (const auto& block : block_list)
				{
					for (int i = 0; i < 30; i++)
					{
						auto nigga = world->get_blocknigger(vec3({ (float)block.x, (float)(block.y + i), (float)block.z }));

						static jobject glass_bs = world->get_block_state_by_id(20);
						world->set_blockstate(nigga, glass_bs);

						ctx->env->DeleteLocalRef(nigga);
					}
				}
			}

			if (should_disable)
			{
				onDisable(ctx);
				should_disable = false;
				enabled = false;
			}
		}

		void onDisable(c_context* ctx)
		{
			auto world = ctx->world->get();

			for (const auto& block : block_list)
			{
				for (int i = 0; i < 30; i++)
				{
					auto nigga = world->get_blocknigger(vec3({ (float)block.x, (float)(block.y + i), (float)block.z }));

					static jobject air_bs = world->get_block_state_by_id(0);
					world->set_blockstate(nigga, air_bs);

					ctx->env->DeleteLocalRef(nigga);
				}
			}

		}
	}
}