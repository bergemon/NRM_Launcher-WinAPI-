#include "get_file_path.h"

//====================================================================
void get_button_bkg_path(std::string& str)
{
	str.clear();
	str = BUTTONS_BKG_FULL_PATH BTN_PREFIX;
}
//====================================================================
void get_button_active_bkg_path(std::string& str)
{
	str.clear();
	str = BUTTONS_BKG_FULL_PATH BTN_PREFIX ACTIVE_PREFIX;
}
//====================================================================
void cut_filename_from_btn_path(std::string& str)
{
	str = str.substr(str.rfind("_") + 1, str.length() - str.rfind("_") - 1);
}
//====================================================================
void get_button_mask_path(std::string& str)
{
	str.clear();
	str = MASKS_FULL_PATH MASK_PREFIX BTN_PREFIX;
}
//====================================================================
void get_main_bkg_path(std::string& str)
{
	str.clear();
	str = APP_DIR SLH;
}
//====================================================================
void get_submods_bkg_path(std::string& str)
{
	str.clear();
	str = APP_DIR SLH SUBMODS_WINDOW_PATH SLH;
}
//====================================================================
void get_submods_btns_bkg_path(std::string& str)
{
	str.clear();
	str = SUBMODS_BUTTONS_FULL_PATH SLH BTN_PREFIX;
}
//====================================================================
void get_submods_active_btns_bkg_path(std::string& str)
{
	str.clear();
	str = SUBMODS_BUTTONS_FULL_PATH SLH BTN_PREFIX ACTIVE_PREFIX;
}
//====================================================================
void get_submods_btns_mask_path(std::string& str)
{
	str.clear();
	str = SUBMODS_BUTTONS_FULL_PATH SLH SUBMODS_MASKS_PATH SLH MASK_PREFIX BTN_PREFIX;
}