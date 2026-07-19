#include "ActionQueue.h"

extern std::map<std::string, sf::Font> g_Fonts;  // 字体变量

DYC_BEGIN

ActionQueue::ActionQueue()
{
	SetUiOrder(-1);

	SetDrawable(std::make_unique<sf::Text>(g_Fonts.at("default")));

	auto text = GetAs<sf::Text>();

	text->setPosition({ 1500.0f, 0.0f });
	text->setFillColor(sf::Color::Black);
	text->setCharacterSize(30U);
}

void ActionQueue::add(Character* c) { if (c) queue.push_back(c); }

void ActionQueue::sort()
{
	std::sort(queue.begin(), queue.end(),
		[](Character* a, Character* b)->bool
		{ return a->GetMoveVal() < b->GetMoveVal(); });
}

bool ActionQueue::empty() { return queue.empty(); }

const std::vector<Character*>& ActionQueue::GetQueue() const { return queue; }

void ActionQueue::clear() { queue.clear(); }

void ActionQueue::draw(sf::RenderWindow* wnd)
{
	// 绘制行动队列
	if (queue.empty()) return;
	std::wstring content;
	int n = 0;
	for (auto c : queue)
	{
		content += (L"[" + std::to_wstring(c->GetMoveVal()) + L"] " + c->GetName() + L"\n");
		++n;
		if (n >= 10) break;
	}
	auto* aq = GetAs<sf::Text>();
	if (aq)
	{
		aq->setString(content);
		wnd->draw(*aq);
	}
}

DYC_END