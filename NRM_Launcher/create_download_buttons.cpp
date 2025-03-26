#include "download_buttons.h"
#include "download_modal.h"

void create_download_buttons()
{
	DOWNLOAD_WINDOW_BUTTONS& download_buttons = DOWNLOAD_WINDOW_BUTTONS::getInstance();

	download_buttons.initialize(
		DOWNLOAD_MODAL_WINDOW::getInstance().getHWnd(),
		DOWNLOAD_STATIC_BUTTON_CLASS
	);

	download_buttons.create_button(
		{ DOWNLOAD_BUTTONS_WIDTH, DOWNLOAD_BUTTONS_HEIGHT },
		DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_STATIC_BUTTON,
		"checkVersion.bmp"
	);
	download_buttons.create_button(
		{ DOWNLOAD_BUTTONS_WIDTH, DOWNLOAD_BUTTONS_HEIGHT },
		DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_STATIC_BUTTON,
		"downloadNewVersion.bmp"
	);
	download_buttons.create_button(
		{ DOWNLOAD_PROGRESSBAR_WIDTH, DOWNLOAD_PROGRESSBAR_HEIGHT },
		DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_PROGRESS_BAR
	);
	download_buttons.create_button(
		{ DOWNLOAD_BUTTONS_WIDTH, DOWNLOAD_BUTTONS_HEIGHT },
		DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_STATIC_BUTTON,
		"decompress.bmp"
	);
	download_buttons.create_button(
		{ DOWNLOAD_PROGRESSBAR_WIDTH, DOWNLOAD_PROGRESSBAR_HEIGHT },
		DOWNLOAD_WINDOW_BUTTONS::DOWNLOAD_PROGRESS_BAR
	);
}