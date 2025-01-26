#include "button.h"
#include "clear_cache.h"
#include "create_process.h"
#include "submods_modal.h"

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
		BUTTON& button = *(BUTTON*)GetProp(hWnd, TEXT("buttonClass"));

		switch (button.get_button_type())
		{
		case BUTTON_TYPE::BTN_PLAY:
		{
			try
			{
				// Clear cache
				clear_cache();
				// Then start a game
				create_process();
			}
			catch (std::exception& e)
			{
				MessageBoxA(GetParent(hWnd), e.what(), "Play button error", MB_OK);
			}
			break;
		}
		case BUTTON_TYPE::BTN_DOWNLOAD:
		{
			MessageBox(GetParent(hWnd), TEXT("Button has no functionality yet"), TEXT("Work in progress"), MB_OK);
			break;
		}
		case BUTTON_TYPE::BTN_SETTINGS:
		{
			MessageBox(GetParent(hWnd), TEXT("Button has no functionality yet"), TEXT("Work in progress"), MB_OK);
			break;
		}
		case BUTTON_TYPE::BTN_SUBMODS:
		{
			SUBMODS_MODAL_WINDOW& submods_modal = SUBMODS_MODAL_WINDOW::getInstance();
			submods_modal.show();
			break;
		}
		case BUTTON_TYPE::BTN_EXIT:
		{
			SendMessage(GetParent(hWnd), WM_CLOSE, NULL, NULL);
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
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//====================================================================