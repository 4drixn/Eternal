#pragma once
#include <jni.h>
#include <vector>

struct clazz_t
{
	jclass clazz;
	
	jfieldID* fields;
	jmethodID* methods;

	int field_count;
	int method_count;
};
