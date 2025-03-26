#include "settings_parser.h"

//====================================================================
std::wstring LAUNCHER_SETTINGS::PARAM_LINE::get_full_param_line()
{
	std::wstring line = description;
	line.append(TO_W_STRING(SETTINGS_PARAM_DELIMETER))
		.append(value).append(TO_W_STRING(SETTINGS_LINE_TERMINATE));

	return line;
}
//====================================================================
void LAUNCHER_SETTINGS::PARAM_LINE::set_value(const wchar_t* param_value)
{
	value = param_value;
}
//====================================================================
LAUNCHER_SETTINGS& LAUNCHER_SETTINGS::getInstance()
{
	static LAUNCHER_SETTINGS settings;

	if (!settings.is_initialized())
	{
		settings.initialize();
	}

	return settings;
}
//====================================================================
bool LAUNCHER_SETTINGS::is_initialized() const
{
	return m_settingsParsed;
}
//====================================================================
void LAUNCHER_SETTINGS::initialize() noexcept(false)
{
	// Settings file must exist
	if (!is_settings_exist())
	{
		throw std::exception("Settings file does not exist");
	}

	m_params.reserve(SETTINGS_DATA_FIELDS);
	m_params.emplace_back(SETTINGS_PARAM::DISCORD, TO_W_STRING("discord_link"), TO_W_STRING("0"));
	m_params.emplace_back(SETTINGS_PARAM::ON_PLAY, TO_W_STRING("on_play"), TO_W_STRING("0"));
	m_params.emplace_back(SETTINGS_PARAM::CLEAR_CACHE, TO_W_STRING("clear_cache"), TO_W_STRING("0"));
	m_params.emplace_back(SETTINGS_PARAM::VERSION, TO_W_STRING("ver"), TO_W_STRING("0"));
	m_params.emplace_back(SETTINGS_PARAM::CHECKSUM, TO_W_STRING("checksum"), TO_W_STRING("0"));
	m_params.emplace_back(SETTINGS_PARAM::THREADS_TO_UNZIP, TO_W_STRING("threads_to_unzip"), TO_W_STRING("0"));

	parse_settings();

	m_settingsParsed = true;
}
//====================================================================
LAUNCHER_SETTINGS::LAUNCHER_SETTINGS() noexcept(true)
{
}
//====================================================================
LAUNCHER_SETTINGS::~LAUNCHER_SETTINGS() noexcept(true)
{
}
//====================================================================
uint32_t LAUNCHER_SETTINGS::get_on_play_state() const
{
	uint32_t num = std::atoi((const char*)m_params[SETTINGS_PARAM::ON_PLAY].value.c_str());
	return num;
}
//====================================================================
uint32_t LAUNCHER_SETTINGS::get_clear_cache_state() const
{
	uint32_t num = std::atoi((const char*)m_params[SETTINGS_PARAM::CLEAR_CACHE].value.c_str());
	return num;
}
//====================================================================
[[nodiscard]] uint32_t LAUNCHER_SETTINGS::get_fields_count() const
{
	return m_count_of_fields;
}
//====================================================================
int32_t LAUNCHER_SETTINGS::get_game_version() const
{
	if (
		m_params[SETTINGS_PARAM::VERSION].value.length() == 1
		&& m_params[SETTINGS_PARAM::VERSION].value[0] == '0'
	)
	{
		return 0;
	}
	else if (m_params[SETTINGS_PARAM::VERSION].value.length() != 4)
	{
		throw std::runtime_error("Game version has wrong number of digits, must be 4");
	}

	// Cut UTF-16 LE char to 1 byte
	char* version_buffer = new char[5];
	for (int i = 0; i < 4; ++i)
	{
		version_buffer[i] = (char)m_params[SETTINGS_PARAM::VERSION].value[i];
	}
	version_buffer[4] = '\0';

	int32_t num = std::atoi(version_buffer);

	delete[] version_buffer;

	return num;
}
//====================================================================
[[nodiscard]] const std::wstring LAUNCHER_SETTINGS::get_game_checksum() const
{
	return m_params[SETTINGS_PARAM::CHECKSUM].value;
}
//====================================================================
[[nodiscard]] char* LAUNCHER_SETTINGS::get_game_checksum_ascii()
{
	size_t length = get_game_checksum().length();

	char* checksum_truncated = nullptr;

	if (!checksum_truncated)
	{
		checksum_truncated = new char[length + 1];
	}

	for (int i = 0; i <= length; ++i)
	{
		checksum_truncated[i] = (char)*(get_game_checksum().data() + i);
	}

	return checksum_truncated;
}
//====================================================================
[[nodiscard]] const int32_t LAUNCHER_SETTINGS::get_num_of_threads() const
	noexcept(true)
{
	std::wstring threads = m_params[SETTINGS_PARAM::THREADS_TO_UNZIP].value;

	char* threads_truncated = new char[threads.length() + 1];

	for (int32_t i = 0; i < threads.length(); ++i)
	{
		threads_truncated[i] = (char)threads[i];
	}
	threads_truncated[threads.length()] = '\0';

	return std::atoi(threads_truncated);
}
//====================================================================
void LAUNCHER_SETTINGS::set_param(SETTINGS_PARAM param, const wchar_t* value_param)
	noexcept(false)
{
	if ((size_t)param >= m_params.size())
	{
		throw std::exception("Wrong param to set");
	}

	m_params[param].value = value_param;
}
//====================================================================
[[nodiscard]] const std::wstring_view LAUNCHER_SETTINGS::get_discord_link() const
{
	return m_params[SETTINGS_PARAM::DISCORD].value;
}
//====================================================================
void LAUNCHER_SETTINGS::rewrite_settings() const
{
	namespace fs = std::filesystem;
	std::string settings_file_path(APP_DIR SLH SETTINGS);

	fs::path settings_path(settings_file_path);

	if (fs::exists(settings_path))
	{
		std::ofstream settings_file(settings_file_path, std::ios::binary);
		std::wstringstream wss;
		if (settings_file.is_open())
		{
			const char bom[] = { (char)0xff, (char)0xfe };
			settings_file.write(bom, 2);

			wss << TO_W_STRING(SETTINGS_HEAD)
				<< TO_W_STRING("\r\n");

			for (auto& param : m_params)
			{
				wss << param.description.c_str()
					<< TO_W_STRING(SETTINGS_PARAM_DELIMETER)
					<< param.value.c_str()
					<< TO_W_STRING(SETTINGS_LINE_TERMINATE)
					<< TO_W_STRING("\r\n");
			}

			settings_file.write((const char*)wss.str().c_str(), wss.str().length() * 2);

			settings_file.close();
		}
	}
}
//====================================================================
[[nodiscard]] bool LAUNCHER_SETTINGS::is_settings_exist() const noexcept(false)
{
	namespace fs = std::filesystem;

	std::string settings_file_path(APP_DIR SLH SETTINGS);

	fs::path settings_path(settings_file_path);

	if (!fs::exists(settings_path))
	{
		return false;
	}

	std::ifstream settings_file;
	settings_file.open(settings_file_path);
	if (!settings_file.is_open())
	{
		return false;
	}

	// String to compare
	std::wstring check = TO_W_STRING(SETTINGS_HEAD);
	// String buffer to accumulate read data
	std::wstring line;
	// Buffer to read data
	wchar_t buffer[2];
	// Zero terminated C string
	buffer[1] = L'\0';
	// Check BOM first
	bool checked_bom = false;

	while (settings_file.read((char*)buffer, 2))
	{
		// Codepage must be UTF-16 LE BOM
		if (!checked_bom)
		{
			if (HIBYTE(buffer[0]) != (wchar_t)0xfe || LOBYTE(buffer[0]) != (wchar_t)0xff)
			{
				throw std::exception("Invalid codepage, must be UTF-16 LE");
			}
			checked_bom = true;
		}
		else
		{
			// If we found line feed character
			if (buffer[0] == 0x000A)
			{
				// Check settings first line for head info
				line.erase(line.length() - 1, 1);
				if (check.find(line.c_str()) == std::wstring::npos)
				{
					if (settings_file.is_open())
					{
						settings_file.close();
					}
					return false;
				}
				break;
			}
			line.append(buffer);
		}
	}

	if (settings_file.is_open())
	{
		settings_file.close();
	}
	return true;
}

