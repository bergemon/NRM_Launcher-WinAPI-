#include "submods_modal.h"
#include "resource.h"
#include "const.h"
#include "parse_submods.h"
#include "submods_utils.h"
#include "submods_buttons.h"
#include "get_file_path.h"

//====================================================================
extern std::list<PARSED_SUBMOD> parsed_submods;
//====================================================================
[[nodiscard]] SUBMODS_MODAL_WINDOW& SUBMODS_MODAL_WINDOW::getInstance()
{
	static SUBMODS_MODAL_WINDOW submodsModal;

	return submodsModal;
}
//====================================================================
HWND SUBMODS_MODAL_WINDOW::getHWnd()
{
	return m_hSubmodsWND;
}
//====================================================================
std::string_view SUBMODS_MODAL_WINDOW::get_background_path()
{
	return m_bkgPath;
}
//====================================================================
CUSTOM_BITMAP& SUBMODS_MODAL_WINDOW::get_background()
{
	return m_background;
}
//====================================================================
void SUBMODS_MODAL_WINDOW::initialize(
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
		throw std::exception("Can not register submods window class");
	}

	// Set background path before creating window
	m_bkgPath = bkgFileName;

	m_hSubmodsWND = CreateWindowEx(
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

	if (!m_hSubmodsWND)
	{
		throw std::exception("Can not create submods modal window");
	}

	// Initialize submods window background BITMAP
	std::string bkgPath;
	get_submods_bkg_path(bkgPath);
	bkgPath.append(m_bkgPath);
	m_background.LoadFromFile(bkgPath.c_str());

	m_hParentWND = hParent;
	m_initialized = true;

	try
	{
		parse_submods();
	}
	catch (std::exception& e)
	{
		MessageBoxA(hParent, e.what(), "Error", MB_OK);
	}
}
//====================================================================
void SUBMODS_MODAL_WINDOW::show()
{
	if (m_initialized)
	{
		LPRECT rect = (RECT*)malloc(sizeof(RECT));

		if (!rect)
		{
			MessageBox(NULL, TEXT("Can not allocate memory"), TEXT("Error"), MB_OKCANCEL);
			std::exit(1);
		}

		// Get submods window rect
		GetWindowRect(m_hSubmodsWND, rect);
		uint32_t x_pos = rect->left;
		uint32_t y_pos = rect->top;

		// Then get main window rect and compare positions
		GetWindowRect(m_hParentWND, rect);

		if (x_pos != rect->left && y_pos != rect->top)
		{
			MoveWindow(
				// Window handle
				m_hSubmodsWND,
				// X pos
				rect->left + SUBMODS_WINDOW_X_OFFSET,
				// Y pos
				rect->top + SUBMODS_WINDOW_Y_OFFSET,
				// Width & height
				SUBMODS_WINDOW_WIDTH, SUBMODS_WINDOW_HEIGHT,
				// Redraw
				FALSE
			);
		}

		ShowWindow(m_hSubmodsWND, SW_SHOW);
		EnableWindow(m_hParentWND, FALSE);
		SetFocus(m_hSubmodsWND);

		free(rect);
	}
}
//====================================================================
void SUBMODS_MODAL_WINDOW::hide()
{
	if (m_initialized)
	{
		ShowWindow(m_hSubmodsWND, SW_HIDE);
		EnableWindow(m_hParentWND, TRUE);
		SetActiveWindow(m_hParentWND);
		SetFocus(m_hParentWND);
		BringWindowToTop(m_hParentWND);
	}
}
//====================================================================
SUBMODS_MODAL_WINDOW::SUBMODS_MODAL_WINDOW()
{
}
//====================================================================
SUBMODS_MODAL_WINDOW::~SUBMODS_MODAL_WINDOW()
{
}
//====================================================================