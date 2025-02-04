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

//====================================================================
extern SETTINGS_BUTTONS_GROUP* settings_on_play;
//====================================================================
extern SETTINGS_BUTTONS_GROUP* settings_clear_cache;
//====================================================================
void create_buttons(LAUNCHER_BUTTONS& btns_singleton);
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
			TEXT("New Realism mod launcher"),
			// hInstance & Cmd
			hInstance, nCmdShow,
			// Menu (optional)
			NULL,
			// Window width and height (optional)
			MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT,
			// Path to bmp image (optional)
			MAIN_BACKGROUND
		);

		// Buttons
		LAUNCHER_BUTTONS& main_buttons = LAUNCHER_BUTTONS::getInstance();
		main_buttons.initialize(
			// Parent handler 
			main_window.getHWnd(),
			// Class name
			TEXT("NRM_launcher_main_window_button"),
			// Cmd
			nCmdShow
		);
		create_buttons(main_buttons);

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
		SUBMODS_BUTTONS& submod_window_buttons = SUBMODS_BUTTONS::getInstance();
		submod_window_buttons.initialize(
			// Parent
			submods_modal.getHWnd(),
			// Classname
			TEXT("NRM_launcher_submods_window_button")
		);
		create_submods_modal_buttons(submod_window_buttons);

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
		create_settings_buttons_groups(hInstance, settings_window.getHWnd());
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK);
	}

	MSG msg;
	BOOL bRet;
	while (bRet = GetMessage(&msg, NULL, 0, 0))
	{
		if (bRet == -1) std::exit(1);

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete settings_on_play;
	delete settings_clear_cache;

	return (int)msg.wParam;
}