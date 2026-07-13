// Logger.hpp 日志头文件
#pragma once

#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

#define DYC_BEGIN namespace dyc {
#define DYC_END }

namespace dyc
{
	using Path = std::filesystem::path;

	// 日志类
	class Logger
	{
	private:
		using TimePoint = std::chrono::system_clock::time_point;

		std::string filename;

		std::atomic<bool> running{ true };
		std::atomic<bool> hasNewLog{ false };
		std::atomic<bool> fileOpen{ false };

		TimePoint start = std::chrono::system_clock::now();

		// 线程相关
		std::thread workerThread;
		std::mutex mtx;
		std::condition_variable cv;

		// 超时检测
		TimePoint lastLogTime;
		std::thread timeoutThread;
		std::mutex timeoutMtx;
		std::condition_variable timeoutCv;

		std::ofstream file;

		// 日志队列
		std::vector<std::string> logQueue;
		std::mutex queueMtx;

		// 获取当前时间：年-月-日 时：分：秒
		std::string* GetTime();

		// 打开文件：如果成功打开则返回true否则为false
		bool OpenFile();

		// 关闭文件
		void CloseFile();

		// 实际写入日志的线程函数
		void WriteThread();

		// 超时检测线程
		void TimeoutThread();

		Logger(const std::string& file);
		~Logger();

	public:
		Logger() = delete;
		// 禁止拷贝和移动
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(Logger&&) = delete;

		// 获取全局唯一实例
		static Logger& GetInstance(const std::string& file_name);

		// 写入日志
		void log(const std::string& content, bool with_time = true);
		void log_error(const std::string& content, bool with_time = true);
		void log_info(const std::string& content, bool with_time = true);

		// 切换日志文件
		void ChangeFile(const std::string& newFilename);

		// 获取当前文件名
		std::string GetCurrentFilename() const;

		// 手动刷新（强制将队列中的日志写入文件）
		void Flush();
	};
}