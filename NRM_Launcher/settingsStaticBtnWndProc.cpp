#include "settings_buttons_group.h"
#include "bitmap.h"

//====================================================================
LRESULT CALLBACK SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		draw_settings_static_button_background(hWnd, hDC);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//====================================================================