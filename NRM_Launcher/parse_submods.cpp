#include "parse_submods.h"
#include "const.h"
#include "windows.h"

//====================================================================
std::list<PARSED_SUBMOD> parsed_submods;
//====================================================================
void parse_submods() throw (std::exception)
{
	namespace fs = std::filesystem;

	std::vector<fs::directory_entry> files;
	files.reserve(10);

	fs::path submods_path("game\\mod");

	if (!fs::exists(submods_path))
	{
		throw std::exception("Submods directory does not exist");
	}

	// Loop for directory iterator
	uint32_t count = 0;
	bool is_mod_file = false;
	for (const fs::directory_entry& file : fs::directory_iterator(submods_path))
	{

		if (!file.is_directory())
		{
			std::string filename = file.path().string().c_str();
			std::string file_extension = filename.substr(filename.rfind(".") + 1, filename.length() - filename.rfind(".") - 1);

			if (file_extension == "mod")
			{
				++count;
				is_mod_file = true;
			}
		}

		if (count <= MAX_SUBMODS_COUNT && is_mod_file)
		{
			files.push_back(file);
		}
		else if (count > MAX_SUBMODS_COUNT)
		{
			break;
		}

		is_mod_file = false;
	}

	// Loop to parse .mod files
	std::ifstream iFile;
	for (const auto& file : files)
	{
		std::string filepath = file.path().string().c_str();
		iFile.open(filepath, std::ios::in);

		std::string parsed_line;
		uint32_t line_count = 0;

		PARSED_SUBMOD submod;

		submod.submod_path = "mod/";
		submod.submod_path.append(file.path().filename().string().c_str());
		
		// Read file line by line
		while (std::getline(iFile, parsed_line))
		{
			++line_count;

			if (line_count > 2)
			{
				break;
			}

			if (parsed_line.find("name = ") != std::string::npos)
			{
				std::string parsed_name = parsed_line.substr(parsed_line.find("=") + 3, parsed_line.length() - parsed_line.find("=") - 3);
				parsed_name = parsed_name.substr(0, parsed_name.length() - 1);
				submod.submod_name = parsed_name.c_str();
			}
		}

		if (iFile.is_open())
		{
			iFile.close();
		}

		parsed_submods.emplace_back(submod.submod_name, submod.submod_path);
	}
}
//====================================================================