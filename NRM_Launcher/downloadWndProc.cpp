#include "download_modal.h"
#include "bitmap.h"

//====================================================================
typedef DWORD(WINAPI *NRM_LIB_PROC)(_In_ LPVOID lpParameter);
//====================================================================
CRITICAL_SECTION cs;
//====================================================================
LRESULT CALLBACK DOWNLOAD_MODAL_WINDOW::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static HMODULE library;
	static CURRENT_OPERATION operation;
	static GAME_VERSION ver;
	static HANDLE check_ver_thread;
	static HANDLE download_game_thread;
	static HANDLE unzip_file_thread;
	static UINT_PTR timer_id;
	static std::vector<UNZIP_INFO> unzip_info_vector;
	static int32_t num_of_threads = 8;

	static std::string zip_file_name(DEFAULT_ZIP_PATH);
	ver.zip_file_name = zip_file_name.data();
	ver.has_custom_zip_name = 1;
	static std::string zip_out_path("game");

	static std::string host("drive.usercontent.google.com");
	static std::string file_id("1hjtR0WkW5e-i5GPU7OSWQ4SdMOafGI7G");

	if (!ver.file_id) ver.file_id = file_id.data();
	if (!ver.host) ver.host = host.data();

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		DOWNLOAD_MODAL_WINDOW& download_window = DOWNLOAD_MODAL_WINDOW::getInstance();

		CUSTOM_BITMAP& background = download_window.get_background();
		background.Draw(hDC, 0, 0,
			DOWNLOAD_WINDOW_WIDTH, DOWNLOAD_WINDOW_HEIGHT,
			0, 0,
			background.GetWidth(),
			background.GetHeight(),
			SRCCOPY
		);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_USER + 1:
	{
		DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
		auto& btns = download_buttons.get_download_buttons();
		btns[0].show();

		LAUNCHER_SETTINGS& settings = LAUNCHER_SETTINGS::getInstance();
		ver.version = settings.get_game_version();
		ver.checksum = settings.get_game_checksum_ascii();

		ver.new_file_id = new char[200];

		library = LoadLibrary(TEXT("download.dll"));
		if (!library)
		{
			MessageBox(NULL, TEXT("Can not load \"download.dll\" library"), TEXT("Error"), MB_OK);
			break;
		}

		NRM_LIB_PROC check_ver;
		check_ver = (NRM_LIB_PROC)GetProcAddress(library, "check_version");

		if (!check_ver) break;

		// Current operation
		operation = CURRENT_OPERATION::OPERATION_CHECK_VERSION;
		// Check version in new thread
		check_ver_thread = CreateThread(NULL, NULL, check_ver, &ver, NULL, NULL);

		timer_id = SetTimer(hWnd, 1, 200, NULL);

		break;
	}
	case WM_USER + 2:
	{
		DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
		auto& btns = download_buttons.get_download_buttons();
		btns[1].show();
		btns[2].show();

		if (!library)
		{
			MessageBox(NULL, TEXT("Library \"download.dll\" is not loaded"), TEXT("Error"), MB_OK);
			break;
		}

		// Get proc address
		NRM_LIB_PROC download_game;
		download_game = (NRM_LIB_PROC)GetProcAddress(library, "download_new_version");

		if (!ver.compare_result)
		{
		agreed_to_download:
			// Current operation
			operation = CURRENT_OPERATION::OPERATION_DOWNLOAD_GAME;
			// Check version in new thread
			download_game_thread = CreateThread(NULL, NULL, download_game, &ver, NULL, NULL);
			timer_id = SetTimer(hWnd, 1, 200, NULL);
		}
		else
		{
			MessageBeep(MB_OK);
			int32_t answer = MessageBox(
				hWnd,
				TEXT("У вас последняя версия New Realism mod. Всё равно скачать?"),
				TEXT("Информация"), MB_YESNO
			);

			if (answer == IDYES) goto agreed_to_download;

			// Hide window and all its buttons
			for (auto& elem : DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons())
			{
				elem.hide();
			}
			DOWNLOAD_MODAL_WINDOW::getInstance().hide();
			FreeLibrary(library);
		}

		break;
	}
	case WM_USER + 3:
	{
		// Current operation
		operation = CURRENT_OPERATION::OPERATION_UNZIP_FILE;

		DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
		auto& btns = download_buttons.get_download_buttons();
		btns[3].show();
		btns[4].show();

		ZIP_INFO zip_info;
		std::string path_to_zip(DEFAULT_ZIP_PATH);
		zip_info.path_to_zip = path_to_zip.data();

		NRM_LIB_PROC get_zip_info;
		get_zip_info = (NRM_LIB_PROC)GetProcAddress(library, "get_zip_info");

		get_zip_info(&zip_info);

		NRM_LIB_PROC unzip_proc;
		unzip_proc = (NRM_LIB_PROC)GetProcAddress(library, "unzip_file");

		size_t entries_per_thread = zip_info.num_of_entries / num_of_threads;
		size_t entries_module = zip_info.num_of_entries % num_of_threads;
		size_t last_index = 0;

		InitializeCriticalSection(&cs);

		// Create directory
		namespace fs = std::filesystem;
		fs::path fs_output_path(zip_out_path);
		if (!fs::exists(fs_output_path))
		{
			fs::create_directory(fs_output_path);
		}
		else
		{
			fs::remove_all(fs_output_path);
		}

		unzip_info_vector.reserve(num_of_threads);
		for (int i = 0; i < num_of_threads; ++i)
		{
			unzip_info_vector.push_back(UNZIP_INFO());
			unzip_info_vector[i].critical_section = &cs;
			unzip_info_vector[i].src_path = zip_file_name.data();
			unzip_info_vector[i].out_path = zip_out_path.data();
			unzip_info_vector[i].start_index = last_index;

			if (i == (num_of_threads - 1))
			{
				unzip_info_vector[i].end_index = (entries_per_thread * (i + 1) + entries_module);
			}
			else
			{
				unzip_info_vector[i].end_index = (entries_per_thread * (i + 1));
			}

			last_index = unzip_info_vector[i].end_index;

			CreateThread(NULL, NULL, unzip_proc, &unzip_info_vector[i], NULL, NULL);
		}

		timer_id = SetTimer(hWnd, 1, 200, NULL);

		break;
	}
	case WM_TIMER:
	{
		switch (operation)
		{
		case CURRENT_OPERATION::OPERATION_CHECK_VERSION:
		{
			KillTimer(hWnd, timer_id);

			if (!ver.is_ended)
			{
				timer_id = SetTimer(hWnd, 1, 200, NULL);
			}
			else
			{
				PostMessage(hWnd, WM_USER + 2, NULL, NULL);
				ver.is_ended = 0;
			}
			break;
		}
		case CURRENT_OPERATION::OPERATION_DOWNLOAD_GAME:
		{
			KillTimer(hWnd, timer_id);
			DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
			HWND progress_bar_hWnd = download_buttons.get_download_buttons()[2].get_hWnd();

			if (ver.status)
			{
				SendMessage(progress_bar_hWnd, PBM_SETPOS, ver.progress, NULL);
				timer_id = SetTimer(hWnd, 1, 200, NULL);
			}
			else
			{
				SendMessage(progress_bar_hWnd, PBM_SETPOS, 100, NULL);
				PostMessage(hWnd, WM_USER + 3, NULL, NULL);
				ver.status = 1;
				ver.progress = 0;
				delete[] ver.new_file_id;
			}
			break;
		}
		case CURRENT_OPERATION::OPERATION_UNZIP_FILE:
		{
			KillTimer(hWnd, timer_id);
			int32_t is_ended = 0;

			DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
			HWND progress_bar_hWnd = download_buttons.get_download_buttons()[4].get_hWnd();

			int32_t global_progress = 0;
			int32_t index = 0;
			for (auto& elem : unzip_info_vector)
			{
				if (elem.status)
				{
					is_ended = is_ended | (elem.status << index++);
				}

				global_progress += elem.progress;
			}

			// If all threads are ended with unziping file
			if (is_ended | 0)
			{
				SendMessage(progress_bar_hWnd, PBM_SETPOS, global_progress / num_of_threads, NULL);
				timer_id = SetTimer(hWnd, 1, 200, NULL);
			}
			else
			{
				SendMessage(progress_bar_hWnd, PBM_SETPOS, 100, NULL);
				DeleteCriticalSection(&cs);

				MessageBeep(MB_OK);
				MessageBox(
					NULL,
					TEXT("Последняя версия игры была скачена и разархивирована. Приятной игры!"),
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

				// Set progress bars position to 0
				SendMessage(download_buttons.get_download_buttons()[4].get_hWnd(), PBM_SETPOS, 0, NULL);
				SendMessage(download_buttons.get_download_buttons()[2].get_hWnd(), PBM_SETPOS, 0, NULL);
			}

			break;
		}
		}

		break;
	}
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			DOWNLOAD_MODAL_WINDOW& download_window = DOWNLOAD_MODAL_WINDOW::getInstance();
			download_window.hide();
		}
		break;
	case WM_CLOSE:
		DOWNLOAD_MODAL_WINDOW::getInstance().hide();
		return WM_USER;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//====================================================================