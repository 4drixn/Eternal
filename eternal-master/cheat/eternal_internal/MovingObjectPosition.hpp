#pragma once
#include "global.hpp"
#include <iostream>
#include "functions.hpp"

class c_movingobjectposition {
private:
	jobject movingobject_obj;
	JNIEnv* env;
public:
	c_movingobjectposition(jobject obj)
	{
		movingobject_obj = obj;
		env = global::jenv;
	}

	jobject getEntityHit()
	{
		auto this_class = env->GetObjectClass(movingobject_obj);

		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "d" : "field_72308_g", global::is_badlion ? "Lpk;" : "Lnet/minecraft/entity/Entity;");

		jobject jo = env->GetObjectField(movingobject_obj, fid);

		env->DeleteLocalRef(this_class);

		return jo;
	}
	int get_typeOfHit()
	{
		auto this_class = env->GetObjectClass(movingobject_obj);

		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "a" : "field_72313_a", global::is_badlion ? "Lauh$a;" : "Lnet/minecraft/util/MovingObjectPosition$MovingObjectType;");
		
		jobject jo =  env->GetObjectField(movingobject_obj, fid);

		env->DeleteLocalRef(this_class);
		//jclass c = getObject(env, "net.minecraft.util.MovingObjectPosition$MovingObjectType");

		//jmethodID triggerMethod = env->GetStaticMethodID(env->GetObjectClass(jo), "ordinal", "(Ljava/lang/String;)Lnet/minecraft/util/MovingObjectPosition$MovingObjectType;");
		//if (!triggerMethod)
		//{
		//	return 0;
		//}
		
		//jstring jstr = env->NewStringUTF("BLOCK");
		//jobject jo2 = env->CallStaticObjectMethod(c, triggerMethod, jstr);
	
		//if (jo == jo2)
		//{
		//	std::cout << "SAME\n";
		//}


		return 1;
	}

	jobject get_block_pos()
	{
		auto this_class = env->GetObjectClass(movingobject_obj);
		jfieldID fid = env->GetFieldID(this_class, global::is_badlion ? "e" : "field_178783_e", global::is_badlion ? "Lcj;" : "Lnet/minecraft/util/BlockPos;");
		env->DeleteLocalRef(this_class);

		return env->GetObjectField(movingobject_obj, fid);
	}
};