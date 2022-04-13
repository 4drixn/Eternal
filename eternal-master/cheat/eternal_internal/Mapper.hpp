#pragma once

#include <vector>
#include <jni.h>
#include <string>
#include <memory>
#include "global.hpp"
#include "jvmutil.hpp"

#include "field_t.hpp"
#include "method_t.hpp"
using namespace global;

class Mapper {

public:
	jclass* classes;
	jint classCount;

	Mapper()
	{
		jvmti->GetLoadedClasses(&this->classCount, &this->classes);
	}


	template <typename T>
	static T searchItem(std::vector<T> list, const char* _name_)
	{
		T t{};
		for (const auto& item : list)
		{
			if ((std::string)item.name == (std::string)_name_)
				return item;
		}

		return t;
	}

	jclass findClass(int method_count, int field_count)
	{
		for (int i = 0; i < this->classCount; i++) {

			jobject clazz = this->classes[i];
			std::string cls_name = jvmutil::getClassName((jclass)clazz);

			if (cls_name.find("net.minecraft") == std::string::npos)
				continue;

			jint m_count;
			jint f_count;
			jmethodID* methods;
			jfieldID* fields;

			jvmti->GetClassMethods((jclass)clazz, &m_count, &methods);
			jvmti->GetClassFields((jclass)clazz, &f_count, &fields);

			if (method_count == m_count && field_count == f_count)
			{
				return (jclass)clazz;
			}

		}

		return nullptr;
	}

	jclass findClass(const char* to_find)
	{
		for (int i = 0; i < classCount; i++) {

			jobject clazz = classes[i];

			jclass cls = jenv->FindClass("java/lang/Class");
			jmethodID mid_getName = jenv->GetMethodID(cls, "getName", "()Ljava/lang/String;");
			jstring jname = (jstring)jenv->CallObjectMethod(clazz, mid_getName);

			const char* name = jenv->GetStringUTFChars(jname, 0);
			jenv->ReleaseStringUTFChars(jname, name);
			jenv->DeleteLocalRef(jname);
			jenv->DeleteLocalRef(cls);

			std::string cls_name = name;

			if (cls_name == to_find)
			{
				return (jclass)clazz;
			}

		}

		return nullptr;
	}

	jclass findClass(int method_count, int field_count, bool $_ignore)
	{
		for (int i = 0; i < this->classCount; i++) {

			jobject clazz = this->classes[i];
			std::string cls_name = jvmutil::getClassName((jclass)clazz);

			if (cls_name.find("net.minecraft") == std::string::npos)
				continue;

			if ($_ignore)
			{
				if (cls_name.find("$") != std::string::npos)
					continue;
			}

			jint m_count;
			jint f_count;
			jmethodID* methods;
			jfieldID* fields;

			jvmti->GetClassMethods((jclass)clazz, &m_count, &methods);
			jvmti->GetClassFields((jclass)clazz, &f_count, &fields);

			if (method_count == m_count && field_count == f_count)
			{
				return (jclass)clazz;
			}

		}

		return nullptr;

	}
};

inline std::shared_ptr<Mapper> mapper = nullptr;