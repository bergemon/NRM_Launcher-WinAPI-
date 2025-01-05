#pragma once
#include <cstdint>
#include <string>

// Main window properties
#define MAIN_WINDOW_WIDTH 600
#define MAIN_WINDOW_HEIGHT 600

// Buttons properties
#define BTN_STD_WIDTH 200
#define BTN_STD_HEIGHT 45
#define BTN_BIG_HEIGHT 50
#define BTNS_STD_PADDING 0
#define BTN_PLAY_EXTRA_PADDING 30
#define BTNS_X_COORD_INVERT ((MAIN_WINDOW_WIDTH - BTN_STD_WIDTH) / 2)

// Discord button (special)
#define BTN_DISCORD_WIDTH 100
#define BTN_DISCORD_HEIGHT 80
#define BTN_DISCORD_EXTRA_PADDING 0
#define BTN_DISCORD_X_COORD 40
#define BTN_DISCORD_Y_COORD 400

// File paths
#define APP_DIR "launcher"
#define SLH "\\"
#define MASK_PREFIX "mask_"
#define MASK_PREFIX_LENGTH 5
#define ACTIVE_PREFIX "active_"
#define ACTIVE_PREFIX_LENGTH 7
#define BTN_PREFIX "btn_"
#define BTN_PREFIX_LENGTH 4

// Make string filepaths macro
#define BACKGROUND_PATH(path) std::string(APP_DIR SLH path).c_str()
#define BTN_BKG_PATH(path) std::string(APP_DIR SLH BTN_PREFIX path).c_str()

// Images names
#define MAIN_BACKGROUND "background.bmp"
#define BKG_BUTTON_QUIT "quit.bmp"
#define BKG_BUTTON_PLAY "play.bmp"
#define BKG_BUTTON_SUBMODS "submods.bmp"
#define BKG_BUTTON_SETTINGS "settings.bmp"
#define BKG_BUTTON_DOWNLOAD "download.bmp"
#define BKG_BUTTON_DISCORD "discord.bmp"