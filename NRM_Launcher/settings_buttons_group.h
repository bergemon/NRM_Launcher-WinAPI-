#pragma once
#include <windows.h>
#include "resource.h"
#include "settings_parser.h"
#include "bitmap.h"

//====================================================================
// Functions and class forward declaration
//====================================================================
class SETTINGS_MODAL_WINDOW;
//====================================================================
void create_settings_buttons_groups(HINSTANCE hInstance, HWND hParent);
//====================================================================
void draw_settings_static_button_background(HWND hWnd, HDC hDC);
//====================================================================
void draw_settings_radio_button_background(HWND hWnd, HDC hDC);
//====================================================================
// Class to group buttons
//====================================================================
class SETTINGS_BUTTONS_GROUP
{
	friend class SETTINGS_MODAL_WINDOW;
	class SETTINGS_STATIC_BUTTON;
	class SETTINGS_RADIO_BUTTON;

public:
	SETTINGS_BUTTONS_GROUP(
		const char* background_file_name,
		LAUNCHER_SETTINGS::SETTINGS_PARAM settings_param
	);
	~SETTINGS_BUTTONS_GROUP();

	static SETTINGS_STATIC_BUTTON& get_static_button_prop(HWND hWnd);
	static SETTINGS_RADIO_BUTTON& get_radio_button_prop(HWND hWnd);
	static CUSTOM_BITMAP* get_radio_checked_icon();
	static CUSTOM_BITMAP* get_radio_unchecked_icon();
	static CUSTOM_BITMAP* get_radio_checked_icon_mask();
	static CUSTOM_BITMAP* get_radio_unchecked_icon_mask();
	LAUNCHER_SETTINGS::SETTINGS_PARAM get_group_param();
	std::list<SETTINGS_RADIO_BUTTON>& get_buttons();

	uint32_t get_static_button_y_pos();
	void create_radio_button(const char* background_file_name) noexcept(false);

private:

	static bool is_initialized();
	static void initialize(
		HINSTANCE hInstance,
		HWND hParent
	) noexcept(false);

	static HWND get_parent();

	friend void draw_settings_static_button_background(HWND hWnd, HDC hDC);
	friend void draw_settings_radio_button_background(HWND hWnd, HDC hDC);

	//====================================================================
	class SETTINGS_STATIC_BUTTON
	{
	public:
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

		SETTINGS_STATIC_BUTTON(
			HWND hParent,
			const char* background_file_name,
			LAUNCHER_SETTINGS::SETTINGS_PARAM settings_param,
			uint32_t xPos,
			uint32_t yPos,
			uint32_t width,
			uint32_t height = SETTINGS_STATIC_BUTTON_HEIGHT
		);
		~SETTINGS_STATIC_BUTTON();

		HWND get_parent();
		uint32_t get_width() const;
		uint32_t get_height() const;
		uint32_t get_xPos() const;
		uint32_t get_yPos() const;
		std::string_view get_background_file_name();
		LAUNCHER_SETTINGS::SETTINGS_PARAM get_group_param();

	private:
		HWND m_hStaticWND;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_xPos;
		uint32_t m_yPos;
		std::string m_bkgFileName;
		LAUNCHER_SETTINGS::SETTINGS_PARAM m_settingsParam;
	};
	//====================================================================
	class SETTINGS_RADIO_BUTTON
	{
	public:
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

		SETTINGS_RADIO_BUTTON(
			HWND hParent,
			SETTINGS_BUTTONS_GROUP& group,
			const char* background_file_name,
			LAUNCHER_SETTINGS::SETTINGS_PARAM settings_param,
			uint32_t button_num,
			uint32_t xPos,
			uint32_t yPos,
			uint32_t width,
			uint32_t height = SETTINGS_STATIC_BUTTON_HEIGHT,
			bool is_checked = false
		);
		~SETTINGS_RADIO_BUTTON();

		HWND get_parent();
		HWND get_hWnd() const;
		uint32_t get_width() const;
		uint32_t get_height() const;
		uint32_t get_xPos() const;
		uint32_t get_yPos() const;
		bool get_checked() const;
		void set_checked(bool);
		void set_xPos(uint32_t xPos);
		std::string_view get_background_file_name();
		LAUNCHER_SETTINGS::SETTINGS_PARAM get_group_param();
		uint32_t get_button_num();
		SETTINGS_BUTTONS_GROUP& get_button_group();

	private:
		HWND m_hRadioWND;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_xPos;
		uint32_t m_yPos;
		bool m_isChecked;
		std::string m_bkgFileName;
		LAUNCHER_SETTINGS::SETTINGS_PARAM m_settingsParam;
		uint32_t m_buttonNum;
		SETTINGS_BUTTONS_GROUP& m_group;
	};
	//====================================================================

	static HWND m_hParent;
	static bool m_initialized;
	static uint32_t m_buttonGroupsCurrentOffset;
	static CUSTOM_BITMAP m_radioIconChecked, m_radioIconUnChecked;
	static CUSTOM_BITMAP m_radioIconCheckedMask, m_radioIconUnCheckedMask;
	LAUNCHER_SETTINGS::SETTINGS_PARAM m_settingsParam;
	uint32_t m_totalRadioButtonsWidth = 0;
	SETTINGS_STATIC_BUTTON* m_staticButton = nullptr;
	std::list<SETTINGS_RADIO_BUTTON> m_radioButtons;
	uint32_t m_buttonsCount = 0;
};
//====================================================================