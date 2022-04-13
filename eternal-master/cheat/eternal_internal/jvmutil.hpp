#pragma once

#include "jni.h"
#include "jvmti.h"
#include "global.hpp"
#include <iostream>
#include <string>
#include <algorithm>

namespace jvmutil {
	
	//__forceinline auto getClassName(jclass c) -> const char*
	//{
	//	jclass cls = global::jenv->FindClass("java/lang/Class");
	//	jmethodID mid_getName = global::jenv->GetMethodID(cls, "getName", "()Ljava/lang/String;");
	//	jstring jname = (jstring)global::jenv->CallObjectMethod(c, mid_getName);

	//	const char* name = global::jenv->GetStringUTFChars(jname, nullptr);
	//	global::jenv->ReleaseStringUTFChars(jname, name);
	//	global::jenv->DeleteLocalRef(jname);
	//	global::jenv->DeleteLocalRef(cls);

	//	return name;
	//}

	__forceinline const char* getClassName(jclass c, bool transform = false)
	{
		if (c == nullptr)
			return "invalid class";
		jclass cls = global::jenv->FindClass("java/lang/Class");
		jmethodID mid_getName = global::jenv->GetMethodID(cls, "getName", "()Ljava/lang/String;");
		jstring jname = (jstring)global::jenv->CallObjectMethod(c, mid_getName);

		const char* name = global::jenv->GetStringUTFChars(jname, 0);
		std::string mc_name = name;
		if (transform)
		{
			std::replace(mc_name.begin(), mc_name.end(), '.', '/');
			mc_name.insert(0, "L");
			mc_name += ";";

			return mc_name.c_str();
		}
		global::jenv->ReleaseStringUTFChars(jname, name);
		global::jenv->DeleteLocalRef(jname);
		global::jenv->DeleteLocalRef(cls);

		return name;
	}
}