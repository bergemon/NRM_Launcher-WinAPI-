#include "remove_all_thread.h"

DWORD WINAPI create_out_path(_In_ LPVOID lpParameter)
{
	CREATE_OUT_PATH_INFO* info = (CREATE_OUT_PATH_INFO*)lpParameter;

	// Create directory
	namespace fs = std::filesystem;
	fs::path fs_output_path(info->output_path);
	if (!fs::exists(fs_output_path))
	{
		fs::create_directory(fs_output_path);
	}
	else
	{
		fs::remove_all(fs_output_path);
		fs::create_directory(fs_output_path);
	}

	info->is_ended = true;
}