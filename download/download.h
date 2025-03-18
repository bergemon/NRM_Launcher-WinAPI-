#ifdef __cplusplus
#define C_extern extern "C"

extern "C"
{
	#include "structures.h"
}

#else
#define C_extern 
#include "structures.h"
#endif

C_extern DWORD WINAPI check_version(_In_ LPVOID lpParameter);
C_extern DWORD WINAPI download_new_version(_In_ LPVOID lpParameter);
C_extern DWORD WINAPI get_zip_info(_In_ LPVOID lpParameter);
C_extern DWORD WINAPI unzip_file(_In_ LPVOID lpParameter);