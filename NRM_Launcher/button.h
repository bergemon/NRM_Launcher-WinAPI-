#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <shellapi.h>
#include "const.h"
#include "stdlib.h"
#include "settings_parser.h"

//====================================================================
struct BUTTON_PROPERTIES
{
	uint32_t width = CW_USEDEFAULT;
	uint32_t height = 0;
};
//====================================================================
struct BUTTON_POSITION
{
	std::string btnBkgName = "";
	uint32_t pos_x = 0;
	uint32_t pos_y = 0;
	uint32_t width = 0;
	uint32_t height = 0;
};
//====================================================================
enum BUTTON_TYPE
{
	BTN_EXIT,
	BTN_PLAY,
	BTN_SUBMODS,
	BTN_SETTINGS,
	BTN_DOWNLOAD,
	BTN_DISCORD
};
//====================================================================
class LAUNCHER_BUTTONS
{
public:

	//====================================================================
	// Class for each button
	class BUTTON
	{
	public:
		BUTTON(
			HWND parent,
			std::wstring_view className,
			BUTTON_TYPE btnType,
			int nCmdShow,
			uint32_t x = CW_USEDEFAULT,
			uint32_t y = 0,
			uint32_t width = CW_USEDEFAULT,
			uint32_t height = 0
		);

		~BUTTON();

		uint32_t getHeight() { return m_height; };

	private:
		HWND m_hBtnWnd;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_buttonType;
		std::wstring_view m_className;
	};
	//====================================================================

	~LAUNCHER_BUTTONS();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	void initialize(
		HWND hParent,
		LPCWSTR className,
		int nCmdShow
	);

	bool create_button(
		BUTTON_TYPE btnType,
		BUTTON_PROPERTIES props,
		const char* bkgFileName = nullptr,
		uint32_t exPadding = 0,
		uint32_t x = 0,
		uint32_t y = 0
	);

	static LAUNCHER_BUTTONS& getInstance()
	{
		static LAUNCHER_BUTTONS btns;
		return btns;
	}

private:
	LAUNCHER_BUTTONS();
	LAUNCHER_BUTTONS(const LAUNCHER_BUTTONS&) = delete;
	LAUNCHER_BUTTONS operator=(const LAUNCHER_BUTTONS&) = delete;

	bool m_initialized = false;
	uint32_t m_buttonsHeight = 70;
	std::wstring m_className;
	int m_nCmdShow;
	HWND m_parent;
	std::list<BUTTON> m_Vbuttons;
};
//====================================================================