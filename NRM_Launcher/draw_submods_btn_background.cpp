#include "submods_buttons.h"
#include "submods_modal.h"
#include "bitmap.h"
#include "get_file_path.h"

//====================================================================
std::map<uint32_t, BUTTON_POSITION> submodsBtnsBkgPaths;
//====================================================================
void draw_submods_btn_background(HWND hWnd, HDC hDC)
{
	uint32_t* btnType = (uint32_t*)GetProp(hWnd, TEXT("buttonType"));
	BUTTON_POSITION btnInfo;
	std::string bkgPath;
	
	try
	{
		btnInfo = submodsBtnsBkgPaths.at(*btnType);
		get_submods_btns_bkg_path(bkgPath);
		bkgPath.append(btnInfo.btnBkgName.c_str());
	}
	catch (std::out_of_range& e)
	{
		btnInfo.pos_x = 0;
		btnInfo.pos_y = 0;
		btnInfo.width = 0;
		btnInfo.height = 0;
		bkgPath = "";
	}

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - btnInfo.pos_y - btnInfo.height;

	HWND parent = SUBMODS_MODAL_WINDOW::getInstance().getHWnd();

	StretchDIBits(
		hDC, 0, 0, btnInfo.width, btnInfo.height,
		btnInfo.pos_x, translated_yPos, btnInfo.width, btnInfo.height,
		GetProp(parent, TEXT("BitmapBits")),
		(BITMAPINFO*)GetProp(parent, TEXT("InfoHeader")),
		DIB_RGB_COLORS, SRCCOPY
	);

	LPRECT rect = (RECT*)malloc(sizeof(RECT));
	if (!rect)
	{
		MessageBox(hWnd, TEXT("Can not allocate memory for rectangle"), TEXT("ERROR"), 0);
		return;
	}
	GetClientRect(hWnd, rect);

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());
	if (!background.CreateBkgMask(get_submods_btns_mask_path))
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath;
	get_submods_btns_mask_path(maskPath);
	maskPath.append(btnInfo.btnBkgName.c_str());

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
void draw_submods_active_btn_background(HWND hWnd, HDC hDC)
{
	uint32_t* btnType = (uint32_t*)GetProp(hWnd, TEXT("buttonType"));
	BUTTON_POSITION btnInfo;
	std::string bkgPath;

	try
	{
		btnInfo = submodsBtnsBkgPaths.at(*btnType);
	}
	catch (std::out_of_range& e)
	{
		btnInfo.pos_x = 0;
		btnInfo.pos_y = 0;
		btnInfo.width = 0;
		btnInfo.height = 0;
		bkgPath = "";
	}

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
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - btnInfo.pos_y - btnInfo.height;
	StretchDIBits(
		hMemDC, 0, 0, btnInfo.width, btnInfo.height,
		btnInfo.pos_x, translated_yPos, btnInfo.width, btnInfo.height,
		GetProp(GetParent(hWnd), TEXT("BitmapBits")),
		(BITMAPINFO*)GetProp(GetParent(hWnd), TEXT("InfoHeader")),
		DIB_RGB_COLORS, SRCCOPY
	);

	// Get button active background path
	get_submods_active_btns_bkg_path(bkgPath);
	bkgPath.append(btnInfo.btnBkgName.c_str());

	CUSTOM_BITMAP background;
	background.LoadFromFile(bkgPath.c_str());

	// Get button mask background path
	std::string maskPath;
	get_submods_btns_mask_path(maskPath);
	maskPath.append(btnInfo.btnBkgName.c_str());

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
//====================================================================
void draw_submods_checkbox_btn_background(HWND hWnd, HDC hDC, SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON& button_class)
{
	std::string bkgPath;
	std::string background_file_name;

	if (!button_class.is_checked())
	{
		background_file_name = SUBMODS_UNCHECKED_BUTTON_BKG;
		get_submods_checkbox_path(bkgPath);
		bkgPath.append(background_file_name);
	}
	else
	{
		background_file_name = SUBMODS_CHECKED_BUTTON_BKG;
		get_submods_checkbox_path(bkgPath);
		bkgPath.append(background_file_name);
	}


	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = SUBMODS_WINDOW_HEIGHT - button_class.get_yPos() - button_class.get_height();

	HWND parent = SUBMODS_MODAL_WINDOW::getInstance().getHWnd();

	StretchDIBits(
		hDC, 0, 0, button_class.get_width(), button_class.get_height(),
		button_class.get_xPos(), translated_yPos, button_class.get_width(), button_class.get_height(),
		GetProp(parent, TEXT("BitmapBits")),
		(BITMAPINFO*)GetProp(parent, TEXT("InfoHeader")),
		DIB_RGB_COLORS, SRCCOPY
	);

	LPRECT rect = (RECT*)malloc(sizeof(RECT));
	if (!rect)
	{
		MessageBox(hWnd, TEXT("Can not allocate memory for rectangle"), TEXT("ERROR"), 0);
		return;
	}
	GetClientRect(hWnd, rect);

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