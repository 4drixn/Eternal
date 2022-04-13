#pragma once

#include "vec3.hpp"
#include "functions.hpp"
#include "Classes.hpp"

class c_item {

private:
	jobject Item;
	JNIEnv* env;
public:
	c_item(jobject _item)
	{
		Item = _item;
		env = getJNI();//global::jenv;
	}
	~c_item()
	{
		env->DeleteLocalRef(Item);
	}

	//bridge$isItemBlock()
	
	bool is_tool() // net.minecraft.v1_8.psssespasphhhspappsepesaa
	{
		auto this_class = env->GetObjectClass(Item);
		jmethodID mid = NULL;
		
		if (global::is_lunar)
			mid = env->GetMethodID(this_class, "ahehseppehpseahessehhasps", "(Lnet/minecraft/v1_8/eahsaehhhhseeessppepahsas;)Z");
		else
			mid = env->GetMethodID(this_class, global::is_badlion ? "f_" : "func_77616_k", "(Lzx;)Z");

		env->DeleteLocalRef(this_class);

		return env->CallBooleanMethod(Item, mid, Item);
	}

	bool is_sword()
	{
		jclass c = global::is_lunar ? mcc::ItemSword : getObject(env, global::is_badlion ? "aay" : "net.minecraft.item.ItemSword");
		jboolean b = env->IsInstanceOf(Item, c);

		if(!global::is_lunar)
			env->DeleteLocalRef(c);
		return b;
	}

	bool is_axe()
	{
		jclass c = global::is_lunar ? mcc::ItemAxe : getObject(env, global::is_badlion ? "yl" : "net.minecraft.item.ItemAxe");
		jboolean b = env->IsInstanceOf(Item, c);

		if (!global::is_lunar)
			env->DeleteLocalRef(c);

		return b;
	}

	bool is_armor()
	{
		jclass c = global::is_lunar ? mcc::ItemArmor : getObject(env, global::is_badlion ? "yj" : "net.minecraft.item.ItemArmor");
		jboolean b = env->IsInstanceOf(Item, c);

		if (!global::is_lunar)
			env->DeleteLocalRef(c);
		return b;
	}
	bool is_block()
	{
		jclass c = global::is_lunar ? mcc::ItemBlock : getObject(env, global::is_badlion ? "yo" : "net.minecraft.item.ItemBlock");
		jboolean b = env->IsInstanceOf(Item, c);

		if (!global::is_lunar)
			env->DeleteLocalRef(c);
		return b;
	}
	bool is_projectile()
	{
		if (global::is_lunar)
		{
			jclass c_1 = mcc::ItemProjectile;
			jboolean b_1 = env->IsInstanceOf(Item, c_1);

			jclass c_2 = mcc::ItemProjectile2;
			jboolean b_2 = env->IsInstanceOf(Item, c_2);

			jclass c_3 = mcc::ItemProjectile3;
			jboolean b_3 = env->IsInstanceOf(Item, c_3);

			if (b_1 || b_2 || b_3)
				return true;

			return false;
		}
		else {
			jclass c_1 = getObject(env, global::is_badlion ? "aaw" : "net.minecraft.item.ItemSnowball");
			jboolean b_1 = env->IsInstanceOf(Item, c_1);

			jclass c_2 = getObject(env, global::is_badlion ? "zg" : "net.minecraft.item.ItemEgg");
			jboolean b_2 = env->IsInstanceOf(Item, c_2);

			env->DeleteLocalRef(c_1);
			env->DeleteLocalRef(c_2);

			if (b_1 || b_2)
				return true;

			return false;
		}
	}

	int get_damageReduceAmount()
	{
		//jclass c = getObject(env, global::is_badlion ? "yj" : "net.minecraft.item.ItemArmor");
		jclass c = env->GetObjectClass(Item);
		jfieldID fid = env->GetFieldID(c, global::is_badlion ? "c" : "field_77879_b", "I");

		int amount = env->GetIntField(Item, fid);
		env->DeleteLocalRef(c);

		return amount;
	}

	jobject get_object()
	{
		return Item;
	}

};