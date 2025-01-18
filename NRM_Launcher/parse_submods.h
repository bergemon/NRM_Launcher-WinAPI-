#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <list>
#include <vector>

//====================================================================
struct PARSED_SUBMOD
{
	std::string submod_name;
	std::string submod_path;
};
//====================================================================
void parse_submods() throw (std::exception);
//====================================================================