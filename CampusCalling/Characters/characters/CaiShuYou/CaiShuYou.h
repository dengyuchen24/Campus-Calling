#pragma once

#include "../../character.h"

namespace dyc
{
	class CaiShuYou : public Student
	{
	public:
		CaiShuYou()
		{
			// 这里也是直接找了一张丝柯克的图片代替一下
			if (!SetTexture("Assets/Pictures/丝柯克.png"))
			{
				LOG_COUT("[ERROR] CaiShuYou::CaiShuYou() 载入图像失败！");
				return;
			}
			SetPosition(sf::Vector2f(0, 0));
			SetBorder(2.0f, sf::Color::Blue);
			mMoveVal = 100;
			mName = L"蔡曙优";
			LOG_COUT("[PASS] 蔡曙优初始化成功！");
		}
		std::string Description() override { return ""; }
		void OnMyTurn() override {}
	};
}