#pragma once
#include <jni.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <ostream>
#include <fstream>
#include "../../jvm/jvm.hpp"

class mc_class {

public:
	jclass clazz;
	std::vector<method_t> methods;
	std::vector<field_t> fields;

	virtual const char* classname() { return "Base"; }
	
	mc_class(jclass cls)
		: clazz{ cls }
	{
		if (cls != nullptr)
		{
			jint method_count;
			jint field_count;
			jmethodID* methods;
			jfieldID* fields;

			cheat_main::get().jvmti->GetClassMethods(clazz, &method_count, &methods);
			cheat_main::get().jvmti->GetClassFields(clazz, &field_count, &fields);

			for (int i = 0; i < field_count; i++)
			{
				auto field = fields[i];

				char* field_name;
				char* signature;

				cheat_main::get().jvmti->GetFieldName(this->clazz, field, &field_name, &signature, nullptr);

				jint accessFlags;
				cheat_main::get().jvmti->GetFieldModifiers(this->clazz, field, &accessFlags);

				field_t _field_{};

				if (accessFlags & 0x0008)
					_field_.is_static = true;
				else
					_field_.is_static = false;

				_field_.name = field_name;
				_field_.sig = signature;

				this->fields.push_back(_field_);

			}
			for (int i = 0; i < method_count; i++)
			{
				auto method = methods[i];

				char* method_name;
				char* signature;

				cheat_main::get().jvmti->GetMethodName(method, &method_name, &signature, nullptr);

				jint accessFlags;
				cheat_main::get().jvmti->GetMethodModifiers(method, &accessFlags);

				method_t _method_ {};

				if (accessFlags & 0x0008)
					_method_.is_static = true;
				else
					_method_.is_static = false;

				_method_.name = method_name;
				_method_.sig = signature;

				this->methods.push_back(_method_);

			}
		}
	}

	
	void dump_class(const std::vector<std::pair<const char*, method_t>>& methods = std::vector<std::pair<const char*, method_t>>(), const std::vector<std::pair<const char*, field_t>>& fields = std::vector<std::pair<const char*, field_t>>())
	{
		namespace fs = std::filesystem;
		
		auto p = fs::path("C:\\");	
		auto file = p.append("SDKGenerator\\");

		std::string class_name = jvmutil::getClassName(this->clazz);
		
		auto path = file.string();
		path += classname();
		path += ".txt";

		std::ofstream ofstream(path, std::ios_base::app);

		if (!ofstream.is_open())
			printf("Unable to open path \"%s\"?\n", path.c_str());

		ofstream << "Class:" << class_name << "\n\n";
		
		for(auto const& method : methods)
		{
			ofstream << method.first << ":" << method.second.name << ":" << method.second.sig << std::endl;
		}

		ofstream.close();
	}

};