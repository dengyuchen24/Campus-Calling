#include "Preparation.h"

#include "StartGameButton.h"
#include "ShopSystem.h"

DYC_BEGIN

PreparationWnd::PreparationWnd(WndManager* mgr)
	: sfWindow(mgr)
{
	LOG_COUT("[INFO] PreparationWnd 初始化开始！");

	// 开始游戏按钮
	AddObj(std::make_unique<StartGameButton>(), "StartGameButton");
	LOG_COUT("[PASS] 开始游戏按钮初始化完毕！");

	// TODO: 添加商店（购买卡牌）、备战席等元素

	// 商店
	AddObj(std::make_unique<ShopSystem>(), "ShopSystem");
	LOG_COUT("[PASS] 商店初始化完毕！");

	LOG_COUT("[PASS] PreparationWnd 初始化完毕！");
}

void PreparationWnd::logic()
{
	// 出战按钮
	auto startgamebutton = GetObjAs<StartGameButton>("StartGameButton");
	if (startgamebutton->mouse_in && !startgamebutton->mouse_in_last)
		startgamebutton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color(255, 60, 0));
	else if (!startgamebutton->mouse_in && startgamebutton->mouse_in_last)
		startgamebutton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color::Cyan);
}

DYC_END