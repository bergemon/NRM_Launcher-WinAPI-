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

#ifdef __cplusplus
extern "C"
{
	//====================================================================
	/* Has signature to invoke from newly created thread,
		need to put pointer to GAME_VERSION structure */
	DWORD WINAPI check_version(_In_ LPVOID lpParameter)
	{
		GAME_VERSION* const ver = (GAME_VERSION*)lpParameter;

		NET_HANDLER inet(ver->file_id, ver->host);

		try
		{
			ver->compare_result = inet.send_message(MESSAGE_GET_VERSION)
				.recieve_message()
				.ver_parse_n_compare(*ver);

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
	DWORD WINAPI download_new_version(_In_ LPVOID lpParameter)
	{


		return 0;
	}
	//====================================================================
	/* Need to put ZIP_INFO struct pointer into procedure,
		can be invoked by newly created thread */
	DWORD WINAPI get_zip_info(_In_ LPVOID lpParameter)
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
	DWORD WINAPI unzip_file(_In_ LPVOID lpParameter)
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

		// Read files by indexes
		for (zip_int64_t i = info->start_index; i < info->end_index; ++i)
		{
			// Getting file info
			zip_stat_t file_info;
			if (zip_stat_index(zip, i, 0, &file_info) != 0)
			{
				zip_close(zip);
				ExitThread(1);
			}

			// Full name of the file (including path)
			const char* file_name = file_info.name;

			// Creating full path to decompress file
			std::string full_path;
			full_path.append(info->out_path).append("\\").append(file_name);
			if (full_path.length() > MAX_PATH)
			{
				info->status = 0;
				info->status_code = STATUS_PATH_TOO_LONG;
				zip_close(zip);
				ExitThread(1);
			}

			// If it is a directory - create it
			if (file_name[strlen(file_name) - 1] == '/')
			{
				namespace fs = std::filesystem;
				fs::path fs_output_path(info->out_path);
				if (!fs::exists(fs_output_path))
				{
					fs::create_directory(fs_output_path);
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
				ExitThread(1);
			}

			// Create file on disk
			std::ofstream out_file(full_path, std::ios::binary | std::ios::app | std::ios::out);
			if (!out_file.is_open())
			{
				info->status = 0;
				info->status_code = STATUS_CANT_OPEN_OUTFILE;
				zip_fclose(file);
				zip_close(zip);
				ExitThread(1);
			}

			// Reading file from archive and write data to disk
			uint32_t buffer_size = 10'000;
			char* buffer = new char[buffer_size];
			zip_int64_t bytes_read;
			bytes_read = zip_fread(file, buffer, buffer_size);
			do
			{
				bytes_read = zip_fread(file, buffer, buffer_size);

				if (bytes_read > 0)
				{
					out_file.write(buffer, bytes_read);
				}

			} while (bytes_read > 0);

			// Close output file
			if (out_file.is_open())
			{
				out_file.close();
			}
			zip_fclose(file);
		}

		// Close archive
		zip_close(zip);
		info->is_ended = 1;

		return 0;
	}
	//====================================================================
}
#endif