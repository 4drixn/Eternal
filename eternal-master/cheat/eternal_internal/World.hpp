#pragma once

#include <jni.h>
#include <vector>
#include <memory>
#include "BlockPos.hpp"
#include "Tesellator.hpp"

class c_world {
private:
	jobject world;
	JNIEnv* env;

public:
	c_world(jobject _world, JNIEnv* _env)
	{
		world = _world;
		env = _env;
	}
	~c_world()
	{
		env->DeleteLocalRef(world);
	}

	std::vector<std::shared_ptr<c_player>> getEntitiesWithinAABB(jobject local_player, const std::shared_ptr<c_axisalignedbb>& bb)
	{
		auto this_clazz = env->GetObjectClass(world);
		jmethodID mid = env->GetMethodID(this_clazz, global::is_badlion ? "b" : "func_72839_b", global::is_badlion ? "(Lpk;Laug;)Ljava/util/List;" : "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/AxisAlignedBB;)Ljava/util/List;");

		jobject var11 = env->CallObjectMethod(world, mid, local_player, bb->get_object());

		jclass list_cls = env->FindClass("java/util/List");
		jmethodID to_array_md = env->GetMethodID(list_cls, "toArray", "()[Ljava/lang/Object;");

		env->DeleteLocalRef(list_cls);
		env->DeleteLocalRef(this_clazz);

		std::vector<std::shared_ptr<c_player>> res;

		auto array_list = reinterpret_cast<jobjectArray>(env->CallObjectMethod(var11, to_array_md));

		if (!array_list)
			return res;

		size_t len = env->GetArrayLength(array_list);

		for (int i = 0; i < len; ++i)
		{
			jobject player = env->GetObjectArrayElement(array_list, i);
			res.push_back(std::make_shared<c_player>(player, env));
		}

		env->DeleteLocalRef(var11);
		env->DeleteLocalRef(array_list);

		return res;
	}
	std::vector<std::shared_ptr<c_player>> get_players()
	{
		auto world_cls = global::is_lunar ? mcc::World : env->GetObjectClass(world);
		jfieldID player_entities;
		
		if(global::is_lunar)
		{
			static const auto& _field_ = mcc::impl::World::fields[8];
			//player_entities = env->GetFieldID(world_cls, "seseeaphshhapspahasheshas", "Ljava/util/List;");
			player_entities = env->GetFieldID(world_cls, _field_.name, "Ljava/util/List;");
		}
		else {
			player_entities = env->GetFieldID(world_cls, global::is_badlion ? "j" : "field_73010_i", "Ljava/util/List;");
		}

		if (!player_entities)
			return std::vector<std::shared_ptr<c_player>>();

		static jclass list_cls = env->FindClass("java/util/List");
		static jmethodID to_array_md = env->GetMethodID(list_cls, "toArray", "()[Ljava/lang/Object;");

		//env->DeleteLocalRef(list_cls);
		if(!global::is_lunar)
			env->DeleteLocalRef(world_cls);

		std::vector<std::shared_ptr<c_player>> res;

		jobject obj_player_entities = env->GetObjectField(world, player_entities);

		if (!obj_player_entities)
			return res;

		auto array_player_list = reinterpret_cast<jobjectArray>(env->CallObjectMethod(obj_player_entities, to_array_md));

		if (!array_player_list)
			return res;

		size_t len = env->GetArrayLength(array_player_list);

		for (int i = 0; i < len; ++i)
		{
			if(jobject player = env->GetObjectArrayElement(array_player_list, i); player != nullptr)
				res.push_back(std::make_shared<c_player>(player, env));
		}

		env->DeleteLocalRef(obj_player_entities);
		env->DeleteLocalRef(array_player_list);

		return res;
	}

