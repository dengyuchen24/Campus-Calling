#include "GameWnd.h"

extern std::map<std::string, sf::Font> g_Fonts;

namespace dyc
{
	Character* ActionQueue::top()
	{
		if (queue.empty())
		{
			LOG_COUT("[ERROR] Action queue is empty!");
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

	void GameWnd::logic()
	{
		// 更新行动队列
		
		std::vector<Character*>& queue = mQueue.GetQueue();
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
		LOG_COUT("[INFO] GameWnd begin to init");

		// 行动队列
		// 新建一个 WndObj 实例
		std::unique_ptr<WndObj> obj = std::make_unique<WndObj>();
		obj->SetUiOrder(-1);
		LOG_COUT("[PASS] obj successful to create");

		std::unique_ptr<sf::Text> aq = std::make_unique<sf::Text>(g_Fonts.at("default"));
		if (!aq) LOG_COUT("[ERROR] aq is nullptr");

		// 现在obj有效，可以正常调用
		obj->SetDrawable(std::move(aq));

		auto text = obj->GetAs<sf::Text>();
		if (!text) LOG_COUT("[ERROR] text is nullptr");
		text->setPosition({ 1500.0f, 0.0f });
		text->setFillColor(sf::Color::Black);
		text->setCharacterSize(30U);

		AddObj(std::move(obj), "ActionQueue");
		LOG_COUT("[PASS] success add obj actionqueue");
		
		// TODO: 修改为根据students和teacters来生成角色绘制对象
		// 添加角色为绘制对象
		auto cai_ptr = std::make_unique<CaiShuYou>();
		AddObj(std::move(cai_ptr), "蔡曙优"); // 传入智能指针，无切片

		// 查找派生类对象
		auto cai2 = GetObjAs<CaiShuYou>("蔡曙优");
		if (!cai2) {
			LOG_CERR("[ERROR] cai2 is nullptr!");
		}
		else {
			LOG_COUT("[PASS] Success get CaiShuYou object, ID: " << cai2->GetID());
			students.push_back(cai2);
			mQueue.add(cai2);
		}

		mQueue.sort();

		LOG_COUT("[PASS] GameWnd 初始化完毕！");
	}
}