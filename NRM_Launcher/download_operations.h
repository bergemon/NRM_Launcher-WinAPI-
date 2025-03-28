#pragma once
#include <windows.h>
#include "remove_all_thread.h"
#include "download_buttons.h"
#include "settings_parser.h"
#include "download_modal.h"
#include "main_window.h"
#include "get_main_titlebar_text.h"
#include "parse_submods.h"
#include "download.h"
#include "submods_buttons.h"

//====================================================================
void timer_check_version(HWND hWnd, GAME_VERSION& ver, UINT_PTR timer_id, HANDLE check_ver_thread);
//====================================================================
void timer_download_game(HWND hWnd, GAME_VERSION& ver, UINT_PTR timer_id, HANDLE download_game_thread);
//====================================================================
void timer_unzip(
	HWND hWnd,
	UINT_PTR timer_id,
	CREATE_OUT_PATH_INFO& out_info,
	bool& output_path_created,
	int32_t& num_of_threads,
	HMODULE library,
	std::vector<HANDLE>& unzip_threads,
	std::vector<UNZIP_INFO>& unzip_info_vector,
	GAME_VERSION& ver,
	ZIP_INFO& zip_info,
	std::string& zip_file_name,
	std::string& zip_out_path,
	HANDLE create_out_path_thread,
	CURRENT_OPERATION& operation,
	CRITICAL_SECTION& cs
);
//====================================================================
LRESULT stop_downloading_operation(
	HWND hWnd,
	int32_t& num_of_threads,
	HANDLE check_ver_thread,
	HANDLE download_game_thread,
	HANDLE create_out_path_thread,
	bool& output_path_created,
	std::vector<HANDLE>& unzip_threads,
	std::vector<UNZIP_INFO>& unzip_info_vector,
	GAME_VERSION& ver,
	std::string& zip_file_name,
	UINT_PTR timer_id,
	CURRENT_OPERATION& operation,
	HMODULE library,
	CRITICAL_SECTION& cs
);
//====================================================================