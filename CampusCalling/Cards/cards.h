#pragma once

#include <string>
#include "../WndManager/sfWindow.h"

class Character;

namespace dyc
{
	enum class CardType : int
	{
		LISTENING = 1,
		HOMEWORK,
		EQUIPMENT,
		EVENT
	};

	class Card : public WndObj
	{
	public:
		std::string mName;
		CardType mCardType;
		virtual std::string Description() = 0;
		virtual void UseCard(Character* user, Character* target) = 0;
	};
}