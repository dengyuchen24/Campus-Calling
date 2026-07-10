#include "PreparationSeat.h"
#include "OpenShopButton.h"

extern dyc::Logger& logger;  // 日志输出变量
extern std::map<std::string, sf::Font> g_Fonts;  // 字体变量
extern dyc::WndManager* g_WndManager;  // 窗口管理器变量
extern dyc::Message* g_Message;
extern int g_Coins;

DYC_BEGIN

Seat::Seat() : WndCard() {}

// TODO: 把所有装备牌在GetInfo完善
CardType Seat::GetInfo() const
{
	if (mSeatType != SeatType::CARD) throw "If seat isn't a card but a character, it doesn't have a card type.";
	if (cardname == L"lll")
	{
		return CardType::EQUIPMENT;
	}
}

SeatType GetSeatType(const std::wstring& str)
{
	if (str == L"丝柯克") return SeatType::CHARACTER;
	else if (str == L"蔡曙优") return SeatType::CHARACTER;
	return SeatType::NULLSEATTYPE;
}

PreparationSeat::PreparationSeat() : WndObj()
{
	SetDrawable(std::make_unique<sf::RectangleShape>(sf::Vector2f(1500.0f, 187.5f)));
	auto mSeat = GetAs<sf::RectangleShape>();
	mSeat->setFillColor(sf::Color::Transparent);
	mSeat->setOutlineColor(sf::Color::Black);
	mSeat->setOutlineThickness(2.0f);
	mSeat->setPosition(sf::Vector2f(208.0f, 888.5f));

	for (float x = 715.0f; x < 1205.0f; x += 170.0f)
	{
		mBlocks.push_back({ x, 430.0f });
	}
	for (float x = 420.0f; x < 1500.0f; x += 155.0f)
	{
		mBlocks.push_back({ x, 650.0f });
	}
	for (float x = 210.0f; x < 1710.0f; x += 150.0f)
	{
		mBlocks.push_back({ x, 890.0f });
	}
}

int PreparationSeat::CountCards(int a, int b)
{
	int res = 0;
	for (auto& [idx, val] : mPreparationSeat)
	{
		if (idx >= a && idx < b) ++res;
		else if (idx >= b) break;
	}
	return res;
}

void PreparationSeat::draw(sf::RenderWindow* wnd)
{
	WndObj::draw(wnd);

	static sf::RectangleShape rectangle({ 150.0f, 200.0f });
	static bool RectInit = false;
	if (!RectInit)
	{
		RectInit = true;
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::Black);
		rectangle.setOutlineThickness(1.0f);
	}
	bool open = g_WndManager->running_wnd->GetObjAs<ShopSystem>("ShopSystem")->GetOpen();
	if (open)
	{
		for (float x = 715.0f; x < 1205.0f; x += 170.0f)
		{
			rectangle.setPosition(sf::Vector2f(x, 430.0f));
			wnd->draw(rectangle);
			mBlocks[(INDEX)((x - 715.0f) / 170.0f)] = { x, 430.0f };
		}
		for (float x = 420.0f; x < 1500.0f; x += 155.0f)
		{
			rectangle.setPosition(sf::Vector2f(x, 650.0f));
			wnd->draw(rectangle);
			mBlocks[(INDEX)((x - 420.0f) / 155.0f + 3)] = { x, 650.0f };
		}
	}
	else
	{
		for (float x = 685.0f; x <= 1085.0f; x += 200.0f)
		{
			rectangle.setPosition(sf::Vector2f(x, 200.0f));
			wnd->draw(rectangle);
			mBlocks[(INDEX)((x - 685.0f) / 200.0f)] = { x, 200.0f };
		}
		for (float x = 400.0f; x < 1520.0f; x += 160.0f)
		{
			rectangle.setPosition(sf::Vector2f(x, 500.0f));
			wnd->draw(rectangle);
			mBlocks[(INDEX)((x - 400.0f) / 160.0f + 3)] = { x, 500.0f };
		}
	}

	static sf::Vertex line[] = {
	sf::Vertex({0, 0}, sf::Color::Black),  // 起点 + 颜色
	sf::Vertex({0, 0}, sf::Color::Black)   // 终点 + 颜色
	};
	for (float x = 360.0f; x < 1710.0f; x += 150.0f)
	{
		line[0].position = { x, 890.0f };
		line[1].position = { x, 1078.0f };
		wnd->draw(line, 2, sf::PrimitiveType::Lines);
	}
	for (auto& c : mPreparationSeat)
	{
		if (c.second)
		{
			if (holding_card != c.second.get())
				c.second->GetAs<sf::Sprite>()
					->setPosition
					(mBlocks[c.first] + sf::Vector2f(5.0f, 9.375f));
			c.second->draw(wnd);
		}
	}

	static sf::Text text(g_Fonts["default"]);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(20U);
	text.setString(L"备战席 " + std::to_wstring(CountCards(10, 20)) + L"/10");
	text.setPosition({ 210.0f, 865.0f });
	wnd->draw(text);
	text.setString(L"出战席 " + std::to_wstring(CountCards(0, 10)) + L"/" + std::to_wstring(max_front));
	text.setCharacterSize(40U);
	text.setPosition({ 15.0f, 100.0f });
	wnd->draw(text);
	if (will_be_sold)
	{
		text.setString(L"松开鼠标按键即可出售");
		text.setFillColor(sf::Color::Red);
		text.setCharacterSize(20U);
		text.setPosition({ 15.0f, 800.0f });
		wnd->draw(text);
		text.setPosition({ 1680.0f, 800.0f });
		wnd->draw(text);
	}
}

