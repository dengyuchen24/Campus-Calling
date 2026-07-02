#include "OpenShopButton.h"
#include "ShopSystem.h"

extern dyc::Logger& logger;
extern dyc::WndManager* g_WndManager;
extern std::map<std::string, sf::Font> g_Fonts;
extern int g_Coins;

DYC_BEGIN

OpenShopButton::OpenShopButton()
	: WndButton(g_Fonts.at("default"))
{
	SetText(std::wstring(L"商店  $" + std::to_wstring(g_Coins)));
	SetButtonSize({ 260.0f, 80.0f });
	SetButtonPosition({ 1600.0f, 580.0f });
	SetTextColor(sf::Color::Black);
	auto rect = GetAs<sf::RectangleShape>();
	rect->setOutlineColor(sf::Color::Black);
	rect->setOutlineThickness(2.0f);
	rect->setFillColor(sf::Color::White);
}

void OpenShopButton::update(const std::optional<sf::Event>& event)
{
	if (!event.has_value()) return;

	mouse_in_last = mouse_in;

	if (event->is<MouseMoved>())
	{
		auto pos = GetMousePos(event);
		auto r = GetAs<sf::RectangleShape>()->getGlobalBounds();
		if (PointInRect(pos, r)) mouse_in = true;
		else mouse_in = false;

		if (mouse_in && !mouse_in_last)
		{
			GetAs<sf::RectangleShape>()->setOutlineColor(sf::Color(127, 127, 127));
		}
		else if (!mouse_in && mouse_in_last)
		{
			GetAs<sf::RectangleShape>()->setOutlineColor(sf::Color::Black);
		}
	}
	else if (event->is<MouseButtonPressed>())
	{
		auto pos = GetMousePos(event);
		if (mouse_in)
		{
			auto obj = g_WndManager->running_wnd
				->GetObjAs<ShopSystem>("ShopSystem");
			obj->SetOpen(!obj->GetOpen());
		}
	}
}

void OpenShopButton::refresh()
{
	SetText(std::wstring(L"商店  $" + std::to_wstring(g_Coins)));
}

DYC_END