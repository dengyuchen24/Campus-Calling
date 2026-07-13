#include "ShopRefreshButton.h"
#include "../../../Helper.h"
#include "ShopSystem.h"

extern dyc::Logger& logger;
extern dyc::WndManager* g_WndManager;
extern std::map<std::string, sf::Font> g_Fonts;

DYC_BEGIN

ShopRefreshButton::ShopRefreshButton()
	: WndButton(g_Fonts.at("default"))
{
	
	mText->setString(L"$2→刷新");
	
	mText->setCharacterSize(25U);
	SetButtonSize({ 200.0f, 55.0f });
	SetButtonPosition({ 1350.0f, 450.0 });
	
	auto rect = GetAs<sf::RectangleShape>();
	if (!rect)
	{
		
		return;
	}
	rect->setFillColor(sf::Color::White);
	rect->setOutlineColor(sf::Color::Black);
	rect->setOutlineThickness(3.0f);
	SetTextColor(sf::Color::Black);
	SetUiOrder(-1);
}

void ShopRefreshButton::update(const std::optional<sf::Event>& event)
{
	if (!g_WndManager->running_wnd
		->GetObjAs<ShopSystem>("ShopSystem")->GetOpen()) return;
	static bool last_mouse_in = false;
	mouse_in_last = last_mouse_in;
	auto p = GetMousePos(event);
	if (p == nullvec2i) return;

	auto bounds = GetAs<sf::RectangleShape>()->getGlobalBounds();

	if (event->is<sf::Event::MouseButtonPressed>())
	{
		if (PointInRect(p, bounds))
		{
			g_WndManager->running_wnd
				->GetObjAs<ShopSystem>("ShopSystem")->Refresh();
		}
	}

	if (PointInRect(p, bounds))
	{
		mouse_in = true;
		last_mouse_in = true;
	}
	else
	{
		mouse_in = false;
		last_mouse_in = false;
	}
}

void ShopRefreshButton::draw(sf::RenderWindow* wnd)
{
	if (!g_WndManager->running_wnd
		->GetObjAs<ShopSystem>("ShopSystem")->GetOpen()) return;
	WndButton::draw(wnd);
}

DYC_END