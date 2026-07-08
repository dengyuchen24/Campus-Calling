#pragma once
#include "Preparation.h"

DYC_BEGIN

class UpgradeLevel : public WndButton
{
private:
	int cur_level = 5;
	int max_level = 10;
public:
	int cur_experience = 0;
	int need_experience = 10;
	UpgradeLevel();
	void SetLevel(int cur_ = 5, int max_ = 10);
	void update(const std::optional<sf::Event>& event) override;
};

DYC_END