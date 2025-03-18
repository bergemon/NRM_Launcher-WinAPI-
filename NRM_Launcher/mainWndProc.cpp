#include "main_window.h"
#include "const.h"
#include "bitmap.h"
#include "get_file_path.h"

//====================================================================
LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	RECT rect;

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
		MainWindow& main_window = MainWindow::getInstance();
		CUSTOM_BITMAP& background = main_window.get_background();

		background.Draw(
			hDC,
			0, 0,
			MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT,
			0, 0,
			background.GetWidth(),
			background.GetHeight(),
			SRCCOPY
		);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYUP:
	{
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
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