void LAUNCHER_SETTINGS::parse_settings()
{
	std::ifstream settings_file;
	// Buffer to read data
	wchar_t buffer[2];
	// Zero terminated C string
	buffer[1] = L'\0';
	// String to compare
	std::wstring check = TO_W_STRING(SETTINGS_HEAD);
	// String buffer to accumulate read data
	std::wstring field;
	// Check BOM first
	bool checked_bom = false;
	// Skip first line
	bool first_field = true;
	// Fields counter to check then if file has too many fields
	size_t fields_count = 0;
	uint32_t param_to_get = 0;

	settings_file.open(APP_DIR SLH SETTINGS);
	if (!settings_file.is_open())
	{
		throw std::exception("Can not open settings file");
	}

	while (settings_file.read((char*)buffer, 2))
	{
		// Skip BOM, we've checked it before
		if (!checked_bom)
		{
			if (HIBYTE(buffer[0]) != (wchar_t)0xfe || LOBYTE(buffer[0]) != (wchar_t)0xff)
			{
				throw std::exception("Invalid codepage, must be UTF-16 LE");
			}
			checked_bom = true;
		}
		else
		{
			// Check line for max permitted length
			if (field.size() > SETTINGS_FIELD_MAX_LENGTH)
			{
				throw std::exception("Read line has too many characters");
			}

			// If we found line feed character
			if (buffer[0] == 0x000A || buffer[1] == 0x000A)
			{
				// Check for max permitted fields
				if (fields_count++ > SETTINGS_DATA_FIELDS)
				{
					throw std::exception("Settings file has too many fields");
				}
				// Skip first field
				if (first_field)
				{
					if (field.find(check.c_str()) == std::wstring::npos)
					{
						throw std::exception("Wrong settings header");
					}
					first_field = false;
					field.clear();
				}
				else
				{
					if (param_to_get == m_params.size())
					{
						break;
					}
					LAUNCHER_SETTINGS::PARAM_LINE& param_s = m_params[param_to_get++];

					// Every settings line except first must be ended by ";" symbol
					if (
						field.rfind(TO_W_STRING(SETTINGS_LINE_TERMINATE))
						!= field.size() - std::wstring(TO_W_STRING(SETTINGS_LINE_TERMINATE)).size() - 1
						)
					{
						throw std::exception("Invalid settings data");
					}
					// Find substring in read line
					if (field.find(param_s.description) != std::wstring::npos)
					{
						std::wstring parsed_param = field.substr(
							// Start pos
							field.find(TO_W_STRING(SETTINGS_PARAM_DELIMETER)) + 1,
							// Count of characters
							field.length() - field.find(TO_W_STRING(SETTINGS_PARAM_DELIMETER))
							- std::wstring(TO_W_STRING(SETTINGS_PARAM_DELIMETER)).length() - 2
						);
						param_s.set_value(parsed_param.c_str());
					}
					// Clear line buffer
					field.clear();
				}
			}
			field.append(buffer);
		}
	}
}
//====================================================================