#include "download_buttons.h"

//====================================================================
DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTONS()
	: m_initialized(false), m_parent(nullptr),
	m_yCurrentOffset(DOWNLOAD_BUTTONS_Y_START)
{
	m_vectButtons.reserve(5);
}
//====================================================================
DOWNLOAD_WINDOW_BUTTONS::~DOWNLOAD_WINDOW_BUTTONS()
{
}
//====================================================================
DOWNLOAD_WINDOW_BUTTONS& DOWNLOAD_WINDOW_BUTTONS::getInstance() noexcept(true)
{
	static DOWNLOAD_WINDOW_BUTTONS download_buttons;
	return download_buttons;
}
//====================================================================
void DOWNLOAD_WINDOW_BUTTONS::initialize(HWND hParent, LPCWSTR className) noexcept(false)
{
	if (m_initialized)
	{
		throw std::runtime_error("Download buttons class is already initialized");
	}

	WNDCLASSEX* window_class = (WNDCLASSEX*)malloc(sizeof(WNDCLASSEX));

	if (window_class == NULL)
	{
		throw std::runtime_error("Cannot allocate memory for WNDCLASSEX structure");
		ExitProcess(1);
	}

	m_className = className;
	m_parent = hParent;

	window_class->cbSize = sizeof(*window_class);
	window_class->lpszClassName = m_className.c_str();
	window_class->lpszMenuName = NULL;
	window_class->style = CS_DBLCLKS;
	window_class->lpfnWndProc = this->WndProc;
	window_class->hInstance = (HINSTANCE)GetWindowLongPtr(m_parent, GWLP_HINSTANCE);
	window_class->hIcon = NULL;
	window_class->hIconSm = NULL;
	window_class->hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_NORMAL));
	window_class->hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	window_class->cbClsExtra = 0;
	window_class->cbWndExtra = 0;

	if (!RegisterClassEx(window_class))
	{
		throw std::runtime_error("Cannot register window class");
		ExitProcess(1);
	}

	free(window_class);
	m_initialized = true;
}
//====================================================================
[[nodiscard]] HWND DOWNLOAD_WINDOW_BUTTONS::getParent() const noexcept(true)
{
	return m_parent;
}
//====================================================================
void DOWNLOAD_WINDOW_BUTTONS::create_button(
	BUTTON_PROPERTIES props,
	DOWNLOAD_BUTTON_TYPE btnType,
	const char* bkgFileName,
	uint32_t x,
	uint32_t y
) noexcept(false)
{
	if (!m_initialized)
	{
		throw std::runtime_error("Download buttons window class not initialized!");
	}

	if (!y && DOWNLOAD_BUTTON_TYPE::DOWNLOAD_STATIC_BUTTON)
	{
		y = m_yCurrentOffset;
		m_yCurrentOffset += DOWNLOAD_BUTTONS_Y_SPACE_BETWEEN + DOWNLOAD_BUTTONS_HEIGHT;
	}
	else if (!y && DOWNLOAD_BUTTON_TYPE::DOWNLOAD_PROGRESS_BAR)
	{
		y = m_yCurrentOffset;
		m_yCurrentOffset += DOWNLOAD_BUTTONS_PROGRESSBAR_TOP_MARGIN + DOWNLOAD_PROGRESSBAR_HEIGHT;
	}

	if (!x && btnType == DOWNLOAD_BUTTON_TYPE::DOWNLOAD_STATIC_BUTTON)
	{
		x = DOWNLOAD_BUTTONS_X_OFFSET;
	}
	else if (!x && btnType == DOWNLOAD_BUTTON_TYPE::DOWNLOAD_PROGRESS_BAR)
	{
		x = DOWNLOAD_PROGRESSBAR_X_OFFSET;
	}

	m_vectButtons.emplace_back(
		m_parent, m_className,
		// Button type - progress bar or static text
		btnType,
		// Button background image file name
		bkgFileName,
		// x and y pos
		x, y,
		// width and height of the button
		props.width, props.height
	);
}
//====================================================================
std::vector<DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON>&
	DOWNLOAD_WINDOW_BUTTONS::get_download_buttons() noexcept(true)
{
	return m_vectButtons;
}
//====================================================================
DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON& get_download_button_prop(HWND hWnd)
	noexcept(true)
{
	return *(DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON*)GetProp(hWnd, TEXT("downloadButtonClass"));
}
//====================================================================
DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::DOWNLOAD_WINDOW_BUTTON(
	HWND parent,
	std::wstring_view className,
	DOWNLOAD_BUTTON_TYPE btnType,
	const char* background_file_name,
	uint32_t x,
	uint32_t y,
	uint32_t width,
	uint32_t height
) noexcept(false) : m_width(width), m_height(height),
	m_className(className), m_xPos(x), m_yPos(y),
	m_bkgFileName(), m_btnType(btnType)
{
	if (btnType == DOWNLOAD_BUTTON_TYPE::DOWNLOAD_STATIC_BUTTON)
	{
		m_bkgFileName = background_file_name;
	}

	std::wstring classname;
	if (btnType == DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_STATIC_BUTTON)
	{
		classname = m_className;
	}
	else if (btnType == DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_PROGRESS_BAR)
	{
		classname = PROGRESS_CLASS;
	}

	long window_styles;
	window_styles = WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON;

	if (btnType == DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_PROGRESS_BAR)
	{
		window_styles = window_styles | PBS_SMOOTH;
	}

	m_hBtnWnd = CreateWindowEx(
		// WindowExStyles
		NULL,
		// Class name and title bar text
		classname.c_str(), TEXT(""),
		// Window styles
		window_styles,
		// Destination
		x, y,
		// Width and height of the creating window
		width, height,
		// Parent window's handle
		parent,
		// Menu
		(HMENU)NULL,
		// hInstance
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		// lpParam
		NULL
	);

	SetProp(m_hBtnWnd, TEXT("downloadButtonClass"), this);

	if (!m_hBtnWnd)
	{
		throw std::exception("Can not create download static button");
	}

	//ShowWindow(m_hMainWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hBtnWnd);
}
//====================================================================
DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::~DOWNLOAD_WINDOW_BUTTON()
{
}
//====================================================================
HWND DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::get_hWnd()
	const noexcept(true)
{
	return m_hBtnWnd;
}
//====================================================================
uint32_t DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::get_xPos()
	const noexcept(true)
{
	return m_xPos;
}
//====================================================================
uint32_t DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::get_yPos()
	const noexcept(true)
{
	return m_yPos;
}
//====================================================================
uint32_t DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::get_width()
	const noexcept(true)
{
	return m_width;
}
//====================================================================
uint32_t DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::get_height()
	const noexcept(true)
{
	return m_height;
}
//====================================================================
DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_BUTTON_TYPE
	DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::get_button_type()
const noexcept(true)
{
	return m_btnType;
}
//====================================================================
std::string_view DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::get_background_file_name()
	noexcept(true)
{
	return m_bkgFileName;
}
//====================================================================
void DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::show() noexcept(true)
{
	ShowWindow(m_hBtnWnd, SW_SHOW);
}
//====================================================================
void DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON::hide() noexcept(true)
{
	ShowWindow(m_hBtnWnd, SW_HIDE);
}
//====================================================================