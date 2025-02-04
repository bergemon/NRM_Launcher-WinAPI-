#include "settings_modal.h"
#include "get_file_path.h"

//====================================================================
[[nodiscard]] SETTINGS_MODAL_WINDOW& SETTINGS_MODAL_WINDOW::getInstance()
{
	static SETTINGS_MODAL_WINDOW settings_window;
	return settings_window;
}
//====================================================================
CUSTOM_BITMAP& SETTINGS_MODAL_WINDOW::get_background()
{
	return m_background;
}
//====================================================================
std::string_view SETTINGS_MODAL_WINDOW::get_background_path()
{
	return m_bkgFileName;
}
//====================================================================
HWND SETTINGS_MODAL_WINDOW::getHWnd()
{
	return m_hSettingsWND;
}
//====================================================================
void SETTINGS_MODAL_WINDOW::initialize(
	HWND hParent,
	LPCWSTR className,
	LPCWSTR windowTitle,
	HINSTANCE hInstance,
	int nCmdShow,
	LPCWSTR menuName,
	uint32_t width,
	uint32_t height,
	const char* bkgFileName
)
{
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
	window_class->style = CS_DBLCLKS;
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
		throw std::exception("Can not register settings window class");
	}

	// Set background path before creating window
	m_bkgFileName = bkgFileName;

	m_hSettingsWND = CreateWindowEx(
		// WindowExStyles
		WS_EX_TRANSPARENT,
		// UTF-16 name of the creating class
		className,
		// Window title
		windowTitle,
		// Window styles
		WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU,
		// Destination x & y
		CW_USEDEFAULT, 0,
		// Window width, height
		width, height,
		// Parent (must be main window)
		hParent,
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

	if (!m_hSettingsWND)
	{
		throw std::exception("Can not create settings modal window");
	}

	// Initialize settings window background BITMAP
	std::string bkgPath;
	get_settings_bkg_path(bkgPath);
	bkgPath.append(m_bkgFileName);
	m_background.LoadFromFile(bkgPath.c_str());

	m_hParentWND = hParent;
	m_initialized = true;
}
//====================================================================
void SETTINGS_MODAL_WINDOW::show()
{
	if (m_initialized)
	{
		LPRECT rect = (RECT*)malloc(sizeof(RECT));

		if (!rect)
		{
			MessageBox(NULL, TEXT("Can not allocate memory"), TEXT("Error"), MB_OKCANCEL);
			std::exit(1);
		}

		// Get settings window rect
		GetWindowRect(m_hSettingsWND, rect);
		uint32_t x_pos = rect->left;
		uint32_t y_pos = rect->top;

		// Then get main window rect and compare positions
		GetWindowRect(m_hParentWND, rect);

		if (x_pos != rect->left && y_pos != rect->top)
		{
			MoveWindow(
				// Window handle
				m_hSettingsWND,
				// X pos
				rect->left + SETTINGS_WINDOW_X_OFFSET,
				// Y pos
				rect->top + SETTINGS_WINDOW_Y_OFFSET,
				// Width & height
				SETTINGS_WINDOW_WIDTH, SETTINGS_WINDOW_HEIGHT,
				// Redraw
				FALSE
			);
		}

		ShowWindow(m_hSettingsWND, SW_SHOW);
		EnableWindow(m_hParentWND, FALSE);
		SetFocus(m_hSettingsWND);

		free(rect);
	}
}
//====================================================================
void SETTINGS_MODAL_WINDOW::hide()
{
	if (m_initialized)
	{
		ShowWindow(m_hSettingsWND, SW_HIDE);
		EnableWindow(m_hParentWND, TRUE);
		SetActiveWindow(m_hParentWND);
		SetFocus(m_hParentWND);
		BringWindowToTop(m_hParentWND);
	}
}
//====================================================================
SETTINGS_MODAL_WINDOW::SETTINGS_MODAL_WINDOW()
{
}
//====================================================================
SETTINGS_MODAL_WINDOW::~SETTINGS_MODAL_WINDOW()
{
}
//====================================================================