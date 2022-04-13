#pragma once
#include "Minecraft.hpp"
#include "global.hpp"
#include "functions.hpp"

class c_guiscreen {
private:
	jobject guiscreen_obj;
	JNIEnv* env;
public:
	c_guiscreen(const jobject& obj);

	~c_guiscreen();

	bool chat_open();

	bool inventory_open();

	jobject get_object();
};