#pragma once

#include <vector>
#include <jni.h>
#include <string>
#include <memory>

#include "clazz_t.hpp"
#include "field_t.hpp"
#include "method_t.hpp"
#include "search_options_t.hpp"
#include "../jvm/jvm.hpp"

class Mapper {

public:
	jclass* classes;
	jint classCount;

	Mapper()
	{
		cheat_main::get().jvmti->GetLoadedClasses(&this->classCount, &this->classes);
	}


	jclass findClass(int method_count, int field_count)
	{
		int classes_found = 0;
		jclass _clazz_ = nullptr;
		
		for (int i = 0; i < this->classCount; i++) {

			jobject clazz = this->classes[i];
			std::string cls_name = jvmutil::getClassName((jclass)clazz);

			if (cls_name.find("net.minecraft") == std::string::npos)
				continue;
		
			jint m_count;
			jint f_count;
			jmethodID* methods;
			jfieldID* fields;

			cheat_main::get().jvmti->GetClassMethods((jclass)clazz, &m_count, &methods);
			cheat_main::get().jvmti->GetClassFields((jclass)clazz, &f_count, &fields);

			if (method_count == m_count && field_count == f_count)
			{
				classes_found++;
				_clazz_ = (jclass)clazz;
			}

		}
		
		if(classes_found == 1)
		{
			//printf("Found %d class\n", classes_found);

			return _clazz_;
		}
		else
		{
			//printf("Found %d classes\n", classes_found);

		}
		
		return nullptr;
	}

	jclass findClass(int method_count, int field_count, bool $_ignore, search_options_t options = search_options_t{})
	{
		std::vector<clazz_t> clazz_list;
		
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

			cheat_main::get().jvmti->GetClassMethods((jclass)clazz, &m_count, &methods);
			cheat_main::get().jvmti->GetClassFields((jclass)clazz, &f_count, &fields);

			if (method_count == m_count && field_count == f_count)
			{
				clazz_list.push_back({ (jclass)clazz, fields, methods, f_count, m_count });
			}
		}

		if (clazz_list.empty())
			return nullptr;
		
		if (clazz_list.size() == 1)
		{
			printf("Found %d class\n", clazz_list.size());

			return clazz_list[0].clazz;
		}
		else
		{
			printf("Found %d classes\n", clazz_list.size());
			
			if (options.methods.empty() && options.fields.empty())
				return nullptr;


			for ( const auto& clazz : clazz_list)
			{
				std::vector<std::string> fields{};
				std::vector<std::string> methods_{};
				
				for (int i = 0; i < clazz.field_count; i++)
				{
					auto field = clazz.fields[i];

					char* field_name;
					char* signature;

					cheat_main::get().jvmti->GetFieldName(clazz.clazz, field, &field_name, &signature, nullptr);

					//jint accessFlags;
					//cheat_main::get().jvmti->GetMethodModifiers(method, &accessFlags);

					field_t _field_{};

					//if (accessFlags & 0x0008)
					//	_method_.is_static = true;
					//else
					//	_method_.is_static = false;

					_field_.name = field_name;
					_field_.sig = signature;

					fields.push_back(signature);


					bool gud = false;
					for (auto const& field_opt : options.fields)
					{
						int res = count(fields.begin(), fields.end(), field_opt.first);
						
						if (res == field_opt.second)
							gud = true;
					}
					if (!gud)
						continue;

					//bool found[10]{ false, false, false, false, false, false, false, false, false };
					//bool gud = true;
					//for (int i = 0; i++ < options.fields.size(); i++)
					//{
					//	auto const& field_opt = options.fields[i];
					//	int res = count(fields.begin(), fields.end(), field_opt.first);


					//	if (res != field_opt.second)
					//		found[i] = true;
					//	//gud = false;
					//}
					//for (int i = 0; i++ < options.fields.size(); i++)
					//	if (found[i] != true)
					//		gud = false;

					//if (!gud)
					//	continue;
					//
					return clazz.clazz;
				}


				if (clazz.methods != nullptr) {
					for (int i = 0; i < clazz.method_count; i++)
					{
						auto method = clazz.methods[i];

						char* method_name;
						char* signature;

						cheat_main::get().jvmti->GetMethodName(method, &method_name, &signature, nullptr);
						method_t _method_{};

						_method_.name = method_name;
						_method_.sig = signature;

						methods_.push_back(signature);

						bool gud = true;
						for (auto const& method_opt : options.methods)
						{
							int res = std::count(methods_.begin(), methods_.end(), method_opt.first);

							if (res != method_opt.second)
								gud = false;
						}
						if (!gud)
							continue;

						return clazz.clazz;
					}
				}
				
					
			}
		}


	}

	jclass findClass(const char* to_find)
	{
		for (int i = 0; i < classCount; i++) {

			jobject clazz = classes[i];

			jclass cls = cheat_main::get().env->FindClass("java/lang/Class");
			jmethodID mid_getName = cheat_main::get().env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
			jstring jname = (jstring)cheat_main::get().env->CallObjectMethod(clazz, mid_getName);

			const char* name = cheat_main::get().env->GetStringUTFChars(jname, 0);
			cheat_main::get().env->ReleaseStringUTFChars(jname, name);
			cheat_main::get().env->DeleteLocalRef(jname);
			cheat_main::get().env->DeleteLocalRef(cls);

			std::string cls_name = name;

			if(cls_name == to_find)
			{
				return (jclass)clazz;
			}

		}

		return nullptr;
	}
};
