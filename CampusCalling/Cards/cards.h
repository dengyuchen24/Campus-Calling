#pragma once

#include <string>
#include "../WndManager/sfWindow.h"
#include "../Characters/char_reader.h"

class Character;

namespace dyc
{
	enum class CardType : int
	{
		EQUIPMENT = 1,
		EVENT = 2
	};

	class Card : public WndCard
	{
	public:
		Card(const std::string& name);
		_Card* card = nullptr;
		virtual void UseCard(Character* user, Character* target) {}
	};
}