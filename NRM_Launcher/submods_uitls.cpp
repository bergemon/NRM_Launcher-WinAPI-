#include "submods_utils.h"
#include "submods_buttons.h"

//====================================================================
void fill_submods_buffer()
{
	// Get list of buttons and then buffer for submod buttons
	std::list<SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON>& buttons = SUBMODS_BUTTONS::getInstance().get_submods_buttons();
	std::vector<SUBMODS_BUTTONS::SUBMOD_BUTTON_BUFFER>& buffer = SUBMODS_BUTTONS::getInstance().get_submods_buttons_buffer();

	buffer.reserve(buttons.size());

	for (auto& button : buttons)
	{
		if (button.get_button_type() == SUBMODS_BUTTON_TYPE::BTN_SUBMOD)
		{
			button.set_buffer_button(buffer.emplace_back(button, button.is_checked()));
		}
	}
}
//====================================================================
void save_checked_from_buffer()
{
	// Get buttons buffer
	std::vector<SUBMODS_BUTTONS::SUBMOD_BUTTON_BUFFER>& buffer = SUBMODS_BUTTONS::getInstance().get_submods_buttons_buffer();

	for (SUBMODS_BUTTONS::SUBMOD_BUTTON_BUFFER& buf_button : buffer)
	{
		buf_button.m_button.set_checked(buf_button.m_isChecked);
	}
}
//====================================================================
void clear_buttons_state_in_buffer()
{
	// Get list of buttons and then buffer for submod buttons
	std::list<SUBMODS_BUTTONS::SUBMODS_WINDOW_BUTTON>& buttons = SUBMODS_BUTTONS::getInstance().get_submods_buttons();
	std::vector<SUBMODS_BUTTONS::SUBMOD_BUTTON_BUFFER>& buffer = SUBMODS_BUTTONS::getInstance().get_submods_buttons_buffer();

	for (auto& button : buttons)
	{
		if (button.get_button_type() == SUBMODS_BUTTON_TYPE::BTN_SUBMOD)
		{
			button.get_buffer_button().m_isChecked = button.is_checked();
		}
	}
}
//====================================================================