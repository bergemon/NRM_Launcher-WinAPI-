#include "main_window.h"
#include "resource.h"
#include "button.h"

//====================================================================
MAIN_WINDOW& MAIN_WINDOW::getInstance()
{
	static MAIN_WINDOW main_window;
	return main_window;
}
//====================================================================
MAIN_WINDOW::MAIN_WINDOW()
{
}
//====================================================================
void MAIN_WINDOW::initialize(
	LPCWSTR className,
	LPCWSTR windowTitle,
	HINSTANCE hInstance,
	int nCmdShow,
	LPCWSTR menuName,
	uint32_t width,
	uint32_t height,
	const char* bkgFileName
) noexcept(false)
{

	if (m_initialized)
	{
		throw std::exception("Main window is already initialized");
	}

	// Init main window background BITMAP
	m_strBkgFileName = bkgFileName;
	std::string bkgPath;
	get_main_bkg_path(bkgPath);
	bkgPath.append(m_strBkgFileName.c_str());
	m_backrgound.LoadFromFile(bkgPath.c_str());

	WNDCLASSEX* window_class = (WNDCLASSEX*)malloc(sizeof(WNDCLASSEX));
	if (window_class == NULL)
	{
		throw std::exception("Can not allocate memory for WNDCLASSEX structure");
	}

	HICON hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_NRMICON));
	if (!hIcon)
	{
		throw std::exception("Can not load icon");
	}

	window_class->cbSize = sizeof(*window_class);
	window_class->lpszClassName = className;
	window_class->lpszMenuName = menuName;
	window_class->style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	window_class->lpfnWndProc = this->WndProc;
	window_class->hInstance = hInstance;
	window_class->hIcon = hIcon;
	window_class->hIconSm = hIcon;
	window_class->hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL));
	window_class->hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	window_class->cbClsExtra = 0;
	window_class->cbWndExtra = 0;

	if (!RegisterClassEx(window_class))
	{
		throw std::exception("Can not register window class");
		return;
	}

	HWND desktopWindow = GetDesktopWindow();
	RECT* desktopRect = (RECT*)malloc(sizeof(RECT));
	if (!desktopRect)
	{
		throw std::exception("Can not allocate memory for window reactangle");
		return;
	}
	bool get_rect_res = GetWindowRect(desktopWindow, desktopRect);
	if (!get_rect_res)
	{
		throw std::exception("Can not get desktop window reactangle");
		return;
	}

	m_hWnd = CreateWindowEx(
		// WindowExStyles
		WS_EX_TRANSPARENT,
		// UTF-16 name of the creating class
		className,
		// Window title
		windowTitle,
		// Window styles
		WS_BORDER | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		// Destination
		(desktopRect->right - width) / 2,
		(desktopRect->bottom - height) / 2,
		// Window width, height
		width, height,
		// Main window don't have parent
		(HWND)NULL,
		// Menu
		(HMENU)NULL,
		// hInstance
		window_class->hInstance,
		// lpParam
		NULL
	);

	DeleteObject(window_class->hbrBackground);
	DestroyIcon(hIcon);
	free(window_class);
	free(desktopRect);

	if (!m_hWnd)
	{
		throw std::exception("Can not create main window");
	}

	m_initialized = true;

	//ShowWindow(m_hMainWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);
}
MAIN_WINDOW::~MAIN_WINDOW()
{
}
//====================================================================
void MAIN_WINDOW::setHWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
//====================================================================
HWND MAIN_WINDOW::getHWnd()
{
	if (!IsWindow(m_hWnd))
	{
		MessageBox(NULL, TEXT("Main window handle is not valid"), TEXT("ERROR"), MB_OK);
	}

	return m_hWnd;
}
//====================================================================
CUSTOM_BITMAP& MAIN_WINDOW::get_background()
{
	return m_backrgound;
}
//====================================================================
const std::string_view MAIN_WINDOW::get_bkg_filename()
{
	return std::string_view(m_strBkgFileName);
}
//====================================================================