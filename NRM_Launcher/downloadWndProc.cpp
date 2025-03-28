#include "download_modal.h"
#include "bitmap.h"
#include "remove_all_thread.h"
#include "main_window.h"
#include "get_main_titlebar_text.h"
#include "download_operations.h"

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
	static ZIP_INFO zip_info;
	static CREATE_OUT_PATH_INFO out_info;
	static HANDLE check_ver_thread;
	static HANDLE download_game_thread;
	static HANDLE create_out_path_thread;
	static std::vector<HANDLE> unzip_threads;
	static UINT_PTR timer_id;
	static std::vector<UNZIP_INFO> unzip_info_vector;
	static bool output_path_created;
	static int32_t num_of_threads = 0;

	static std::string zip_file_name(DEFAULT_ZIP_PATH);
	ver.zip_file_name = zip_file_name.data();
	ver.has_custom_zip_name = 1;
	static std::string zip_out_path(DEFAULT_OUT_PATH);

	static std::string host(GOOGLE_DRIVE_HOST);
	static std::string file_id(GOOGLE_VERSION_FILE_ID);

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
		// Operation is only starting (we do not need to properly close threads)
		operation = CURRENT_OPERATION::OPERATION_START;

		DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();
		auto& btns = download_buttons.get_download_buttons();
		btns[0].show();

		LAUNCHER_SETTINGS& settings = LAUNCHER_SETTINGS::getInstance();
		ver.version = settings.get_game_version();
		ver.checksum = settings.get_game_checksum_ascii();

		ver.new_file_id = new char[200];

		library = LoadLibrary(TEXT(DOWNLOAD_HANDLER_LIB_NAME));
		if (!library)
		{
			MessageBox(hWnd, TEXT("Can not load \"download.dll\" library"), TEXT("Error"), MB_OK);
			PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;
		}

		NRM_LIB_PROC check_ver;
		check_ver = (NRM_LIB_PROC)GetProcAddress(library, "check_version");

		if (!check_ver) break;

		// Current operation
		operation = CURRENT_OPERATION::OPERATION_CHECK_VERSION;
		// Check version in new thread
		check_ver_thread = CreateThread(NULL, NULL, check_ver, &ver, NULL, NULL);

		// Set timer to handle operation using progress bar
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
			MessageBox(hWnd, TEXT("Library \"download.dll\" is not loaded"), TEXT("Error"), MB_OK);
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
			// Set timer to handle operation using progress bar
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

			// warning! goto command, but it is correctly used here
			if (answer == IDYES) goto agreed_to_download;

			// Hide window and all its buttons
			DOWNLOAD_MODAL_WINDOW::getInstance().hide();
			for (auto& elem : DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons())
			{
				elem.hide();
			}
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

		std::string path_to_zip(DEFAULT_ZIP_PATH);
		zip_info.path_to_zip = path_to_zip.data();

		/* Get number of files in archive */
		NRM_LIB_PROC get_zip_info;
		get_zip_info = (NRM_LIB_PROC)GetProcAddress(library, "get_zip_info");
		get_zip_info(&zip_info);

		/* Create output directory or delete existing before extraction of zip file
			in another thread */
		out_info.output_path = zip_out_path.data();
		output_path_created = false;
		out_info.is_ended = false;
		create_out_path_thread = CreateThread(NULL, NULL, create_out_path, &out_info, NULL, NULL);

		// Set timer to handle operation using progress bar
		timer_id = SetTimer(hWnd, 1, 200, NULL);

		break;
	}
	case WM_TIMER:
	{
		switch (operation)
		{
		case CURRENT_OPERATION::OPERATION_CHECK_VERSION:
		{
			timer_check_version(
				// Download window handle
				hWnd,
				/* GAME_VERSION struct, need to provide to newly
					created thread to track the download operation */
				ver,
				// WIN32 timer identifier
				timer_id,
				// Check version operation thread handle
				check_ver_thread
			);
			break;
		}
		case CURRENT_OPERATION::OPERATION_DOWNLOAD_GAME:
		{
			timer_download_game(
				// Download window handle
				hWnd,
				/* GAME_VERSION struct, need to provide to newly
					created thread to track the download operation */
				ver,
				// WIN32 timer identifier
				timer_id,
				// Download game operation thread handle
				download_game_thread
			);
			break;
		}
		case CURRENT_OPERATION::OPERATION_UNZIP_FILE:
		{
			timer_unzip(
				// Download window handle
				hWnd,
				// WIN32 timer identifier
				timer_id,
				// Create output path info structure
				out_info,
				// is output path created flag
				output_path_created,
				// number of threads
				num_of_threads,
				// download.dll hmodule, need to release it
				library,
				// Vectors of threads and unzip_info structures
				unzip_threads, unzip_info_vector,
				// GAME_VERSION structure
				ver,
				// ZIP_INFO structure to get num of entries in archive
				zip_info,
				// Strings
				zip_file_name, zip_out_path,
				// Create out path thread handle
				create_out_path_thread,
				// Current operation (need to set that download operation is ended)
				operation,
				// Critical section (to uninitialize it)
				cs
			);
			break;
		}
		}
		break;
	}
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostMessage(hWnd, WM_CLOSE, NULL, NULL);
		}
		break;
	case WM_CLOSE:
	{
		// Close download window if we could not load download shared library
		if (operation == CURRENT_OPERATION::OPERATION_START)
		{
			// Download operation is done
			operation = CURRENT_OPERATION::OPERATION_DONE;

			// Hide window and all its buttons
			DOWNLOAD_MODAL_WINDOW::getInstance().hide();
			for (auto& elem : DOWNLOAD_WINDOW_BUTTONS::getInstance().get_download_buttons())
			{
				elem.hide();
			}

			return WM_USER;
		}

		return stop_downloading_operation(
			// Download window handle
			hWnd,
			// Number of threads (reference)
			num_of_threads,
			// Operations threads handles (to wait and correctly close them)
			check_ver_thread, download_game_thread, create_out_path_thread,
			// is output path created flag
			output_path_created,
			// Vectors of threads and unzip_info structures
			unzip_threads, unzip_info_vector,
			// GAME_VERSION structure to stop download thread
			ver,
			// Name of the zip file to delete
			zip_file_name,
			// WIN32 timer identifier
			timer_id,
			// Current operation (need to set that download operation is ended)
			operation,
			// download.dll hmodule, need to release it
			library,
			// Critical section (to uninitialize it)
			cs
		);
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//====================================================================