#pragma once
#include <cstdint>
#include <string>

//====================================================================
// Main window properties
#define MAIN_WINDOW_WIDTH 600
#define MAIN_WINDOW_HEIGHT 600
#define MAIN_WINDOW_CLASS TEXT("NRM_launcher_main_window")
//====================================================================
// Buttons properties
#define BTN_STD_WIDTH 200
#define BTN_STD_HEIGHT 45
#define BTN_BIG_HEIGHT 50
#define BTNS_STD_PADDING 0
#define BTN_PLAY_EXTRA_PADDING 30
#define BTNS_X_COORD_INVERT ((MAIN_WINDOW_WIDTH - BTN_STD_WIDTH) / 2)
//====================================================================
// Discord button (special)
#define BTN_DISCORD_WIDTH 100
#define BTN_DISCORD_HEIGHT 80
#define BTN_DISCORD_EXTRA_PADDING 0
#define BTN_DISCORD_X_COORD 40
#define BTN_DISCORD_Y_COORD 400
//====================================================================
// Play button
#define VIC_EXE_NAME "vic2.exe"
//====================================================================
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
#define ICON_PREFIX "icon_"
//====================================================================
// Images names
#define MAIN_BACKGROUND "background.bmp"
#define BKG_BUTTON_QUIT "quit.bmp"
#define BKG_BUTTON_PLAY "play.bmp"
#define BKG_BUTTON_SUBMODS "submods.bmp"
#define BKG_BUTTON_SETTINGS "settings.bmp"
#define BKG_BUTTON_DOWNLOAD "download.bmp"
#define BKG_BUTTON_DISCORD "discord.bmp"
//====================================================================
// Settings file
#define SETTINGS "settings"
#define SETTINGS_HEAD "NRM_LAUNCHER_SETTINGS"
#define SETTINGS_LINE_TERMINATE ";"
#define SETTINGS_PARAM_DELIMETER ":"
#define SETTINGS_DATA_LINES 4
#define SETTINGS_LINE_MAX_LENGTH 200
//====================================================================
// Utility
#define TO_W_STRING(quote) __to_wide_str(quote)
#define __to_wide_str(quote) L##quote
//====================================================================
// Submods window
#define SUBMODS_WINDOW_PATH "submods_window"
#define SUBMODS_WINDOW_BACKGROUND "submods_background.bmp"
#define SUBMODS_BUTTONS_PATH "buttons"
#define SUBMODS_MASKS_PATH "masks"
#define SUBMODS_BUTTONS_FULL_PATH APP_DIR SLH SUBMODS_WINDOW_PATH SLH SUBMODS_BUTTONS_PATH
#define SUBMODS_WINDOW_WIDTH 250
#define SUBMODS_WINDOW_HEIGHT 400
#define SUBMODS_WINDOW_CLASS TEXT("NRM_launcher_sumbods_modal_window")
#define SUBMODS_WINDOW_STD_BUTTONS_X_OFFSET 45
#define SUBMODS_BUTTONS_X_OFFSET 20
#define SUBMOD_BUTTONS_Y_START 10
#define SUBMOD_BUTTONS_Y_PADDING 10
#define SUBMODS_STD_BUTTON_WIDTH 150
#define SUBMODS_STD_BUTTON_HEIGHT 30
#define SUBMODS_CHECKBOX_BUTTON_WIDTH 200
#define SUBMODS_CHECKBOX_BUTTON_HEIGHT 20
#define SUBMOD_NAME_MAX_LENGTH_TO_DRAW 14
#define SUBMODS_UNCHECKED_BUTTON_BKG "unchecked.bmp"
#define SUBMODS_CHECKED_BUTTON_BKG "checked.bmp"
#define SUBMODS_STD_BUTTON_Y_START 320
#define SUBMODS_STD_BUTTON_Y_SPACE 5
#define SUBMOD_BUTTON_HEIGHT 20
#define BTNS_SUBMODS_X_COORD_INVERT(width) ((SUBMODS_WINDOW_WIDTH - width) / 2)
#define SUBMODS_WINDOW_X_OFFSET ((MAIN_WINDOW_WIDTH - SUBMODS_WINDOW_WIDTH) / 2)
#define SUBMODS_WINDOW_Y_OFFSET ((MAIN_WINDOW_HEIGHT - SUBMODS_WINDOW_HEIGHT) / 2)
#define MAX_SUBMODS_COUNT 9
//====================================================================
// Settings window
#define SETTINGS_WINDOW_WIDTH 400
#define SETTINGS_WINDOW_HEIGHT 400
#define BORDER_CONST 10
#define SETTINGS_WINDOW_CLASS TEXT("NRM_launcher_settings_window")
#define SETTINGS_WINDOW_BACKGROUND "settings_backgrond.bmp"
#define SETTINGS_WINDOW_PATH "settings_window"
#define SETTINGS_BUTTONS_PATH "buttons"
#define SETTINGS_MASKS_PATH "masks"
#define SETTINGS_FULL_PATH APP_DIR SLH SETTINGS_WINDOW_PATH SLH
#define SETTINGS_WINDOW_X_OFFSET (MAIN_WINDOW_WIDTH - SETTINGS_WINDOW_WIDTH) / 2
#define SETTINGS_WINDOW_Y_OFFSET (MAIN_WINDOW_HEIGHT - SETTINGS_WINDOW_HEIGHT) / 2
#define SETTINGS_STATIC_BUTTON_CLASS TEXT("NRM_launcher_settings_static_button")
#define SETTINGS_RADIO_BUTTON_CLASS TEXT("NRM_launcher_settings_radio_button")
#define SETTINGS_STATIC_BUTTON_HEIGHT 30
#define SETTINGS_RADIO_BUTTON_HEIGHT 30
#define SETTINGS_GROUP_BUTTONS_VERTICAL_SPACE 5
#define SETTINGS_GROUPS_SPACE_BETWEEN 30
#define SETTINGS_NEXT_GROUP_POSITION_ADD_COORD SETTINGS_STATIC_BUTTON_HEIGHT + SETTINGS_RADIO_BUTTON_HEIGHT + SETTINGS_GROUP_BUTTONS_SPACE + SETTINGS_GROUPS_SPACE_BETWEEN
#define SETTINGS_WINDOW_BUTTONS_GROUP_START_Y_POS 10
#define SETTINGS_STATIC_BUTTON_WIDTH 350
#define SETTINGS_WINDOW_STATIC_BUTTON_OFFSET ((SETTINGS_WINDOW_WIDTH - SETTINGS_STATIC_BUTTON_WIDTH - BORDER_CONST) / 2)
#define SETTINGS_RADIO_BUTTON_CHECKED_ICON_WIDTH 30
#define SETTINGS_RADIO_BUTTON_CHECKED_ICON_HEIGHT 30
#define SETTINGS_RADIO_BUTTON_CION_PATH SETTINGS_FULL_PATH SLH SETTINGS_BUTTONS_PATH SLH ICON_PREFIX
#define SETTINGS_RADIO_BUTTON_CHECKED_FILENAME "radioChecked.bmp"
#define SETTINGS_RADIO_BUTTON_UNCHECKED_FILENAME "radioUnchecked.bmp"