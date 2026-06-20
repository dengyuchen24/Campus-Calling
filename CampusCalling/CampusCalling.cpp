// CampusCalling.cpp: 定义应用程序的入口点。
// 在Logger.h中定义NCONSOLE是否启用控制台输出
//

#include "CampusCalling.h"

// 全局变量定义
dyc::Logger& logger = dyc::Logger::GetInstance("CampusCallingLog.txt");  // 日志输出变量
std::map<std::string, sf::Font> g_Fonts;  // 字体变量
dyc::WndManager g_WndManager;  // 窗口管理器变量
int g_Coins = 0;

int main()
{
	try
	{
		LOG_COUT("[INFO] CampusCalling 项目成功启动！");
		logger.log_info("CampusCalling 游戏程序启动！");
		sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "CampusCalling", sf::State::Fullscreen);
		window.setVerticalSyncEnabled(true);

		g_Fonts["default"] = sf::Font("Assets/Fonts/Deng.ttf");  // 使用等线为默认字体

		g_WndManager.SetWindow(&window);
		g_WndManager.AddSfWnd("preparation window", std::make_unique<dyc::PreparationWnd>(&g_WndManager));
		g_WndManager.SetRunning(g_WndManager.GetSfWnd()->at("preparation window").get());

		g_WndManager.run();
		LOG_COUT("[INFO] CampusCalling 项目退出...");
		logger.log_info("CampusCalling 游戏程序退出！");
	}
	catch (const std::exception& e)
	{
		LOG_CERR("[ERROR] Main function error: " << e.what());
		return -1;
	}
	return 0;
}