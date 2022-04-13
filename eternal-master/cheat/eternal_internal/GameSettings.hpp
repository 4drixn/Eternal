#pragma once
#include <jni.h>
#include "global.hpp"
#include "KeyBinding.hpp"

class c_gamesettings {
private:
	jobject gamesettings_obj;
	JNIEnv* env;

public:
	c_gamesettings(jobject obj)
	{
		gamesettings_obj = obj;
		env = global::jenv;
	}

	float get_sens()
	{

		jclass this_class = env->GetObjectClass(gamesettings_obj);
		jfieldID fid;
		
		if (global::is_lunar) {
			static auto field = mcc::impl::GameSettings::fields[11];
			fid = env->GetFieldID(this_class, field.name, "F");
		}
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_74341_c", "F");
		
		env->DeleteLocalRef(this_class);

		return env->GetFloatField(gamesettings_obj, fid);
	}

	std::shared_ptr<c_keybinding> keybindSneak()
	{
		jclass this_class = env->GetObjectClass(gamesettings_obj);
		jfieldID fid;
		
		if (global::is_lunar) {
			static auto field = mcc::impl::GameSettings::fields[68];
			fid = env->GetFieldID(this_class, field.name, field.sig);
		}
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "ad" : "field_74311_E", global::is_badlion ? "Lavb;" : "Lnet/minecraft/client/settings/KeyBinding;");
	
		env->DeleteLocalRef(this_class);

		return std::make_shared<c_keybinding>(env->GetObjectField(gamesettings_obj, fid));
	}
	jobject get_object()
	{
		return gamesettings_obj;
	}
};