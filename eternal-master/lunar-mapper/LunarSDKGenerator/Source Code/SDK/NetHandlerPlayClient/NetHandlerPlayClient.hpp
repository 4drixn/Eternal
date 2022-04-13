#pragma once
#include "../mc_class/mc_class.hpp"
#include <jni.h>

class NetHandlerPlayClient : public mc_class
{
public:
	const char* classname() { return "NetHandlerPlayClient"; }


	NetHandlerPlayClient(jclass cls) : mc_class(cls)
	{

	}
};
