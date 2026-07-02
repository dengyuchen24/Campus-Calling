#include "PreparationSeat.h"

extern dyc::Logger& logger;  // 日志输出变量
extern std::map<std::string, sf::Font> g_Fonts;  // 字体变量
extern dyc::WndManager* g_WndManager;  // 窗口管理器变量
extern dyc::Message* g_Message;
extern int g_Coins;

DYC_BEGIN

PreparationSeat::PreparationSeat() : WndObj()
{
	SetDrawable(std::make_unique<sf::RectangleShape>(sf::Vector2f(1500.0f, 187.5f)));
	auto mSeat = GetAs<sf::RectangleShape>();
	mSeat->setFillColor(sf::Color::Transparent);
	mSeat->setOutlineColor(sf::Color::Black);
	mSeat->setOutlineThickness(2.0f);
	mSeat->setPosition(sf::Vector2f(208.0f, 888.5f));
}

void PreparationSeat::draw(sf::RenderWindow* wnd)
{
	WndObj::draw(wnd);
	static sf::Vertex line[] = {
	sf::Vertex({0, 0}, sf::Color::Black),  // 起点 + 颜色
	sf::Vertex({0, 0}, sf::Color::Black)   // 终点 + 颜色
	};
	for (float x = 360.0f; x < 1710.0f; x += 150.0f)
	{
		line[0].position = { x, 890.0f };
		line[1].position = { x, 1080.0f };
		wnd->draw(line, 2, sf::PrimitiveType::Lines);
	}
	for (auto& c : mPreparationSeat)
	{
		if (c.second) c.second->draw(wnd);
	}
}

void PreparationSeat::AddSeat(const std::wstring& name)
{
	float idx = (float)mPreparationSeat.size();
	if ((int)idx >= max_seat) return;
	std::unique_ptr<WndCard> NewCard =
		std::make_unique<WndCard>();
	NewCard->SetTexture(L"Assets/Pictures/" + name + L".png");
	NewCard->SetScale(0.34f, 0.34f);
	NewCard->SetPosition(sf::Vector2f(215.0f + idx * 150, 899.375f));
	mPreparationSeat[SeatNum()] = std::move(NewCard);
}

int PreparationSeat::SeatNum() const
{
	int last = 0;
	for (auto& p : mPreparationSeat)
	{
		if (p.first != last) break;
		else ++last;
	}
	return last;
}

bool PreparationSeat::IsSeatFull() const
{
	return SeatNum() >= max_seat;
}

void PreparationSeat::update(const std::optional<sf::Event>& event)
{
	if (holding_card && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		auto pos = sf::Mouse::getPosition(*g_WndManager->GetWnd());
		float dx = static_cast<float>(pos.x - mouse_last_pos.x);
		float dy = static_cast<float>(pos.y - mouse_last_pos.y);
		holding_card->Move(sf::Vector2f(dx, dy));
		mouse_last_pos = pos;
	}
	if (event->is<MouseButtonPressed>())
	{
		auto pos = GetMousePos(event);
		if (!holding_card)
		{
			if (pos.y >= 890 && pos.x >= 210 && pos.x <= 1710)
			{
				int idx = (pos.x - 210) / 150;
				holding_card = mPreparationSeat[idx].get();
			}
		}
		mouse_last_pos = pos;
	}
	else if (event->is<sf::Event::MouseButtonReleased>())
	{
		auto pos = GetMousePos(event);
		if (!holding_card) return;
		holding_card = nullptr;
		// TODO: 完成释放鼠标后卡牌的落位
		mouse_last_pos = pos;
	}
}

DYC_END