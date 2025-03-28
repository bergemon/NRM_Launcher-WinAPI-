#include "settings_buttons_group.h"
#include "get_file_path.h"
#include "main_window.h"

//====================================================================
// settings buttons group class static members below
//====================================================================
CUSTOM_BITMAP SETTINGS_BUTTONS_GROUP::m_radioIconChecked, SETTINGS_BUTTONS_GROUP::m_radioIconUnChecked;
//====================================================================
CUSTOM_BITMAP SETTINGS_BUTTONS_GROUP::m_radioIconCheckedMask, SETTINGS_BUTTONS_GROUP::m_radioIconUnCheckedMask;
//====================================================================
bool SETTINGS_BUTTONS_GROUP::m_initialized = false;
//====================================================================
HWND SETTINGS_BUTTONS_GROUP::m_hParent = nullptr;
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::m_buttonGroupsCurrentOffset = SETTINGS_WINDOW_BUTTONS_GROUP_START_Y_POS;
//====================================================================
bool SETTINGS_BUTTONS_GROUP::is_initialized()
{
	return m_initialized;
}
//====================================================================
SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON& SETTINGS_BUTTONS_GROUP::get_static_button_prop(HWND hWnd)
{
	return *(SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON*)GetProp(hWnd, TEXT("ButtonClass"));
}
//====================================================================
SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON& SETTINGS_BUTTONS_GROUP::get_radio_button_prop(HWND hWnd)
{
	return *(SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON*)GetProp(hWnd, TEXT("ButtonClass"));
}
//====================================================================
CUSTOM_BITMAP* SETTINGS_BUTTONS_GROUP::get_radio_checked_icon()
{
	return &m_radioIconChecked;
}
//====================================================================
CUSTOM_BITMAP* SETTINGS_BUTTONS_GROUP::get_radio_unchecked_icon()
{
	return &m_radioIconUnChecked;
}
//====================================================================
CUSTOM_BITMAP* SETTINGS_BUTTONS_GROUP::get_radio_checked_icon_mask()
{
	return &m_radioIconCheckedMask;
}
//====================================================================
CUSTOM_BITMAP* SETTINGS_BUTTONS_GROUP::get_radio_unchecked_icon_mask()
{
	return &m_radioIconUnCheckedMask;
}
//====================================================================
LAUNCHER_SETTINGS::SETTINGS_PARAM SETTINGS_BUTTONS_GROUP::get_group_param()
{
	return m_settingsParam;
}
//====================================================================
std::list<SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON>& SETTINGS_BUTTONS_GROUP::get_buttons()
{
	return m_radioButtons;
}
//====================================================================
HWND SETTINGS_BUTTONS_GROUP::get_parent()
{
	return m_hParent;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::get_static_button_y_pos()
{
	if (!m_staticButton)
	{
		throw std::exception("Settings static button is not initialized");
	}

	return m_staticButton->get_yPos();
}
//====================================================================
void SETTINGS_BUTTONS_GROUP::create_radio_button(const char* background_file_name) noexcept(false)
{
	// Get bitmap image width
	std::string bkgFilePath = "";
	get_settings_button_path(bkgFilePath);
	bkgFilePath.append(background_file_name);
	uint32_t width = CUSTOM_BITMAP::GetBitmapWidth(bkgFilePath.c_str());

	if ((m_totalRadioButtonsWidth + width) > SETTINGS_STATIC_BUTTON_WIDTH)
	{
		std::string error("Can not create more radio buttons. Width is limited by ");
		error.append(std::to_string(SETTINGS_STATIC_BUTTON_WIDTH));
		error.append(" pixels.");
		throw std::exception(error.c_str());
	}

	m_totalRadioButtonsWidth += width;

	m_radioButtons.emplace_back(
		SETTINGS_BUTTONS_GROUP::get_parent(),
		*this,
		background_file_name,
		// Radio button settings parameter
		m_settingsParam,
		// Number of button
		m_buttonsCount++,
		// Radio button xPos and yPos
		0, get_static_button_y_pos() + SETTINGS_STATIC_BUTTON_HEIGHT + SETTINGS_GROUP_BUTTONS_VERTICAL_SPACE,
		// Width and height of the radio button
		width, SETTINGS_RADIO_BUTTON_HEIGHT
	);

	uint32_t each_button_has_space = (uint32_t)SETTINGS_STATIC_BUTTON_WIDTH / (uint32_t)m_radioButtons.size();

	// 10 is starting offset for radio buttons
	uint32_t current_offset = 10;

	for (auto& button : m_radioButtons)
	{
		uint32_t width = button.get_width();
		if (width > each_button_has_space)
		{
			width = each_button_has_space;
			MessageBox(
				MAIN_WINDOW::getInstance().getHWnd(),
				TEXT("Radio button width is longer than each radio button can have in a group"),
				TEXT("Error"),
				MB_OK
			);
		}

		button.set_xPos(current_offset + ((each_button_has_space - width) / 2));

		MoveWindow(
			// Window handle
			button.get_hWnd(),
			// X pos
			button.get_xPos(),
			// Y pos
			button.get_yPos(),
			// Width & height
			button.get_width(), button.get_height(),
			// Redraw
			TRUE
		);

		current_offset += each_button_has_space;
	}
}
//====================================================================
void SETTINGS_BUTTONS_GROUP::initialize(
	HINSTANCE hInstance,
	HWND hParent
) noexcept(false)
{
	if (m_initialized)
	{
		throw std::exception("Settings buttons group is already initialized");
	}

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

	// Initialize static button window
	window_class->cbSize = sizeof(*window_class);
	window_class->lpszClassName = SETTINGS_STATIC_BUTTON_CLASS;
	window_class->lpszMenuName = TEXT("");
	window_class->style = CS_DBLCLKS;
	window_class->lpfnWndProc = SETTINGS_STATIC_BUTTON::WndProc;
	window_class->hInstance = hInstance;
	window_class->hIcon = hIcon;
	window_class->hIconSm = hIcon;
	window_class->hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL));
	window_class->hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	window_class->cbClsExtra = 0;
	window_class->cbWndExtra = 0;

	if (!RegisterClassEx(window_class))
	{
		throw std::exception("Can not register settings static button window class");
	}

	// Initialize radio button window
	window_class->cbSize = sizeof(*window_class);
	window_class->lpszClassName = SETTINGS_RADIO_BUTTON_CLASS;
	window_class->lpfnWndProc = SETTINGS_RADIO_BUTTON::WndProc;
	window_class->hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SELECTED));

	if (!RegisterClassEx(window_class))
	{
		throw std::exception("Can not register settings radio button window class");
	}

	DeleteObject(window_class->hbrBackground);
	DestroyIcon(hIcon);
	free(window_class);

	// Init radio buttons icons bitmap
	// Checked button icon
	std::string radio_button_checked_icon_path;
	get_settings_button_icon_path(radio_button_checked_icon_path);
	radio_button_checked_icon_path.append(SETTINGS_RADIO_BUTTON_CHECKED_FILENAME);
	if (!m_radioIconChecked.LoadFromFile(radio_button_checked_icon_path.c_str()))
	{
		throw std::exception(m_radioIconChecked.GetError());
	}
	if (!m_radioIconChecked.CreateBkgMask(get_settings_button_icon_mask_path)) 
	{
		throw std::exception(m_radioIconChecked.GetError());
	}
	get_settings_button_icon_mask_path(radio_button_checked_icon_path);
	radio_button_checked_icon_path.append(SETTINGS_RADIO_BUTTON_CHECKED_FILENAME);
	if (!m_radioIconCheckedMask.LoadFromFile(radio_button_checked_icon_path.c_str()))
	{
		throw std::exception(m_radioIconCheckedMask.GetError());
	}

	// Unchecked icon
	std::string radio_button_unchecked_icon_path;
	get_settings_button_icon_path(radio_button_unchecked_icon_path);
	radio_button_unchecked_icon_path.append(SETTINGS_RADIO_BUTTON_UNCHECKED_FILENAME);
	if (!m_radioIconUnChecked.LoadFromFile(radio_button_unchecked_icon_path.c_str()))
	{
		throw std::exception(m_radioIconUnChecked.GetError());
	}
	if (!m_radioIconUnChecked.CreateBkgMask(get_settings_button_icon_mask_path))
	{
		throw std::exception(m_radioIconUnChecked.GetError());
	}
	get_settings_button_icon_mask_path(radio_button_checked_icon_path);
	radio_button_checked_icon_path.append(SETTINGS_RADIO_BUTTON_UNCHECKED_FILENAME);
	if (!m_radioIconUnCheckedMask.LoadFromFile(radio_button_checked_icon_path.c_str()))
	{
		throw std::exception(m_radioIconUnCheckedMask.GetError());
	}

	m_hParent = hParent;
	m_initialized = true;
}
//====================================================================
SETTINGS_BUTTONS_GROUP::SETTINGS_BUTTONS_GROUP(
	const char* background_file_name,
	LAUNCHER_SETTINGS::SETTINGS_PARAM settings_param
) : m_settingsParam(settings_param)
{
	if (!m_initialized)
	{
		throw std::exception("SETTINGS_BUTTONS_GROUP class is not initialized");
	}
	// m_buttonGroupsCurrentOffset
	m_staticButton = new SETTINGS_STATIC_BUTTON(
		m_hParent,
		background_file_name,
		settings_param,
		SETTINGS_WINDOW_STATIC_BUTTON_OFFSET,
		m_buttonGroupsCurrentOffset,
		SETTINGS_STATIC_BUTTON_WIDTH,
		SETTINGS_STATIC_BUTTON_HEIGHT
	);

	m_buttonGroupsCurrentOffset += SETTINGS_STATIC_BUTTON_HEIGHT + SETTINGS_RADIO_BUTTON_HEIGHT
		+ SETTINGS_GROUP_BUTTONS_VERTICAL_SPACE + SETTINGS_GROUPS_SPACE_BETWEEN;
}
//====================================================================
SETTINGS_BUTTONS_GROUP::~SETTINGS_BUTTONS_GROUP()
{
	if (m_staticButton)
	{
		delete m_staticButton;
	}
}
//====================================================================
SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::SETTINGS_STATIC_BUTTON(
	HWND hParent,
	const char* background_file_name,
	LAUNCHER_SETTINGS::SETTINGS_PARAM settings_param,
	uint32_t xPos,
	uint32_t yPos,
	uint32_t width,
	uint32_t height
) : m_width(width), m_height(height), m_xPos(xPos), m_yPos(yPos),
	m_bkgFileName(background_file_name), m_settingsParam(settings_param)
{
	m_hStaticWND = CreateWindowEx(
		// WindowExStyles
		WS_EX_TRANSPARENT,
		// UTF-16 name of the creating class
		SETTINGS_STATIC_BUTTON_CLASS,
		// Window title
		TEXT(""),
		// Window styles
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		// Destination x & y
		xPos, yPos,
		// Window width, height
		width, height,
		// Parent (must be main window)
		hParent,
		// Menu
		(HMENU)NULL,
		// hInstance
		GetModuleHandle(NULL),
		// lpParam
		NULL
	);

	if (!m_hStaticWND)
	{
		throw std::exception("Can not create settings static button");
	}

	SetProp(m_hStaticWND, TEXT("ButtonClass"), this);
}
//====================================================================
SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::~SETTINGS_STATIC_BUTTON()
{
}
//====================================================================
std::string_view SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::get_background_file_name()
{
	return m_bkgFileName;
}
//====================================================================
LAUNCHER_SETTINGS::SETTINGS_PARAM SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::get_group_param()
{
	return m_settingsParam;
}
//====================================================================
HWND SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::get_parent()
{
	return SETTINGS_BUTTONS_GROUP::get_parent();
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::get_width() const
{
	return m_width;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::get_height() const
{
	return m_height;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::get_xPos() const
{
	return m_xPos;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_STATIC_BUTTON::get_yPos() const
{
	return m_yPos;
}
//====================================================================
SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::SETTINGS_RADIO_BUTTON(
	HWND hParent,
	SETTINGS_BUTTONS_GROUP& group,
	const char* background_file_name,
	LAUNCHER_SETTINGS::SETTINGS_PARAM settings_param,
	uint32_t button_num,
	uint32_t xPos,
	uint32_t yPos,
	uint32_t width,
	uint32_t height,
	bool is_checked
) : m_width(width), m_height(height), m_xPos(xPos), m_yPos(yPos),
	m_bkgFileName(background_file_name), m_isChecked(is_checked),
	m_buttonNum(button_num), m_settingsParam(settings_param),
	m_group(group)
{
	m_hRadioWND = CreateWindowEx(
		// WindowExStyles
		WS_EX_TRANSPARENT,
		// UTF-16 name of the creating class
		SETTINGS_RADIO_BUTTON_CLASS,
		// Window title
		TEXT(""),
		// Window styles
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		// Destination x & y
		xPos, yPos,
		// Window width, height
		width, height,
		// Parent (must be main window)
		hParent,
		// Menu
		(HMENU)NULL,
		// hInstance
		GetModuleHandle(NULL),
		// lpParam
		NULL
	);

	if (!m_hRadioWND)
	{
		throw std::exception("Can not create settings radio button");
	}

	SetProp(m_hRadioWND, TEXT("ButtonClass"), this);
}
//====================================================================
SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::~SETTINGS_RADIO_BUTTON()
{
}
//====================================================================
std::string_view SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_background_file_name()
{
	return m_bkgFileName;
}
//====================================================================
LAUNCHER_SETTINGS::SETTINGS_PARAM SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_group_param()
{
	return m_settingsParam;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_button_num()
{
	return m_buttonNum;
}
//====================================================================
SETTINGS_BUTTONS_GROUP& SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_button_group()
{
	return m_group;
}
//====================================================================
HWND SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_parent()
{
	return SETTINGS_BUTTONS_GROUP::get_parent();
}
//====================================================================
HWND SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_hWnd() const
{
	return m_hRadioWND;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_width() const
{
	return m_width;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_height() const
{
	return m_height;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_xPos() const
{
	return m_xPos;
}
//====================================================================
bool SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_checked() const
{
	return m_isChecked;
}
//====================================================================
void SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::set_checked(bool state)
{
	m_isChecked = state;
}
//====================================================================
uint32_t SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::get_yPos() const
{
	return m_yPos;
}
//====================================================================
void SETTINGS_BUTTONS_GROUP::SETTINGS_RADIO_BUTTON::set_xPos(uint32_t xPos)
{
	m_xPos = xPos;
}
//====================================================================