#include "StartGameButton.h"

extern std::map<std::string, sf::Font> g_Fonts;

DYC_BEGIN

StartGameButton::StartGameButton()
	: WndButton(g_Fonts.at("default"))
{
	LOG_COUT("StartGameButton开始创建！");
	mText->setString(L"出战 →");
	LOG_COUT("设置文字内容为出战");
	SetButtonSize({ 200.0f, 100.0f });
	SetButtonPosition({ 50.0f, 50.0f });
	LOG_COUT("设置位置成功");
	auto rect = GetAs<sf::RectangleShape>();
	if (!rect)
	{
		LOG_COUT("创建失败");
		return;
	}
	rect->setFillColor(sf::Color::White);
	rect->setOutlineColor(sf::Color::Black);
	rect->setOutlineThickness(3.0f);
	SetTextColor(sf::Color::Black);
	SetUiOrder(-1);
}
void StartGameButton::update(const std::optional<sf::Event>& event)
{
	if (event->is<sf::Event::MouseButtonPressed>())
	{
		sf::Vector2i mouse_pos = event->getIf<sf::Event::MouseButtonPressed>()->position;
		auto rect = GetAs<sf::RectangleShape>();
		sf::FloatRect r = rect->getGlobalBounds();
		if (PointInRect(mouse_pos, r))
		{
			if (!g_WndManager.GetSfWnd()->contains("game window"))
				g_WndManager.AddSfWnd("game window", std::make_unique<dyc::GameWnd>(&g_WndManager));
			g_WndManager.SetRunning(g_WndManager.GetSfWnd()->at("game window").get());
		}
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