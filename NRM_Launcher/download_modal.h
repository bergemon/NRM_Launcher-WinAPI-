#pragma once
#include "windows.h"
#include "stdlibs.h"
#include "bitmap.h"
#include "resource.h"
#include "get_file_path.h"
#include "settings_parser.h"
#include "download_buttons.h"
#include "download.h"

class DOWNLOAD_MODAL_WINDOW
{
public:
	static DOWNLOAD_MODAL_WINDOW& getInstance();

	void initialize(
		HWND hParent,
		LPCWSTR className,
		LPCWSTR windowTitle,
		HINSTANCE hInstance,
		int nCmdShow,
		LPCWSTR menuName,
		uint32_t width,
		uint32_t height,
		const char* bkgFileName
	);

	CUSTOM_BITMAP& get_background();
	std::string_view get_background_path();
	HWND getHWnd() const;
	void show();
	void hide();

	static LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	DOWNLOAD_MODAL_WINDOW();
	~DOWNLOAD_MODAL_WINDOW();
	DOWNLOAD_MODAL_WINDOW(const DOWNLOAD_MODAL_WINDOW&) = delete;
	DOWNLOAD_MODAL_WINDOW operator=(const DOWNLOAD_MODAL_WINDOW&) = delete;

	bool m_initialized;
	HWND m_hDownloadWnd;
	HWND m_hParentWND;
	CUSTOM_BITMAP m_background;
	std::string m_bkgFileName;
};