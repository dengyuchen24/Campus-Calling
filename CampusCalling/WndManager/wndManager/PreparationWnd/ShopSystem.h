// TODO: 完成ShopSystem
#pragma once

#include "../../../Characters/character.h"

DYC_BEGIN

class SellingCard : public WndCard
{
private:
	int mCost = 0;
	std::wstring mName;
	std::unique_ptr<sf::Text> mText;

public:
	SellingCard(int cost, const std::wstring& name);

	void draw(sf::RenderWindow* wnd) override;
};

class ShopSystem : public WndObj
{
private:
	std::vector<std::unique_ptr<SellingCard>> mCards;
	std::map<std::wstring, int> mCardPool;  // 牌库 {name, num}
	bool mIsOpen = true;

public:
	ShopSystem();  // 构造函数

	void draw(sf::RenderWindow* wnd) override;

	void SetOpen(bool open);  // 设置是否开启

	void Refresh();  // 刷新商店
};

DYC_END