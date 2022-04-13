#include "Reach.hpp"
#include "AxisAlignedBB.hpp"
#include "Timer.h"
#include "player_utils.hpp"
#include "config.hpp"
#include "FriendList.hpp"

#include "functions.hpp"
#include "global.hpp"
#include "SelfDestruct.hpp"
#include "MovingObjectPosition.hpp"

jobject hitvec;

void getMouseOver(double reach)
{
	/* Variables */

	//auto local_player = ctx->local_player->get();
	//auto world = ctx->world->get();
	//auto minecraft = ctx->minecraft->get();
	//auto timer_t = std::make_shared<timer>(minecraft->getTimer());

	//double partialTicks = timer_t->get_renderPartialTicks();

	///* Start */

	//double d0 = reach;
	//double d1 = d0;

	//auto var6 = local_player->getPositionEyes(partialTicks);
	//auto var7 = local_player->getLook(partialTicks)->get_native_vec3();
	////auto var7 = var7_->get_native_vec3();
	//
	//auto var8 = var6->addVector(var7.x * reach, var7.y * reach, var7.z * reach);

	//minecraft->set_objectMouseOver(local_player->rayTrace(d0, partialTicks));

	//bool flag = false;
	//bool flag1 = true;

	//minecraft->reset_pointedentity();

	///* part 2 */

	//jobject var9 = NULL;

	//float var10 = 1.0F;
	//auto boundingbox = local_player->get_bounding_box()->addCoord(var7.x * d0, var7.y * d0, var7.z * d0)->expand((double)var10, (double)var10, (double)var10);
	//auto var11 = world->getEntitiesWithinAABB(local_player->get_object(), boundingbox);

	//std::shared_ptr<c_vec3> vector3 = nullptr;

	//double var12 = d1;

	//for (const auto& entity : var11)
	//{
	//	if (entity->canBeCollidedWith())
	//	{
	//		float f1 = entity->getCollisionBorderSize(); // hitbox size

	//		auto axisalignedbb = entity->get_bounding_box()->expand((double)f1, (double)f1, (double)f1);
	//		auto movingobjectposition = axisalignedbb->calculateIntercept(var6, var8);

	//		if (movingobjectposition != NULL)
	//		{
	//			hitvec = ctx->env->GetObjectField(movingobjectposition, hitVecf);
	//		}

	//		if (axisalignedbb->isVecInside(var6))
	//		{
	//			if (0.0 < d1 || d1 == 0.0)
	//			{
	//				minecraft->set_pointedentity(entity->get_object());
	//				var9 = movingobjectposition == NULL ? var6->get_object() : hitvec;
	//				d1 = 0.0;
	//			}
	//		}

	//		else if (movingobjectposition != NULL)
	//		{
	//			auto other_vec = std::make_shared<c_vec3>(hitvec)->get_native_vec3(); // Correct
	//			float dist = var6->get_native_vec3().distanceTo(other_vec);

	//			if (dist < d1 || d1 == 0.0f)
	//			{
	//				std::cout << "Distance: " << dist << std::endl;
	//				minecraft->set_pointedentity(entity->get_object());
	//				var9 = hitvec;
	//				var12 = dist;
	//			}
	//		}
	//	}

	//}
	//if (minecraft->get_pointed_entity() != NULL && d1 > 3.0)
	//{
	//	jclass c = getObject(ctx->env, global::is_badlion ? "auh" : "net.minecraft.util.MovingObjectPosition");
	//	jmethodID mid = ctx->env->GetMethodID(c, "<init>", global::is_badlion ? "(Lpk;Laui;)V" : "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/Vec3;)V");

	//	minecraft->set_objectMouseOver(ctx->env->NewObject(c, mid, minecraft->get_pointed_entity(), var9));
	//	ctx->env->DeleteLocalRef(c);
	//}

}
//
//void getMouseOver_(float p_78473_1_, JNIEnv* env, double reach)
//{
//	if (renderViewEntity != NULL)
//	{
//		if (theWorld != NULL)
//		{
//			double var2 = reach;
//
//			double var4 = var2;
//
//			jobject var6 = env->CallObjectMethod(renderViewEntity, getPositionf, p_78473_1_);
//
//			jobject var7 = env->CallObjectMethod(renderViewEntity, getLookf, p_78473_1_);
//			xCoord = env->GetDoubleField(var7, xCoordf);
//			yCoord = env->GetDoubleField(var7, yCoordf);
//			zCoord = env->GetDoubleField(var7, zCoordf);
//			jobject var8 = env->CallObjectMethod(var6, addVectorf, xCoord * var2, yCoord * var2, zCoord * var2);
//			env->SetObjectField(EntityRenderer, pointedEntityf, NULL);
//			jobject var9 = NULL;
//			float var10 = 1.0F;
//			boundingBox = env->GetObjectField(renderViewEntity, boundingBoxf);
//			jobject var11 = env->CallObjectMethod(theWorld, getEntitiesWithinAABBExcludingEntityf, renderViewEntity, env->CallObjectMethod(env->CallObjectMethod(boundingBox, addCoordf, xCoord * var2, yCoord * var2, zCoord * var2), expandf, (double)var10, (double)var10, (double)var10));
//			double var12 = var4;
//			for (int var14 = 0; var14 < env->CallIntMethod(var11, sizef); ++var14)
//			{
//				jobject var15 = env->CallObjectMethod(var11, getf, var14);
//
//				if (env->CallBooleanMethod(var15, canBeCollidedWithf))
//				{
//					float var16 = var15.getCollisionBorderSize();
//					float var16 = env->CallFloatMethod(var15, getCollisionBorderSizef);
//					jobject var17 = env->CallObjectMethod(env->GetObjectField(var15, boundingBoxf), expandf, (double)var16, (double)var16, (double)var16);
//					jobject var18 = env->CallObjectMethod(var17, calculateInterceptf, var6, var8);
//					if (var18 != NULL)
//						hitVec = env->GetObjectField(var18, hitVecf);
//
//					if (env->CallBooleanMethod(var17, isVecInsidef, var6))
//					{
//						if (0.0 < var12 || var12 == 0.0)
//						{
//							env->SetObjectField(EntityRenderer, pointedEntityf, var15);
//							var9 = var18 == NULL ? var6 : hitVec;
//							var12 = 0.0;
//						}
//					}
//					else if (var18 != NULL)
//					{
//						double var19 = env->CallDoubleMethod(var6, distanceTof, hitVec);
//						if (var19 < var12 || var12 == 0.0)
//						{
//							ridingEntity = env->GetObjectField(renderViewEntity, ridingEntityf);
//							if (var15 == ridingEntity)
//							{
//								if (var12 == 0.0)
//								{
//									env->SetObjectField(EntityRenderer, pointedEntityf, var15);
//									var9 = hitVec;
//								}
//							}
//							else
//							{
//								env->SetObjectField(EntityRenderer, pointedEntityf, var15);
//								var9 = hitVec;
//								var12 = var19;
//							}
//						}
//					}
//				}
//			}
//
//			if (this.pointedEntity != null && (var12 < var4 || this.mc.objectMouseOver == null))
//			if (env->GetObjectField(EntityRenderer, pointedEntityf) != NULL && var12 > 3.0f)
//			{
//				env->SetObjectField(mc, objectMouseOverf, env->NewObject(MovingObjectPosition, movingObjectPositionc, env->GetObjectField(EntityRenderer, pointedEntityf), var9));
//			}
//		}
//	}
//}

