#pragma once

#include <SFML/Graphics.hpp>
#include "../dycLogger/Logger.h"
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

namespace dyc
{
	inline bool PointInRect(const sf::Vector2i& point, const sf::FloatRect& rect)
	{
        return point.x >= rect.position.x && point.y >= rect.position.y
			&& point.x < rect.position.x + rect.size.x
			&& point.y < rect.position.y + rect.size.y;
	}

	inline sf::Vector2i GetMousePos(const std::optional<sf::Event>& event)
	{
		if (event->is<sf::Event::MouseMoved>())
		{
			return event->getIf<sf::Event::MouseMoved>()->position;
		}
		else if (event->is<sf::Event::MouseButtonPressed>())
		{
			return event->getIf<sf::Event::MouseButtonPressed>()->position;
		}
		else
		{
			return sf::Vector2i(-1, -1);
		}
	}

	class WndObj
	{
	protected:
		std::unique_ptr<sf::Drawable> drawable;
		int obj_id = 0;
		int ui_order = 0;  // 值越小越后绘制
	
	public:
		virtual void draw(sf::RenderWindow* wnd)  // 绘制
		{
			if (wnd && drawable) wnd->draw(*drawable);
			else LOG_COUT("wnd or drawable is nullptr");
		}

		virtual void update(const std::optional<sf::Event>& event) {}

		// 从WndObj获取drawable
		template <typename T> requires std::is_base_of_v<sf::Drawable, T>
		T* GetAs() { return dynamic_cast<T*>(drawable.get()); }
		
		WndObj() { static int id = 0; obj_id = ++id; }  // 构造函数
		
		virtual ~WndObj() = default;
		
		WndObj(const WndObj&) = delete;
		
		int GetID() const { return obj_id; }
		
		void SetUiOrder(int order) { ui_order = order; }  // 值越小越后绘制
		
		int GetOrder() const { return ui_order; }
		
		void SetDrawable(std::unique_ptr<sf::Drawable> d)
		{ drawable = std::move(d); }
	};

	class WndCard : public WndObj
	{
	protected:
		sf::Texture texture;
		sf::RectangleShape border;
	
	public:
		using WndObj::WndObj;
		
		bool SetTexture(const std::filesystem::path& path);  // 设置纹理

		bool SetPosition(sf::Vector2f leftup);  // 设置纹理位置

		// 设置缩放
		// 当use_default为true时，scalex 和 scaley 都改为1.0f
		void SetScale(float scalex, float scaley, bool use_default = false);

		void SetBorder(float thickness = 0.0f, sf::Color color = sf::Color::Transparent);
		
		void draw(sf::RenderWindow* wnd) override;
	};

	class WndButton : public WndObj
	{
	protected:
		// std::unique_ptr<sf::Drawable> drawable; -> sf::RectangleShape
		std::unique_ptr<sf::Text> mText;

		void updateTextPosition();  // 更新文字位置

	public:
		bool mouse_in = false;
		bool mouse_in_last = false;

		WndButton(const sf::Font& font);

		void SetText(const std::wstring& text);  // 设置文字

        void SetTextFont(const sf::Font& font);  // 设置字体

		void SetTextColor(sf::Color color);  // 设置文字颜色

        void SetButtonPosition(sf::Vector2f leftup);  // 设置按钮位置

		void SetButtonSize(sf::Vector2f size);  // 设置按钮大小

		void draw(sf::RenderWindow* wnd) override;
	};

	class WndManager;

	class sfWindow
	{
	protected:
		sf::RenderWindow* window = nullptr;
		std::vector<std::unique_ptr<WndObj>> objects;
		std::map<std::string, size_t> obj_id_map;
		WndManager* manager = nullptr;

		void draw()
		{
			for (auto& pObj : objects)
				pObj->draw(window);
		}
		
		void update(const std::optional<sf::Event>& event)
		{
			for (auto& obj : objects) obj->update(event);
		}
		
		virtual void logic() {};

	public:
		sfWindow() = delete;
		
		sfWindow(WndManager* m);

		sf::RenderWindow* GetWindow() const { return window; }

		// 在sfWindow里添加WndObj
		void AddObj(std::unique_ptr<dyc::WndObj> obj, const std::string& str);

		// 在sfWindow里获取以str为标识的WndObj
		WndObj* GetObj(const std::string& str);

		// 在sfWindow里获取以str为标识的WndObj的T类型实例
		template<typename T>
		T* GetObjAs(const std::string& name)
		{
			// 查找逻辑...
			for (auto& p : objects)
				if (p->GetID() == obj_id_map[name])
					return dynamic_cast<T*>(p.get());
			return nullptr;
		}

		void OnWhile();

		WndManager* GetManager() const { return manager; }
	};

	class WndManager
	{
		std::map<std::string, std::unique_ptr<sfWindow>> m_windows;
		sf::RenderWindow* window = nullptr;

	public:
		sfWindow* running_wnd = nullptr;

		sf::RenderWindow* GetWnd() const
		{
			if (window) return window;
			LOG_COUT("[WARNING] WndManager's window is nullptr!");
			return nullptr;
		}

		const std::map<std::string, std::unique_ptr<sfWindow>>* GetSfWnd() const { return &m_windows; }
		
		void AddSfWnd(const std::string& key, std::unique_ptr<sfWindow> wnd) { m_windows[key] = std::move(wnd); }
		
		void SetRunning(sfWindow* wnd) { running_wnd = wnd; }
		
		void run()
		{
			while (window->isOpen() && running_wnd) { running_wnd->OnWhile(); }
		}
		
		void SetWindow(sf::RenderWindow* w) { window = w; }
	};
}

#include "wndManager/GameWnd/GameWnd.h"
#include "wndManager/PreparationWnd/Preparation.h"