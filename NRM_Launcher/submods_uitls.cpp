#include "submods_utils.h"
#include "submods_buttons.h"

void set_submods_unchecked()
{
	std::list<SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON>& submod_buttons = SUBMODS_BUTTONS::getInstance().get_submods_buttons();

	for (auto& button : submod_buttons)
	{
		button.set_checked(false);
	}
}