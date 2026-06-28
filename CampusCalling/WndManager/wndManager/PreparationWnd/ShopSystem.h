// TODO: 完成ShopSystem
#pragma once

#include "../../../Characters/character.h"

DYC_BEGIN

class SellingCard : public WndCard
{
private:
	std::wstring mName;
	std::unique_ptr<sf::Text> mText;

public:

	float mScale = 0.1f;  // 高250px，宽200px
	int index = -1;
	int mCost = 0;
	bool last_in = false;
	bool now_in = false;
	bool sold = false;

	SellingCard(int cost, const std::wstring& name, int i = 0);

	void set(int cost, const std::wstring& name, int i);

	void draw(sf::RenderWindow* wnd) override;

	void update(const std::optional<sf::Event>& event) override;
};

class ShopSystem : public WndObj
{
private:
	std::vector<std::unique_ptr<SellingCard>> mCards;
	std::map<std::wstring, int> mCardPool;  // 牌库 {name, num}
	std::map<std::wstring, int> mCardCost;
	bool mIsOpen = true;

public:
	ShopSystem();  // 构造函数

	void draw(sf::RenderWindow* wnd) override;

	void SetOpen(bool open);  // 设置是否开启

	void Refresh();  // 刷新商店

	void update(const std::optional<sf::Event>& event) override;
};

DYC_END