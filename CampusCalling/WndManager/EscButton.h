#pragma once

#include "sfWindow.h"

DYC_BEGIN

class EscButton : public WndButton
{
public:
	EscButton();

	void update(const std::optional<sf::Event>& event) override;
};


DYC_END