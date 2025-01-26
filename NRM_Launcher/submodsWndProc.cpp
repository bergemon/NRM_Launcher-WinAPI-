#include "submods_modal.h"
#include "get_file_path.h"
#include "submods_utils.h"
#include "bitmap.h"

//====================================================================
LRESULT CALLBACK SUBMODS_MODAL_WINDOW::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	RECT rect;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		SUBMODS_MODAL_WINDOW& submods_window = SUBMODS_MODAL_WINDOW::getInstance();

		CUSTOM_BITMAP& background = submods_window.get_background();
		background.Draw(hDC, 0, 0,
			SUBMODS_WINDOW_WIDTH, SUBMODS_WINDOW_HEIGHT,
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
			SUBMODS_MODAL_WINDOW& submods_modal = SUBMODS_MODAL_WINDOW::getInstance();
			submods_modal.hide();
		}
		break;
	case WM_CLOSE:
		clear_buttons_state_in_buffer();
		SUBMODS_MODAL_WINDOW::getInstance().hide();
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