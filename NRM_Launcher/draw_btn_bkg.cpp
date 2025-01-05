#include "button.h"
#include "bitmap.h"

//====================================================================
std::map<uint32_t, BUTTON_POSITION> btnsBkgPaths;
//====================================================================
void draw_button_background(HWND hWnd, HDC hDC)
{
	uint32_t* btnType = (uint32_t*)GetProp(hWnd, TEXT("buttonType"));
	BUTTON_POSITION btnPos;
	std::string bkgPath = "";
	try
	{
		btnPos = btnsBkgPaths.at(*btnType);
		bkgPath = btnPos.btnBkgPath.c_str();
	}
	catch (std::out_of_range& e)
	{
		btnPos.pos_x = 0;
		btnPos.pos_y = 0;
		btnPos.width = 0;
		btnPos.height = 0;
		bkgPath = "";
	}

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = MAIN_WINDOW_HEIGHT - btnPos.pos_y - btnPos.height;
	StretchDIBits(
		hDC, 0, 0, btnPos.width, btnPos.height,
		btnPos.pos_x, translated_yPos, btnPos.width, btnPos.height,
		GetProp(GetParent(hWnd), TEXT("BitmapBits")),
		(BITMAPINFO*)GetProp(GetParent(hWnd), TEXT("InfoHeader")),
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
	if (!background.CreateBkgMask())
	{
		MessageBox(hWnd, TEXT("Can not create mask for a button background"), TEXT("Error"), 0);
	}

	std::string maskPath = APP_DIR SLH MASK_PREFIX;
	maskPath.append(bkgPath.substr(bkgPath.find("\\") + 1, bkgPath.size() - 1 - bkgPath.find("\\")));
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
	uint32_t* btnType = (uint32_t*)GetProp(hWnd, TEXT("buttonType"));
	BUTTON_POSITION btnPos;
	std::string bkgPath = "";
	try
	{
		btnPos = btnsBkgPaths.at(*btnType);
		bkgPath = btnPos.btnBkgPath.c_str();
	}
	catch (std::out_of_range& e)
	{
		btnPos.pos_x = 0;
		btnPos.pos_y = 0;
		btnPos.width = 0;
		btnPos.height = 0;
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
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(
		hMemDC,
		CreateCompatibleBitmap(hDC, rect->right, rect->bottom)
	);

	// Need to translate, y = 0 starts from bottom
	int32_t translated_yPos = MAIN_WINDOW_HEIGHT - btnPos.pos_y - btnPos.height;
	StretchDIBits(
		hMemDC, 0, 0, btnPos.width, btnPos.height,
		btnPos.pos_x, translated_yPos, btnPos.width, btnPos.height,
		GetProp(GetParent(hWnd), TEXT("BitmapBits")),
		(BITMAPINFO*)GetProp(GetParent(hWnd), TEXT("InfoHeader")),
		DIB_RGB_COLORS, SRCCOPY
	);

	CUSTOM_BITMAP background;
	bkgPath = APP_DIR;
	bkgPath.append(SLH)
		.append(BTN_PREFIX)
		.append(ACTIVE_PREFIX)
		.append(
			btnPos.btnBkgPath.substr(
				btnPos.btnBkgPath.find("\\") + 1 + BTN_PREFIX_LENGTH,
				btnPos.btnBkgPath.size() - 1 - btnPos.btnBkgPath.find("\\") - BTN_PREFIX_LENGTH
			)
		);
	background.LoadFromFile(bkgPath.c_str());

	std::string maskPath = APP_DIR SLH MASK_PREFIX BTN_PREFIX;
	maskPath.append(
		bkgPath.substr(
			bkgPath.find("\\") + 1 + BTN_PREFIX_LENGTH + ACTIVE_PREFIX_LENGTH,
			bkgPath.size() - 1 - bkgPath.find("\\") - BTN_PREFIX_LENGTH - ACTIVE_PREFIX_LENGTH
		)
	);

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

	DeleteObject(SelectObject(hDC, hOldBitmap));
	DeleteDC(hMemDC);
}