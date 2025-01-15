#include "submods_modal.h"
#include "get_file_path.h"
#include "bitmap.h"

//====================================================================
LRESULT CALLBACK SUBMODS_MODAL_WINDOW::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		get_submods_bkg_path(bkgPath);
		bkgPath.append(SUBMODS_MODAL_WINDOW::getInstance().get_background_path().data());
		background.LoadFromFile(bkgPath.c_str());

		// We will get this data when we need to draw submods window background on buttons
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