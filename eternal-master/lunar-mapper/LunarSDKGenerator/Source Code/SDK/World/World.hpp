#pragma once
#include "../mc_class/mc_class.hpp"
#include <jni.h>

class World : public mc_class
{
public:
	const char* classname() { return "World"; }

	World(jclass cls) : mc_class(cls)
	{

	}
};
