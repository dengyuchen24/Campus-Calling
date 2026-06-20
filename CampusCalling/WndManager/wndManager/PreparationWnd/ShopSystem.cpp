#include "ShopSystem.h"

extern dyc::Logger& logger;
extern dyc::WndManager g_WndManager;
extern std::map<std::string, sf::Font> g_Fonts;

DYC_BEGIN

// SellingCard

SellingCard::SellingCard(int cost, const std::wstring& name)
	: WndCard(), mCost(cost), mName(name)
{
	mText = std::make_unique<sf::Text>(g_Fonts.at("default"), name);
}

void SellingCard::draw(sf::RenderWindow* wnd)
{
	WndCard::draw(wnd);

	static std::filesystem::path pic_path(L"Assets/Pictures/" + mName + L".png");
	static sf::Texture pic_texture(pic_path);
	static sf::Sprite pic(pic_texture);

	wnd->draw(pic);

	static sf::RectangleShape rect(sf::Vector2f(100.f, 100.f));
	static bool init = false;
	if (!init)
	{
		init = true;
		rect.setFillColor(sf::Color(50, 50, 50));
	}

	wnd->draw(*mText);
}

// ShopSystem

ShopSystem::ShopSystem() : WndObj()
{

}

void ShopSystem::draw(sf::RenderWindow* wnd)
{
	for (auto& obj : mCards)
	{
		obj->draw(wnd);
	}
}

void ShopSystem::SetOpen(bool open)
{
	mIsOpen = open;
}

void ShopSystem::Refresh()
{

}

DYC_END