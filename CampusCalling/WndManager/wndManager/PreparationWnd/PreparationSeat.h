#pragma once

#include "ShopSystem.h"
#include "../../../Characters/character.h"

#include <map>

DYC_BEGIN

class PreparationSeat : public WndObj
{
private:
	std::vector<std::pair<int, std::wstring>> mGoFighting;  // 出战队员和卡牌{int: 1 为队员，2 为卡牌}
	int max_students = 3, max_hands = 5;
	int max_seat = 10;
	std::map<int, std::unique_ptr<WndCard>> mPreparationSeat;  // 备战席
	WndCard* holding_card = nullptr;
	sf::Vector2i mouse_last_pos;

public:
	PreparationSeat();

	void draw(sf::RenderWindow* wnd) override;

	void AddSeat(const std::wstring& name);

	int SeatNum() const;  // 返回第一个空位的索引

	bool IsSeatFull() const;

	void update(const std::optional<sf::Event>& event) override;
};

DYC_END