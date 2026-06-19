#include "Preparation.h"

#include "StartGameButton.h"

DYC_BEGIN

PreparationWnd::PreparationWnd(WndManager* mgr)
	: sfWindow(mgr)
{
	LOG_COUT("PreparationWnd 初始化开始！");

	// 开始游戏按钮
	AddObj(std::make_unique<StartGameButton>(), "StartGameButton");
	LOG_COUT("按钮图形已创建");
	LOG_COUT("开始游戏按钮初始化完毕！");

	LOG_COUT("PreparationWnd 初始化完毕！");
}

void PreparationWnd::logic()
{
	// 出战按钮
	auto startgamebutton = GetObjAs<StartGameButton>("StartGameButton");
	if (startgamebutton->mouse_in && !startgamebutton->mouse_in_last)
		startgamebutton->GetAs<sf::RectangleShape>()->setOutlineColor(sf::Color(127, 127, 127));
	else if (!startgamebutton->mouse_in && startgamebutton->mouse_in_last)
		startgamebutton->GetAs<sf::RectangleShape>()->setOutlineColor(sf::Color::Black);
}

DYC_END