#include "get_main_titlebar_text.h"

std::wstring& get_main_titlebar_text()
{
	LAUNCHER_SETTINGS& settings = LAUNCHER_SETTINGS::getInstance();

	std::wstringstream wss;
	wss << L"Victoria II New Realism mod";

	if (settings.get_game_version() && (settings.get_game_checksum().length() > 1))
	{
		std::wstring game_version = std::to_wstring(settings.get_game_version());

		wss << L" v"
			<< game_version.substr(0, 1) << "."
			<< game_version.substr(1, 2) << "."
			<< game_version.substr(3, 1)
			<< L" checksum<"
			<< settings.get_game_checksum()
			<< L">";
	}
	else
	{
		wss << L" <Игра не скачена>";
	}

	static std::wstring wstr;
	wstr.clear();
	wstr = wss.str();

	return wstr;
}