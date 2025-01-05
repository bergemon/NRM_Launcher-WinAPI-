#include "main_window.h"
#include "resource.h"
#include "button.h"

//====================================================================
std::string MainWindow::m_strBkgFileName;
//====================================================================
MainWindow::MainWindow(
	LPCWSTR className,
	LPCWSTR windowTitle,
	HINSTANCE hInstance,
	int nCmdShow,
	LPCWSTR menuName,
	uint32_t classStyle,
	uint32_t width,
	uint32_t height,
	DWORD windowStyle,
	DWORD windowExStyle,
	const char* bkgFileName
)
{
	m_strBkgFileName = bkgFileName;

	WNDCLASSEX* window_class = (WNDCLASSEX*)malloc(sizeof(WNDCLASSEX));

	if (window_class == NULL)
	{
		MessageBox(NULL, L"Can not allocate memory for WNDCLASSEX structure", L"Error", MB_OK);
		return;
	}

	HICON hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_NRMICON));
	if (!hIcon) {
		MessageBox(NULL, L"Can not load icon", L"Error", MB_OK);
	}

	window_class->cbSize = sizeof(*window_class);
	window_class->lpszClassName = className;
	window_class->lpszMenuName = menuName;
	window_class->style = classStyle;
	window_class->lpfnWndProc = this->WndProc;
	window_class->hInstance = hInstance;
	window_class->hIcon = hIcon;
	window_class->hIconSm = hIcon;
	window_class->hCursor = LoadCursorW(NULL, IDC_ARROW);
	window_class->hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	window_class->cbClsExtra = 0;
	window_class->cbWndExtra = 0;

	if (!RegisterClassEx(window_class))
	{
		MessageBox(NULL, L"Cannot register window class", L"Error", MB_OK);
		return;
	}

	HWND desktopWindow = GetDesktopWindow();
	RECT* desktopRect = (RECT*)malloc(sizeof(RECT));
	if (!desktopRect)
	{
		MessageBox(NULL, L"Cannot allocate memory for window reactangle", L"Error", MB_OK);
		return;
	}
	bool get_rect_res = GetWindowRect(desktopWindow, desktopRect);
	if (!get_rect_res)
	{
		MessageBox(NULL, L"Cannot get desktop window reactangle", L"Error", MB_OK);
		return;
	}

	m_hMainWnd = CreateWindowExW(
		// WindowExStyles
		windowExStyle,
		// UTF-16 name of the creating class
		className,
		// Window title
		windowTitle,
		// Window styles
		windowStyle,
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

	free(window_class);
	free(desktopRect);

	if (!m_hMainWnd)
	{
		MessageBox(NULL, TEXT("Cannot create main window"), TEXT("Error"), MB_OK);
		return;
	}

	ShowWindow(m_hMainWnd, nCmdShow);
	UpdateWindow(m_hMainWnd);
}
//====================================================================
MainWindow::~MainWindow()
{
}
//====================================================================