void PreparationSeat::AddSeat(const std::wstring& name, SeatType type, int cost)
{
	float idx = (float)CountCards(10, 20);
	if ((int)idx >= 10) return;
	std::unique_ptr<Seat> NewCard =
		std::make_unique<Seat>();
	NewCard->SetTexture(L"Assets/Pictures/" + name + L".png");
	NewCard->SetScale(0.34f, 0.34f);
	NewCard->SetPosition(sf::Vector2f(215.0f + idx * 150, 899.375f));
	NewCard->mSeatType = type;
	NewCard->mcost = cost;
	mPreparationSeat[SeatNum()] = std::move(NewCard);
}

int PreparationSeat::SeatNum() const
{
	int last = 10;
	for (auto& p : mPreparationSeat)
	{
		if (p.first < 10) continue;
		if (p.first != last) break;
		else ++last;
	}
	return last;
}

bool PreparationSeat::IsSeatFull() const
{
	return SeatNum() >= 20;
}

void PreparationSeat::update(const std::optional<sf::Event>& event)
{
	if (holding_card && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		auto pos = sf::Mouse::getPosition(*g_WndManager->GetWnd());
		float dx = static_cast<float>(pos.x - mouse_last_pos.x);
		float dy = static_cast<float>(pos.y - mouse_last_pos.y);
		holding_card->Move(sf::Vector2f(dx, dy));
		if ((pos.x < 150 || pos.x > 1770) && pos.y > 880)
		{
			holding_card->SetBorder(5.0f, sf::Color::Red);
			will_be_sold = true;
		}
		else
		{
			holding_card->SetBorder();
			will_be_sold = false;
		}
		mouse_last_pos = pos;
	}

	if (!event.has_value())
		return;

	if (event->is<MouseButtonPressed>())
	{
		auto pos = GetMousePos(event);
		if (!holding_card)
		{
			for (auto& c : mPreparationSeat)
			{
				sf::FloatRect rect =
					c.second->GetAs <sf::Sprite>()->getGlobalBounds();
				if (PointInRect(pos, rect))
				{
					holding_card = c.second.get();
					card_last_idx = c.first;
					break;
				}
			}
		}
		mouse_last_pos = pos;
	}
	else if (event->is<sf::Event::MouseButtonReleased>())
	{
		auto pos = sf::Mouse::getPosition(*g_WndManager->GetWnd());
		if (!holding_card) return;

		auto hpos = holding_card->GetAs<sf::Sprite>()->getPosition();

		int idx = 0;
		bool processed = false;
		// 角色牌卖钱，其他卡牌不卖钱
		if ((pos.x < 150 || pos.x > 1770) && pos.y > 880)
		{
			int delete_idx = card_last_idx;

			int cost = dynamic_cast<Seat*>(holding_card)->mcost;

			holding_card = nullptr;

			mPreparationSeat.erase(delete_idx);

			g_Coins += cost;
			g_WndManager->running_wnd->GetObjAs<OpenShopButton>("OpenShopButton")
				->SetText(L"商店 $" + std::to_wstring(g_Coins));
			will_be_sold = false;
			processed = true;
		}
		else
		{
			for (auto& block : mBlocks)
			{
				float x = block.x + 5.0f;
				float y = block.y + 9.375f;
				if (std::abs(x - hpos.x) < 75.0f && std::abs(y - hpos.y) < 75.0f)
				{
					if (!mPreparationSeat.contains(idx))
					{
						LOG_COUT("[DEBUG] " << card_last_idx << " to " << idx);
						LOG_COUT("[DEBUG] count cards: " << CountCards(0, 10));
						if (card_last_idx >= 10 && idx < 10 && CountCards(0, 10) >= max_front)
						{
							g_Message->NewMsg(L"出战席已满");
							break;
						}
						LOG_COUT("[DEBUG] Allow to put");
						if (Seat* seat = dynamic_cast<Seat*>(holding_card))
						{
							LOG_COUT("[DEBUG] seat is good");
							if (seat->mSeatType == SeatType::CARD && idx < 3)
							{
								g_Message->NewMsg(L"不要把卡牌和角色位错位放置");
								break;
							}
							if (seat->mSeatType == SeatType::CHARACTER && idx > 2 && idx < 10)
							{
								g_Message->NewMsg(L"不要把角色和卡牌位错位放置");
								break;
							}
							if (seat->mSeatType == SeatType::CHARACTER)
							{
								bool bk = false;
								for (auto& [i, c] : mPreparationSeat)
								{
									if (i > 2) { bk = false; break; }
									if (c->cardname == seat->cardname)
									{
										g_Message->NewMsg(L"不可以同时出战相同的角色");
										bk = true;
										break;
									}
								}
								if (bk) break;
							}
						}
						else break;
						mPreparationSeat[idx] = std::move(mPreparationSeat[card_last_idx]);
						auto ptr = mPreparationSeat[idx].get();
						ptr->GetAs<sf::Sprite>()->setPosition({ x, y });
						mPreparationSeat.erase(card_last_idx);
						LOG_COUT("[DEBUG] Successful to put");
					}
					else
					{
						if (Seat* seat = dynamic_cast<Seat*>(holding_card))
						{
							LOG_COUT("[DEBUG] seat is good");
							if (seat->mSeatType == SeatType::CARD && idx < 3)
							{
								if (seat->GetInfo() == CardType::EQUIPMENT && mPreparationSeat.at(idx)->equipment_name.empty())
								{
									mPreparationSeat.at(idx)->equipment_name = seat->cardname;
									holding_card = nullptr;
									mPreparationSeat.erase(card_last_idx);
									processed = true;
									break;
								}
								g_Message->NewMsg(L"不要把卡牌和角色位错位放置");
								break;
							}
							if (seat->mSeatType == SeatType::CHARACTER && idx > 2 && idx < 10)
							{
								g_Message->NewMsg(L"不要把角色和卡牌位错位放置");
								break;
							}
							if (seat->mSeatType == SeatType::CHARACTER)
							{
								bool bk = false;
								for (auto& [i, c] : mPreparationSeat)
								{
									if (i == idx) continue;
									if (i > 2) { bk = false; break; }
									if (c->cardname == seat->cardname)
									{
										g_Message->NewMsg(L"不可以同时出战相同的角色");
										bk = true;
										break;
									}
								}
								if (bk) break;
							}
						}
						else break;
						std::unique_ptr<Seat> ptr = std::move(mPreparationSeat[idx]);
						mPreparationSeat[idx] = std::move(mPreparationSeat[card_last_idx]);
						mPreparationSeat[card_last_idx] = std::move(ptr);
						auto _ptr = mPreparationSeat[idx].get();
						_ptr->GetAs<sf::Sprite>()->setPosition({ x, y });
						auto __ptr = mPreparationSeat[card_last_idx].get();
						__ptr->GetAs<sf::Sprite>()->setPosition(mBlocks[card_last_idx] + sf::Vector2f(5.0f, 9.375f));
						LOG_COUT("[DEBUG] Successful to change");
					}
					processed = true;
					break;
				}
				++idx;
			}
		}
		if (!processed)
		{
			holding_card->GetAs<sf::Sprite>()->setPosition(mBlocks[card_last_idx] + sf::Vector2f(5.0f, 9.375f));
		}

		if (holding_card) holding_card = nullptr;
		mouse_last_pos = pos;
	}
}

PreparationSeat::~PreparationSeat()
{
	holding_card = nullptr;
	will_be_sold = false;

	mPreparationSeat.clear();

	mBlocks.clear();
}

DYC_END