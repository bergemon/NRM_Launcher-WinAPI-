#pragma once
#include <windows.h>
#include "stdlib.h"

//====================================================================
class SUBMODS_MODAL_WINDOW
{
public:
	[[nodiscard]] static SUBMODS_MODAL_WINDOW& getInstance();

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

	void show();
	void hide();

	~SUBMODS_MODAL_WINDOW();

	HWND getHWnd()
	{
		return m_hSubmodsWND;
	}

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	std::string_view get_background_path()
	{
		return m_bkgPath;
	}

private:
	SUBMODS_MODAL_WINDOW();
	SUBMODS_MODAL_WINDOW(const SUBMODS_MODAL_WINDOW&) = delete;
	SUBMODS_MODAL_WINDOW operator=(const SUBMODS_MODAL_WINDOW&) = delete;

	bool m_initialized;
	HWND m_hSubmodsWND;
	HWND m_hParentWND;
	std::string m_bkgPath;
	static std::string m_bkgFileName;
};
//====================================================================