#pragma once
#include <vector>
#include <utility>

struct search_options_t
{
	std::vector<std::pair<const char*, int>> methods {};
	std::vector<std::pair<const char*, int>> fields {};


	search_options_t(std::vector<std::pair<const char*, int>> _methods_, std::vector<std::pair<const char*, int>> _fields_)
	{
		this->methods = _methods_;
		this->fields = _fields_;
	}

	search_options_t()
	{

	}
};

//struct method_search_options_t
//{
//
//};