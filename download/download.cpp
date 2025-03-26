#include <ws2tcpip.h>
#include <windows.h>
#include <filesystem>
#include <string>
#include <fstream>
#include "openssl/ssl.h"
#include "openssl/conf.h"
#include "openssl/err.h"
#include "download.h"
#include "net_handler.h"
#include "zip.h"

//====================================================================
/* Has signature to invoke from newly created thread,
	need to put pointer to GAME_VERSION structure */
extern "C" DWORD WINAPI check_version(_In_ LPVOID lpParameter)
{
	GAME_VERSION* const ver = (GAME_VERSION*)lpParameter;

	try
	{
		NET_HANDLER inet(ver->file_id, ver->host);

		ver->compare_result = inet.set_info_to_handler(ver)
			.send_message(MESSAGE_GET_VERSION)
			.recieve_message()
			.ver_parse_n_compare();

		ver->is_ended = 1;
	}
	catch (NET_STATUS_CODES code)
	{
		ver->status = 0;
		ver->status_code = code;
		return 1;
	}

	return 0;
}
//====================================================================
extern "C" DWORD WINAPI download_new_version(_In_ LPVOID lpParameter)
{
	GAME_VERSION* const ver = (GAME_VERSION*)lpParameter;

	NET_HANDLER inet(ver->new_file_id, ver->host);

	try
	{
		if (ver->has_custom_zip_name)
		{
			inet.set_custom_zip_path(ver->zip_file_name);
		}

		inet.set_info_to_handler(ver)
			.send_message(MESSAGE_TRY_GET_FILE)
			.recieve_message()
			.send_message(MESSAGE_GET_FILE)
			.recieve_message();

		ver->status = 0;
		ver->status_code = NET_STAT_OK;
	}
	catch (NET_STATUS_CODES code)
	{
		ver->status = 0;
		ver->status_code = code;
		return 1;
	}

	return 0;
}
//====================================================================
/* Need to put ZIP_INFO struct pointer into procedure,
	can be invoked by newly created thread */
extern "C" DWORD WINAPI get_zip_info(_In_ LPVOID lpParameter)
{
	ZIP_INFO* info = (ZIP_INFO*)lpParameter;

	// Open ZIP archive
	int32_t err;
	zip_t* zip = zip_open(info->path_to_zip, 0, &err);
	if (!zip)
	{
		/*zip_error_t error;
		zip_error_init_with_code(&error, err);
		zip_error_strerror(&error);*/
		info->status = 0;
		info->status_code = STATUS_CANT_OPEN_ARCHIVE;
		return 1;
	}

	// Get number of files in archive
	info->num_of_entries = zip_get_num_entries(zip, 0);

	return 0;
}
//====================================================================
/* Need to put UNZIP_INFO struct pointer into procedure, can be invoked
	by multiple threads, but need to do it manually, by providing
	start and end id of entries in UNZIP_INFO struct to handle by ziplib */
extern "C" DWORD WINAPI unzip_file(_In_ LPVOID lpParameter)
{
	UNZIP_INFO* info = (UNZIP_INFO*)lpParameter;

	// Open ZIP archive
	int32_t err;
	zip_t* zip = zip_open(info->src_path, 0, &err);
	if (!zip)
	{
		info->status = 0;
		info->status_code = STATUS_CANT_OPEN_ARCHIVE;
		ExitThread(1);
	}

	/*EnterCriticalSection(info->critical_section);
	// Create directory
	namespace fs = std::filesystem;
	fs::path fs_output_path(info->out_path);
	if (!fs::exists(fs_output_path))
	{
		fs::create_directory(fs_output_path);
	}
	else
	{
		fs::remove_all(fs_output_path);
	}
	LeaveCriticalSection(info->critical_section);*/

	// Allocate memory for buffer
	uint32_t buffer_size = 1'000'000;
	char* buffer = new char[buffer_size];

	// Read files by indexes
	for (zip_int64_t i = info->start_index; i < info->end_index; ++i)
	{
		info->progress = 
			((i - info->start_index) / (long double)(info->end_index - info->start_index)) * 100;

		// Getting file info
		zip_stat_t file_info;
		if (zip_stat_index(zip, i, 0, &file_info) != 0)
		{
			zip_close(zip);
			info->status = 0;
			info->status_code = STATUS_UNKNOWN_ERROR;
			delete[] buffer;
			ExitThread(1);
		}

		// Full name of the file (including path)
		const char* file_name = file_info.name;

		// Creating full path to decompress file
		/*std::string full_path;
		full_path.append(info->out_path).append("\\").append(file_name);
		if (full_path.length() > MAX_PATH)
		{
			info->status = 0;
			info->status_code = STATUS_PATH_TOO_LONG;
			zip_close(zip);
			ExitThread(1);
		}*/

		// If it is a directory - create it
		if (file_name[strlen(file_name) - 1] == '/')
		{
			namespace fs = std::filesystem;
			fs::path fs_output_path(file_name);
			if (!fs::exists(fs_output_path))
			{
				EnterCriticalSection(info->critical_section);
				fs::create_directory(fs_output_path);
				LeaveCriticalSection(info->critical_section);
			}
			continue;
		}

		// Open file in archive
		zip_file_t* file = zip_fopen_index(zip, i, NULL);
		if (!file)
		{
			info->status = 0;
			info->status_code = STATUS_CANT_OPEN_ARCHIVE_FILE;
			zip_close(zip);
			delete[] buffer;
			ExitThread(1);
		}

		// Create directory if it is not exist yes
		std::string fstr = file_name;

		std::vector<std::string> paths;
		while (fstr.find("/") != std::string::npos)
		{
			fstr = fstr.substr(0, fstr.rfind("/"));
			paths.push_back(fstr);
		}
		EnterCriticalSection(info->critical_section);
		namespace fs = std::filesystem;
		fs::create_directories(fstr = fstr.substr(0, fstr.rfind("\\")));
		for (auto iter = paths.rbegin(); iter != paths.rend(); ++iter)
		{
			if (!fs::exists(iter->c_str()))
			{
				fs::create_directory(iter->c_str());
			}
		}
		LeaveCriticalSection(info->critical_section);

		// Create file on disk
		HANDLE hFile = CreateFileA(
			file_name,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			DWORD error = GetLastError();
			info->status = 0;
			info->status_code = STATUS_CANT_OPEN_OUTFILE;
			zip_fclose(file);
			zip_close(zip);
			delete[] buffer;
			ExitThread(1);
		}

		// Reading file from archive and write data to disk
		zip_int64_t bytes_read;
		int32_t bytes_written;
		do
		{
			bytes_read = zip_fread(file, buffer, buffer_size);

			if (bytes_read > 0)
			{
				WriteFile(hFile, buffer, bytes_read, (LPDWORD)&bytes_written, NULL);
			}

		} while (bytes_read > 0);

		// Close output file
		CloseHandle(hFile);

		zip_fclose(file);
	}
	delete[] buffer;

	// Close archive
	zip_close(zip);
	info->status = 0;
	info->status_code = STATUS_OK;

	return 0;
}
//====================================================================