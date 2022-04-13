#pragma once
#include <jni.h>

#include "functions.hpp"

class c_networkplayerinfo
{
	private:
		jobject the_obj;
		JNIEnv* env;
	public:
		c_networkplayerinfo(jobject obj)
		{
			the_obj = obj;
			env = global::jenv;
		}

		jobject get_location_skin()
		{
			if (the_obj != nullptr) {
				static auto this_class = env->GetObjectClass(the_obj);
				//jfieldID fid = env->GetFieldID(this_class, "", "locationSkin");
				jmethodID mid;
				if (global::is_lunar)
				{
					//original$getLocationSkin
					mid = env->GetMethodID(this_class, "original$getLocationSkin", "()Lnet/minecraft/v1_8/esaesahpaeepspephpaepaaea;");
				}
				else
				{
					mid = env->GetMethodID(this_class, global::is_badlion ? "g" : "func_178837_g", global::is_badlion ? "()Ljy;" : "()Lnet/minecraft/util/ResourceLocation;");
				}
				
				if (mid != nullptr) {
					return env->CallObjectMethod(the_obj, mid);
				}
			}
			
			return nullptr;
		}
};
