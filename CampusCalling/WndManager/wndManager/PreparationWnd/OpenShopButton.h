#pragma once

#include "Preparation.h"

DYC_BEGIN

class OpenShopButton : public WndButton
{
public:
	OpenShopButton();

	void update(const std::optional<sf::Event>& event) override;

	void refresh();  // 刷新显示的文字
};

DYC_END