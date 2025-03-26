#include "submods_buttons.h"
#include "submods_modal.h"
#include "parse_submods.h"
#include "submods_utils.h"

//====================================================================
extern std::list<PARSED_SUBMOD> parsed_submods;
//====================================================================
void create_submods_modal_buttons()
{
	SUBMODS_BUTTONS& submod_window_buttons = SUBMODS_BUTTONS::getInstance();

	try
	{
		submod_window_buttons.initialize(
			// Parent
			SUBMODS_MODAL_WINDOW::getInstance().getHWnd(),
			// Classname
			TEXT("NRM_launcher_submods_window_button")
		);

		submod_window_buttons.create_button(
			SUBMODS_BUTTON_TYPE::BTN_SAVE,
			// width & height
			{ SUBMODS_STD_BUTTON_WIDTH, SUBMODS_STD_BUTTON_HEIGHT },
			// background file name
			"save.bmp",
			// extra padding
			NULL,
			// X pos
			SUBMODS_WINDOW_STD_BUTTONS_X_OFFSET,
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
			NULL,
			// X pos
			SUBMODS_WINDOW_STD_BUTTONS_X_OFFSET,
			// Y pos
			SUBMODS_STD_BUTTON_Y_START
		);

		for (const auto& submod : parsed_submods)
		{
			submod_window_buttons.create_submod_button(
				{ SUBMODS_CHECKBOX_BUTTON_WIDTH, SUBMODS_CHECKBOX_BUTTON_HEIGHT },
				submod.submod_name.c_str(),
				submod.submod_path.c_str()
			);

			if (&submod == &parsed_submods.back())
			{
				// Fill buffer for submod buttons
				fill_submods_buffer();
			}
		}
	}
	catch (std::exception& e)
	{
		MessageBoxA(submod_window_buttons.getParent(), e.what(), "Error", MB_OK);
	}
}
//====================================================================