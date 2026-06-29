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
	LOG_COUT("[INFO] ShopRefreshButton开始创建！");
	mText->setString(L"$2→刷新");
	LOG_COUT("[PASS] 设置文字内容完成");
	mText->setCharacterSize(25U);
	SetButtonSize({ 200.0f, 55.0f });
	SetButtonPosition({ 1350.0f, 450.0 });
	LOG_COUT("[PASS] 设置位置成功");
	auto rect = GetAs<sf::RectangleShape>();
	if (!rect)
	{
		LOG_COUT("[ERROR] 创建失败");
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