#include "ShopSystem.h"
#include "../../../Helper.h"

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
	SetDrawable(std::make_unique<sf::RectangleShape>(sf::Vector2f(1220.0f, 300.0f)));
	auto r = GetAs<sf::RectangleShape>();
	if (!r)
	{
		LOG_COUT("[ERROR] ShopSystem: Failed to get drawable!");
		return;
	}
	r->setPosition(sf::Vector2f(350.0f, 100.0f));
	r->setFillColor(sf::Color::Transparent);
	r->setOutlineThickness(5.0f);
	r->setOutlineColor(sf::Color::Black);
	LOG_COUT("[PASS] ShopSystem: Created!");

}

void ShopSystem::draw(sf::RenderWindow* wnd)
{
	if (!mIsOpen) return;
    WndObj::draw(wnd);
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
	for (int i = 0; i < 5; ++i)
	{
		// TODO: 把随机取改为按权重
		std::wstring name = RandomElement(mCardPool);

		mCards[i]->SetTexture(Path(L"Assets/Pictures/" + name + L".png"));
		mCards[i]->SetPosition(sf::Vector2f(i * 100.0f, 0.0f));
		LOG_COUT("[DEBUG] Card " << (1 + i) << ": " << WStrToStr(name));
	}
}

DYC_END