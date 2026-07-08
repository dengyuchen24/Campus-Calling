#include "UpgradeLevel.h"
#include "PreparationSeat.h"
#include "OpenShopButton.h"

extern std::map<std::string, sf::Font> g_Fonts;  // 字体变量
extern dyc::Message* g_Message;
extern int g_Coins;
extern dyc::WndManager* g_WndManager;

DYC_BEGIN

void UpgradeLevel::SetLevel(int cur_, int max_)
{
	if (cur_ > max_) return;
	if (max_ > 10) return;
	cur_level = cur_;
	max_level = max_;
}

UpgradeLevel::UpgradeLevel()
	: WndButton(g_Fonts.at("default"))
{
	LOG_COUT("[INFO] UpgradeLevel开始创建！");
	mText->setString(L"当前等级：Lv." + std::to_wstring(cur_level)
		+ L"\n当前经验：\n" + std::to_wstring(cur_experience)  + L"/" + std::to_wstring(need_experience)
		+ L"\n$4 → 4经验\n点击购买经验");
	mText->setCharacterSize(40U);
	SetButtonSize({ 320.0f, 240.0f });
	SetButtonPosition({ 5.0f, 200.0f });
	LOG_COUT("[PASS] 设置位置成功");
	auto rect = GetAs<sf::RectangleShape>();
	if (!rect)
	{
		LOG_COUT("[ERROR] 创建失败");
		return;
	}
	rect->setFillColor(sf::Color::White);
	rect->setOutlineColor(sf::Color::White);
	rect->setOutlineThickness(2.0f);
	SetTextColor(sf::Color::Black);
	SetUiOrder(-1);
}
void UpgradeLevel::update(const std::optional<sf::Event>& event)
{
	if (event->is<sf::Event::MouseButtonPressed>() && cur_level < max_level)
	{
		sf::Vector2i mouse_pos = event->getIf<sf::Event::MouseButtonPressed>()->position;
		auto rect = GetAs<sf::RectangleShape>();
		sf::FloatRect r = rect->getGlobalBounds();
		if (PointInRect(mouse_pos, r))
		{
			if (g_Coins >= 4)
			{
				g_Coins -= 4;
				cur_experience += 4;
				if (cur_experience >= need_experience)
				{
					cur_experience -= need_experience;
					SetLevel(cur_level + 1);
					g_WndManager->running_wnd->GetObjAs<PreparationSeat>("PreparationSeat")->max_front = cur_level;
					if (cur_level >= max_level)
					{
						mText->setString(L"当前等级：Lv." + std::to_wstring(cur_level)
							+ L"\n已满级");
						SetButtonSize({ 320.0f, 100.0f });
						return;
					}
					switch (cur_level)
					{
					case 5:
						need_experience = 10;
						break;
					case 6:
						need_experience = 20;
						break;
					case 7:
						need_experience = 40;
						break;
					case 8:
						need_experience = 60;
						break;
					case 9:
						need_experience = 80;
						break;
					}
				}
				if (cur_level < max_level)
				{
					mText->setString(L"当前等级：Lv." + std::to_wstring(cur_level)
						+ L"\n当前经验：\n" + std::to_wstring(cur_experience) + L"/" + std::to_wstring(need_experience)
						+ L"\n$4 → 4经验\n点击购买经验");
					g_WndManager->running_wnd->GetObjAs<OpenShopButton>("OpenShopButton")->SetText(L"商店 $" + std::to_wstring(g_Coins));
				}
			}
			else
			{
				g_Message->NewMsg(L"金币不足");
			}
		}
	}
	else if (event->is<sf::Event::MouseMoved>())
	{
		static bool last_mouse_in = false;
		mouse_in_last = last_mouse_in;
		sf::Vector2i mouse_pos = event->getIf<sf::Event::MouseMoved>()->position;
		auto rect = GetAs<sf::RectangleShape>();
		sf::FloatRect r = rect->getGlobalBounds();
		if (PointInRect(mouse_pos, r))
		{
			mouse_in = true;
			last_mouse_in = true;
		}
		else
		{
			mouse_in = false;
			last_mouse_in = false;
		}

	}
}

DYC_END