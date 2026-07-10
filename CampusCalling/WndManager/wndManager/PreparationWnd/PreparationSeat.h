#pragma once

#include "ShopSystem.h"
#include "../../../Characters/character.h"

#include <map>

DYC_BEGIN

using INDEX = unsigned __int64;

enum class SeatType : int
{
	NULLSEATTYPE = 0,
	CHARACTER = 1,
	CARD = 2
};

class Seat : public WndCard
{
public:
	Seat();

	SeatType mSeatType = SeatType::NULLSEATTYPE;  // 牌的类型
	int mcost = 0;
	std::wstring cardname;
	CardType GetInfo() const;  // 获取卡牌信息（如果是卡牌的话，返回CardType）
	std::wstring equipment_name;  // 如果是角色，储存装备信息
};

SeatType GetSeatType(const std::wstring& str);

class PreparationSeat : public WndObj
{
private:
	// idx 0~2 出战角色 3~9 出战卡牌 10~19 备战席
	std::map<int, std::unique_ptr<Seat>> mPreparationSeat;
	WndCard* holding_card = nullptr;
	sf::Vector2i mouse_last_pos;
	std::vector<sf::Vector2f> mBlocks;
	int card_last_idx = -1;
	bool will_be_sold = false;
public:
	int CountCards(int a, int b);  // mPreparationSeat的索引在 [a, b) 之间的个数

	int max_front = 5;
	PreparationSeat();

	~PreparationSeat();

	void draw(sf::RenderWindow* wnd) override;

	void AddSeat(const std::wstring& name, SeatType type = SeatType::NULLSEATTYPE, int cost = 0);

	int SeatNum() const;  // 返回第一个空位的索引

	bool IsSeatFull() const;

	void update(const std::optional<sf::Event>& event) override;
};

DYC_END