#include "ShopRefreshButton.h"

extern dyc::Logger& logger;
extern dyc::WndManager g_WndManager;
extern std::map<std::string, sf::Font> g_Fonts;

DYC_BEGIN

ShopRefreshButton::ShopRefreshButton()
	: WndButton(g_Fonts.at("default"))
{

}

DYC_END