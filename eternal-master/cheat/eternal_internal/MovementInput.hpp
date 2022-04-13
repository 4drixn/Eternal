#include <jni.h>
#include "global.hpp"


class c_movementinput {
private: 
	jobject movement_obj;
	JNIEnv* env;

public:
	c_movementinput(jobject obj)
	{
		movement_obj = obj;
		env = global::jenv;
	}

	float get_move_forward()
	{
		auto this_class = env->GetObjectClass(movement_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "b" : "field_78900_b", "F");
		env->DeleteLocalRef(this_class);

		return env->GetFloatField(movement_obj, fid);
	}
	float get_move_strafe()
	{
		auto this_class = env->GetObjectClass(movement_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_78902_a", "F");
		env->DeleteLocalRef(this_class);

		return env->GetFloatField(movement_obj, fid);
	}
	void set_move_forward(float val)
	{
		auto this_class = env->GetObjectClass(movement_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "b" : "field_78900_b", "F");
		env->DeleteLocalRef(this_class);

		env->SetFloatField(movement_obj, fid, val);
	}
	void set_move_strafe(float val) 
	{
		auto this_class = env->GetObjectClass(movement_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_78902_a", "F");
		env->DeleteLocalRef(this_class);

		env->SetFloatField(movement_obj, fid, val);
	}

	jobject get_object()
	{
		return movement_obj;
	}
};