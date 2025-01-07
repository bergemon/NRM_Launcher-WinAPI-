#include "clear_cache.h"

// This function can throw en exception!
[[nodiscard]] bool clear_cache()
{
	namespace fs = std::filesystem;

	char* docs_path = (char*)malloc(MAX_PATH);
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
		return false;
	}

	std::string end_path = docs_path;
	free(docs_path);
	end_path.append("\\Paradox Interactive\\Victoria II\\map");

	fs::path cache_path(end_path);
	if (fs::exists(cache_path))
	{
		bool delete_res = fs::remove_all(cache_path);
		
		if (fs::exists(cache_path) || !delete_res)
		{
			throw std::exception("Can not delete cache directory");
		}
	}
}