#pragma once

#include "Preparation.h"

extern dyc::WndManager g_WndManager;

DYC_BEGIN

class StartGameButton : public WndButton
{
public:
	StartGameButton();

	void update(const std::optional<sf::Event>& event) override;
};

DYC_END