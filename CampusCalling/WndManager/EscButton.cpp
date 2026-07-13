#include "EscButton.h"

extern std::map<std::string, sf::Font> g_Fonts;
extern dyc::WndManager* g_WndManager;

DYC_BEGIN

EscButton::EscButton()
	: WndButton(g_Fonts.at("default"))
{
	
	mText->setString(L"退出游戏 ×");
	mText->setCharacterSize(20U);
	
	SetButtonSize({ 100.0f, 50.0f });
	SetButtonPosition({ 1800.0f, 50.0 });
	
	auto rect = GetAs<sf::RectangleShape>();
	if (!rect)
	{
		
		return;
	}
	rect->setFillColor(sf::Color::Transparent);
	rect->setOutlineColor(sf::Color::Transparent);
	SetTextColor(sf::Color::Black);
	SetUiOrder(-100);
}
void EscButton::update(const std::optional<sf::Event>& event)
{
	if (event->is<sf::Event::MouseButtonPressed>())
	{
		if (!mouse_in) return;
		g_WndManager->SetRunning(nullptr);
        
	}
	else if (event->is<sf::Event::MouseMoved>())
	{
		static bool last_mouse_in = false;
		mouse_in_last = last_mouse_in;
		sf::Vector2i mouse_pos = event->getIf<sf::Event::MouseMoved>()->position;
		auto rect = GetAs<sf::RectangleShape>();
		sf::FloatRect r = rect->getGlobalBounds();
		if (PointInRect(mouse_pos, r))
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
}

DYC_END