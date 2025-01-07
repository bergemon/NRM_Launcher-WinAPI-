#pragma once
#include <fstream>
#include <filesystem>
#include <string>
#include "const.h"
#include "windows.h"

[[nodiscard]] bool is_settings_exist();
[[nodiscard]] std::wstring settings_find_line(std::wstring substr) throw (std::exception);