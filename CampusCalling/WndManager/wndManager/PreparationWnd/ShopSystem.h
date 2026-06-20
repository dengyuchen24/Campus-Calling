// TODO: 完成ShopSystem
#pragma once

#include "../../../Characters/character.h"

DYC_BEGIN

// 获取指定范围的随机整数
inline int randint(int a, int b) {
	if (a > b) { std::swap(a, b); }
	static std::mt19937_64 engine(std::random_device{}());
	std::uniform_int_distribution<int> distribution(a, b);
	return distribution(engine);
}

template <typename T>
inline T& RandomElement(const std::map<T, int>& _map)
{
	int total = 0;
	for (auto& dict : _map) total += dict.second;
	int index = randint(0, total - 1);

	for (auto& dict : _map)
	{
        if (index < dict.second) return dict.first;
		index -= dict.second;
	}
	throw std::runtime_error("RandomElement: index out of range");
}

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