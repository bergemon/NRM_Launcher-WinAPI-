#pragma once
#include <windows.h>
#include "stdlibs.h"
#include "bitmap.h"
#include "get_file_path.h"

class MAIN_WINDOW
{
public:
	~MAIN_WINDOW();

	static MAIN_WINDOW& getInstance();

	void initialize(
		LPCWSTR className,
		LPCWSTR windowName,
		HINSTANCE hInstance,
		int nCmdShow,
		LPCWSTR menuName = NULL,
		uint32_t width = CW_USEDEFAULT,
		uint32_t height = 0,
		const char* bkgFileName = nullptr
	) noexcept(false);

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	HWND getHWnd();
	void setHWnd(HWND hWnd);

	CUSTOM_BITMAP& get_background();

	const std::string_view get_bkg_filename();

private:
	MAIN_WINDOW();
	MAIN_WINDOW(const MAIN_WINDOW&) = delete;
	MAIN_WINDOW operator=(const MAIN_WINDOW) = delete;

	HWND m_hWnd;
	bool m_initialized;
	std::string m_strBkgFileName;
	CUSTOM_BITMAP m_backrgound;
};