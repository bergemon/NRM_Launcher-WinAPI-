#include "settings_buttons_group.h"
#include "settings_modal.h"

//====================================================================
void create_settings_buttons_groups(HWND hParent)
{
	using PARAM = LAUNCHER_SETTINGS::SETTINGS_PARAM;

	try
	{
		SETTINGS_MODAL_WINDOW& settings_window = SETTINGS_MODAL_WINDOW::getInstance();

		SETTINGS_BUTTONS_GROUP& settings_on_play = settings_window.create_buttons_group(
			"onPlay.bmp",
			PARAM::ON_PLAY
		);
		settings_on_play.create_radio_button("minimize.bmp");
		settings_on_play.create_radio_button("close.bmp");

		SETTINGS_BUTTONS_GROUP& settings_clear_cache = settings_window.create_buttons_group(
			"clearCache.bmp",
			PARAM::CLEAR_CACHE
		);
		settings_clear_cache.create_radio_button("clearCacheNo.bmp");
		settings_clear_cache.create_radio_button("clearCacheYes.bmp");


		SETTINGS_BUTTONS_GROUP& settings_threads_to_unzip = settings_window.create_buttons_group(
			"threadsToUnzip.bmp",
			PARAM::THREADS_TO_UNZIP
		);
		settings_threads_to_unzip.create_radio_button("threads4.bmp");
		settings_threads_to_unzip.create_radio_button("threads8.bmp");
		settings_threads_to_unzip.create_radio_button("threads16.bmp");
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK);
	}
	
}
//====================================================================