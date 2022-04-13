#pragma once
#include "../mc_class/mc_class.hpp"
#include <jni.h>
#include "../../utils/utilities.hpp"

class EntityLivingBase : public mc_class
{
public:
	const char* classname() { return "EntityLivingBase"; }

	EntityLivingBase(jclass cls) : mc_class(cls)
	{

	}
};
