#pragma once
#include "../mc_class/mc_class.hpp"
#include <jni.h>

class FontRenderer : public mc_class
{
public:
	const char* classname() { return "FontRenderer"; }
	
	std::vector<std::pair<const char*, method_t>> get_methods()
	{
		std::vector<std::pair<const char*, method_t>> list;
		int index = 0;
		for (method_t method : this->methods)
		{
			std::string name = method.name;
			std::string signature = method.sig;

			if (index == 7)
			{
				if (signature == "(Ljava/lang/String;FFIZ)I")
					list.emplace_back("drawString", method);
				else
					printf("[DEBUG] drawString: Invalid \n");
			}

			index++;
		}

		return list;
	}

	FontRenderer(jclass cls) : mc_class(cls)
	{
		//this->dump_class({}, );
	}
};
