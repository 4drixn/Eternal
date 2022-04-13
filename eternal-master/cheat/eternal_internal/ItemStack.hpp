#pragma once

#include "vec3.hpp"
#include "functions.hpp"
#include "Item.hpp"
#include "Block.hpp"

class c_itemstack {

private:
	jobject itemstack;
	JNIEnv* env;
public:
	c_itemstack(jobject stack)
	{
		itemstack = stack;
		env = getJNI();
	}
	~c_itemstack()
	{
		env->DeleteLocalRef(itemstack);
	}
	
	jobject get_item()
	{
		auto this_class = env->GetObjectClass(itemstack);
		jmethodID mid = nullptr;
		
		if (global::is_lunar)
		{
			// bridge$getItem
			static auto method = mapper->searchItem(mcc::impl::ItemStack::methods, "hpesesappeaeeasehhphhpaas");
			
			mid = env->GetMethodID(this_class, method.name, method.sig);
		
			if (mid == nullptr) {
				std::cout << "[X]" << method.name << ":" << method.sig << std::endl;
				return nullptr;
			}		
		}
		else {
			mid = env->GetMethodID(this_class, global::is_badlion ? "b" : "func_77973_b", global::is_badlion ? "()Lzw;" : "()Lnet/minecraft/item/Item;");
		}
	
		env->DeleteLocalRef(this_class);

		jobject obj = env->CallObjectMethod(itemstack, mid);

		return obj;
	}

	std::string get_name()
	{
		auto this_class = env->GetObjectClass(itemstack);

		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "q" : "func_82833_r", "()Ljava/lang/String;");
		jstring jstr = (jstring)env->CallObjectMethod(itemstack, mid);
		const char* strReturn = env->GetStringUTFChars(jstr, 0);
		env->ReleaseStringUTFChars(jstr, strReturn);
		env->DeleteLocalRef(jstr);
		env->DeleteLocalRef(this_class);

		return strReturn;
	}

	float getItemDamageVsBlock(std::shared_ptr<c_block>* block)
	{
		auto this_class = env->GetObjectClass(itemstack);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_150997_a", "(Lafh;)F");
		env->DeleteLocalRef(this_class);

		return env->CallFloatMethod(itemstack, mid, block->get()->get_object());
	}

	std::string getUnlocalizedName()
	{
		auto this_class = env->GetObjectClass(itemstack);
		jmethodID mid = env->GetMethodID(this_class, global::is_badlion ? "a" : "func_77977_a", "()Ljava/lang/String;");
		jstring jstr = (jstring)env->CallObjectMethod(itemstack, mid);
		const char* strReturn = env->GetStringUTFChars(jstr, 0);
		env->ReleaseStringUTFChars(jstr, strReturn);
		env->DeleteLocalRef(jstr);
		env->DeleteLocalRef(this_class);

		return strReturn;
	}

	jobject get_object()
	{
		return itemstack;
	}
};