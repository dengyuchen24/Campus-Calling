#include "Preparation.h"

#include "StartGameButton.h"
#include "ShopSystem.h"
#include "OpenShopButton.h"
#include "ShopRefreshButton.h"

DYC_BEGIN

PreparationWnd::PreparationWnd(WndManager* mgr)
	: sfWindow(mgr)
{
	LOG_COUT("[INFO] PreparationWnd 初始化开始！");

	// 开始游戏按钮
	AddObj(std::make_unique<StartGameButton>(), "StartGameButton");
	LOG_COUT("[PASS] 开始游戏按钮初始化完毕！");

	// TODO: 备战席等元素

	// 商店
	AddObj(std::make_unique<ShopSystem>(), "ShopSystem");
	LOG_COUT("[PASS] 商店初始化完毕！");

    // 添加商店刷新按钮
    AddObj(std::make_unique<ShopRefreshButton>(), "ShopRefreshButton");
	LOG_COUT("[PASS] 商店刷新按钮初始化完毕！");

	// 商店开关按钮
	AddObj(std::make_unique<OpenShopButton>(), "OpenShopButton");
	LOG_COUT("[PASS] 商店开关按钮初始化完毕！");

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

	// 商店刷新按钮
	auto shoprefreshbutton = GetObjAs<ShopRefreshButton>("ShopRefreshButton");
	if (shoprefreshbutton->mouse_in && !shoprefreshbutton->mouse_in_last)
		shoprefreshbutton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color(240, 240, 240));
	else if (!shoprefreshbutton->mouse_in && shoprefreshbutton->mouse_in_last)
		shoprefreshbutton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color::White);
}

DYC_END