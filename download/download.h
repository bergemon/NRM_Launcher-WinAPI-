#ifdef __cplusplus
extern "C"
{
	#include "structures.h"
	DWORD WINAPI check_version(_In_ LPVOID lpParameter);
	DWORD WINAPI download_new_version(_In_ LPVOID lpParameter);
	DWORD WINAPI get_zip_info(_In_ LPVOID lpParameter);
	DWORD WINAPI unzip_file(_In_ LPVOID lpParameter);
}
#else
	#include "structures.h"
	DWORD WINAPI check_version(_In_ LPVOID lpParameter);
	DWORD WINAPI download_new_version(_In_ LPVOID lpParameter);
	DWORD WINAPI get_zip_info(_In_ LPVOID lpParameter);
	DWORD WINAPI unzip_file(_In_ LPVOID lpParameter);
#endif