	jobject new_bounding(s_axisalignedbb bb)
	{
		if (global::is_lunar)
		{
			jclass c = mcc::AxisAlignedBB;
			jmethodID mid = env->GetMethodID(c, "<init>", "(DDDDDD)V");

			return env->NewObject(c, mid, bb.minX, bb.minY, bb.minZ, bb.maxX, bb.maxY, bb.maxZ);
		}
		else {
			jclass c = getObject(env, global::is_badlion ? "aug" : "net.minecraft.util.AxisAlignedBB");
			jmethodID mid = env->GetMethodID(c, "<init>", "(DDDDDD)V");
			jobject newObj = env->NewObject(c, mid, bb.minX, bb.minY, bb.minZ, bb.maxX, bb.maxY, bb.maxZ);

			env->DeleteLocalRef(c);

			return newObj;
		}
	}
	
	jobject get_blocknigger(vec3 ok)
	{
		if (global::is_lunar)
		{
			jclass c = mcc::BlockPos;
			jmethodID mid = env->GetMethodID(c, "<init>", "(DDD)V");

			return env->NewObject(c, mid, static_cast<double>(ok.x), static_cast<double>(ok.y), static_cast<double>(ok.z));
		}
		else {
			jclass c = getObject(env, global::is_badlion ? "cj" : "net.minecraft.util.BlockPos");
			jmethodID mid = env->GetMethodID(c, "<init>", "(DDD)V");
			jobject newObj = env->NewObject(c, mid, ok.x, ok.y, ok.z);

			env->DeleteLocalRef(c);

			return newObj;
		}
	}

	jobject get_block_state_by_id(int id)
	{
		jclass c = global::is_lunar ? mcc::Block : getObject(env, global::is_badlion ? "afh" : "net.minecraft.block.Block");
		jmethodID mid;
		
		if(global::is_lunar)
			mid = env->GetStaticMethodID(c, "aeahheeahpaesppshphehseep", "(I)Lnet/minecraft/v1_8/hhshpeapeessahseaahpesaap;");
		else 
			mid = env->GetStaticMethodID(c, global::is_badlion ? "d" : "func_176220_d", global::is_badlion ? "(I)Lalz;" : "(I)Lnet/minecraft/block/state/IBlockState;");

		auto k = env->CallStaticObjectMethod(c, mid, id);

		if(!global::is_lunar)
			env->DeleteLocalRef(c);

		return k;
	}

