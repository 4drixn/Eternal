#pragma once
#include "../mc_class/mc_class.hpp"
#include <jni.h>

class Minecraft : public mc_class
{
public:
	const char* classname() { return "Minecraft"; }



	Minecraft(jclass cls) : mc_class(cls)
	{

	}
};
