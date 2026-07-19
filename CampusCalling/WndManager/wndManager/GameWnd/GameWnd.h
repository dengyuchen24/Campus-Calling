#pragma once

#include "../../../Characters/character.h"
#include "ActionQueue.h"

namespace dyc
{
	class GameWnd;
	class Student;
	class Teacher;
	class Card;
	class Character;
	class Seat;
	class ActionQueue;
	struct _Character;

	class GameWnd : public sfWindow
	{
	private:
		std::vector<std::unique_ptr<Student>> students;
		std::vector<std::unique_ptr<Teacher>> teachers;
		std::vector<std::unique_ptr<Card>> hands;  // 手牌
		ActionQueue* mQueue = nullptr;

		void logic() override;

	public:
		GameWnd(WndManager* m);
		void Reset(std::map<int, _Character>& seats, std::vector<_Character> teachers);
	};
}