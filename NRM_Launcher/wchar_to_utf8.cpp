#include "wchar_to_utf8.h"

std::string wchar_to_utf8(const std::wstring wstr)
{
	int size_needed = WideCharToMultiByte(
		CP_UTF8,
		WC_NO_BEST_FIT_CHARS,
		&wstr[0],
		(int)wstr.length(),
		NULL,
		0,
		NULL,
		NULL
	);

	std::string utf8_str(size_needed, 0);

	WideCharToMultiByte(
		CP_UTF8,
		WC_NO_BEST_FIT_CHARS,
		&wstr[0],
		(int)wstr.length(),
		&utf8_str[0],
		size_needed,
		NULL,
		NULL
	);
	
	return utf8_str;
}