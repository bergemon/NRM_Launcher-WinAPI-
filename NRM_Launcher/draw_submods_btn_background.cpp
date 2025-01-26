#include "submods_buttons.h"
#include "submods_modal.h"
#include "bitmap.h"
#include "get_file_path.h"

//====================================================================
void draw_submods_btn_background(HWND hWnd, HDC hDC)
{
	SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON& button = get_submod_button_prop(hWnd);
	std::string bkgPath;

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - button.get_yPos() - button.get_height();

	// Get submod window background image file path
	get_submods_btns_bkg_path(bkgPath);
	bkgPath.append(button.get_background_file_name());

	// Get parent BITMAP background
	SUBMODS_MODAL_WINDOW& submods_window = SUBMODS_MODAL_WINDOW::getInstance();
	CUSTOM_BITMAP& submods_window_background = submods_window.get_background();

	StretchDIBits(
		hDC, 0, 0, button.get_width(), button.get_height(),
		button.get_xPos(), translated_yPos, button.get_width(), button.get_height(),
		submods_window_background.GetBitmapBits(),
		(BITMAPINFO*)submods_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());
	if (!background.CreateBkgMask(get_submods_btns_mask_path))
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath;
	get_submods_btns_mask_path(maskPath);
	maskPath.append(button.get_background_file_name());

	// First draw mask, then background
	CUSTOM_BITMAP mask_bkg;
	mask_bkg.LoadFromFile(maskPath.c_str());
	mask_bkg.Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		button.get_width(), button.get_height(),
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
		button.get_width(), button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		background.GetWidth(), background.GetHeight(),
		// Raster-operation code
		SRCINVERT
	);
}
//====================================================================
void draw_submods_active_btn_background(HWND hWnd, HDC hDC)
{
	SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON& button = get_submod_button_prop(hWnd);
	std::string bkgPath;

	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, button.get_width(), button.get_height());
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - button.get_yPos() - button.get_height();

	// Get parent BITMAP background
	SUBMODS_MODAL_WINDOW& submods_window = SUBMODS_MODAL_WINDOW::getInstance();
	CUSTOM_BITMAP& submods_window_background = submods_window.get_background();

	StretchDIBits(
		hMemDC, 0, 0, button.get_width(), button.get_height(),
		button.get_xPos(), translated_yPos, button.get_width(), button.get_height(),
		submods_window_background.GetBitmapBits(),
		(BITMAPINFO*)submods_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	// Get button active background path
	get_submods_active_btns_bkg_path(bkgPath);
	bkgPath.append(button.get_background_file_name());

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());

	// Get button mask background path
	std::string maskPath;
	get_submods_btns_mask_path(maskPath);
	maskPath.append(button.get_background_file_name());

	// First - draw mask, then active button background
	CUSTOM_BITMAP mask_bkg;
	mask_bkg.LoadFromFile(maskPath.c_str());
	mask_bkg.Draw(
		hMemDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		button.get_width(), button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		mask_bkg.GetWidth(), mask_bkg.GetHeight(),
		// Raster-operation code
		SRCAND
	);
	background.Draw(
		hMemDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		button.get_width(), button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		background.GetWidth(), background.GetHeight(),
		// Raster-operation code
		SRCINVERT
	);

	BitBlt(hDC, 0, 0, background.GetWidth(), background.GetHeight(), hMemDC, 0, 0, SRCCOPY);

	SelectObject(hDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);
}
//====================================================================
void draw_submods_checkbox_btn_background(HWND hWnd, HDC hDC)
{
	using SUBMOD_BTN = SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON;
	SUBMOD_BTN& button = get_submod_button_prop(hWnd);

	std::string bkgPath;
	std::string background_file_name;
	
	if (button.get_buffer_button().m_isChecked)
	{
		background_file_name = SUBMODS_CHECKED_BUTTON_BKG;
		get_submods_checkbox_path(bkgPath);
		bkgPath.append(background_file_name);
	}
	else
	{
		background_file_name = SUBMODS_UNCHECKED_BUTTON_BKG;
		get_submods_checkbox_path(bkgPath);
		bkgPath.append(background_file_name);
	}


	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - button.get_yPos() - button.get_height();

	// Get parent BITMAP background
	SUBMODS_MODAL_WINDOW& submods_window = SUBMODS_MODAL_WINDOW::getInstance();
	CUSTOM_BITMAP& submods_window_background = submods_window.get_background();

	StretchDIBits(
		hDC, 0, 0, button.get_width(), button.get_height(),
		button.get_xPos(), translated_yPos, button.get_width(), button.get_height(),
		submods_window_background.GetBitmapBits(),
		(BITMAPINFO*)submods_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());
	if (!background.CreateBkgMask(get_submods_checkbox_mask_path))
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath;
	get_submods_checkbox_mask_path(maskPath);
	maskPath.append(background_file_name);

	// First draw mask, then background
	CUSTOM_BITMAP mask_bkg;
	mask_bkg.LoadFromFile(maskPath.c_str());
	mask_bkg.Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		button.get_width(), button.get_height(),
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
		button.get_width(), button.get_height(),
		// Source x and y
		0, 0,
		// Source width and height
		background.GetWidth(), background.GetHeight(),
		// Raster-operation code
		SRCINVERT
	);
}
//====================================================================