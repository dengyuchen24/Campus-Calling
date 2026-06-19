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
		std::vector<Character*>& GetQueue() { return queue; }
	};

	class GameWnd : public sfWindow
	{
	private:
		std::vector<Student*> students;
		std::vector<Teacher*> teachers;
		std::vector<std::unique_ptr<Card>> hands;
		ActionQueue mQueue;

		void logic() override;

	public:
		GameWnd(WndManager* m);
	};
}