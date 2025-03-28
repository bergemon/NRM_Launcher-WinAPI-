#include "button.h"
#include "main_window.h"
#include "const.h"

//====================================================================
void create_buttons(int nCmdShow)
{
	LAUNCHER_BUTTONS& main_buttons = LAUNCHER_BUTTONS::getInstance();
	main_buttons.initialize(
		// Parent handler 
		MAIN_WINDOW::getInstance().getHWnd(),
		// Class name
		MAIN_BUTTONS_CLASS,
		// Cmd
		nCmdShow
	);

	main_buttons.create_button(
		BUTTON_TYPE::BTN_EXIT,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BKG_BUTTON_QUIT
	);
	main_buttons.create_button(
		BUTTON_TYPE::BTN_SUBMODS,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BKG_BUTTON_SUBMODS
	);
	main_buttons.create_button(
		BUTTON_TYPE::BTN_SETTINGS,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BKG_BUTTON_SETTINGS
	);
	main_buttons.create_button(
		BUTTON_TYPE::BTN_DOWNLOAD,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BKG_BUTTON_DOWNLOAD
	);
	main_buttons.create_button(
		BUTTON_TYPE::BTN_PLAY,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_BIG_HEIGHT },
		BKG_BUTTON_PLAY,
		BTN_PLAY_EXTRA_PADDING
	);
	main_buttons.create_button(
		BUTTON_TYPE::BTN_DISCORD,
		BUTTON_PROPERTIES{ BTN_DISCORD_WIDTH, BTN_DISCORD_HEIGHT },
		BKG_BUTTON_DISCORD,
		// Extra padding
		BTN_DISCORD_EXTRA_PADDING,
		// x and y coords (if needed)
		BTN_DISCORD_X_COORD, BTN_DISCORD_Y_COORD
	);
}
//====================================================================