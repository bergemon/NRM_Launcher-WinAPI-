#include <windows.h>
#include "resource.h"
#include "const.h"
#include "stdlibs.h"
#include "main_window.h"
#include "button.h"
#include "submods_modal.h"
#include "submods_buttons.h"
#include "settings_modal.h"
#include "settings_buttons_group.h"
#include "download_modal.h"
#include "download_buttons.h"

// Modern styles for Windows common controls
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//====================================================================
int WINAPI MyWinMain(_In_ HINSTANCE hInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		// Main window
		MainWindow& main_window = MainWindow::getInstance();
		main_window.initialize(
			// Window class name
			MAIN_WINDOW_CLASS,
			// Titlebar text
			MAIN_WINDOW_CLASS,
			// hInstance & Cmd
			hInstance, nCmdShow,
			// Menu (optional)
			NULL,
			// Window width and height (optional)
			MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT,
			// Path to bmp image (optional)
			MAIN_BACKGROUND
		);
		// Main window buttons
		create_buttons(nCmdShow);

		// Submods modal window
		SUBMODS_MODAL_WINDOW& submods_modal = SUBMODS_MODAL_WINDOW::getInstance();
		submods_modal.initialize(
			// Parent window handle
			main_window.getHWnd(),
			// Window class
			SUBMODS_WINDOW_CLASS,
			// Titlebar text
			TEXT("Select submods to play"),
			// hInstance & Cmd
			hInstance, nCmdShow,
			// Menu
			NULL,
			// Widht & height
			SUBMODS_WINDOW_WIDTH, SUBMODS_WINDOW_HEIGHT,
			// Background file name
			SUBMODS_WINDOW_BACKGROUND
		);
		// Submods buttons
		create_submods_modal_buttons();

		// Settings window
		SETTINGS_MODAL_WINDOW& settings_window = SETTINGS_MODAL_WINDOW::getInstance();
		settings_window.initialize(
			// Parent window handle
			main_window.getHWnd(),
			// Window class
			SETTINGS_WINDOW_CLASS,
			// Titlebar text
			TEXT("NRM launcher settings"),
			// hInstance & Cmd
			hInstance, nCmdShow,
			// Menu
			NULL,
			// Widht & height
			SETTINGS_WINDOW_WIDTH, SETTINGS_WINDOW_HEIGHT,
			// Background file name
			SETTINGS_WINDOW_BACKGROUND
		);
		// Settings window buttons
		create_settings_buttons_groups(settings_window.getHWnd());

		// Download window
		DOWNLOAD_MODAL_WINDOW& download_window = DOWNLOAD_MODAL_WINDOW::getInstance();
		download_window.initialize(
			// Parent window handle
			main_window.getHWnd(),
			// Window class
			DOWNLOAD_WINDOW_CLASS,
			// Titlebar text
			TEXT("NRM download game window"),
			// hInstance & Cmd
			hInstance, nCmdShow,
			// Menu
			NULL,
			// Widht & height
			DOWNLOAD_WINDOW_WIDTH, DOWNLOAD_WINDOW_HEIGHT,
			// Background file name
			DOWNLOAD_WINDOW_BACKGROUND
		);
		// Download window buttons
		create_download_buttons();
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK);
	}

	MSG msg;
	BOOL bRet;
	while (bRet = GetMessage(&msg, NULL, 0, 0))
	{
		if (bRet == -1)
		{
			ExitThread(1);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}