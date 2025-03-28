#include "button.h"
#include "clear_cache.h"
#include "create_process.h"

//====================================================================
void draw_button_background(HWND hWnd, HDC hDC);
//====================================================================
void draw_active_button_background(HWND hWnd, HDC hDC);
//====================================================================
LRESULT CALLBACK LAUNCHER_BUTTONS::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		draw_button_background(hWnd, hDC);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONUP:
	{
		btnType = (uint32_t*)GetProp(hWnd, TEXT("buttonType"));

		if (btnType == nullptr)
		{
			break;
		}

		switch (*btnType)
		{
		case BUTTON_TYPE::BTN_EXIT:
		{
			SendMessage(GetParent(hWnd), WM_CLOSE, 0, 0);
			break;
		}
		case BUTTON_TYPE::BTN_DISCORD:
		{
			try
			{
				std::wstring link = settings_find_line(L"discord_link:");
				if ((int32_t)ShellExecute(NULL, L"open", link.c_str(), NULL, NULL, SW_SHOW) <= 32)
				{
					MessageBox(GetParent(hWnd), TEXT("Can not open link"), TEXT("Discord button error"), MB_OK);
				}
			}
			catch (std::exception& e)
			{
				MessageBoxA(GetParent(hWnd), e.what(), "Discord button error", MB_OK);
			}

			break;
		}
		case BUTTON_TYPE::BTN_PLAY:
			try
			{
				//clear_cache();
				create_process();
			}
			catch (std::exception& e)
			{
				MessageBoxA(GetParent(hWnd), e.what(), "Play button error", MB_OK);
			}
		}

		btnType = nullptr;
		break;
	}
	case WM_SETCURSOR:
	{
		hDC = GetDC(hWnd);

		LPTRACKMOUSEEVENT lpTrackMouseEvt = (LPTRACKMOUSEEVENT)malloc(sizeof(TRACKMOUSEEVENT));
		if (!lpTrackMouseEvt)
		{
			MessageBox(hWnd, TEXT("Can not allocate memory for TRACKMOUSEEVENT"), TEXT("Error"), MB_OK);
			break;
		}
		lpTrackMouseEvt->cbSize = sizeof(TRACKMOUSEEVENT);
		lpTrackMouseEvt->dwFlags = TME_LEAVE;
		lpTrackMouseEvt->hwndTrack = hWnd;
		lpTrackMouseEvt->dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(lpTrackMouseEvt);

		draw_active_button_background(hWnd, hDC);

		free(lpTrackMouseEvt);
		ReleaseDC(hWnd, hDC);
		break;
	}
	case WM_MOUSELEAVE:
	{
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, FALSE);
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