#include "download_modal.h"
#include "download_buttons.h"

//====================================================================
void draw_download_btn_background(HWND hWnd, HDC hDC);
//====================================================================
LRESULT CALLBACK DOWNLOAD_WINDOW_BUTTONS::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	RECT rect;
	uint32_t* btnType = nullptr;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		hDC = GetDC(hWnd);
		ReleaseDC(hWnd, hDC);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON& button = get_download_button_prop(hWnd);
		
		if (button.get_button_type() == DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_STATIC_BUTTON)
		{
			draw_download_btn_background(hWnd, hDC);
		}

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