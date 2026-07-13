// CampusCalling.cpp: 定义应用程序的入口点。
// 在CMakeLists.txt中修改自己的SFML库的位置
// 需要自行将SFML的dll库复制到exe文件的同目录下
//

#include "CampusCalling.h"

// 全局变量定义
dyc::Logger& logger = dyc::Logger::GetInstance("CampusCallingLog.txt");  // 日志输出变量
std::map<std::string, sf::Font> g_Fonts;  // 字体变量
dyc::WndManager* g_WndManager = nullptr;  // 窗口管理器变量
dyc::Message* g_Message;
int g_Coins = 1000;
std::map<std::string, _Student> g_Students;
std::map<std::string, _Teacher> g_Teachers;
std::map<std::string, _Effect> g_Effects;
std::map<std::string, _Card> g_Cards;

int WinMain()
{
	try
	{
		
		logger.log_info("CampusCalling 游戏程序启动！");
		sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "CampusCalling", sf::State::Fullscreen);
		window.setVerticalSyncEnabled(true);
		g_WndManager = new dyc::WndManager();
		g_Fonts["default"] = sf::Font("Assets/Fonts/Deng.ttf");  // 使用等线为默认字体
		g_Message = new dyc::Message();

		// 加载学生
		{
			std::ifstream file("Assets/Data/Students.json");
			dyc::json root = dyc::json::parse(file);
			g_Students = parseArrayToMap<_Student>(root["students"]);
		}
		// 加载教师
		{
			std::ifstream file("Assets/Data/Teachers.json");
			dyc::json root = dyc::json::parse(file);
			g_Teachers = parseArrayToMap<_Teacher>(root["teachers"]);
		}
		// 加载效果
		{
			std::ifstream file("Assets/Data/Effects.json");
			dyc::json root = dyc::json::parse(file);
			g_Effects = parseArrayToMap<_Effect>(root["effects"]);
		}
		// 加载卡牌
		{
			std::ifstream file("Assets/Data/Cards.json");
			dyc::json root = dyc::json::parse(file);
			g_Cards = parseArrayToMap<_Card>(root["cards"]);
		}

		g_WndManager->SetWindow(&window);
		g_WndManager->AddSfWnd("preparation window", std::make_unique<dyc::PreparationWnd>(g_WndManager));
		g_WndManager->SetRunning(g_WndManager->GetSfWnd()->at("preparation window").get());

		g_WndManager->run();

		if (window.isOpen()) window.close();
		 // 最后关闭窗口
		logger.log_info("CampusCalling 游戏程序退出！");
	}
	catch (const std::exception& e)
	{
		logger.log_error(e.what());
		return -1;
	}
	return 0;
}