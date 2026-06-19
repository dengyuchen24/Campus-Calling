#pragma once

#include "../../../Characters/character.h"

extern int g_Coins;

DYC_BEGIN

class PreparationWnd : public sfWindow
{
private:

public:
	PreparationWnd(WndManager* mgr);  // 构造函数

	void logic() override;
};

DYC_END