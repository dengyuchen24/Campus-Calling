#pragma once

#include "Preparation.h"

DYC_BEGIN

class ShopRefreshButton : public WndButton
{
public:
    ShopRefreshButton();

    void update(const std::optional<sf::Event>& event) override;

    void draw(sf::RenderWindow* wnd) override;
};

DYC_END