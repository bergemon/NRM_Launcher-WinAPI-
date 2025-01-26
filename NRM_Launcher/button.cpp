#include "const.h"
#include "button.h"
#include "resource.h"

//====================================================================
std::string_view LAUNCHER_BUTTONS::BUTTON::get_background_file_name()
{
	return m_bkgFileName;
}
//====================================================================
uint32_t LAUNCHER_BUTTONS::BUTTON::getHeight()
{
	return m_height;
}
//====================================================================
uint32_t LAUNCHER_BUTTONS::BUTTON::getWidth()
{
	return m_width;
}
//====================================================================
uint32_t LAUNCHER_BUTTONS::BUTTON::get_posX()
{
	return m_posX;
}
//====================================================================
uint32_t LAUNCHER_BUTTONS::BUTTON::get_posY()
{
	return m_posY;
}
//====================================================================
uint32_t LAUNCHER_BUTTONS::BUTTON::get_button_type()
{
	return m_buttonType;
}
//====================================================================
LAUNCHER_BUTTONS& LAUNCHER_BUTTONS::getInstance()
{
	static LAUNCHER_BUTTONS btns;
	return btns;
}
//====================================================================
LAUNCHER_BUTTONS::LAUNCHER_BUTTONS() {}
//====================================================================
LAUNCHER_BUTTONS::~LAUNCHER_BUTTONS()
{
	DeleteObject(m_parent);
}
//====================================================================
void LAUNCHER_BUTTONS::initialize(
	HWND hParent,
	LPCWSTR className,
	int nCmdShow
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

	m_nCmdShow = nCmdShow;
	free(window_class);
	m_initialized = true;
}
//====================================================================
bool LAUNCHER_BUTTONS::create_button(
	BUTTON_TYPE btnType,
	BUTTON_PROPERTIES props,
	const char* bkgFileName,
	uint32_t exPadding,
	uint32_t x,
	uint32_t y
)
{
	if (!m_initialized)
	{
		MessageBox(NULL, TEXT("Buttons window class not initialized!"), TEXT("Need to initialize buttons window class"), MB_OK);
		return false;
	}

	uint32_t x_coord;
	uint32_t y_coord;
	bool std_btn = false;

	if (!x)
	{
		x_coord = MAIN_WINDOW_WIDTH - props.width - BTNS_X_COORD_INVERT;
		y_coord = MAIN_WINDOW_HEIGHT - props.height - m_buttonsHeight;
		std_btn = true;
	}
	else
	{
		x_coord = x;
		y_coord = y;
	}

	uint32_t width = props.width;
	uint32_t height = props.height;
	
	m_Vbuttons.emplace_back(
		m_parent, m_className,
		// Button type
		btnType,
		// Button background file name
		bkgFileName,
		// Cmd
		m_nCmdShow,
		// X and Y coords
		x_coord, y_coord - exPadding,
		// Width & height
		width, height
	);

	if (std_btn)
	{
		m_buttonsHeight += props.height + BTNS_STD_PADDING + exPadding;
	}

	return true;
}
//====================================================================
LAUNCHER_BUTTONS::BUTTON::BUTTON(
	HWND parent,
	std::wstring_view className,
	BUTTON_TYPE btnType,
	const char* background_file_name,
	int nCmdShow,
	uint32_t x,
	uint32_t y,
	uint32_t width,
	uint32_t height
) : m_className(className), m_width(width), m_height(height), m_posX(x), m_posY(y),
	m_buttonType(btnType), m_bkgFileName(background_file_name)
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
	
	std::wstring error(L"Can not create");
	error.append(L" button ");

	switch (btnType)
	{
	case BUTTON_TYPE::BTN_EXIT:
		error.append(L"exit");
		break;
	case BUTTON_TYPE::BTN_PLAY:
		error.append(L"play");
		break;
	case BUTTON_TYPE::BTN_SUBMODS:
		error.append(L"submods");
		break;
	case BUTTON_TYPE::BTN_SETTINGS:
		error.append(L"settings");
		break;
	case BUTTON_TYPE::BTN_DOWNLOAD:
		error.append(L"download");
		break;
	case BUTTON_TYPE::BTN_DISCORD:
		error.append(L"discord");
		break;
	default:
		error.append(L"\"ERROR - CAN NOT DEFINITE BUTTON\"");
		break;
	}
	error.append(L".");

	SetProp(m_hBtnWnd, TEXT("buttonClass"), this);

	if (!m_hBtnWnd)
	{
		MessageBox(NULL, error.c_str(), TEXT("Error"), MB_OK);
		return;
	}

	//ShowWindow(m_hMainWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hBtnWnd);
}
//====================================================================
LAUNCHER_BUTTONS::BUTTON::~BUTTON()
{
	RemoveProp(m_hBtnWnd, TEXT("buttonType"));
	DestroyWindow(m_hBtnWnd);
	m_hBtnWnd = nullptr;
}
//====================================================================
LAUNCHER_BUTTONS::BUTTON& get_button_prop(HWND hWnd)
{
	return *(LAUNCHER_BUTTONS::BUTTON*)GetProp(hWnd, TEXT("ButtonClass"));
}
//====================================================================