#pragma once

#include "Minecraft.hpp"

struct s_blockpos {
	int x, y, z;
};

class c_blockpos {
private:
	jobject blockpos_obj;
	JNIEnv* env;
public:
	c_blockpos(jobject, JNIEnv* env_);
	~c_blockpos();

	void set_blockpos(int x, int y, int z);

	jobject get_object();

};