#pragma once
#include <windows.h>
#include "const.h"
#include "resource.h"
#include "stdlib.h"
#include "bitmap.h"
#include "settings_buttons_group.h"

//====================================================================
class SETTINGS_MODAL_WINDOW
{
public:
	[[nodiscard]] static SETTINGS_MODAL_WINDOW& getInstance();

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

	SETTINGS_BUTTONS_GROUP& create_buttons_group(
		const char* background_file_name,
		LAUNCHER_SETTINGS::SETTINGS_PARAM settings_param
	);

	CUSTOM_BITMAP& get_background();
	std::string_view get_background_path();
	HWND getHWnd();
	void show();
	void hide();

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	SETTINGS_MODAL_WINDOW();
	~SETTINGS_MODAL_WINDOW();
	SETTINGS_MODAL_WINDOW(const SETTINGS_MODAL_WINDOW&) = delete;
	SETTINGS_MODAL_WINDOW operator=(const SETTINGS_MODAL_WINDOW&) = delete;

	bool m_initialized;
	HWND m_hSettingsWND;
	HWND m_hParentWND;
	CUSTOM_BITMAP m_background;
	std::string m_bkgFileName;
	std::list<SETTINGS_BUTTONS_GROUP> m_buttonsGroups;
};
//====================================================================