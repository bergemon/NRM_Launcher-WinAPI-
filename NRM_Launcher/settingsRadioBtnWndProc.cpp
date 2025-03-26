#include "settings_buttons_group.h"
#include "settings_modal.h"
#include "bitmap.h"

//====================================================================
LRESULT CALLBACK SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;

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
				if (button.get_button_num() == settings.get_clear_cache_state())
				{
					button.set_checked(true);
				}
				break;
			case PARAM::THREADS_TO_UNZIP:
				switch (settings.get_num_of_threads())
				{
				case 4:
					if (button.get_button_num() == 0) button.set_checked(true);
					break;
				case 8:
					if (button.get_button_num() == 1) button.set_checked(true);
					break;
				case 16:
					if (button.get_button_num() == 2) button.set_checked(true);
					break;
				}
				break;
			}
		}
		catch (std::exception& e)
		{
			SETTINGS_MODAL_WINDOW& settings_modal = SETTINGS_MODAL_WINDOW::getInstance();
			MessageBoxA(settings_modal.getHWnd(), e.what(), "Error", MB_OK);
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
					// Set new param
					settings.set_param(PARAM::ON_PLAY, std::to_wstring(button.get_button_num()).c_str());
					// Rewrite settings
					settings.rewrite_settings();
					// Invalidate all radio buttons in group
					for (auto& button : button.get_button_group().get_buttons())
					{
						button.set_checked(false);
						InvalidateRect(button.get_hWnd(), NULL, FALSE);
					}
				}
				break;
			case PARAM::CLEAR_CACHE:
				if (!button.get_checked())
				{
					// Set new param
					settings.set_param(PARAM::CLEAR_CACHE, std::to_wstring(button.get_button_num()).c_str());
					// Rewrite settings
					settings.rewrite_settings();
					// Invalidate all radio buttons in group
					for (auto& button : button.get_button_group().get_buttons())
					{
						button.set_checked(false);
						InvalidateRect(button.get_hWnd(), NULL, FALSE);
					}
				}
				break;
			case PARAM::THREADS_TO_UNZIP:
				if (!button.get_checked())
				{
					// Get number of threads
					int32_t num_of_threads = std::pow(2, button.get_button_num() + 2);
					// Set new param
					settings.set_param(PARAM::THREADS_TO_UNZIP, std::to_wstring(num_of_threads).c_str());
					// Rewrite settings
					settings.rewrite_settings();
					// Invalidate all radio buttons in group
					for (auto& button : button.get_button_group().get_buttons())
					{
						button.set_checked(false);
						InvalidateRect(button.get_hWnd(), NULL, FALSE);
					}
				}
				break;
			}
		}
		catch (std::exception& e)
		{
			SETTINGS_MODAL_WINDOW& settings_modal = SETTINGS_MODAL_WINDOW::getInstance();
			MessageBoxA(settings_modal.getHWnd(), e.what(), "Error", MB_OK);
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