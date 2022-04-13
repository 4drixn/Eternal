#pragma once
#include <jni.h>

#include "Classes.hpp"

class c_keybinding {
private:
	jobject keybinding_obj;
	JNIEnv* env;
public:

	c_keybinding(jobject obj)
	{
		keybinding_obj = obj;
		env = global::jenv;
	}

	void set_pressed(bool state)
	{
		
		jclass this_class = env->GetObjectClass(keybinding_obj);
		jfieldID fid;

		if (global::is_lunar) {
			static auto field = mcc::impl::KeyBinding::fields[7];
			fid = env->GetFieldID(this_class, field.name, field.sig);
		}
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "h" : "field_74513_e", "Z");

		env->DeleteLocalRef(this_class);

		env->SetBooleanField(keybinding_obj, fid, state);
	}
	bool get_pressed()
	{
		
		jclass this_class = env->GetObjectClass(keybinding_obj);
		jfieldID fid;
		
		if (global::is_lunar) {
			static auto field = mcc::impl::KeyBinding::fields[7];
			fid = env->GetFieldID(this_class, field.name, field.sig);
		}
		else
			fid = env->GetFieldID(this_class, global::is_badlion ? "h" : "field_74513_e", "Z");

		env->DeleteLocalRef(this_class);

		return env->GetBooleanField(keybinding_obj, fid);
	}

	jobject get_object()
	{
		return keybinding_obj;
	}
};
