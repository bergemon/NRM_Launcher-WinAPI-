#include "submods_buttons.h"
#include "submods_modal.h"

//====================================================================
LRESULT CALLBACK SUBMODS_BUTTONS::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		draw_submods_btn_background(hWnd, hDC);

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
		case SUBMODS_BUTTON_TYPE::BTN_CANCEL:
			SUBMODS_MODAL_WINDOW::getInstance().hide();
			break;
		}

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

		draw_submods_active_btn_background(hWnd, hDC);

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
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//====================================================================