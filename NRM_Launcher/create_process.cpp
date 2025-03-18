#include "create_process.h"
#include "submods_buttons.h"

// This function can throw an exception!
void create_process() noexcept(false)
{
	namespace fs = std::filesystem;

	STARTUPINFOA startUpInfo{ 0 };
	startUpInfo.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION processInfo;

	// Cmd string
	std::string nCmd = "";
	auto& submods_buttons = SUBMODS_BUTTONS::getInstance().get_submods_buttons();
	// Loop to bypass list
	for (auto& button : submods_buttons)
	{
		if (
			button.get_button_type() == SUBMODS_BUTTON_TYPE::BTN_SUBMOD
			&& button.is_checked()
		)
		{
			nCmd.append(" -mod=");
			nCmd.append(button.get_submod_path());
		}
	}
	
	for (auto it = nCmd.begin(); it != nCmd.end(); ++it)
	{
		if (*it == '\\')
		{
			*it = '/';
		}
	}

	fs::path originalPath(fs::current_path());
	fs::path game_path(fs::current_path());
	game_path += SLH GAME_PATH;

	if (!fs::exists(game_path))
	{
		throw std::exception("Game path does not exist");
	}

	// We must change current path
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

	// Then return original program path
	fs::current_path(originalPath);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}