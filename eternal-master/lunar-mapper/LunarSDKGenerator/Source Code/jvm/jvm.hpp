#pragma once
#include <jni.h>
#include <jvmti.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <memory>
#include "../singleton.hpp"


class Mapper;
typedef jint(*hJNI_GetCreatedJavaVMs)(JavaVM** vmBuf, jsize bufLen, jsize* nVMs);
hJNI_GetCreatedJavaVMs oJNI_GetCreatedJavaVMs;

class cheat_main : public singleton<cheat_main>
{
private:
	std::shared_ptr<Mapper> _mapper_;

	public:
		JavaVM* jvm;
		JNIEnv* env;
		jvmtiEnv* jvmti;
	
		int init()
		{
			HMODULE jvmHandle = GetModuleHandleA("jvm.dll");
			void* func_JNI_GetCreatedJavaVMs = GetProcAddress(jvmHandle, "JNI_GetCreatedJavaVMs");
			oJNI_GetCreatedJavaVMs = (hJNI_GetCreatedJavaVMs)func_JNI_GetCreatedJavaVMs;

			JavaVMInitArgs vm_args;

			JavaVMOption options;
			vm_args.version = JNI_VERSION_1_6;

			vm_args.nOptions = 1;

			jint returnOF = oJNI_GetCreatedJavaVMs(&jvm, 1, NULL);

			int getEnvStat_JNI = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
			jvm->AttachCurrentThread((void**)&env, NULL);

			int getEnvStat = jvm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_1);

			if (getEnvStat != JNI_OK) {
				printf("Unable to access JVMTI\n");

				return 0;
			}

			jvmtiCapabilities capabilities;
			(void)memset(&capabilities, 0, sizeof(jvmtiCapabilities));

			capabilities.can_retransform_classes = 1;
			capabilities.can_generate_exception_events = 1;
			capabilities.can_access_local_variables = 1;
			capabilities.can_get_constant_pool = 1;
			capabilities.can_get_bytecodes = 1;
			capabilities.can_generate_all_class_hook_events = 1;

			jvmti->AddCapabilities(&capabilities);
			jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, (jthread)nullptr);

			_mapper_ = std::make_shared<Mapper>();
			
			return 1;
		}

		std::shared_ptr<Mapper> mapper() {
			return this->_mapper_;
		}
};






namespace jvmutil {


	__forceinline const char* getClassName(jclass c, bool transform=false)
	{
		if (c == nullptr)
			return "invalid class";
		jclass cls = cheat_main::get().env->FindClass("java/lang/Class");
		jmethodID mid_getName = cheat_main::get().env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
		jstring jname = (jstring)cheat_main::get().env->CallObjectMethod(c, mid_getName);

		const char* name = cheat_main::get().env->GetStringUTFChars(jname, 0);
		std::string mc_name = name;
		if(transform)
		{
			std::replace(mc_name.begin(), mc_name.end(), '.', '/');
			mc_name.insert(0, "L");
			mc_name += ";";

			return mc_name.c_str();
		}
		cheat_main::get().env->ReleaseStringUTFChars(jname, name);
		cheat_main::get().env->DeleteLocalRef(jname);
		cheat_main::get().env->DeleteLocalRef(cls);

		return name;
	}

	__forceinline void print_class_info(jclass c, std::string originalName, std::string obfuscatedName)
	{

		if (c != NULL)
		{
			jint m_count;
			jint f_count;
			jmethodID* methods;
			jfieldID* fields;

			cheat_main::get().jvmti->GetClassMethods(c, &m_count, &methods);
			cheat_main::get().jvmti->GetClassFields(c, &f_count, &fields);

			std::cout << originalName << ": " << obfuscatedName << " | " << jvmutil::getClassName(c) << " | valid: " << (obfuscatedName == (std::string)jvmutil::getClassName(c)) << " | " << m_count << ",  " << f_count << std::endl;
		}
		else {
			std::cout << originalName << ": " << obfuscatedName << " | valid: " << 0 << std::endl;
		}
	}

	__forceinline void dump_class(const jclass& c, std::string class_name)
	{

		jint m_count;
		jint f_count;
		jmethodID* methods;
		jfieldID* fields;

		cheat_main::get().jvmti->GetClassMethods(c, &m_count, &methods);
		cheat_main::get().jvmti->GetClassFields(c, &f_count, &fields);
		std::string real_class_name = jvmutil::getClassName(c);

		std::ostringstream stringStream;
		stringStream << class_name << ".txt";

		char buff[1000];
		snprintf(buff, sizeof(buff), "Class Name: %s\nMethod Count:%d\nField Count:%d\n\n", real_class_name.c_str(), m_count, f_count);
		std::string strr = buff;


		for (int i = 0; i < m_count; i++)
		{
			auto method = methods[i];

			char* method_name;
			char* signature;

			cheat_main::get().jvmti->GetMethodName(method, &method_name, &signature, nullptr);

			jint accessFlags;
			cheat_main::get().jvmti->GetMethodModifiers(method, &accessFlags);

			if (accessFlags & 0x0008)
			{
				char buff[1000];
				snprintf(buff, sizeof(buff), "%d: static %s | sig: %s", i, method_name, signature);
				std::string buffAsStdStr = buff;

				std::cout << buffAsStdStr << std::endl;
			}

			else {
				char buff[1000];
				snprintf(buff, sizeof(buff), "%d: %s | sig: %s", i, method_name, signature, 1);
				std::string buffAsStdStr = buff;

				std::cout << buffAsStdStr << std::endl;
			}
		}

		for (int i = 0; i < f_count; i++)
		{
			auto field = fields[i];

			char* field_name;
			char* signature;

			cheat_main::get().jvmti->GetFieldName(c, field, &field_name, &signature, nullptr);

			jint accessFlags;
			cheat_main::get().jvmti->GetFieldModifiers(c, field, &accessFlags);

			if (accessFlags & 0x0008)
			{
				char buff[1000];
				snprintf(buff, sizeof(buff), "%d: static %s | sig: %s\n", i, field_name, signature);
				std::string buffAsStdStr = buff;
				
				std::cout << buffAsStdStr << std::endl;
			}

			else {
				char buff[1000];
				snprintf(buff, sizeof(buff), "%d: %s | sig: %s", i, field_name, signature, 1);
				std::string buffAsStdStr = buff;

				std::cout << buffAsStdStr << std::endl;
			}
		}
	}

	__forceinline void print_class_info(jclass c)
	{
		jint m_count;
		jint f_count;
		jmethodID* methods;
		jfieldID* fields;

		cheat_main::get().jvmti->GetClassMethods(c, &m_count, &methods);
		cheat_main::get().jvmti->GetClassFields(c, &f_count, &fields);
		std::cout << "[dbg] " << m_count << ", " << f_count << std::endl;
	}

}
