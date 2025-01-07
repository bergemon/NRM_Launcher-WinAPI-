#include "settings_parser.h"
#ifdef DEBUG
	#include "windows.h"
#endif

[[nodiscard]] bool is_settings_exist()
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
			if (buffer[0] != (wchar_t)0xfeff)
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
				line.erase(line.length() - 1, line.length() - 1);
				if (check.find(line.c_str()) == std::wstring::npos)
				{
					if (settings_file.is_open())
					{
						settings_file.close();
					}
					return false;
				}
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

[[nodiscard]] std::wstring settings_find_line(std::wstring substr)
{
	// Settings file must exist
	if (!is_settings_exist())
	{
		throw std::exception("Settings file does not exist");
	}

	std::ifstream settings_file;
	// Buffer to read data
	wchar_t buffer[2];
	// Zero terminated C string
	buffer[1] = L'\0';
	// String buffer to accumulate read data
	std::wstring line;
	// Check BOM first
	bool checked_bom = false;
	// Skip first line
	bool first_line = true;
	// Lines counter to check then if file has too many lines
	size_t lines_count = 0;

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
			++lines_count;
			checked_bom = true;
		}
		else
		{
			// Check line for max permitted length
			if (line.size() > SETTINGS_LINE_MAX_LENGTH)
			{
				throw std::exception("Read line has too many characters");
			}

			// If we found line feed character
			if (buffer[0] == 0x000A)
			{
				// Check for max permitted lines
				if (++lines_count > SETTINGS_DATA_LINES)
				{
					throw std::exception("Settings file has too many lines");
				}
				// Skip first line
				if (first_line)
				{
					first_line = false;
					line.clear();
				}
				else
				{
					// Every settings line except first must be ended by ";" symbol
					if (
						line.rfind(
							TO_W_STRING(SETTINGS_LINE_TERMINATE))
							!=
							(line.size() - std::wstring(TO_W_STRING(SETTINGS_LINE_TERMINATE)).size() - 1)
						)
					{
						throw std::exception("Invalid settings data");
					}
					// Find substring in read line
					if (line.find(substr.c_str()) != std::wstring::npos)
					{
						break;
					}
					// Clear line buffer if didn't find
					else
					{
						line.clear();
					}
				}
			}
			line.append(buffer);
		}
	}

	return line.substr(
		line.find(substr.c_str()) + substr.length(),
		line.length() - line.find(substr) - substr.length() - std::wstring(TO_W_STRING(SETTINGS_LINE_TERMINATE)).length()
	);
}