#include "submods_buttons.h"

//====================================================================
void create_submods_modal_buttons(SUBMODS_BUTTONS& submod_window_buttons)
{
	submod_window_buttons.create_button(
		SUBMODS_BUTTON_TYPE::BTN_SAVE,
		// width & height
		{ SUBMODS_STD_BUTTON_WIDTH, SUBMODS_STD_BUTTON_HEIGHT },
		// background file name
		"save.bmp",
		// extra padding
		0,
		// X pos
		SUBMODS_WINDOW_BUTTONS_X_OFFSET,
		// Y pos
		SUBMODS_STD_BUTTON_Y_START - SUBMODS_STD_BUTTON_Y_SPACE - (SUBMODS_STD_BUTTON_HEIGHT * 1)
	);
	submod_window_buttons.create_button(
		SUBMODS_BUTTON_TYPE::BTN_CANCEL,
		// width & height
		{ SUBMODS_STD_BUTTON_WIDTH, SUBMODS_STD_BUTTON_HEIGHT },
		// background file name
		"cancel.bmp",
		// extra padding
		0,
		// X pos
		SUBMODS_WINDOW_BUTTONS_X_OFFSET,
		// Y pos
		SUBMODS_STD_BUTTON_Y_START
	);
}
//====================================================================