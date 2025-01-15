#pragma once
#include <windows.h>
#include <string>
#include <list>
#include "button.h"

//====================================================================
enum SUBMODS_BUTTON_TYPE
{
	BTN_SAVE,
	BTN_CANCEL,
	BTN_SUBMOD
};
//====================================================================
class SUBMODS_BUTTONS
{
public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	[[nodiscard]] static SUBMODS_BUTTONS& getInstance();

	void initiliaze(
		HWND hParent,
		LPCWSTR className
	);

	[[nodiscard]] HWND getParent();

	bool create_button(
		SUBMODS_BUTTON_TYPE btnType,
		BUTTON_PROPERTIES props,
		const char* bkgFileName = nullptr,
		uint32_t exPadding = 0,
		uint32_t x = 0,
		uint32_t y = 0
	);

	~SUBMODS_BUTTONS();

private:
	//====================================================================
	class SUBMODS_WINDOW_BUTTON
	{
	public:
		SUBMODS_WINDOW_BUTTON(
			HWND parent,
			std::wstring_view className,
			SUBMODS_BUTTON_TYPE btnType,
			uint32_t x = CW_USEDEFAULT,
			uint32_t y = 0,
			uint32_t width = CW_USEDEFAULT,
			uint32_t height = 0
		);
		~SUBMODS_WINDOW_BUTTON();

	private:
		HWND m_hBtnWnd;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_buttonType;
		std::wstring_view m_className;
		//====================================================================
		// If it is submod button
		//====================================================================
		const char* m_submod_name;
		const char* m_submod_path;
		bool m_checked;
	};
	//====================================================================

	SUBMODS_BUTTONS();
	SUBMODS_BUTTONS(const SUBMODS_BUTTONS&) = delete;
	SUBMODS_BUTTONS operator=(const SUBMODS_BUTTONS&) = delete;

	bool m_initialized;
	uint32_t m_submodBtnOffset = 50;
	std::wstring m_className;
	HWND m_parent;
	std::list<SUBMODS_WINDOW_BUTTON> m_listButtons;
};
//====================================================================
void create_submods_modal_buttons(SUBMODS_BUTTONS& submod_window_buttons);
//====================================================================
void draw_submods_btn_background(HWND hWnd, HDC hDC);
//====================================================================
void draw_submods_active_btn_background(HWND hWnd, HDC hDC);
//====================================================================