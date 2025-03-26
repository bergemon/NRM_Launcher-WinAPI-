#pragma once
#include <fstream>
#include <filesystem>
#include <string>
#include "stdlibs.h"
#include "const.h"
#include "windows.h"

//====================================================================
class LAUNCHER_SETTINGS
{
public:
	enum SETTINGS_PARAM
	{
		DISCORD,
		ON_PLAY,
		CLEAR_CACHE,
		VERSION,
		CHECKSUM,
		THREADS_TO_UNZIP
	};

	[[nodiscard]] const std::wstring_view get_discord_link() const;
	[[nodiscard]] uint32_t get_on_play_state() const;
	[[nodiscard]] uint32_t get_clear_cache_state() const;
	[[nodiscard]] bool is_settings_exist() const noexcept(false);
	[[nodiscard]] uint32_t get_fields_count() const;
	[[nodiscard]] int32_t get_game_version() const;
	[[nodiscard]] const std::wstring get_game_checksum() const;
	[[nodiscard]] char* get_game_checksum_ascii();
	[[nodiscard]] const int32_t get_num_of_threads() const noexcept(true);
	void set_param(SETTINGS_PARAM param, const wchar_t* value_param) noexcept(false);
	void rewrite_settings() const;

	static LAUNCHER_SETTINGS& getInstance();

private:
	struct PARAM_LINE
	{
		SETTINGS_PARAM param;
		std::wstring description;
		std::wstring value;

		void set_value(const wchar_t* value);
		std::wstring get_full_param_line();
	};

	LAUNCHER_SETTINGS() noexcept(true);
	LAUNCHER_SETTINGS(const LAUNCHER_SETTINGS&) = delete;
	LAUNCHER_SETTINGS operator=(const LAUNCHER_SETTINGS&) = delete;
	~LAUNCHER_SETTINGS() noexcept(true);

	bool is_initialized() const;
	void initialize() noexcept(false);
	void parse_settings() noexcept(false);

	// settings params
	std::vector<PARAM_LINE> m_params;

	// other stuff
	bool m_settingsParsed = false;
	uint32_t m_count_of_fields = SETTINGS_DATA_FIELDS;
};
//====================================================================