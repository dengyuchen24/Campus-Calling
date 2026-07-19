#include "ShopSystem.h"
#include "OpenShopButton.h"
#include "PreparationSeat.h"
#include "../../../Helper.h"

extern dyc::Logger& logger;
extern dyc::WndManager* g_WndManager;
extern std::map<std::string, sf::Font> g_Fonts;
extern int g_Coins;
extern dyc::Message* g_Message;
extern std::map<std::string, _Card> g_Cards;
extern std::map<std::string, _Student> g_Students;

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

			g_WndManager->running_wnd
				->GetObjAs<ShopSystem>("ShopSystem")
				->BuyCard(mName);

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
	logger.log_info("ShopSystem开始初始化...");
	SetDrawable(std::make_unique<sf::RectangleShape>(sf::Vector2f(1220.0f, 300.0f)));
	auto r = GetAs<sf::RectangleShape>();
	if (!r)
	{
		return;
	}
	r->setPosition(sf::Vector2f(350.0f, 100.0f));
	r->setFillColor(sf::Color::White);
	r->setOutlineThickness(5.0f);
	r->setOutlineColor(sf::Color::Black);
	SetUiOrder(-3);

	for (auto& [n, v] : g_Students)
	{
		mCardPool[dto_wstring(n)] = 10;
		mCardCost[dto_wstring(n)] = v.cost;
	}
	for (auto& [n, v] : g_Cards)
	{
		mCardPool[dto_wstring(n)] = 5;
		mCardCost[dto_wstring(n)] = v.cost;
	}

	logger.log_info("mCardPool、mCardCost初始化完毕");

	mCards.clear();
	mCards.resize(5);
	logger.log_info("mCards清空成功");
	
	for (int i = 0; i < 5; ++i)
	{
		std::wstring name = RandomElement(mCardPool);
		mCards[i] = std::make_unique<SellingCard>(mCardCost[name], name, i);
		mCards[i]->sold = false;
	}
	
	logger.log_info("mCards初始化完毕");
	logger.log_info("商店系统成功创建");
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
		
		return;
	}
	g_Coins -= 2;
	g_WndManager->running_wnd
		->GetObjAs<OpenShopButton>("OpenShopButton")->refresh();
	if (mCardPool.empty())
	{
		
		return;
	}
	for (int i = 0; i < 5; ++i)
	{
		// TODO: 把随机取改为按权重
		std::wstring name = RandomElement(mCardPool);

		mCards[i]->set(mCardCost[name], name, i);
		mCards[i]->sold = false;
	}
	
}

void ShopSystem::update(const std::optional<sf::Event>& event)
{
	for (auto& c : mCards)
	{
		c->update(event);
	}
}

void ShopSystem::BuyCard(const std::wstring& name)
{
	bool last = true;
	for (auto& [n, v] : mCardPool)
	{
		if (v != 0) { last = false; break; }
	}

	if (!last) mCardPool[name]--;
}

void ShopSystem::SetCardPool(const std::wstring& name, int num)
{
	mCardPool.at(name) += num;
}

DYC_END