bool keep = false;
jobject pointed = NULL;

void reach::reach_()
{
	
	//while (!selfdestruct::should_destruct)
	{
		auto env =getJNI();

		if (env != nullptr)
		{
			auto minecraft = std::make_unique<c_minecraft>(getMC(env), env);

			if (minecraft->get_object() != NULL && minecraft->getPlayer() != NULL && minecraft->getWorld() != NULL) // Ingame check
			{
				auto local_player = std::make_unique<c_player>(minecraft->getPlayer(), env);
				auto world = std::make_unique<c_world>(minecraft->getWorld(), env);

				if (!GetAsyncKeyState(VK_LBUTTON) && !local_player->isUsingItem())
				{
					auto movingobject = std::make_shared<c_movingobjectposition>(minecraft->objectMouseOver());
					if (movingobject == NULL)
						return;

					if (!keep && movingobject->getEntityHit() != NULL)
						keep = true;

					if (keep)
					{
						auto render_view_ent = std::make_shared<c_player>(minecraft->getrenderviewentity(), env);
						auto timer_t = std::make_unique<timer>(minecraft->getTimer());

						double partialTicks = 1.f;//timer_t->get_renderPartialTicks();

						/* Start */

						double d0 = reach::m_reach;
						double d1 = d0;

						auto var6 = render_view_ent->getPositionEyes(partialTicks);
						auto var7 = render_view_ent->getLook(partialTicks)->get_native_vec3();

						auto var8 = var6->addVector(var7.x * (double)reach::m_reach, var7.y * (double)reach::m_reach, var7.z * (double)reach::m_reach);


						//minecraft->set_objectMouseOver(local_player->rayTrace(d0, partialTicks));

						/* part 2 */

						jobject var9 = nullptr;

						float var10 = 1.0F;
						auto boundingbox = render_view_ent->get_bounding_box()->addCoord(var7.x * d0, var7.y * d0, var7.z * d0)->expand((double)var10, (double)var10, (double)var10);
						auto var11 = world->getEntitiesWithinAABB(render_view_ent->get_object(), boundingbox);

						std::shared_ptr<c_vec3> vector3 = nullptr;

						double var12 = d1;

						for (const auto& entity : var11)
						{
							if (entity->canBeCollidedWith())
							{
								float f1 = entity->getCollisionBorderSize(); // hitbox size

								auto axisalignedbb = entity->get_bounding_box()->expand((double)f1, (double)f1, (double)f1);
								auto movingobjectposition = axisalignedbb->calculateIntercept(var6, var8);

								if (movingobjectposition != nullptr)
								{
									hitvec = env->GetObjectField(movingobjectposition, hitVecf);
								}

								if (axisalignedbb->isVecInside(var6))
								{
									//if (0.0 < d1 || d1 == 0.0)
									if (d1 >= 0.) 
									{
										minecraft->set_pointedentity(entity->get_object());
										var9 = movingobjectposition == nullptr ? var6->get_object() : hitvec;
										d1 = 0.0;
									}
								}

								else if (movingobjectposition != nullptr)
								{
									auto other_vec = std::make_shared<c_vec3>(hitvec)->get_native_vec3(); // Correct
									float dist = var6->get_native_vec3().distanceTo(other_vec);

									if (dist < d1 || d1 == 0.0f)
									{
										minecraft->set_pointedentity(entity->get_object());
										pointed = entity->get_object();
										var9 = hitvec;
										var12 = dist;
									}
								}
							}

						}
						if (minecraft->get_pointed_entity() != nullptr && var12 > 3.0 )//&& minecraft->objectMouseOver() == NULL
						{
							static jclass movingobject = getObject(env, global::is_badlion ? "auh" : "net.minecraft.util.MovingObjectPosition");
							jmethodID mid = env->GetMethodID(movingobject, "<init>", global::is_badlion ? "(Lpk;Laui;)V" : "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/Vec3;)V");

							jobject obj = env->NewObject(movingobject, mid, pointed, hitvec);

							if (obj != NULL)
							{
								//auto hitty = std::make_shared<c_vec3>(hitvec)->get_native_vec3();
								//std::cout << hitty.x << ", " << hitty.y << ", " << hitty.x 
								std::cout << "Distance: " << var12 << std::endl;
								minecraft->set_objectMouseOver(obj);
								minecraft->set_pointedentity(pointed);
							}
						}
					}
				//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

			}

		}

	//	std::this_thread::sleep_for(std::chrono::milliseconds(200));
		
	}
}

void reach::reach(c_context* ctx)
{
	const auto is_sane = [&]() {
		return (ctx->local_player->get()->get_object() && ctx->world->get()->get_object());
	};

	static std::shared_ptr<c_timer> timer_ = std::make_shared<c_timer>();
	auto timer_t = std::make_unique<timer>(ctx->minecraft->get()->getTimer());

	if (!timer_->has_passed(5))
		return;

	timer_->reset();

	if (reach::m_enabled)
	{
		//getMouseOver(6.f, ctx);
		auto distance = reach::m_reach - 3.0f;

		const auto position = ctx->local_player->get()->get_position();

		if (config::reach::selected[0])
		{
			if (!ctx->local_player->get()->is_sprinting())
				return;
		}
		if (config::reach::selected[2])
		{
			if (ctx->local_player->get()->is_inwater())
				return;
		}

		//getMouseOver(, getJNI(), reach::m_reach);
		for (const auto& entity : ctx->world->get()->get_players())
		{
			if (entity->is_invisible())
				continue;

			if (!entity->get_health() > 0.0f)
				continue;

			if (ctx->env->IsSameObject(ctx->local_player->get()->get_object(), entity->get_object()))
				continue;


			if (auto itr = std::find(targetinfo::FriendList.begin(), targetinfo::FriendList.end(), entity->get_name()); itr != targetinfo::FriendList.end())
				continue;
			
			//if (is_npc(&entity) || is_hypixel_npc(&entity))
			//{
			//	vec3 my_pos = ctx->local_player->get()->get_position();
			//	entity->set_pos(vec3{ (float)my_pos.x, my_pos.y + 2, (float)my_pos.z }, false);
			//}

			//if (ctx->local_player->get()->get_distance_to(entity) < 12)
			//	continue;

			auto entity_position = entity->get_position();

			if (auto hypothenuse_distance = hypot(position.x - entity_position.x, position.z - entity_position.z); distance > hypothenuse_distance)
				distance -= hypothenuse_distance;

			auto angles = util::get_angles(position, entity_position);

			auto difference = util::wrap_to_180(-(ctx->local_player->get()->get_yaw() - angles.first));

			//if (std::abs(difference) > 180.0f)
			//	continue;

			auto cos = std::cos(util::deg_to_radiants(angles.first + 90.0f));
			auto sin = std::sin(util::deg_to_radiants(angles.first + 90.0f));

			auto x = entity_position.x, z = entity_position.z;
			x -= (cos * distance);
			z -= (sin * distance);

			auto entity_width = 0.6f;

			auto bb_width = entity_width / 2.0f;

			auto current_boundingbox = entity->get_bounding_box()->get_native_boundingbox();

			float theval = hitbox_size / 2;
			s_axisalignedbb bb{};
			bb.minX = x - bb_width - theval;
			bb.minY = current_boundingbox.minY;
			bb.minZ = z - bb_width - theval;

			bb.maxX = x + bb_width + theval;
			bb.maxY = current_boundingbox.maxY;
			bb.maxZ = z + bb_width + theval;

			//entity->set_newPos(vec3{ (float)x, (float)entity->get_position().y, (float)z });
			//entity->set_pos(vec3{ (float)x, (float)entity->get_position().y, (float)z}, false);

			if (config::reach::selected[0])
			{
				if (!ctx->local_player->get()->is_sprinting())
					return;
			}
			if (config::reach::selected[1])
			{
				if (!ctx->local_player->get()->is_visible(entity->get_object()))
					return;			
			}
			if (config::reach::selected[2])
			{
				if (ctx->local_player->get()->is_inwater())
					return;
			}

			entity->get_bounding_box()->set_native_boundingbox(bb);
			
		}

	}
}