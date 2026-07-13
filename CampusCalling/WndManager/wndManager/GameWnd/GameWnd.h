#pragma once

#include "../../../Characters/character.h"

namespace dyc
{
	class GameWnd;
	class Student;
	class Teacher;
	class Card;
	class Character;

	class ActionQueue
	{
	private:
		std::vector<Character*> queue;

	public:
		Character* top();
		void add(Character* c);  // 需要手动sort
		void sort();
		bool empty();
		void clear();
		const std::vector<Character*>& GetQueue() const;
	};

	class GameWnd : public sfWindow
	{
	private:
		std::vector<Student*> students;
		std::vector<Teacher*> teachers;
		std::vector<Card*> hands;  // 手牌
		ActionQueue mQueue;

		void logic() override;

	public:
		GameWnd(WndManager* m);
		void SetChar(std::vector<WndCard*> s, std::vector<WndCard*> c, std::vector<WndCard*> t);
	};
}