#include "button.h"
#include "const.h"

//====================================================================
void create_buttons(LAUNCHER_BUTTONS& btns_singleton)
{
	btns_singleton.create_button(
		BUTTON_TYPE::BTN_EXIT,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BTN_BKG_PATH(BKG_BUTTON_QUIT)
	);
	btns_singleton.create_button(
		BUTTON_TYPE::BTN_PLAY,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_BIG_HEIGHT },
		BTN_BKG_PATH(BKG_BUTTON_PLAY)
	);
	btns_singleton.create_button(
		BUTTON_TYPE::BTN_SUBMODS,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BTN_BKG_PATH(BKG_BUTTON_SUBMODS)
	);
	btns_singleton.create_button(
		BUTTON_TYPE::BTN_SETTINGS,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BTN_BKG_PATH(BKG_BUTTON_SETTINGS)
	);
	btns_singleton.create_button(
		BUTTON_TYPE::BTN_DOWNLOAD,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_STD_HEIGHT },
		BTN_BKG_PATH(BKG_BUTTON_DOWNLOAD)
	);
	btns_singleton.create_button(
		BUTTON_TYPE::BTN_DISCORD,
		BUTTON_PROPERTIES{ BTN_STD_WIDTH, BTN_BIG_HEIGHT },
		BTN_BKG_PATH(BKG_BUTTON_DISCORD),
		// Extra padding
		BTN_DISCORD_EXTRA_PADDING
	);
}
//====================================================================