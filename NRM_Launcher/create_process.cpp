#include "create_process.h"

// This function can throw an exception!
[[nodiscard]] void create_process()
{
	namespace fs = std::filesystem;

	STARTUPINFOA startUpInfo{ 0 };
	startUpInfo.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION processInfo;
	std::string nCmd = "";

	fs::path originalPath(fs::current_path());
	fs::path cPath(fs::current_path());
	cPath += GAME_PATH SLH;
	fs::current_path(cPath);

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
	fs::current_path(originalPath);

	if (!result)
	{
		throw std::exception("Can not create vic2 process");
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}