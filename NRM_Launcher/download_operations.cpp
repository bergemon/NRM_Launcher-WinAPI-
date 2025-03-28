#include "download_operations.h"

//====================================================================
/* We will use this procedure only in this source file */
void unzip_after_output_dir_created(
	HWND hWnd,
	int32_t& num_of_threads,
	std::vector<HANDLE>& unzip_threads,
	std::vector<UNZIP_INFO>& unzip_info_vector,
	GAME_VERSION& ver,
	int32_t& is_ended,
	UINT_PTR timer_id,
	std::string& zip_file_name,
	CURRENT_OPERATION& operation,
	HMODULE library,
	CRITICAL_SECTION& cs
);
//====================================================================
void timer_check_version(HWND hWnd, GAME_VERSION& ver, UINT_PTR timer_id, HANDLE thread)
{
	KillTimer(hWnd, timer_id);

	// Set timer again if check version operation is not completed yet
	if (!ver.is_ended)
	{
		timer_id = SetTimer(hWnd, 1, 200, NULL);
	}
	else
	{
		// Exit if download thread returned error
		if (ver.status_code != NET_STATUS_CODES::NET_STAT_OK) return;

		PostMessage(hWnd, WM_USER + 2, NULL, NULL);
		ver.is_ended = 0;

		// Close thread handle
		CloseHandle(thread);
	}
}
//====================================================================
void timer_download_game(HWND hWnd, GAME_VERSION& ver, UINT_PTR timer_id, HANDLE thread)
{
	KillTimer(hWnd, timer_id);
	DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
	HWND progress_bar_hWnd = download_buttons.get_download_buttons()[2].get_hWnd();

	// Set timer again we have not downloaded the game yet
	if (ver.status)
	{
		PostMessage(progress_bar_hWnd, PBM_SETPOS, ver.progress, NULL);
		timer_id = SetTimer(hWnd, 1, 200, NULL);
	}
	else
	{
		// Exit if download thread returned error
		if (ver.status_code != NET_STATUS_CODES::NET_STAT_OK) return;

		PostMessage(progress_bar_hWnd, PBM_SETPOS, 100, NULL);
		PostMessage(hWnd, WM_USER + 3, NULL, NULL);
		ver.status = 1;
		ver.progress = 0;
		delete[] ver.new_file_id;

		// Close thread handle
		CloseHandle(thread);
	}
}
//====================================================================
void timer_unzip(
	HWND hWnd,
	UINT_PTR timer_id,
	CREATE_OUT_PATH_INFO& out_info,
	bool& output_path_created,
	int32_t& num_of_threads,
	HMODULE library,
	std::vector<HANDLE>& unzip_threads,
	std::vector<UNZIP_INFO>& unzip_info_vector,
	GAME_VERSION& ver,
	ZIP_INFO& zip_info,
	std::string& zip_file_name,
	std::string& zip_out_path,
	HANDLE create_out_path_thread,
	CURRENT_OPERATION& operation,
	CRITICAL_SECTION& cs
)
{
	KillTimer(hWnd, timer_id);
	int32_t is_ended = 0;

	/* If creating output directory still in progress, create timer again then */
	if (!out_info.is_ended)
	{
		timer_id = SetTimer(hWnd, 1, 200, NULL);
	}
	/* If we sucessfully created output directory */
	else if (out_info.is_ended && !output_path_created)
	{
		output_path_created = true;

		InitializeCriticalSection(&cs);

		typedef DWORD(WINAPI* NRM_LIB_PROC)(_In_ LPVOID lpParameter);

		NRM_LIB_PROC unzip_proc;
		unzip_proc = (NRM_LIB_PROC)GetProcAddress(library, "unzip_file");

		// Initialize static variable with number of threads from settings
		if (!num_of_threads)
		{
			num_of_threads = LAUNCHER_SETTINGS::getInstance().get_num_of_threads();
		}
		size_t entries_per_thread = zip_info.num_of_entries / num_of_threads;
		size_t entries_module = zip_info.num_of_entries % num_of_threads;
		size_t last_index = 0;

		unzip_info_vector.reserve(num_of_threads);
		unzip_threads.reserve(num_of_threads);
		for (int32_t i = 0; i < num_of_threads; ++i)
		{
			unzip_info_vector.push_back(UNZIP_INFO());
			unzip_info_vector[i].critical_section = &cs;
			unzip_info_vector[i].src_path = zip_file_name.data();
			unzip_info_vector[i].out_path = zip_out_path.data();
			unzip_info_vector[i].start_index = last_index;

			size_t mult = entries_per_thread * (i + 1);
			if (i == (num_of_threads - 1))
			{
				unzip_info_vector[i].end_index = mult + entries_module;
			}
			else
			{
				unzip_info_vector[i].end_index = mult;
			}

			last_index = unzip_info_vector[i].end_index;

			unzip_threads.push_back(CreateThread(NULL, NULL, unzip_proc, &unzip_info_vector[i], NULL, NULL));
		}

		// We also need to set new timer for unzip operation
		timer_id = SetTimer(hWnd, 1, 200, NULL);

		// Close thread handle
		CloseHandle(create_out_path_thread);
	}
	/* If we successfully created output directory */
	else if (out_info.is_ended && output_path_created)
	{
		// then unzip game to output dir
		unzip_after_output_dir_created(
			// Download window handle
			hWnd,
			// Number of threads
			num_of_threads,
			/* Unzip threads handles and
				unzip info structures vectors */
			unzip_threads, unzip_info_vector,
			/* GAME_VERSION structure
				(need to get version and checksum from there) */
			ver,
			// Is unzip work is done or not
			is_ended,
			// Id of the WIN32 timer
			timer_id,
			// Name of the zip file to delete after extraction
			zip_file_name,
			// Current operation (need to set it ended)
			operation,
			// download.dll shared library handle
			library,
			// Critical section (to uninitialize it)
			cs
		);
	}
}
//====================================================================
void unzip_after_output_dir_created(
	HWND hWnd,
	int32_t& num_of_threads,
	std::vector<HANDLE>& unzip_threads,
	std::vector<UNZIP_INFO>& unzip_info_vector,
	GAME_VERSION& ver,
	int32_t& is_ended,
	UINT_PTR timer_id,
	std::string& zip_file_name,
	CURRENT_OPERATION& operation,
	HMODULE library,
	CRITICAL_SECTION& cs
)
{
	DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
	HWND progress_bar_hWnd = download_buttons.get_download_buttons()[4].get_hWnd();

	int32_t global_progress = 0;
	int32_t index = 0;

	// Get each thread end of routine flag
	for (auto& elem : unzip_info_vector)
	{
		if (elem.status)
		{
			is_ended = is_ended | (elem.status << index++);
		}

		global_progress += elem.progress;
	}

	// Unzip work in progress
	if (is_ended | 0)
	{
		PostMessage(progress_bar_hWnd, PBM_SETPOS, global_progress / num_of_threads, NULL);
		timer_id = SetTimer(hWnd, 1, 200, NULL);
	}
	// If all threads are ended with unziping file
	else
	{
		// Exit if one of the threads was terminated or returned error
		for (auto& elem : unzip_info_vector)
		{
			if (elem.status_code != UNZIP_STATUS_CODES::STATUS_OK) break;
		}

		// Set progress to 100 in progress bar
		PostMessage(progress_bar_hWnd, PBM_SETPOS, 100, NULL);
		DeleteCriticalSection(&cs);

		MessageBeep(MB_OK);
		MessageBox(
			hWnd,
			TEXT("Последняя версия игры была скачана и разархивирована. Приятной игры!"),
			TEXT("Готово"),
			MB_OK
		);

		// Hide window and all its buttons
		for (auto& elem : DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons())
		{
			elem.hide();
		}
		DOWNLOAD_MODAL_WINDOW::getInstance().hide();
		FreeLibrary(library);

		// Set new version and checksum
		LAUNCHER_SETTINGS& settings = LAUNCHER_SETTINGS::getInstance();
		settings.set_param(LAUNCHER_SETTINGS::SETTINGS_PARAM::VERSION, std::to_wstring(ver.version).c_str());
		wchar_t* w_checksum = new wchar_t[strlen(ver.checksum) + 1];
		for (int i = 0; i < strlen(ver.checksum); ++i)
		{
			w_checksum[i] = (wchar_t)ver.checksum[i];
		}
		w_checksum[strlen(ver.checksum)] = (short)0;
		settings.set_param(LAUNCHER_SETTINGS::SETTINGS_PARAM::CHECKSUM, w_checksum);
		delete[] w_checksum;
		settings.rewrite_settings();

		// Clear vector for unzip info structures
		unzip_info_vector.clear();

		// Delete temp archive
		if (std::filesystem::exists(zip_file_name))
		{
			std::string current_path = std::filesystem::current_path().string();
			current_path.append("\\");
			current_path.append(zip_file_name);
			DeleteFileA(current_path.c_str());
		}

		// Set progress bars position to 0
		PostMessage(download_buttons.get_download_buttons()[4].get_hWnd(), PBM_SETPOS, 0, NULL);
		PostMessage(download_buttons.get_download_buttons()[2].get_hWnd(), PBM_SETPOS, 0, NULL);

		// Set new titlebar
		SetWindowText(
			MAIN_WINDOW::getInstance().getHWnd(),
			get_main_titlebar_text().c_str()
		);

		// Properly close all unzip threads handles
		for (HANDLE elem : unzip_threads)
		{
			CloseHandle(elem);
		}
		unzip_threads.clear();

		// Parse submods again
		create_submods_buttons_again();

		// Download operation is done
		operation = CURRENT_OPERATION::OPERATION_DONE;
	}
}
//====================================================================
LRESULT stop_downloading_operation(
	HWND hWnd,
	int32_t& num_of_threads,
	HANDLE check_ver_thread,
	HANDLE download_game_thread,
	HANDLE create_out_path_thread,
	bool& output_path_created,
	std::vector<HANDLE>& unzip_threads,
	std::vector<UNZIP_INFO>& unzip_info_vector,
	GAME_VERSION& ver,
	std::string& zip_file_name,
	UINT_PTR timer_id,
	CURRENT_OPERATION& operation,
	HMODULE library,
	CRITICAL_SECTION& cs
)
{
	if (operation != CURRENT_OPERATION::OPERATION_DONE)
	{
		MessageBeep(MB_OK);
		DWORD answer = MessageBox(
			hWnd,
			TEXT("В данный момент идёт загрузка/распаковка. Остановить?"),
			TEXT("Остановка загрузки/распаковки"),
			MB_YESNO
		);

		if (answer == IDNO)
		{
			return WM_USER;
		}

		// Kill the timer
		KillTimer(hWnd, timer_id);

		switch (operation)
		{
		case CURRENT_OPERATION::OPERATION_CHECK_VERSION:
		{
			// Exit download thread
			ver.terminate = 1;

			// Wait for download thread signaling and delete temp zip file
			DWORD wait = WaitForSingleObject(check_ver_thread, INFINITE);

			CloseHandle(check_ver_thread);
			ver.is_ended = 0;
			ver.terminate = 0;
			ver.status = 1;
			ver.status_code = NET_STATUS_CODES::NET_STAT_OK;

			break;
		}
		case CURRENT_OPERATION::OPERATION_DOWNLOAD_GAME:
		{
			// Exit download thread
			ver.terminate = 1;

			// Wait for download thread signaling and delete temp zip file
			DWORD wait = WaitForSingleObject(download_game_thread, INFINITE);

			CloseHandle(download_game_thread);
			ver.status = 1;
			ver.terminate = 0;
			ver.progress = 0;
			ver.status_code = NET_STATUS_CODES::NET_STAT_OK;
			delete[] ver.new_file_id;

			// Clear progress bar
			PostMessage(
				DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons()[2].get_hWnd(),
				PBM_SETPOS, 0, NULL
			);

			if (wait == WAIT_OBJECT_0 && std::filesystem::exists(zip_file_name.c_str()))
			{
				std::filesystem::remove(zip_file_name.c_str());
			}
			break;
		}
		case CURRENT_OPERATION::OPERATION_UNZIP_FILE:
			if (output_path_created)
			{
				// Exit unzip threads
				for (auto& elem : unzip_info_vector)
				{
					elem.terminate = 1;
				}

				// Wait untill all unzip threads become signaling
				DWORD wait = WaitForMultipleObjects(num_of_threads, unzip_threads.data(), TRUE, INFINITE);

				if (wait == WAIT_OBJECT_0)
				{
					unzip_info_vector.clear();
					for (HANDLE thread : unzip_threads)
					{
						CloseHandle(thread);
					}
					unzip_threads.clear();
				}

				// Clear progress bar
				PostMessage(
					DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons()[2].get_hWnd(),
					PBM_SETPOS, 0, NULL
				);
				PostMessage(
					DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons()[4].get_hWnd(),
					PBM_SETPOS, 0, NULL
				);

				// Delete temp zip file
				if (std::filesystem::exists(zip_file_name.c_str()))
				{
					std::filesystem::remove(zip_file_name.c_str());
				}
				DeleteCriticalSection(&cs);
			}
			else
			{
				// Wait for download thread signaling and delete temp zip file
				DWORD wait = WaitForSingleObject(create_out_path_thread, INFINITE);

				CloseHandle(create_out_path_thread);

				// Clear progress bar
				PostMessage(
					DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons()[2].get_hWnd(),
					PBM_SETPOS, 0, NULL
				);

				// Delete temp zip file
				if (std::filesystem::exists(zip_file_name.c_str()))
				{
					std::filesystem::remove(zip_file_name.c_str());
				}
			}
			break;
		}
	}

	// Download operation is done
	operation = CURRENT_OPERATION::OPERATION_DONE;

	// Hide window and all its buttons
	DOWNLOAD_MODAL_WINDOW::getInstance().hide();
	for (auto& elem : DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons())
	{
		elem.hide();
	}
	FreeLibrary(library);

	return WM_USER;
}
//====================================================================