#pragma once
#include "../../../Characters/character.h"
#include "../../sfWindow.h"

DYC_BEGIN

class Character;

class ActionQueue : public WndObj
{
private:
	std::vector<Character*> queue;

public:
	ActionQueue();
	Character* top();
	void add(Character* c);  // 需要手动sort
	void sort();
	bool empty();
	void clear();
	const std::vector<Character*>& GetQueue() const;

	void draw(sf::RenderWindow* wnd) override;
};

DYC_END