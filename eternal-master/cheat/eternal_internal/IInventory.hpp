#pragma once

#include "functions.hpp"
#include "ItemStack.hpp"

class c_IInventory {
private:
	jobject inventory_obj;
	JNIEnv* env;
public:
	c_IInventory(jobject obj)
	{
		inventory_obj = obj;
		env = global::jenv;
	}

	jobject get_object()
	{
		return inventory_obj;
	}

	int getSizeInventory()
	{
		auto this_class = env->GetObjectClass(inventory_obj);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "o_" : "func_70302_i_", "()I");
		env->DeleteLocalRef(this_class);

		return env->CallIntMethod(inventory_obj, mid);
	}

	std::shared_ptr<c_itemstack> getStackInSlot(int slot)
	{
		auto this_class = env->GetObjectClass(inventory_obj);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_70301_a", global::is_badlion ? "(I)Lzx;" : "(I)Lnet/minecraft/item/ItemStack;");
		env->DeleteLocalRef(this_class);

		return std::make_shared<c_itemstack>(env->CallObjectMethod(inventory_obj, mid, slot));
	}

	jobject* getMainInventory()
	{
		auto this_class = env->GetObjectClass(inventory_obj);

		jobject ItemStacks[36];

		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_70462_a", global::is_badlion ? "[Lzx;" : "[Lnet/minecraft/item/ItemStack;");
		jobjectArray paramArray = (jobjectArray)env->GetObjectField(inventory_obj, fid);

		for (int i = 0; i < 36; i++)
			ItemStacks[i] = ((jobject)env->GetObjectArrayElement(paramArray, i));

		env->DeleteLocalRef(paramArray);

		return ItemStacks;
	}

	int get_current_item()
	{
		auto this_class = env->GetObjectClass(inventory_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "c" : "field_70461_c", "I");
		env->DeleteLocalRef(this_class);

		return env->GetIntField(inventory_obj, fid);
	}

	void set_current_item(int item_slot)
	{
		auto this_class = env->GetObjectClass(inventory_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "c" : "field_70461_c", "I");
		env->DeleteLocalRef(this_class);

		return env->SetIntField(inventory_obj, fid, item_slot);
	}
};