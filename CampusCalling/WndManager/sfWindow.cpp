#include "sfWindow.h"

#include "EscButton.h"

extern std::map<std::string, sf::Font> g_Fonts;

dyc::sfWindow::sfWindow(WndManager* m)
{
	if (m) { manager = m; window = m->GetWnd(); }
	else { LOG_CERR("[ERROR] Creating sfWindow, wnd must not be nullptr."); }
    AddObj(std::make_unique<EscButton>(), "EscButton");
}

void dyc::sfWindow::OnWhile()
{
	while (manager->running_wnd == this && window->isOpen())
	{
		// 1. 事件处理：轮询所有待处理的事件
		while (const std::optional<sf::Event> event = window->pollEvent())
		{
			update(event);
		}

		if (manager->running_wnd != this) break;
		// 2. 逻辑
		logic();

		if (manager->running_wnd != this) break;

		// 退出按钮
		auto escbutton = GetObjAs<EscButton>("EscButton");
		if (escbutton->mouse_in && !escbutton->mouse_in_last)
			escbutton->SetTextColor(sf::Color::Red);
		else if (!escbutton->mouse_in && escbutton->mouse_in_last)
			escbutton->SetTextColor(sf::Color::Black);

		// 3. 渲染
		window->clear(sf::Color::White);
		draw();
		window->display();
	}
}

DYC_BEGIN

bool WndCard::SetTexture(const std::filesystem::path& path)
{
	if (!texture.loadFromFile(path))
	{
		LOG_COUT("[ERROR] Can't load texture from " << path);
		return false;
	}
	texture.setSmooth(true);
	SetDrawable(std::make_unique<sf::Sprite>(texture));
	sf::Sprite* sprite = GetAs<sf::Sprite>();
	sprite->setPosition(sf::Vector2f(0, 0));
	LOG_COUT("[INFO] Texture: " << path << " loaded successfully!");
	return true;
}

WndObj* sfWindow::GetObj(const std::string& str)
{
	for (auto& obj : objects)
		if (obj->GetID() == obj_id_map.at(str))
			return obj.get();
	LOG_COUT("[ERROR] Object with str " << str << " not found!");
	return nullptr;
}

void sfWindow::AddObj(std::unique_ptr<dyc::WndObj> obj, const std::string& str)
{
	if (!obj) return;
	obj_id_map[str] = obj->GetID();
	objects.emplace_back(std::move(obj));
	std::sort(objects.begin(), objects.end(),
		[](std::unique_ptr<WndObj>& a, std::unique_ptr<WndObj>& b)->bool
		{ return a->GetOrder() > b->GetOrder(); });
}

bool WndCard::SetPosition(sf::Vector2f leftup)
{
	if (!drawable) return false;
	auto sprite = dynamic_cast<sf::Sprite*>(drawable.get());
	if (!sprite) return false;
	sprite->setPosition(leftup);
	return true;
}

void WndCard::SetScale(float scalex, float scaley, bool use_default)
{
	auto sprite = GetAs<sf::Sprite>();
	if (!sprite)
	{
		LOG_COUT("[ERROR] WndCard::SetTexturePosition() failed!");
		return;
	}
	if (use_default)
	{
		sprite->setScale(sf::Vector2f(1.0f, 1.0f));
		return;
	}
	sprite->setScale(sf::Vector2f(scalex, scaley));
}

void WndButton::SetText(const std::wstring& text)
{
	mText->setString(text);
	updateTextPosition();
}

void WndButton::SetTextFont(const sf::Font& font)
{
	mText->setFont(font);
	updateTextPosition();
}

void WndButton::SetTextColor(sf::Color color)
{
	mText->setFillColor(color);
}

void WndButton::SetButtonPosition(sf::Vector2f leftup)
{
	auto rect = GetAs<sf::RectangleShape>();
	if (!rect)
	{
		LOG_COUT("[ERROR] WndButton::SetButtonPosition() failed!");
		return;
	}
	rect->setPosition(leftup);
	LOG_COUT("[PASS] Button position set successfully");
	updateTextPosition();
	LOG_COUT("[PASS] Button position update successfully");
}

void WndButton::SetButtonSize(sf::Vector2f size)
{
	auto rect = GetAs<sf::RectangleShape>();
	rect->setSize(size);
	updateTextPosition();
}

void WndButton::updateTextPosition()
{
	// 设置文字在矩形中居中
	LOG_COUT("[INFO] Updating text position...");
	auto rect = GetAs<sf::RectangleShape>();
	LOG_COUT("[INFO] Get rect: " << rect);
	LOG_COUT("[INFO] Get text: " << mText);
	if (!rect || !mText)
	{
		LOG_COUT("[ERROR] WndButton::updateTextPosition() failed!");
		return;
	}
	// sf::Text::getFont() 返回 const sf::Font*。如果返回 nullptr，说明字体未设置或已失效。
	if (mText->getFont().getInfo().family.empty())
	{
		LOG_COUT("[ERROR] Font is not set or invalid for mText!");
		// 这里可以选择设置一个默认字体，或者直接返回
		mText->setFont(g_Fonts.at("default"));
		return;
	}
	sf::FloatRect textBounds = mText->getLocalBounds();
	sf::Vector2f rectCenter = rect->getPosition() + rect->getSize() / 2.0f;
	LOG_COUT("[INFO] Before mText->setPosition() is ok.");
	mText->setPosition(sf::Vector2f(
		rectCenter.x - textBounds.size.x / 2.0f - textBounds.position.x,
		rectCenter.y - textBounds.size.y / 2.0f - textBounds.position.y
	));
	LOG_COUT("[PASS] Text position update successfully");
}

void WndButton::draw(sf::RenderWindow* wnd)
{
	wnd->draw(*drawable);
	wnd->draw(*mText);
}

WndButton::WndButton(const sf::Font& font) : WndObj()
{
	SetDrawable(std::make_unique<sf::RectangleShape>(sf::Vector2f(100, 50)));
	mText = std::make_unique<sf::Text>(font);
	mText->setString(L"按钮");
}

void WndCard::SetBorder(float thickness, sf::Color color)
{
    border.setOutlineThickness(thickness);
	border.setOutlineColor(color);
    border.setFillColor(sf::Color::Transparent);
	border.setSize(GetAs<sf::Sprite>()->getGlobalBounds().size);
	border.setPosition(GetAs<sf::Sprite>()->getPosition());
}

void WndCard::draw(sf::RenderWindow* wnd)
{
	wnd->draw(*drawable);
	wnd->draw(border);
}

void sfWindow::update(const std::optional<sf::Event>& event)
{
	for (auto& obj : objects)
	{
		if (manager->running_wnd != this) break;
		obj->update(event);
	}
}

DYC_END