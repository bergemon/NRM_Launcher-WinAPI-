#include "clear_cache.h"

// This function can throw en exception!
void clear_cache()
{
	namespace fs = std::filesystem;

	char* docs_path = (char*)malloc(MAX_PATH);

	if (!docs_path)
	{
		throw std::exception("Can not allocate memory for path buffer");
	}

	docs_path[MAX_PATH - 1] = '\0';

	HRESULT res = SHGetFolderPathA(
		NULL,
		CSIDL_PERSONAL,
		NULL,
		SHGFP_TYPE_CURRENT,
		docs_path
	);

	if (res != S_OK)
	{
		throw std::exception("Can not get current user documents folder");
	}

	std::string mapFolderPath = docs_path;
	std::string flagsFolderPath = docs_path;
	free(docs_path);

	mapFolderPath.append("\\Paradox Interactive\\Victoria II\\map");
	flagsFolderPath.append("\\Paradox Interactive\\Victoria II\\gfx");

	fs::path map_cache_path(mapFolderPath);
	fs::path flags_cache_path(flagsFolderPath);

	if (fs::exists(map_cache_path) || fs::exists(flags_cache_path))
	{
		bool delete_res = fs::remove_all(map_cache_path);
		
		if (fs::exists(map_cache_path) && !delete_res)
		{
			throw std::exception("Can not delete map cache directory");
		}

		delete_res = fs::remove_all(flags_cache_path);

		if (fs::exists(flags_cache_path) && !delete_res)
		{
			throw std::exception("Can not delete flags cache directory");
		}
	}
}