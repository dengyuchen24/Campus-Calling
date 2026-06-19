#pragma once

#include "../../character.h"

namespace dyc
{
	class CaiShuYou : public Student
	{
	public:
		CaiShuYou()
		{
			if (!SetTexture("E:/dd/FILES/图像/GS/丝柯克.png"))
			{
				LOG_COUT("CaiShuYou::CaiShuYou() 载入图像失败！");
				return;
			}
			SetPosition(sf::Vector2f(0, 0));
			SetBorder(2.0f, sf::Color::Blue);
			mMoveVal = 100;
			mName = L"蔡曙优";
			LOG_COUT("蔡曙优初始化成功！");
		}
		std::string Description() override { return ""; }
		void OnMyTurn() override {}
	};
}