#pragma once
#include "stdlib.h"
#include <windows.h>

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
	) throw (std::exception);

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	const HWND getHWnd()
	{
		return m_hMainWnd;
	}

	const std::string_view get_bkg_filename()
	{
		return std::string_view(m_strBkgFileName);
	};

private:
	MainWindow();
	MainWindow(const MainWindow&) = delete;
	MainWindow operator=(const MainWindow) = delete;

	HWND m_hMainWnd;
	bool m_initialized;
	std::string m_strBkgFileName;
};