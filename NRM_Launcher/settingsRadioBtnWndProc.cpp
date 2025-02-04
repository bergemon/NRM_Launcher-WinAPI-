#include "settings_buttons_group.h"
#include "bitmap.h"

//====================================================================
LRESULT CALLBACK SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	RECT rect;

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		try {
			LAUNCHER_SETTINGS& settings = LAUNCHER_SETTINGS::getInstance();
			SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON& button = SETTINGS_BUTTONS_GROUP::get_radio_button_prop(hWnd);
			using PARAM = LAUNCHER_SETTINGS::SETTINGS_PARAM;

			switch (button.get_group_param())
			{
			case PARAM::ON_PLAY:
				if (button.get_button_num() == settings.get_on_play_state())
				{
					button.set_checked(true);
				}
				break;
			case PARAM::CLEAR_CACHE:
				break;
			}
		}
		catch (std::exception& e)
		{
			MessageBoxA(NULL, e.what(), "Error", MB_OK);
		}

		draw_settings_radio_button_background(hWnd, hDC);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_LBUTTONUP:
		try {
			LAUNCHER_SETTINGS& settings = LAUNCHER_SETTINGS::getInstance();
			SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON& button = SETTINGS_BUTTONS_GROUP::get_radio_button_prop(hWnd);
			using PARAM = LAUNCHER_SETTINGS::SETTINGS_PARAM;

			switch (button.get_group_param())
			{
			case PARAM::ON_PLAY:
				if (!button.get_checked())
				{
					settings.set_param(PARAM::ON_PLAY, std::to_wstring(button.get_button_num()).c_str());
					settings.rewrite_settings();
				}
				for (auto& button : button.get_button_group().get_buttons())
				{
					button.set_checked(false);
					InvalidateRect(button.get_hWnd(), NULL, FALSE);
				}
				break;
			case PARAM::CLEAR_CACHE:
				break;
			}
		}
		catch (std::exception& e)
		{
			MessageBoxA(NULL, e.what(), "Error", MB_OK);
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