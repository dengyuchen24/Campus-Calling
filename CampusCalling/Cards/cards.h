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
		_Card* card;
		virtual void UseCard(Character* user, Character* target) = 0;
	};
}