#include "cards.h"

extern std::map<std::string, _Card> g_Cards;

DYC_BEGIN

Card::Card(const std::string& name)
{
	card = &g_Cards.at(name);
}

DYC_END