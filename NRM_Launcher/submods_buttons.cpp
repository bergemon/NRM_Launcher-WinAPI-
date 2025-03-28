#include "submods_buttons.h"
#include "resource.h"

//====================================================================
[[nodiscard]] SUBMODS_BUTTONS& SUBMODS_BUTTONS::getInstance()
{
	static SUBMODS_BUTTONS btns;
	return btns;
}
//====================================================================
[[nodiscard]] HWND SUBMODS_BUTTONS::getParent() const
{
	return m_parent;
}
//====================================================================
SUBMODS_BUTTONS::SUBMODS_BUTTONS() : m_initialized(false), m_parent(nullptr)
{
}
//====================================================================
SUBMODS_BUTTONS::~SUBMODS_BUTTONS()
{
}
//====================================================================
std::list<SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON>& SUBMODS_BUTTONS::get_submods_buttons()
{
	return m_listButtons;
}
//====================================================================
std::vector<SUBMODS_BUTTONS::SUBMOD_BUTTON_BUFFER>& SUBMODS_BUTTONS::get_submods_buttons_buffer()
{
	return m_submodButtonsBuffer;
}
//====================================================================
void SUBMODS_BUTTONS::clear_submod_buttons()
{
	m_listButtons.remove_if(
		[&] (SUBMODS_WINDOW_BUTTON& btn)
		{
			if (btn.get_button_type() == SUBMODS_BUTTON_TYPE::BTN_SUBMOD)
			{
				lower_y_offset(btn.get_height(), btn.get_exPadding());
				ShowWindow(btn.get_hWnd(), SW_HIDE);
				return true;
			}
			return false;
		}
	);
}
//====================================================================
void SUBMODS_BUTTONS::initialize(HWND hParent, LPCWSTR className) noexcept(false)
{
	if (m_initialized)
	{
		throw std::runtime_error("Submods buttons class is already initialized");
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
	window_class->hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_SELECTED));
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
bool SUBMODS_BUTTONS::is_initialized() const noexcept(true)
{
	return m_initialized;
}
//====================================================================
void SUBMODS_BUTTONS::create_submod_button(
	BUTTON_PROPERTIES props,
	const char* submod_name,
	const char* submod_path,
	uint32_t exPadding
)
{
	if (!m_initialized)
	{
		throw std::runtime_error("Submods buttons window class not initialized!");
	}

	uint32_t x_coord;
	uint32_t y_coord;

	x_coord = SUBMODS_BUTTONS_X_OFFSET;
	y_coord = m_submodBtnOffset;

	m_listButtons.emplace_back(
		m_parent, m_className,
		// Submod button has no background
		"",
		// Button type
		SUBMODS_BUTTON_TYPE::BTN_SUBMOD,
		// x and y coords
		x_coord, y_coord,
		// width and height
		props.width, props.height,
		// submod name and path
		submod_name, submod_path,
		// button exPadding
		exPadding
	);

	m_submodBtnOffset += props.height + SUBMOD_BUTTONS_Y_PADDING + exPadding;
}
//====================================================================
void SUBMODS_BUTTONS::create_button(
	SUBMODS_BUTTON_TYPE btnType,
	BUTTON_PROPERTIES props,
	const char* bkgFileName,
	uint32_t exPadding,
	uint32_t x,
	uint32_t y
)
{
	if (!m_initialized)
	{
		throw std::runtime_error("Submods buttons window class not initialized!");
	}

	m_listButtons.emplace_back(
		m_parent, m_className,
		// Button background image file name
		bkgFileName,
		// Button type
		btnType,
		// x and y pos
		x, y,
		// width and height of the button
		props.width, props.height
	);
}
//====================================================================
HWND SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_hWnd() const
{
	return m_hBtnWnd;
}
//====================================================================
void SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::set_checked(bool is_checked)
{
	m_checked = is_checked;
}
//====================================================================
SUBMODS_BUTTON_TYPE SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_button_type() const
{
	return m_buttonType;
}
//====================================================================
bool SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::is_checked() const
{
	return m_checked;
}
//====================================================================
uint32_t SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_xPos() const
{
	return m_xPos;
}
//====================================================================
uint32_t SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_yPos() const
{
	return m_yPos;
}
//====================================================================
uint32_t SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_width() const
{
	return m_width;
}
//====================================================================
uint32_t SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_height() const
{
	return m_height;
}
//====================================================================
uint32_t SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_exPadding() const
{
	return m_exPadding;
}
//====================================================================
std::string_view SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_background_file_name()
{
	return m_bkgFileName;
}
//====================================================================
void SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::set_buffer_button(SUBMOD_BUTTON_BUFFER& buff_button)
{
	m_bufferButton = &buff_button;
}
//====================================================================
SUBMODS_BUTTONS::SUBMOD_BUTTON_BUFFER& SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_buffer_button()
{
	return *m_bufferButton;
}
//====================================================================
const std::string& SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_submod_name() const
{
	return m_submod_name;
}
//====================================================================
const std::string& SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::get_submod_path() const
{
	return m_submod_path;
}
//====================================================================
void SUBMODS_BUTTONS::lower_y_offset(int32_t height, uint32_t exPadding) 
{
	m_submodBtnOffset -= (height + SUBMOD_BUTTONS_Y_PADDING + exPadding);
}
//====================================================================
SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::SUBMODS_WINDOW_BUTTON(
	HWND parent,
	std::wstring_view className,
	const char* background_file_name,
	SUBMODS_BUTTON_TYPE btnType,
	uint32_t x,
	uint32_t y,
	uint32_t width,
	uint32_t height,
	const char* submod_name,
	const char* submod_path,
	bool checked,
	uint32_t exPadding
) : m_width(width), m_height(height), m_buttonType(btnType), m_className(className),
	m_submod_name(submod_name), m_submod_path(submod_path), m_checked(checked), m_xPos(x), m_yPos(y),
	m_bkgFileName(background_file_name), m_bufferButton(nullptr), m_exPadding(exPadding)
{
	m_hBtnWnd = CreateWindowEx(
		// WindowExStyles
		NULL,
		// Class name and title bar text
		m_className.data(), TEXT(""),
		// Window styles
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
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

	std::string error("Can not create");
	error.append(" button ");

	switch (btnType)
	{
	case SUBMODS_BUTTON_TYPE::BTN_SAVE:
		error.append("save");
		break;
	case SUBMODS_BUTTON_TYPE::BTN_CANCEL:
		error.append("cancel");
		break;
	case SUBMODS_BUTTON_TYPE::BTN_SUBMOD:
		error.append("submod");
		break;
	default:
		error.append("\"ERROR - CAN NOT DEFINITE BUTTON\"");
		break;
	}
	error.append(".");

	SetProp(m_hBtnWnd, TEXT("submodButtonClass"), this);

	if (!m_hBtnWnd)
	{
		throw std::exception(error.c_str());
	}

	//ShowWindow(m_hMainWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hBtnWnd);
}
//====================================================================
SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::~SUBMODS_WINDOW_BUTTON()
{
	RemoveProp(m_hBtnWnd, TEXT("buttonType"));
	RemoveProp(m_hBtnWnd, TEXT("ButtonClass"));
}
//====================================================================
void draw_text_unchecked_submod(
	HWND hWnd,
	HDC hDC,
	const std::string& submod_name,
	LOGFONT& lf,
	COLORREF& outline,
	COLORREF& red,
	RECT& rect
)
{
	// Create font indirect
	HFONT font, oldFont;
	font = CreateFontIndirect(&lf);
	oldFont = (HFONT)SelectObject(hDC, font);

	// Transparent text background
	SetBkMode(hDC, TRANSPARENT);
	// Create font indirect
	oldFont = (HFONT)SelectObject(hDC, CreateFontIndirect(&lf));

	// Cut submod name if it is too long
	std::string final_submod_name = submod_name;
	if (submod_name.length() > SUBMOD_NAME_MAX_LENGTH_TO_DRAW)
	{
		final_submod_name = submod_name.substr(0, SUBMOD_NAME_MAX_LENGTH_TO_DRAW - 3);
		final_submod_name.append("...");
	}

	// Text outline by drawing text with offset
	SetTextColor(hDC, outline);
	rect.left = 28;
	rect.top = 2;
	DrawTextA(hDC, final_submod_name.c_str(), -1, &rect, DT_LEFT);
	rect.left = 30;
	rect.top = 0;
	DrawTextA(hDC, final_submod_name.c_str(), -1, &rect, DT_LEFT);

	// Text to draw
	SetTextColor(hDC, red);
	rect.left = 29;
	rect.top = 1;
	DrawTextA(hDC, final_submod_name.c_str(), -1, &rect, DT_LEFT);

	// Delete font
	DeleteObject(SelectObject(hDC, oldFont));
}
//====================================================================
void draw_text_checked_submod(
	HWND hWnd,
	HDC hDC,
	const std::string& submod_name,
	LOGFONT& lf,
	COLORREF& outline,
	COLORREF& green,
	RECT& rect
)
{
	// Create font indirect
	HFONT font, oldFont;
	font = CreateFontIndirect(&lf);
	oldFont = (HFONT)SelectObject(hDC, font);

	// Cut submod name if it is too long
	std::string final_submod_name = submod_name;
	if (submod_name.length() > SUBMOD_NAME_MAX_LENGTH_TO_DRAW)
	{
		final_submod_name = submod_name.substr(0, SUBMOD_NAME_MAX_LENGTH_TO_DRAW - 3);
		final_submod_name.append("...");
	}

	// Transparent text background
	SetBkMode(hDC, TRANSPARENT);
	// Create font indirect
	oldFont = (HFONT)SelectObject(hDC, CreateFontIndirect(&lf));

	// Text outline by drawing text with offset
	SetTextColor(hDC, outline);
	rect.left = 28;
	rect.top = 2;
	DrawTextA(hDC, final_submod_name.c_str(), -1, &rect, DT_LEFT);
	rect.left = 30;
	rect.top = 0;
	DrawTextA(hDC, final_submod_name.c_str(), -1, &rect, DT_LEFT);

	// Text to draw
	SetTextColor(hDC, green);
	rect.left = 29;
	rect.top = 1;
	DrawTextA(hDC, final_submod_name.c_str(), -1, &rect, DT_LEFT);

	// Delete font
	DeleteObject(SelectObject(hDC, oldFont));
}
//====================================================================
SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON& get_submod_button_prop(HWND hWnd)
{
	return *(SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON*)GetProp(hWnd, TEXT("submodButtonClass"));
}
//====================================================================