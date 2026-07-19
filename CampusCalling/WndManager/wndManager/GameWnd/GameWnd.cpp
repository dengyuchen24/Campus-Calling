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

	void GameWnd::logic()
	{
		// 更新行动队列
		
		const std::vector<Character*>& queue = mQueue->GetQueue();
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

		NEWOBJ(ActionQueue);

		mQueue = GETOBJAS(ActionQueue);

		logger.log_info("GameWnd创建完毕！");
	}

	void GameWnd::Reset
	(std::map<int, _Character>& seats
		, std::vector<_Character> teachers
	)
	{
		logger.log_info("GameWnd::Reset() 被调用");

		if (seats.empty()) {
			logger.log_error("GameWnd::Reset() 收到的 seats 指针为空，放弃重置！");
			return;
		}
		this->students.clear();
		this->teachers.clear();
		this->hands.clear();
		if (mQueue) mQueue->clear();
		logger.log_info("GameWnd::Reset() 清空了学生、老师、手牌和行动队列");
		logger.log_info("GameWnd::Reset() seat.size() : " + std::to_string(seats.size()));
		for (auto& [pos, val] : seats)
		{
			logger.log_info("pos: " + std::to_string(pos) + ", cardname: " + val.name);
			if (pos >= 0 && pos < 3)
			{
				this->students.push_back(std::make_unique<Student>(val));
				if (!this->students.back())
				{
					logger.log_error("GameWnd::Reset() NewStudent失败，cardname: " + val.name);
					continue;
				}
				mQueue->add(this->students.back().get());
			}
			else if (pos >= 3 && pos < 10)
			{
				this->hands.push_back(std::make_unique<Card>(val.name));
			}
			else break;
		}

		logger.log_info("GameWnd::Reset() 添加了学生和手牌到GameWnd");

		for (auto& n : teachers)
		{
			this->teachers.push_back(std::make_unique<Teacher>(n));
			mQueue->add(this->teachers.back().get());
		}

		logger.log_info("GameWnd::Reset() 添加了老师到GameWnd");
		mQueue->sort();
		logger.log_info("GameWnd::Reset() 行动队列已排序，已全部完成！");
	}
}