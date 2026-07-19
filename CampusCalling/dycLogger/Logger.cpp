// Logger.cpp 日志文件实现
#include "Logger.h"

std::string* dyc::Logger::GetTime() {
	// 使用指针，永远不会被析构
	static std::mutex* mtx = new std::mutex;
	static std::string* cachedTime = new std::string;
	static std::chrono::steady_clock::time_point* lastUpdate = new std::chrono::steady_clock::time_point;
	static bool* initialized = new bool(false);

	auto now = std::chrono::steady_clock::now();

	if (!*initialized || now - *lastUpdate >= std::chrono::seconds(1)) {
		auto systemNow = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(systemNow);
		std::tm localTime;
		localtime_s(&localTime, &time);

		std::stringstream ss;
		ss << std::put_time(&localTime, "[%Y-%m-%d %H:%M:%S]");

		{
			std::lock_guard<std::mutex> lock(*mtx);
			*cachedTime = ss.str();
			*lastUpdate = now;
			*initialized = true;
		}
	}

	std::lock_guard<std::mutex> lock(*mtx);
	return cachedTime;
}

bool dyc::Logger::OpenFile()
{
	if (fileOpen.load())
	{
		return true;
	}

	file.open(filename, std::ios::app);
	if (file.is_open())
	{
		fileOpen.store(true);
		return true;
	}
	
	return false;
}

void dyc::Logger::CloseFile()
{
	if (fileOpen.load())
	{
		file.flush();
		file.close();
		fileOpen.store(false);
	}
}

void dyc::Logger::WriteThread()
{
	while (running.load())
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait_for(lock, std::chrono::seconds(1), [this] {
			return !running.load() || hasNewLog.load();
			});

		// 收集待写入的日志（即使 running=false 也要把剩余日志写完）
		std::vector<std::string> logs;
		{
			std::lock_guard<std::mutex> qLock(queueMtx);
			logs.swap(logQueue);
		}

		if (!logs.empty())
		{
			OpenFile();
			for (const auto& log : logs)
			{
				if (file.is_open())
				{
					file << log << std::flush;
				}
			}

			{
				std::lock_guard<std::mutex> tLock(timeoutMtx);
				lastLogTime = std::chrono::system_clock::now();
			}
			timeoutCv.notify_one();
		}
	}

	// 退出前把剩余日志写完（再次检查）
	std::vector<std::string> remainingLogs;
	{
		std::lock_guard<std::mutex> qLock(queueMtx);
		remainingLogs.swap(logQueue);
	}
	if (!remainingLogs.empty())
	{
		OpenFile();
		for (const auto& log : remainingLogs)
		{
			if (file.is_open())
			{
				file << log << std::flush;
			}
		}
	}

	// 最后关闭文件
	CloseFile();
}

void dyc::Logger::TimeoutThread()
{
	while (running.load())
	{
		std::unique_lock<std::mutex> lock(timeoutMtx);

		// 等待有日志写入事件，或者每1秒检查一次
		timeoutCv.wait_for(lock, std::chrono::seconds(1), [this] {
			return !running.load();
			});

		if (!running.load())
			break;

		// 检查是否超过1秒没有新日志
		if (fileOpen.load())
		{
			auto now = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastLogTime).count();

			if (elapsed >= 1)
			{
				// 超时，关闭文件
				CloseFile();
			}
		}
	}
}

dyc::Logger::Logger(const std::string& file) : filename(file)
{
	// 初始化最后日志时间
	lastLogTime = std::chrono::system_clock::now();

	// 启动写线程
	workerThread = std::thread(&Logger::WriteThread, this);

	// 启动超时检测线程
	timeoutThread = std::thread(&Logger::TimeoutThread, this);

	log_info("---------- Logger created ----------");
}

dyc::Logger::~Logger()
{
	log_info("---------- Logger destroyed ----------\n");

	// 1. 先通知所有线程停止
	running.store(false);
	cv.notify_all();
	timeoutCv.notify_all();

	// 2. 等待线程结束（此时写线程会执行 CloseFile）
	if (workerThread.joinable())
		workerThread.join();
	if (timeoutThread.joinable())
		timeoutThread.join();

	// 3. 确保文件关闭（如果写线程没有关闭的话）
	if (file.is_open()) {
		file.flush();
		file.close();
	}
}

void dyc::Logger::log(const std::string& content, bool with_time)
{
	std::string formattedLog = (with_time ? *GetTime() : std::string()) + " " + content + "\n";

	{
		std::lock_guard<std::mutex> lock(queueMtx);
		logQueue.push_back(formattedLog);
	}

	hasNewLog.store(true);
	cv.notify_one();
}

void dyc::Logger::ChangeFile(const std::string& newFilename)
{
	std::lock_guard<std::mutex> lock(mtx);

	// 关闭当前打开的文件
	CloseFile();

	

	// 更新文件名
	filename = newFilename;
}

std::string dyc::Logger::GetCurrentFilename() const
{
	return filename;
}

void dyc::Logger::Flush()
{
	// 通知写线程立即处理
	cv.notify_one();
}

void dyc::Logger::log_error(const std::string& content, bool with_time)
{
	log(std::string("[ERROR] ") + content, with_time);
	Flush();  // 错误日志立即刷新
}

void dyc::Logger::log_info(const std::string& content, bool with_time)
{
	log(std::string("[INFO] ") + content, with_time);
}

dyc::Logger& dyc::Logger::GetInstance(const std::string& file_name)
{
	static Logger logger(file_name);
	return logger;
}