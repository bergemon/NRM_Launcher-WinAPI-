#pragma once
#include "windows.h"
#include "filesystem"

struct CREATE_OUT_PATH_INFO
{
	bool is_ended = false;
	char* output_path = nullptr;
};

DWORD WINAPI create_out_path(_In_ LPVOID lpParameter);