#pragma once
#include "stdlibs.h"

//====================================================================
struct PARSED_SUBMOD
{
	std::string submod_name;
	std::string submod_path;
};
//====================================================================
void parse_submods() noexcept(false);
//====================================================================