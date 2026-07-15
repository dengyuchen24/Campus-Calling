#include "Preparation.h"

#include "StartGameButton.h"
#include "ShopSystem.h"
#include "OpenShopButton.h"
#include "ShopRefreshButton.h"
#include "PreparationSeat.h"
#include "UpgradeLevel.h"

extern dyc::Logger& logger;

DYC_BEGIN

PreparationWnd::PreparationWnd(WndManager* mgr)
	: sfWindow(mgr)
{
	

	// 开始游戏按钮
	NEWOBJ(StartGameButton);
	logger.log_info("开始按钮创建成功");

	// 商店
	NEWOBJ(ShopSystem);
	logger.log_info("商店系统创建成功");

    // 添加商店刷新按钮
	NEWOBJ(ShopRefreshButton);
	

	// 商店开关按钮
	NEWOBJ(OpenShopButton);
	

	// 备战席
	NEWOBJ(PreparationSeat);
	

	// 升级按钮
	NEWOBJ(UpgradeLevel);
	

	
}

void PreparationWnd::logic()
{
	// 出战按钮
	GETOBJAS_var(StartGameButton);
	if (var_StartGameButton->mouse_in && !var_StartGameButton->mouse_in_last)
		var_StartGameButton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color(255, 60, 0));
	else if (!var_StartGameButton->mouse_in && var_StartGameButton->mouse_in_last)
		var_StartGameButton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color::Cyan);

	// 商店刷新按钮
	GETOBJAS_var(ShopRefreshButton);
	if (var_ShopRefreshButton->mouse_in && !var_ShopRefreshButton->mouse_in_last)
		var_ShopRefreshButton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color(240, 240, 240));
	else if (!var_ShopRefreshButton->mouse_in && var_ShopRefreshButton->mouse_in_last)
		var_ShopRefreshButton->GetAs<sf::RectangleShape>()
		->setFillColor(sf::Color::White);

	// 等级升级按钮
	GETOBJAS_var(UpgradeLevel);
	if (var_UpgradeLevel->mouse_in && !var_UpgradeLevel->mouse_in_last)
		var_UpgradeLevel->GetAs<sf::RectangleShape>()
		->setOutlineColor(sf::Color::Black);
	else if (!var_UpgradeLevel->mouse_in && var_UpgradeLevel->mouse_in_last)
		var_UpgradeLevel->GetAs<sf::RectangleShape>()
		->setOutlineColor(sf::Color::White);
}

DYC_END