#include "settings_buttons_group.h"

//====================================================================
SETTINGS_BUTTONS_GROUP* settings_on_play;
//====================================================================
SETTINGS_BUTTONS_GROUP* settings_clear_cache;
//====================================================================
void create_settings_buttons_groups(HINSTANCE hInstance, HWND hParent)
{
	using PARAM = LAUNCHER_SETTINGS::SETTINGS_PARAM;

	try
	{
		SETTINGS_BUTTONS_GROUP::initialize(hInstance, hParent);

		settings_on_play = new SETTINGS_BUTTONS_GROUP("onPlay.bmp", PARAM::ON_PLAY);
		settings_on_play->create_radio_button("minimize.bmp");
		settings_on_play->create_radio_button("close.bmp");


		settings_clear_cache = new SETTINGS_BUTTONS_GROUP("clearCache.bmp", PARAM::CLEAR_CACHE);
		settings_clear_cache->create_radio_button("clearCacheNo.bmp");
		settings_clear_cache->create_radio_button("clearCacheYes.bmp");
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK);
	}
	
}
//====================================================================