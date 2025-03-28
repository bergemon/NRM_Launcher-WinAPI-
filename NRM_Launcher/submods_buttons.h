#pragma once
#include <windows.h>
#include <string>
#include <list>
#include "button.h"
#include "parse_submods.h"

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
private:
	class SUBMODS_WINDOW_BUTTON;
	
	struct SUBMOD_BUTTON_BUFFER
	{
		SUBMODS_WINDOW_BUTTON& m_button;
		bool m_isChecked;
	};
public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	[[nodiscard]] static SUBMODS_BUTTONS& getInstance();

	void initialize(HWND hParent, LPCWSTR className) noexcept(false);

	bool is_initialized() const noexcept(true);

	[[nodiscard]] HWND getParent() const;

	void create_button(
		SUBMODS_BUTTON_TYPE btnType,
		BUTTON_PROPERTIES props,
		const char* bkgFileName = nullptr,
		uint32_t exPadding = 0,
		uint32_t x = 0,
		uint32_t y = 0
	) noexcept(false);

	void create_submod_button(
		BUTTON_PROPERTIES props,
		const char* submod_name,
		const char* submod_path,
		uint32_t exPadding = 0
	) noexcept(false);
	void clear_submod_buttons();

	std::list<SUBMODS_WINDOW_BUTTON>& get_submods_buttons();
	std::vector<SUBMOD_BUTTON_BUFFER>& get_submods_buttons_buffer();

	friend void fill_submods_buffer();
	friend void save_checked_from_buffer();
	friend SUBMODS_WINDOW_BUTTON& get_submod_button_prop(HWND hWnd);
	friend void clear_buttons_state_in_buffer();
	friend void create_process() noexcept(false);
	friend void draw_submods_btn_background(HWND hWnd, HDC hDC);
	friend void draw_submods_active_btn_background(HWND hWnd, HDC hDC);
	friend void draw_submods_checkbox_btn_background(HWND hWnd, HDC hDC);

	~SUBMODS_BUTTONS();

private:
	//====================================================================
	class SUBMODS_WINDOW_BUTTON
	{
	public:
		SUBMODS_WINDOW_BUTTON(
			HWND parent,
			std::wstring_view className,
			const char* background_file_name,
			SUBMODS_BUTTON_TYPE btnType,
			uint32_t x = CW_USEDEFAULT,
			uint32_t y = 0,
			uint32_t width = CW_USEDEFAULT,
			uint32_t height = 0,
			const char* submod_name = "",
			const char* submod_path = "",
			bool checked = false,
			uint32_t exPadding = 0
		);
		~SUBMODS_WINDOW_BUTTON();

		HWND get_hWnd() const;
		void set_checked(bool is_checked);
		bool is_checked() const;
		SUBMODS_BUTTON_TYPE get_button_type() const;
		uint32_t get_xPos() const;
		uint32_t get_yPos() const;
		uint32_t get_width() const;
		uint32_t get_height() const;
		uint32_t get_exPadding() const;
		std::string_view get_background_file_name();
		SUBMOD_BUTTON_BUFFER& get_buffer_button();
		void set_buffer_button(SUBMOD_BUTTON_BUFFER& buff_button);
		const std::string& get_submod_name() const;
		const std::string& get_submod_path() const;

	private:
		HWND m_hBtnWnd;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_xPos = 0;
		uint32_t m_yPos = 0;
		uint32_t m_exPadding = 0;
		const std::wstring_view m_className;
		const SUBMODS_BUTTON_TYPE m_buttonType;
		std::string m_bkgFileName;
		//====================================================================
		// If it is submod button
		//====================================================================
		const std::string m_submod_name;
		const std::string m_submod_path;
		SUBMOD_BUTTON_BUFFER* m_bufferButton;
		bool m_checked;
	};
	//====================================================================

	SUBMODS_BUTTONS();
	SUBMODS_BUTTONS(const SUBMODS_BUTTONS&) = delete;
	SUBMODS_BUTTONS operator=(const SUBMODS_BUTTONS&) = delete;
	void lower_y_offset(int32_t height, uint32_t exPadding);

	bool m_initialized;
	uint32_t m_submodBtnOffset = SUBMOD_BUTTONS_Y_START;
	std::wstring m_className;
	HWND m_parent;
	std::list<SUBMODS_WINDOW_BUTTON> m_listButtons;
	std::vector<SUBMOD_BUTTON_BUFFER> m_submodButtonsBuffer;
};
//====================================================================
void create_submods_modal_buttons();
//====================================================================
void create_submods_buttons_again();
//====================================================================
void draw_submods_btn_background(HWND hWnd, HDC hDC);
//====================================================================
void draw_submods_active_btn_background(HWND hWnd, HDC hDC);
//====================================================================
void draw_submods_checkbox_btn_background(HWND hWnd, HDC hDC);
//====================================================================
void draw_text_unchecked_submod(
	HWND hWnd, HDC hDC, const std::string& submod_name, LOGFONT& lf, COLORREF& outline, COLORREF& red, RECT& rect
);
//====================================================================
void draw_text_checked_submod(
	HWND hWnd, HDC hDC, const std::string& submod_name, LOGFONT& lf, COLORREF& outline, COLORREF& green, RECT& rect
);
//====================================================================
SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON& get_submod_button_prop(HWND hWnd);
//====================================================================