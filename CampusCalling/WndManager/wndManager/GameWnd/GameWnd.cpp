#include "GameWnd.h"
#include "../../../Helper.h"
#include "../PreparationWnd/PreparationSeat.h"
#include "../../../Characters/character.h"

extern std::map<std::string, sf::Font> g_Fonts;
extern std::map<std::string, _Student> g_Students;
extern std::map<std::string, _Teacher> g_Teachers;
extern dyc::Logger& logger;

namespace dyc
{

	Character* ActionQueue::top()
	{
		if (queue.empty())
		{
			
			return nullptr;
		}
		return queue[0];
	}

	void ActionQueue::add(Character* c) { queue.push_back(c); }

	void ActionQueue::sort()
	{
		std::sort(queue.begin(), queue.end(),
			[](Character* a, Character* b)->bool
			{ return a->GetMoveVal() < b->GetMoveVal(); });
	}

	bool ActionQueue::empty() { return queue.empty(); }

	const std::vector<Character*>& ActionQueue::GetQueue() const { return queue; }

	void ActionQueue::clear() { queue.clear(); }

	void GameWnd::logic()
	{
		// 更新行动队列
		
		const std::vector<Character*>& queue = mQueue.GetQueue();
		static std::wstring content;
		content.clear();
		int n = 0;
		for (auto c : queue)
		{
			content += (L"[" + std::to_wstring(c->GetMoveVal()) + L"] " + c->GetName() + L"\n");
			++n;
			if (n >= 10) break;
		}
		auto* obj = GetObj("ActionQueue");
		auto* aq = obj->GetAs<sf::Text>();
		aq->setString(content);
	}

	GameWnd::GameWnd(WndManager* m) : sfWindow(m)
	{
		logger.log_info("GameWnd开始创建...");
		// 行动队列
		// 新建一个 WndObj 实例
		std::unique_ptr<WndObj> obj = std::make_unique<WndObj>();
		obj->SetUiOrder(-1);

		std::unique_ptr<sf::Text> aq = std::make_unique<sf::Text>(g_Fonts.at("default"));
		if (!aq)

			// 现在obj有效，可以正常调用
			obj->SetDrawable(std::move(aq));

		auto text = obj->GetAs<sf::Text>();
		if (!text)
			text->setPosition({ 1500.0f, 0.0f });
		text->setFillColor(sf::Color::Black);
		text->setCharacterSize(30U);

		AddObj(std::move(obj), "ActionQueue");

		logger.log_info("GameWnd创建完毕！");
	}

	std::unique_ptr<Student> GameWnd::NewStudent(std::string name)
	{
		if (!g_Students.contains(name))
			return nullptr;
		return std::make_unique<Student>(name);
	}

	void GameWnd::Reset
	(const std::map<int, std::unique_ptr<Seat>>& seats
		, std::vector<std::string> teachers
	)
	{
		this->students.clear();
		this->teachers.clear();
		this->hands.clear();
		mQueue.clear();
		
		for (auto& [pos, val] : seats)
		{
			if (pos >= 0 && pos < 3)
			{
				this->students.push_back(NewStudent(dto_string(val->cardname)));
				mQueue.add(this->students.back().get());
			}
			else if (pos >= 3 && pos < 10)
			{
				this->hands.push_back(std::make_unique<Card>(dto_string(val->cardname)));
			}
			else break;
		}

		for (auto& n : teachers)
		{
			this->teachers.push_back(std::make_unique<Teacher>(n));
			mQueue.add(this->teachers.back().get());
		}
		mQueue.sort();
	}
}