#include "settings_buttons_group.h"
#include "settings_modal.h"
#include "get_file_path.h"

//====================================================================
void draw_settings_static_button_background(HWND hWnd, HDC hDC)
{
	SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON& static_button = SETTINGS_BUTTONS_GROUP::get_static_button_prop(hWnd);
	std::string bkgPath;

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - static_button.get_yPos() - static_button.get_height();

	// Get submod window background image file path
	get_settings_button_path(bkgPath);
	bkgPath.append(static_button.get_background_file_name());

	// Get parent BITMAP background
	SETTINGS_MODAL_WINDOW& settings_window = SETTINGS_MODAL_WINDOW::getInstance();
	CUSTOM_BITMAP& settings_window_background = settings_window.get_background();

	StretchDIBits(
		hDC, 0, 0, static_button.get_width(), static_button.get_height(),
		static_button.get_xPos(), translated_yPos, static_button.get_width(), static_button.get_height(),
		settings_window_background.GetBitmapBits(),
		(BITMAPINFO*)settings_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());
	if (!background.CreateBkgMask(get_settings_button_mask_path))
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath;
	get_settings_button_mask_path(maskPath);
	maskPath.append(static_button.get_background_file_name());

	// First draw mask, then background
	CUSTOM_BITMAP mask_bkg;
	mask_bkg.LoadFromFile(maskPath.c_str());
	mask_bkg.Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		static_button.get_width(), static_button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		mask_bkg.GetWidth(), mask_bkg.GetHeight(),
		// Raster-operation code
		SRCAND
	);
	background.Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		static_button.get_width(), static_button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		background.GetWidth(), background.GetHeight(),
		// Raster-operation code
		SRCINVERT
	);
}
//====================================================================
void draw_settings_radio_button_background(HWND hWnd, HDC hDC)
{
	SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON& radio_button = SETTINGS_BUTTONS_GROUP::get_radio_button_prop(hWnd);
	std::string bkgPath;

	bool is_checked = radio_button.get_checked();

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - radio_button.get_yPos() - radio_button.get_height();

	// Get submod window background image file path
	if (is_checked)
	{
		get_settings_active_button_path(bkgPath);
	}
	else
	{
		get_settings_button_path(bkgPath);
	}
	bkgPath.append(radio_button.get_background_file_name());

	// Get parent BITMAP background
	SETTINGS_MODAL_WINDOW& settings_window = SETTINGS_MODAL_WINDOW::getInstance();
	CUSTOM_BITMAP& settings_window_background = settings_window.get_background();

	StretchDIBits(
		hDC, 0, 0, radio_button.get_width(), radio_button.get_height(),
		radio_button.get_xPos(), translated_yPos, radio_button.get_width(), radio_button.get_height(),
		settings_window_background.GetBitmapBits(),
		(BITMAPINFO*)settings_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());
	if (!background.CreateBkgMask(get_settings_button_mask_path))
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath;
	get_settings_button_mask_path(maskPath);
	maskPath.append(radio_button.get_background_file_name());

	// First draw mask, then background
	CUSTOM_BITMAP mask_bkg;
	mask_bkg.LoadFromFile(maskPath.c_str());
	mask_bkg.Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		radio_button.get_width(), radio_button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		mask_bkg.GetWidth(), mask_bkg.GetHeight(),
		// Raster-operation code
		SRCAND
	);
	background.Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		radio_button.get_width(), radio_button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		background.GetWidth(), background.GetHeight(),
		// Raster-operation code
		SRCINVERT
	);

	// Draw radio button icon
	CUSTOM_BITMAP* radio_icon;
	CUSTOM_BITMAP* radio_icon_mask;
	if (is_checked)
	{
		radio_icon = SETTINGS_BUTTONS_GROUP::get_radio_checked_icon();
		radio_icon_mask = SETTINGS_BUTTONS_GROUP::get_radio_checked_icon_mask();
	}
	else
	{
		radio_icon = SETTINGS_BUTTONS_GROUP::get_radio_unchecked_icon();
		radio_icon_mask = SETTINGS_BUTTONS_GROUP::get_radio_unchecked_icon_mask();
	}

	radio_icon_mask->Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		radio_icon_mask->GetWidth(), radio_icon_mask->GetHeight(),
		// Source x and y
		0, 0,
		// Source width and height
		radio_icon_mask->GetWidth(), radio_icon_mask->GetHeight(),
		// Raster-operation code
		SRCAND
	);
	radio_icon->Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		radio_icon->GetWidth(), radio_icon->GetHeight(),
		// Source x and y
		0, 0,
		// Source width and height
		radio_icon->GetWidth(), radio_icon->GetHeight(),
		// Raster-operation code
		SRCINVERT
	);
}
//====================================================================