#pragma once
#include "stdlib.h"
#include <windows.h>

class MainWindow
{
public:
	MainWindow(
		LPCWSTR className,
		LPCWSTR windowName,
		HINSTANCE hInstance,
		int nCmdShow,
		LPCWSTR menuName = NULL,
		uint32_t classStyle = CS_HREDRAW | CS_VREDRAW,
		uint32_t width = CW_USEDEFAULT,
		uint32_t height = 0,
		DWORD windowStyle = WS_OVERLAPPEDWINDOW,
		DWORD windowExStyle = WS_EX_TRANSPARENT,
		const char* bkgFileName = nullptr
	);

	~MainWindow();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	const HWND getHWnd() { return m_hMainWnd; }

private:
	HWND m_hMainWnd;
	static std::string m_strBkgFileName;
};