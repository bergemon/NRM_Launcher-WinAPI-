#include "const.h"
#include <windows.h>
#include "stdlib.h"
#include "main_window.h"
#include "button.h"

//====================================================================
void create_buttons(LAUNCHER_BUTTONS& btns_singleton);
//====================================================================
int WINAPI MyWinMain(_In_ HINSTANCE hInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	MainWindow main_window(
		TEXT("MainWindow"), TEXT("New Realism mod launcher"),
		// hInstance & Cmd
		hInstance, nCmdShow,
		// Menu (optional)
		NULL,
		// Class styles (optional)
		CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		// Window width and height (optional)
		MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT,
		// Window styles (optional)
		WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX,
		// Windowex styles (optional)
		WS_EX_TRANSPARENT,
		// Path to bmp image (optional)
		BACKGROUND_PATH(MAIN_BACKGROUND)
	);

	// Buttons
	LAUNCHER_BUTTONS& btns_singleton = LAUNCHER_BUTTONS::getButtonsClass();
	btns_singleton.initialize(
		// Parent handler 
		main_window.getHWnd(),
		// Class name
		TEXT("CUSTOM_BUTTONS_NRM"),
		// Cmd
		nCmdShow
	);
	create_buttons(btns_singleton);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}