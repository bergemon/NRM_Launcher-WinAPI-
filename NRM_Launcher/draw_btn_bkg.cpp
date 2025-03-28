#include "button.h"
#include "bitmap.h"
#include "main_window.h"
#include "get_file_path.h"

//====================================================================
void draw_button_background(HWND hWnd, HDC hDC)
{
	LAUNCHER_BUTTONS::BUTTON& button = *(LAUNCHER_BUTTONS::BUTTON*)GetProp(hWnd, TEXT("buttonClass"));
	std::string bkgPath;

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = MAIN_WINDOW_HEIGHT - button.get_posY() - button.getHeight();
	
	// Main window bakground
	MAIN_WINDOW& main_window = MAIN_WINDOW::getInstance();
	CUSTOM_BITMAP& main_window_background = main_window.get_background();
	StretchDIBits(
		hDC, 0, 0, button.getWidth(), button.getHeight(),
		button.get_posX(), translated_yPos, button.getWidth(), button.getHeight(),
		main_window_background.GetBitmapBits(),
		(BITMAPINFO*)main_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	LPRECT rect = (RECT*)malloc(sizeof(RECT));
	if (!rect)
	{
		MessageBox(hWnd, TEXT("Can not allocate memory for rectangle"), TEXT("ERROR"), 0);
		return;
	}
	GetClientRect(hWnd, rect);

	// Get button background path
	get_button_bkg_path(bkgPath);
	bkgPath.append(button.get_background_file_name());

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());
	if (!background.CreateBkgMask(get_button_mask_path))
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath;
	get_button_mask_path(maskPath);
	maskPath.append(button.get_background_file_name());

	// First draw mask, then background
	CUSTOM_BITMAP mask_bkg;
	mask_bkg.LoadFromFile(maskPath.c_str());
	mask_bkg.Draw(
		hDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		rect->right, rect->bottom,
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
		rect->right, rect->bottom,
		// Source x and y
		0, 0,
		// Source width and height
		background.GetWidth(), background.GetHeight(),
		// Raster-operation code
		SRCINVERT
	);

	free(rect);
}
//====================================================================
void draw_active_button_background(HWND hWnd, HDC hDC)
{
	LAUNCHER_BUTTONS::BUTTON& button = *(LAUNCHER_BUTTONS::BUTTON*)GetProp(hWnd, TEXT("buttonClass"));
	std::string bkgPath;

	LPRECT rect = (RECT*)malloc(sizeof(RECT));
	if (!rect)
	{
		MessageBox(hWnd, TEXT("Can not allocate memory for rectangle"), TEXT("ERROR"), 0);
		return;
	}
	GetClientRect(hWnd, rect);

	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rect->right, rect->bottom);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = MAIN_WINDOW_HEIGHT - button.get_posY() - button.getHeight();

	// Main window bakground
	MAIN_WINDOW& main_window = MAIN_WINDOW::getInstance();
	CUSTOM_BITMAP& main_window_background = main_window.get_background();
	StretchDIBits(
		hMemDC, 0, 0, button.getWidth(), button.getHeight(),
		button.get_posX(), translated_yPos, button.getWidth(), button.getHeight(),
		main_window_background.GetBitmapBits(),
		(BITMAPINFO*)main_window_background.GetInfoHeader(),
		DIB_RGB_COLORS, SRCCOPY
	);

	// Get button active background path
	get_button_active_bkg_path(bkgPath);
	bkgPath.append(button.get_background_file_name());

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());

	// Get button mask background path
	std::string maskPath;
	get_button_mask_path(maskPath);
	maskPath.append(button.get_background_file_name());

	// First - draw mask, then active button background
	CUSTOM_BITMAP mask_bkg;
	mask_bkg.LoadFromFile(maskPath.c_str());
	mask_bkg.Draw(
		hMemDC,
		// Destination x and y
		0, 0,
		// Destination width and height
		rect->right, rect->bottom,
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
		rect->right, rect->bottom,
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