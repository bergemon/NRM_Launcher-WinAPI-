#include "const.h"
#include <windows.h>
#include "stdlib.h"
#include "main_window.h"
#include "button.h"
#include "submods_modal.h"
#include "submods_buttons.h"

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
			TEXT("MainWindow"),
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
			TEXT("CUSTOM_BUTTONS_NRM"),
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
			TEXT("SubmodsModal"),
			// Titlebar text
			TEXT("Choose submods to play"),
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
		submod_window_buttons.initiliaze(
			submods_modal.getHWnd(),
			TEXT("SubmodsModalButtons")
		);
		create_submods_modal_buttons(submod_window_buttons);
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK);
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}