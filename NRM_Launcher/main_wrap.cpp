#include <windows.h>

int WINAPI MyWinMain(_In_ HINSTANCE hInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	return MyWinMain(hInstance, lpCmdLine, nCmdShow);
}