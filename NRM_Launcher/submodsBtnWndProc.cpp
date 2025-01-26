#include "submods_buttons.h"
#include "submods_modal.h"
#include "submods_utils.h"

//====================================================================
extern std::list<PARSED_SUBMOD> parsed_submods;
//====================================================================
LRESULT CALLBACK SUBMODS_BUTTONS::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	RECT rect;
	SUBMODS_BUTTON_TYPE btnType;
	static LOGFONT lf;
	COLORREF outline = RGB(173, 191, 184), green = RGB(19, 91, 61), red = RGB(151, 23, 29);

	switch (uMsg)
	{
	case WM_CREATE:
	{
		hDC = GetDC(hWnd);

		// Init LOGFONT struct
		lf.lfPitchAndFamily = FIXED_PITCH | FF_ROMAN;
		lf.lfItalic = FALSE;
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 20;
		lf.lfWidth = 10;
		lf.lfCharSet = DEFAULT_CHARSET;
		lstrcpy((LPWSTR)&lf.lfFaceName, TEXT("Verdana"));

		ReleaseDC(hWnd, hDC);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		SUBMODS_WINDOW_BUTTON& button_class = get_submod_button_prop(hWnd);
		btnType = button_class.get_button_type();

		if (btnType == SUBMODS_BUTTON_TYPE::BTN_SUBMOD)
		{
			const std::string& submod_name = button_class.get_submod_name();
			bool checked = button_class.get_buffer_button().m_isChecked;

			GetClientRect(hWnd, &rect);

			// Draw background
			draw_submods_checkbox_btn_background(hWnd, hDC);

			// Draw text
			if (!checked)
				draw_text_unchecked_submod(hWnd, hDC, submod_name, lf, outline, red, rect);
			else
				draw_text_checked_submod(hWnd, hDC, submod_name, lf, outline, green, rect);
		}
		else
		{
			draw_submods_btn_background(hWnd, hDC);
		}

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONUP:
	{
		SUBMODS_WINDOW_BUTTON& button_class = get_submod_button_prop(hWnd);
		btnType = button_class.get_button_type();

		switch (btnType)
		{
		case SUBMODS_BUTTON_TYPE::BTN_SAVE:
			save_checked_from_buffer();
			SUBMODS_MODAL_WINDOW::getInstance().hide();
			break;
		case SUBMODS_BUTTON_TYPE::BTN_SUBMOD:
		{
			hDC = GetDC(hWnd);

			const std::string& submod_name = button_class.get_submod_name();

			SUBMOD_BUTTON_BUFFER& button_buffer = button_class.get_buffer_button();

			bool checked = button_buffer.m_isChecked;

			GetClientRect(hWnd, &rect);

			if (checked)
			{
				button_buffer.m_isChecked = false;

				// Draw background
				draw_submods_checkbox_btn_background(hWnd, hDC);

				// Draw text
				draw_text_unchecked_submod(hWnd, hDC, submod_name, lf, outline, red, rect);
			}
			else
			{
				button_buffer.m_isChecked = true;

				// Draw background
				draw_submods_checkbox_btn_background(hWnd, hDC);

				// Draw text
				draw_text_checked_submod(hWnd, hDC, submod_name, lf, outline, green, rect);
			}

			ReleaseDC(hWnd, hDC);
			break;
		}
		case SUBMODS_BUTTON_TYPE::BTN_CANCEL:
			clear_buttons_state_in_buffer();
			SUBMODS_MODAL_WINDOW::getInstance().hide();
			break;
		}

		break;
	}
	case WM_SETCURSOR:
	{
		SUBMODS_WINDOW_BUTTON& button_class = get_submod_button_prop(hWnd);
		btnType = button_class.get_button_type();

		switch (btnType)
		{
		case SUBMODS_BUTTON_TYPE::BTN_SUBMOD:
			break;
		default:
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
		}

		break;
	}
	case WM_MOUSELEAVE:
	{
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, FALSE);
		break;
	}
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//====================================================================