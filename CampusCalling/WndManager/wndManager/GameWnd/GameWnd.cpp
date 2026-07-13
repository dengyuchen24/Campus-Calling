#include "GameWnd.h"

extern std::map<std::string, sf::Font> g_Fonts;

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

	}

	void GameWnd::SetChar(std::vector<WndCard*> s, std::vector<WndCard*> c, std::vector<WndCard*> t)
	{
		//students = s;
		//teachers = t;
		//hands = c;

		objects.clear();  // 清空绘制对象
		mQueue.clear();  // 清空行动队列

		float x = 0;
		for (auto stu : students)
		{
			stu->SetScale(1.0f, 1.0f, true);
			stu->SetPosition({ x, 500 });
			
			x += 450;
			mQueue.add(stu);
		}
		x = 0;
		for (auto tea : teachers)
		{
			tea->SetScale(1.0f, 1.0f, true);
			tea->SetPosition({ x, 0 });
			x += 450;
			mQueue.add(tea);
		}
		mQueue.sort();
		x = 0;
		for (auto han : hands)
		{
			han->SetScale(0.8f, 0.8f);
			han->SetPosition({ x, 1000 });
			x += 400;
		}

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
		
	}
}