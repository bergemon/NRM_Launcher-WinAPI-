#include "download_buttons.h"
#include "download_modal.h"
#include "bitmap.h"
#include "get_file_path.h"

//====================================================================
void draw_download_btn_background(HWND hWnd, HDC hDC)
{
	DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON& button = get_download_button_prop(hWnd);
	std::string bkgPath;

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = DOWNLOAD_WINDOW_HEIGHT - button.get_yPos() - button.get_height();

	// Get submod window background image file path
	get_download_btns_bkg_path(bkgPath);
	bkgPath.append(button.get_background_file_name());

	// Get parent BITMAP background
	DOWNLOAD_MODAL_WINDOW& download_window = DOWNLOAD_MODAL_WINDOW::getInstance();
	CUSTOM_BITMAP& download_window_background = download_window.get_background();

	StretchDIBits(
		hDC, 0, 0, button.get_width(), button.get_height(),
		button.get_xPos(), translated_yPos, button.get_width(), button.get_height(),
		download_window_background.GetBitmapBits(),
		(BITMAPINFO*)download_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());
	if (!background.CreateBkgMask(get_download_btns_mask_path))
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath;
	get_download_btns_mask_path(maskPath);
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