	jobject get_blockstate(jobject blockpos)
	{
		if(global::is_lunar)
		{
			auto this_class = env->GetObjectClass(world);
			jmethodID mid = env->GetMethodID(this_class,  "aehphaeasashsssspsaaahppe",  "(Lnet/minecraft/v1_8/aesepsshepasspseahppsseep;)Lnet/minecraft/v1_8/spsphshaeapshhaehpahpaaps;");

			env->DeleteLocalRef(this_class);
			return env->CallObjectMethod(world, mid, blockpos);
		}
		else {
			auto this_class = env->GetObjectClass(world);
			jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "p" : "func_180495_p", global::is_badlion ? "(Lcj;)Lalz;" : "(Lnet/minecraft/util/BlockPos;)Lnet/minecraft/block/state/IBlockState;");

			env->DeleteLocalRef(this_class);
			return env->CallObjectMethod(world, mid, blockpos);
		}
	}

	jobject get_block(jobject IBlockState)
	{

		auto this_class = env->GetObjectClass(IBlockState);
		jmethodID mid;

		if (global::is_lunar)
			mid = env->GetMethodID(this_class, "hseaphhshsseepseppeeeappp", "()Lnet/minecraft/v1_8/aeahheeahpaesppshphehseep;");
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "c" : "func_177230_c", global::is_badlion ? "()Lafh;" : "()Lnet/minecraft/block/Block;");
		env->DeleteLocalRef(this_class);

		return env->CallObjectMethod(IBlockState, mid);
	}

	void set_blockstate(jobject bp, jobject IBlockState)
	{
		auto this_class = env->GetObjectClass(world);
		jmethodID mid;
		
		if(global::is_lunar)
			mid = env->GetMethodID(this_class, "hpspsspaesesapapeeehsspeh", "(Lnet/minecraft/v1_8/paaahssaehhehsshassaahheh;Lnet/minecraft/v1_8/ppspsapppspphhaspheeasspa;)Z");
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_175656_a", global::is_badlion ? "(Lcj;Lalz;)Z" : "(Lnet/minecraft/util/BlockPos;Lnet/minecraft/block/state/IBlockState;)Z");
		env->DeleteLocalRef(this_class);

		env->CallVoidMethod(world, mid, bp, IBlockState);
	}

	bool is_diamond(jobject block)
	{
		static jclass c = getObject(env, global::is_badlion ? "afg" : "net.minecraft.block.BlockOre");
		jboolean b = env->IsInstanceOf(block, c);
		//env->DeleteLocalRef(c);

		return b;
	}
	
	bool is_bed(jobject block)
	{
		static jclass c = getObject(env, global::is_badlion ? "afg" : "net.minecraft.block.BlockBed");
		jboolean b = env->IsInstanceOf(block, c);
		//env->DeleteLocalRef(c);

		return b;
	}

	void remove_entity(const std::shared_ptr<c_player>& player)
	{
		static jmethodID mid = nullptr;

		if (static bool once = false; !false)
		{
			if (global::is_lunar)
			{
				mid = env->GetMethodID(mcc::World, "epppapsessepheapphseapphe", "(Lnet/minecraft/v1_8/espphaeeepaspshsppspasehe;)V");

				once = true;
			}
			else
			{
				auto this_class = env->GetObjectClass(world);
				mid = env->GetMethodID(this_class, global::is_badlion ? "e" : "func_72900_e", global::is_badlion ? "(Lpk;)V" : "(Lnet/minecraft/entity/Entity;)V");

				env->DeleteLocalRef(this_class);
				
				once = true;
			}
		}

		if(mid != nullptr)
			env->CallVoidMethod(world, mid, player->get_object());
		
	}

	bool is_air_block(jobject block) // @todo - auto find this method
	{
		//30000000 
	
		jmethodID mid;
		
		if (global::is_lunar) {
			//mid = env->GetMethodID(mcc::World, "phphhahahhhpphhpeeehspaah", "(Lnet/minecraft/v1_8/paaahssaehhehsshassaahheh;)Z");
			//const auto& _method_ = mcc::impl::World::methods[94];
			mid = env->GetMethodID(mcc::World, "hhhehsaaasshpepphppsashee", "(Lnet/minecraft/v1_8/aesepsshepasspseahppsseep;)Z");
		}
		else {
			auto this_class = env->GetObjectClass(world);
			mid = env->GetMethodID(this_class, global::is_badlion ? "d" : "func_175623_d", global::is_badlion ? "(Lcj;)Z" : "(Lnet/minecraft/util/BlockPos;)Z");

			env->DeleteLocalRef(this_class);
		}

		
		return env->CallBooleanMethod(world, mid, block);
	}

	void break_block(jobject bp, jobject block, jobject state)
	{
		auto this_class = env->GetObjectClass(block);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "b" : "func_180663_b", global::is_badlion ? "(Ladm;Lcj;Lalz;)V" : "(Lnet/minecraft/world/World;Lnet/minecraft/util/BlockPos;Lnet/minecraft/block/state/IBlockState;)V");

		env->CallVoidMethod(block, mid, world, bp, state);
		env->DeleteLocalRef(this_class);
	}

	std::shared_ptr<c_tesellator> get_tessellator_instance()
	{
		jclass c = getObject(env, global::is_badlion ? "bfx" : "net.minecraft.client.renderer.Tessellator");
		
		jfieldID fid = env->GetFieldID(c, global::is_badlion ? "c" : "field_78398_a", global::is_badlion ? "Lbfx;" : "Lnet/minecraft/client/renderer/Tessellator;");


		auto ret = std::make_shared<c_tesellator>(env->GetObjectField(c, fid));
		env->DeleteLocalRef(c);
		return ret;
	}

	jobject get_object()
	{
		return world;
	}

	bool checkBlockCollision(jobject boundingbox)
	{
		auto this_class = env->GetObjectClass(world);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "c" : "func_72829_c", global::is_badlion ? "(Laug;)Z" : "(Lnet/minecraft/util/AxisAlignedBB;)Z");

		return env->CallBooleanMethod(this_class, mid, boundingBox);
	}
	size_t getCollidingBoundingBoxes(jobject player, jobject boundingbox)
	{
		auto this_class = env->GetObjectClass(world);
		jmethodID mid = nullptr;
		
		if (global::is_lunar) {
			//48: epsehehpaeeaesshpeheppsap | sig: (Lnet/minecraft/v1_8/haseshssssaahpaaasspphpee;Lnet/minecraft/v1_8/aahapsaahsehhaeaesaeahsah;)Ljava/util/List;
			mid = env->GetMethodID(this_class, "eapheaspsphsepsasehpshpes", "(Lnet/minecraft/v1_8/espphaeeepaspshsppspasehe;Lnet/minecraft/v1_8/hpspaspaasspaaapheppseaps;)Ljava/util/List;");
		}
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_72945_a", global::is_badlion ? "(Lpk;Laug;)Ljava/util/List;" : "(Lnet/minecraft/entity/Entity;Lnet/minecraft/util/AxisAlignedBB;)Ljava/util/List;");

		if (mid == nullptr)
			return 0;

		jclass list_cls = env->FindClass("java/util/List");
		jmethodID to_array_md = env->GetMethodID(list_cls, "toArray", "()[Ljava/lang/Object;");

		env->DeleteLocalRef(list_cls);
		env->DeleteLocalRef(this_class);

		std::vector<jobject> res;

		jobject obj_player_entities = env->CallObjectMethod(world, mid, player, boundingbox);

		auto array_objects = reinterpret_cast<jobjectArray>(env->CallObjectMethod(obj_player_entities, to_array_md));

		size_t len = env->GetArrayLength(array_objects);
		
		//for (int i = 0; i < len; ++i)
		//{
		//	jobject bb = env->GetObjectArrayElement(array_objects, i);
		//	res.push_back(bb);
		//}

		env->DeleteLocalRef(array_objects);
		env->DeleteLocalRef(obj_player_entities);

		return len;
	}

	//c_axisalignedbb init_boundingbox()
	//{
	//	auto this_clazz = getObject();

	//	auto minX_fid = env->GetFieldID(this_clazz, "field_72340_a", "D"),
	//		minY_fid = env->GetFieldID(this_clazz, "field_72338_b", "D"),
	//		minZ_fid = env->GetFieldID(this_clazz, "field_72339_c", "D");

	//	auto maxX_fid = env->GetFieldID(this_clazz, "field_72336_d", "D"),
	//		maxY_fid = env->GetFieldID(this_clazz, "field_72337_e", "D"),
	//		maxZ_fid = env->GetFieldID(this_clazz, "field_72334_f", "D");

	//	env->DeleteLocalRef(this_clazz);

	//	s_axisalignedbb bb{};
	//	bb.minX = env->GetDoubleField(axis_aligned_obj, minX_fid);
	//	bb.minY = env->GetDoubleField(axis_aligned_obj, minY_fid);
	//	bb.minZ = env->GetDoubleField(axis_aligned_obj, minZ_fid);

	//	bb.maxX = env->GetDoubleField(axis_aligned_obj, maxX_fid);
	//	bb.maxY = env->GetDoubleField(axis_aligned_obj, maxY_fid);
	//	bb.maxZ = env->GetDoubleField(axis_aligned_obj, maxZ_fid);

	//	return bb;
	//}

};