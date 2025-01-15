#pragma once
#include "defines.h"
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

// Play button
#define VIC_EXE_NAME "vic2.exe"

// File paths
#define APP_DIR "launcher"
#define SLH "\\"
#define GAME_PATH "game"
#define BUTTONS_PATH "buttons"
#define MASKS_PATH "masks"
#define BUTTONS_BKG_FULL_PATH APP_DIR SLH BUTTONS_PATH SLH
#define MASKS_FULL_PATH APP_DIR SLH BUTTONS_PATH SLH MASKS_PATH SLH
#define MASK_PREFIX "mask_"
#define ACTIVE_PREFIX "active_"
#define BTN_PREFIX "btn_"

// Images names
#define MAIN_BACKGROUND "background.bmp"
#define BKG_BUTTON_QUIT "quit.bmp"
#define BKG_BUTTON_PLAY "play.bmp"
#define BKG_BUTTON_SUBMODS "submods.bmp"
#define BKG_BUTTON_SETTINGS "settings.bmp"
#define BKG_BUTTON_DOWNLOAD "download.bmp"
#define BKG_BUTTON_DISCORD "discord.bmp"

// Settings file
#define SETTINGS "settings"
#define SETTINGS_HEAD "NRM_LAUNCHER_SETTINGS"
#define SETTINGS_LINE_TERMINATE ";"
#define SETTINGS_DATA_LINES 2
#define SETTINGS_LINE_MAX_LENGTH 200

// Utility
#define TO_W_STRING(quote) __to_wide_str(quote)
#define __to_wide_str(quote) L##quote

// Submods window
#define SUBMODS_WINDOW_PATH "submods_window"
#define SUBMODS_WINDOW_BACKGROUND "submods_background.bmp"
#define SUBMODS_BUTTONS_PATH "buttons"
#define SUBMODS_MASKS_PATH "masks"
#define SUBMODS_BUTTONS_FULL_PATH APP_DIR SLH SUBMODS_WINDOW_PATH SLH SUBMODS_BUTTONS_PATH
#define SUBMODS_WINDOW_WIDTH 250
#define SUBMODS_WINDOW_HEIGHT 400
#define SUBMODS_WINDOW_BUTTONS_X_OFFSET 50
#define SUBMODS_STD_BUTTON_WIDTH 150
#define SUBMODS_STD_BUTTON_HEIGHT 30
#define SUBMODS_STD_BUTTON_Y_START 350
#define SUBMODS_STD_BUTTON_Y_SPACE 10
#define SUBMOD_BUTTON_WIDTH 150
#define SUBMOD_BUTTON_HEIGHT 20
#define BTNS_SUBMODS_X_COORD_INVERT(width) ((SUBMODS_WINDOW_WIDTH - width) / 2)
#define SUBMODS_WINDOW_X_OFFSET (MAIN_WINDOW_WIDTH - SUBMODS_WINDOW_WIDTH) / 2
#define SUBMODS_WINDOW_Y_OFFSET (MAIN_WINDOW_HEIGHT - SUBMODS_WINDOW_HEIGHT) / 2