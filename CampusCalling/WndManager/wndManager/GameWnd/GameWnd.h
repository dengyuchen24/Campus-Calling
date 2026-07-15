#pragma once

#include "../../../Characters/character.h"

namespace dyc
{
	class GameWnd;
	class Student;
	class Teacher;
	class Card;
	class Character;
	class Seat;

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
		std::vector<std::unique_ptr<Student>> students;
		std::vector<std::unique_ptr<Teacher>> teachers;
		std::vector<std::unique_ptr<Card>> hands;  // 手牌
		ActionQueue mQueue;

		void logic() override;

		std::unique_ptr<Student> NewStudent(std::string name);

	public:
		GameWnd(WndManager* m);
		void Reset(const std::map<int, std::unique_ptr<Seat>>& seats, std::vector<std::string> teachers);
	};
}