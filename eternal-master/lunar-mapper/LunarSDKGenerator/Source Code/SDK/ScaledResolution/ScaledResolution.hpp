#pragma once
#include "../mc_class/mc_class.hpp"
#include <jni.h>

class ScaledResolution : public mc_class
{
public:
	const char* classname() { return "ScaledResolution"; }

	std::vector<std::pair<const char*, field_t>> get_fields()
	{
		std::vector<std::pair<const char*, field_t>> list;
		int index = 0;
		for (field_t field : this->fields)
		{
			std::string name = field.name;
			std::string signature = field.sig;
		
			//list.emplace_back("{index} ", field);
			
			index++;
		}

		return list;
	}

	ScaledResolution(jclass cls) : mc_class(cls)
	{
		this->dump_class({}, get_fields());
	}
};



//std::vector<std::pair<const char*, method_t>> get_methods()
//{
//	std::vector<std::pair<const char*, method_t>> list;
//	int index = 0;
//	for (method_t method : this->methods)
//	{
//		std::string name = method.name;
//		std::string signature = method.sig;

//		if(index == 3 && signature == "()I")
//			list.emplace_back("getScaledWidth", method);
//		else if (index == 5 && signature == "()I")
//			list.emplace_back("getScaledResolution", method);
//		index++;
//	}

//	return list;
//}
