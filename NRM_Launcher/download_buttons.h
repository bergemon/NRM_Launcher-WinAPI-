#pragma once
#include "stdlibs.h"
#include "const.h"
#include "windows.h"
#include "resource.h"
#include "button.h"
#include "CommCtrl.h"

//====================================================================
enum CURRENT_OPERATION
{
	OPERATION_START,
	OPERATION_CHECK_VERSION,
	OPERATION_DOWNLOAD_GAME,
	OPERATION_UNZIP_FILE,
	OPERATION_DONE
};
//====================================================================
class DOWNLOAD_WINDOW_BUTTONS
{
public:
	enum DOWNLOAD_BUTTON_TYPE
	{
		DOWNLOAD_STATIC_BUTTON,
		DOWNLOAD_PROGRESS_BAR
	};

private:
	//====================================================================
	class DOWNLOAD_WINDOW_BUTTON
	{
	public:
		DOWNLOAD_WINDOW_BUTTON(
			HWND parent,
			std::wstring_view className,
			DOWNLOAD_BUTTON_TYPE btnType,
			const char* background_file_name,
			uint32_t x = CW_USEDEFAULT,
			uint32_t y = 0,
			uint32_t width = CW_USEDEFAULT,
			uint32_t height = 0
		) noexcept(false);
		~DOWNLOAD_WINDOW_BUTTON();

		HWND get_hWnd() const noexcept(true);
		uint32_t get_xPos() const noexcept(true);
		uint32_t get_yPos() const noexcept(true);
		uint32_t get_width() const noexcept(true);
		uint32_t get_height() const noexcept(true);
		DOWNLOAD_BUTTON_TYPE get_button_type() const noexcept(true);
		std::string_view get_background_file_name() noexcept(true);

		void show() noexcept(true);
		void hide() noexcept(true);

	private:
		HWND m_hBtnWnd;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_xPos = 0;
		uint32_t m_yPos = 0;
		DOWNLOAD_BUTTON_TYPE m_btnType;
		const std::wstring_view m_className;
		std::string m_bkgFileName;
	};
	//====================================================================

public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	[[nodiscard]] static DOWNLOAD_WINDOW_BUTTONS& getInstance() noexcept(true);

	void initialize(HWND hParent, LPCWSTR className) noexcept(false);

	[[nodiscard]] HWND getParent() const noexcept(true);

	void create_button(
		BUTTON_PROPERTIES props,
		DOWNLOAD_BUTTON_TYPE btnType,
		const char* bkgFileName = nullptr,
		uint32_t x = 0,
		uint32_t y = 0
	) noexcept(false);

	std::vector<DOWNLOAD_WINDOW_BUTTON>& get_download_buttons() noexcept(true);

	friend DOWNLOAD_WINDOW_BUTTON& get_download_button_prop(HWND hWnd) noexcept(true);
	friend void draw_download_btn_background(HWND hWnd, HDC hDC);

	~DOWNLOAD_WINDOW_BUTTONS();

private:
	DOWNLOAD_WINDOW_BUTTONS();
	DOWNLOAD_WINDOW_BUTTONS(const DOWNLOAD_WINDOW_BUTTONS&) = delete;
	DOWNLOAD_WINDOW_BUTTONS operator=(const DOWNLOAD_WINDOW_BUTTONS&) = delete;

	bool m_initialized;
	uint32_t m_submodBtnOffset = SUBMOD_BUTTONS_Y_START;
	std::wstring m_className;
	HWND m_parent;
	std::vector<DOWNLOAD_WINDOW_BUTTON> m_vectButtons;
	int32_t m_yCurrentOffset;
};
//====================================================================
DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_WINDOW_BUTTON& get_download_button_prop(HWND hWnd)
	noexcept(true);
//====================================================================
void create_download_buttons();
//====================================================================