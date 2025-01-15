#include "create_process.h"

// This function can throw an exception!
void create_process() throw (std::exception)
{
	namespace fs = std::filesystem;

	STARTUPINFOA startUpInfo{ 0 };
	startUpInfo.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION processInfo;
	std::string nCmd = "";

	fs::path originalPath(fs::current_path());
	fs::path game_path(fs::current_path());
	game_path += SLH GAME_PATH;

	if (!fs::exists(game_path))
	{
		throw std::exception("Game path does not exist");
	}

	fs::current_path(game_path);

	bool result = CreateProcessA(
		VIC_EXE_NAME,
		nCmd.data(),
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&startUpInfo,
		&processInfo
	);

	if (!result)
	{
		throw std::exception("Can not create victoria 2 process");
	}

	fs::current_path(originalPath);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}