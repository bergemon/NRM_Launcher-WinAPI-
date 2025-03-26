#include "settings_modal.h"
#include "bitmap.h"

//====================================================================
LRESULT CALLBACK SETTINGS_MODAL_WINDOW::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		SETTINGS_MODAL_WINDOW& settings_window = SETTINGS_MODAL_WINDOW::getInstance();

		CUSTOM_BITMAP& background = settings_window.get_background();
		background.Draw(hDC, 0, 0,
			SETTINGS_WINDOW_WIDTH, SETTINGS_WINDOW_HEIGHT,
			0, 0,
			background.GetWidth(),
			background.GetHeight(),
			SRCCOPY
		);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			SETTINGS_MODAL_WINDOW& settings_window = SETTINGS_MODAL_WINDOW::getInstance();
			settings_window.hide();
		}
		break;
	case WM_CLOSE:
		SETTINGS_MODAL_WINDOW::getInstance().hide();
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