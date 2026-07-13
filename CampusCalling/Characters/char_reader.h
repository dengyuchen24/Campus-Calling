#pragma once

#include <string>
#include <vector>
#include <set>

#include "../json.hpp"

struct _Skill
{
	std::string name;
	std::string description;
};

struct _Student
{
	std::string name;
	int speed;
	int maxfocus;
	int stealth;
	int defense;
	float heal;
	float behealed;
	float breakval;
	int cost;
	std::set<int> subjects;
	std::vector<_Skill> skills;
};

struct _Teacher
{
	std::string name;
	int speed;
	int maxhomework;
	int weakness;
	std::set<int> subjects;
	std::vector<_Skill> skills;
};

struct _Effect
{
	std::string name;
	std::string description;
	bool forever;
	bool unlimitedstack;
	bool limitedstack;
	bool separatestack;
	bool triggerother;
	bool needupdate;
	unsigned int triggertiming;
};

struct _Card
{
	std::string name;
	std::string description;
	int cardtype;
	int cost;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_Skill, name, description)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_Student, name, speed, maxfocus, stealth, defense, heal, behealed, breakval, cost, subjects, skills)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_Teacher, name, speed, maxhomework, weakness, subjects, skills)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_Effect, name, description, forever, unlimitedstack, limitedstack, separatestack, triggerother, needupdate, triggertiming)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_Card, name, description, cardtype, cost)