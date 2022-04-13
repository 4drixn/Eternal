#pragma once
#include <jni.h>
#include <iostream>
#include "global.hpp"
#include "functions.hpp"
	
class c_block {
private:
	jobject block_obj;
	JNIEnv* env;
public:

	c_block(jobject bS)
	{
		env = global::jenv;
		block_obj = bS;
	}
	~c_block()
	{
		env->DeleteLocalRef(block_obj);
	}

	bool is_air()
	{
		jclass c = getObject(env, global::is_badlion ? "aey" : "net.minecraft.block.BlockAir");
		jboolean b = env->IsInstanceOf(block_obj, c);
		env->DeleteLocalRef(c);

		return b;
	}

	jobject get_object()
	{
		return block_obj;
	}
};