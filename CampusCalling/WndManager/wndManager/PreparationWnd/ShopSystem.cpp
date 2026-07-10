#include "ShopSystem.h"
#include "OpenShopButton.h"
#include "PreparationSeat.h"
#include "../../../Helper.h"

extern dyc::Logger& logger;
extern dyc::WndManager* g_WndManager;
extern std::map<std::string, sf::Font> g_Fonts;
extern int g_Coins;
extern dyc::Message* g_Message;

DYC_BEGIN

// SellingCard

SellingCard::SellingCard(int cost, const std::wstring& name, int i)
	: WndCard(), index(i), mCost(cost)
{
	set(cost, name, i);
}

void SellingCard::set(int cost, const std::wstring& name, int i)
{
	mName = name;
	mText.reset(nullptr);
	mText = std::make_unique<sf::Text>(g_Fonts.at("default"), std::wstring(L"$" + std::to_wstring(cost) + L" " + name));
	mText->setFillColor(sf::Color::White);
	mText->setOutlineColor(sf::Color::Black);
	mText->setOutlineThickness(1.0f);
	mText->setPosition(sf::Vector2f(i * 236.67f + 386.67f, 125.0f));
	mText->setCharacterSize(20U);
	SetTexture(L"Assets/Pictures/" + name + L".png");
	SetDrawable(std::make_unique<sf::Sprite>(texture));
	mScale = 0.5f;
	SetScale(mScale, mScale);
	SetPosition(sf::Vector2f(i * 236.67f + 386.67f, 125.0f));
	SetBorder(2.0f, sf::Color::Black);
}

void SellingCard::draw(sf::RenderWindow* wnd)
{
	WndCard::draw(wnd);
	wnd->draw(*mText);
}

void SellingCard::update(const std::optional<sf::Event>& event)
{
	if (!g_WndManager->running_wnd
		->GetObjAs<ShopSystem>("ShopSystem")->GetOpen()) return;
	if (!event.has_value()) return;

	last_in = now_in;

	if (event->is<sf::Event::MouseMoved>())
	{
		sf::Vector2i pos = GetMousePos(event);
		auto rect = border.getGlobalBounds();
		if (PointInRect(pos, rect))
		{
			now_in = true;
		}
		else
		{
			now_in = false;
		}
		if (now_in && !last_in && !sold)
		{
			border.setOutlineColor(sf::Color::Transparent);
		}
		else if (!now_in && last_in)
		{
			border.setOutlineColor(sf::Color::Black);
		}
	}
	else if (event->is<sf::Event::MouseButtonPressed>())
	{
		sf::Vector2i pos = GetMousePos(event);
		if (!sold && now_in)
		{
			if (g_Coins < mCost)
			{
				g_Message->NewMsg(L"金币不足");
				return;
			}
			else if (g_WndManager->running_wnd
				->GetObjAs<PreparationSeat>("PreparationSeat")
				->IsSeatFull())
			{
				g_Message->NewMsg(L"备战席已满");
				return;
			}


			// TODO: 添加卡池减少一张牌，保证mCardPool中至少有一张牌
			g_WndManager->running_wnd
				->GetObjAs<ShopSystem>("ShopSystem")
				->BuyCard(mName);

			// TODO: 把购买的卡牌转移到玩家拥有
			g_WndManager->running_wnd
				->GetObjAs<PreparationSeat>("PreparationSeat")
				->AddSeat(mName, GetSeatType(mName), mCost);

			set(0, L"sold", index);
			mText->setString(L"Has been sold~");
			mText->setOutlineColor(sf::Color::Transparent);
			mText->setFillColor(sf::Color::Black);
			sold = true;
			g_Coins -= mCost;
			g_WndManager->running_wnd
				->GetObjAs<OpenShopButton>("OpenShopButton")->refresh();
		}
	}
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
	r->setFillColor(sf::Color::White);
	r->setOutlineThickness(5.0f);
	r->setOutlineColor(sf::Color::Black);
	SetUiOrder(-3);

	mCardPool[L"丝柯克"] = 100;  // TODO: mCardPool初始化

	for (int i = 0; i < 5; ++i)
	{
		// TODO: 完成初始化商店中的卡牌
		mCardCost[L"丝柯克"] = 5;
		mCards.push_back(std::make_unique<SellingCard>(5, L"丝柯克", i));
	}

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
	if (g_Coins < 2)
	{
		g_Message->NewMsg(L"金币不足");
		LOG_COUT("[MESSAGE] 金币不足");
		return;
	}
	g_Coins -= 2;
	g_WndManager->running_wnd
		->GetObjAs<OpenShopButton>("OpenShopButton")->refresh();
	if (mCardPool.empty())
	{
		LOG_COUT("[WARNING] mCardPool is empty!");
		return;
	}
	for (int i = 0; i < 5; ++i)
	{
		// TODO: 把随机取改为按权重
		std::wstring name = RandomElement(mCardPool);

		mCards[i]->set(mCardCost[name], name, i);
		mCards[i]->sold = false;
	}
	LOG_COUT("[INFO] Shop refreshed!");
}

void ShopSystem::update(const std::optional<sf::Event>& event)
{
	for (auto& c : mCards)
	{
		c->update(event);
	}
}

DYC_END