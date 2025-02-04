#pragma once
#include <windows.h>
#include "stdlibs.h"
#include "bitmap.h"
#include "get_file_path.h"

class MainWindow
{
public:
	~MainWindow();

	static MainWindow& getInstance();

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

	const HWND getHWnd();

	CUSTOM_BITMAP& get_background();

	const std::string_view get_bkg_filename();

private:
	MainWindow();
	MainWindow(const MainWindow&) = delete;
	MainWindow operator=(const MainWindow) = delete;

	HWND m_hMainWnd;
	bool m_initialized;
	std::string m_strBkgFileName;
	CUSTOM_BITMAP m_backrgound;
};