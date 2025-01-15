#include "main_window.h"
#include "const.h"
#include "bitmap.h"
#include "get_file_path.h"

//====================================================================
LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CUSTOM_BITMAP background;
	HDC hDC;
	RECT rect;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		hDC = GetDC(hWnd);

		std::string bkgPath;
		get_main_bkg_path(bkgPath);

		MainWindow& main_window = MainWindow::getInstance();
		bkgPath.append(main_window.get_bkg_filename().data());
		background.LoadFromFile(bkgPath.c_str());

		// We will get this data when we need to draw main window background on buttons
		// to make them transparent
		SetProp(hWnd, TEXT("BitmapBits"), background.GetBitmapBits());
		SetProp(hWnd, TEXT("InfoHeader"), background.GetInfoHeader());

		ReleaseDC(hWnd, hDC);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

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
		RemoveProp(hWnd, TEXT("BitmapBits"));
		RemoveProp(hWnd, TEXT("InfoHeader"));
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//====================================================================