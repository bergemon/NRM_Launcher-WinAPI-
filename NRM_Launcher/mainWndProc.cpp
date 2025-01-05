#include "main_window.h"
#include "const.h"
#include "bitmap.h"

//====================================================================
LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CUSTOM_BITMAP background;
	HDC hDC;
	RECT rect;

	switch (uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		background.LoadFromFile(m_strBkgFileName.c_str());
		ReleaseDC(hWnd, hDC);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		SetProp(hWnd, TEXT("BitmapBits"), background.GetBitmapBits());
		SetProp(hWnd, TEXT("InfoHeader"), background.GetInfoHeader());

		background.Draw(hDC, 0, 0,
			MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT,
			0, 0,
			background.GetWidth(),
			background.GetHeight(),
			SRCCOPY
		);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
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