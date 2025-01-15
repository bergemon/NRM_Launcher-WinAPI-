#include "submods_buttons.h"
#include "resource.h"

//====================================================================
extern std::map<uint32_t, BUTTON_POSITION> submodsBtnsBkgPaths;
//====================================================================
[[nodiscard]] SUBMODS_BUTTONS& SUBMODS_BUTTONS::getInstance()
{
	static SUBMODS_BUTTONS btns;
	return btns;
}
//====================================================================
[[nodiscard]] HWND SUBMODS_BUTTONS::getParent()
{
	return m_parent;
}
//====================================================================
SUBMODS_BUTTONS::SUBMODS_BUTTONS()
{
}
//====================================================================
SUBMODS_BUTTONS::~SUBMODS_BUTTONS()
{
}
//====================================================================
void SUBMODS_BUTTONS::initiliaze(
	HWND hParent,
	LPCWSTR className
)
{
	if (m_initialized)
	{
		MessageBox(NULL, TEXT("Button class is already initialized"), TEXT("Error"), MB_OK);
		return;
	}

	WNDCLASSEX* window_class = (WNDCLASSEX*)malloc(sizeof(WNDCLASSEX));

	if (window_class == NULL)
	{
		MessageBox(NULL, TEXT("Cannot allocate memory for WNDCLASSEX structure"), TEXT("Error"), MB_OK);
		std::exit(1);
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
		MessageBox(NULL, TEXT("Cannot register window class"), TEXT("Error"), MB_OK);
		std::exit(1);
	}

	free(window_class);
	m_initialized = true;
}
//====================================================================
bool SUBMODS_BUTTONS::create_button(
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
		MessageBox(NULL, TEXT("Submods buttons window class not initialized!"), TEXT("Need to initialize buttons window class"), MB_OK);
		return false;
	}

	uint32_t x_coord;
	uint32_t y_coord;
	bool std_btn = false;

	if (!x)
	{
		x_coord = SUBMODS_WINDOW_WIDTH - props.width - BTNS_SUBMODS_X_COORD_INVERT(props.width);
		y_coord = SUBMODS_WINDOW_HEIGHT - props.height - m_submodBtnOffset;
		std_btn = true;
	}
	else
	{
		x_coord = x;
		y_coord = y;
	}

	uint32_t width = props.width;
	uint32_t height = props.height;
	submodsBtnsBkgPaths[btnType] = { bkgFileName, x_coord, y_coord - exPadding, width, height };

	m_listButtons.emplace_back(
		m_parent, m_className, btnType,
		x_coord, y_coord,
		width, height
	);

	if (std_btn)
	{
		m_submodBtnOffset += props.height + BTNS_STD_PADDING + exPadding;
	}

	return true;
}
//====================================================================
SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::SUBMODS_WINDOW_BUTTON(
	HWND parent,
	std::wstring_view className,
	SUBMODS_BUTTON_TYPE btnType,
	uint32_t x,
	uint32_t y,
	uint32_t width,
	uint32_t height
) : m_width(width), m_height(height), m_buttonType(btnType), m_className(className)
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

	std::wstring error(TEXT("Can not create"));
	error.append(TEXT(" button "));

	switch (btnType)
	{
	case SUBMODS_BUTTON_TYPE::BTN_SAVE:
		error.append(TEXT("save"));
		break;
	case SUBMODS_BUTTON_TYPE::BTN_CANCEL:
		error.append(TEXT("cancel"));
		break;
	case SUBMODS_BUTTON_TYPE::BTN_SUBMOD:
		error.append(TEXT("submod"));
		break;
	default:
		error.append(L"\"ERROR - CAN NOT DEFINITE BUTTON\"");
		break;
	}
	error.append(L".");

	SetProp(m_hBtnWnd, TEXT("buttonType"), &m_buttonType);

	if (!m_hBtnWnd)
	{
		MessageBox(NULL, error.c_str(), TEXT("Error"), MB_OK);
		return;
	}

	//ShowWindow(m_hMainWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hBtnWnd);
}
//====================================================================
SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON::~SUBMODS_WINDOW_BUTTON()
{
}
//====================================================================