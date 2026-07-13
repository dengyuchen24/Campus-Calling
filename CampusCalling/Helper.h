#pragma once

#include "dycLogger/Logger.h"
#include <random>
#include <map>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <concepts>
#include <fstream>
#include <string>
#include <windows.h>

DYC_BEGIN

constexpr sf::Vector2i nullvec2i = sf::Vector2i(-1, -1);

// 获取指定范围的随机整数
inline int randint(int a, int b) {
	if (a > b) { std::swap(a, b); }
	static std::mt19937_64 engine(std::random_device{}());
	std::uniform_int_distribution<int> distribution(a, b);
	return distribution(engine);
}

// 随机获取一个元素
template <typename T>
inline T RandomElement(const std::map<T, int>& _map)
{
	int total = 0;
	for (auto& dict : _map) total += dict.second;
	int index = randint(0, total - 1);

	for (auto& dict : _map)
	{
		if (index < dict.second) return dict.first;
		index -= dict.second;
	}
	throw std::runtime_error("RandomElement: index out of range");
}

typedef std::chrono::high_resolution_clock high_rClock;
typedef std::chrono::system_clock low_rClock;

template <typename T>
concept ClockType =
std::same_as<T, high_rClock> ||  // 匹配高精度时钟
std::same_as<T, low_rClock>;     // 匹配系统时钟

template <ClockType T = low_rClock>
std::chrono::time_point<T> CurrentTime()
{
	return T::now();
}

template <ClockType T>
class Timer {
private:
	using CLOCK = low_rClock;  // 使用系统时钟，精度较低
public:
	inline void start() { start_time = CLOCK::now(); durations.clear(); }
	inline void stop() { end_time = CLOCK::now(); durations.push_back(elapsed_sec()); }
	long long elapsed_sec() const {  // 精确到秒
		return std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
	}
	double elapsed_ms() const {  // 毫秒，但精度为秒
		return elapsed_sec() * 1000.0;
	}
	double elapsed_min() const {  // 分钟
		return elapsed_sec() / 60.0;
	}
	double elapsed_hr() const {  // 小时
		return elapsed_sec() / 3600.0;
	}
	std::chrono::time_point<CLOCK> now() {
		return CLOCK::now();
	}
	std::vector<long long>* splitTimes() const { return &durations; }
private:
	std::chrono::time_point<CLOCK> start_time;
	std::chrono::time_point<CLOCK> end_time;
	std::vector<long long> durations;
};

template <>
class Timer<high_rClock> {
private:
	using CLOCK = high_rClock;
public:
	inline void start() { start_time = CLOCK::now(); durations.clear(); }
	inline void stop() { end_time = CLOCK::now(); durations.push_back(elapsed_ns()); }
	long long elapsed_ns() const {  // 纳秒
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
	}
	double elapsed_us() const {  // 微秒
		return elapsed_ns() * 1.0E-3;
	}
	double elapsed_ms() const {  // 毫秒
		return elapsed_ns() * 1.0E-6;
	}
	double elapsed_sec() const {  // 秒
		return elapsed_ns() * 1.0E-9;
	}
	std::chrono::time_point<CLOCK> now() {
		return CLOCK::now();
	}
	std::vector<long long>* splitTimes() { return &durations; }
private:
	std::chrono::time_point<CLOCK> start_time;
	std::chrono::time_point<CLOCK> end_time;
	std::vector<long long> durations;
};

// 将 UTF‑8 std::string 转换为 UTF‑16 std::wstring
inline std::wstring dto_wstring(const std::string& utf8str) {
	if (utf8str.empty()) return {};

	// 计算所需缓冲区大小（含终止符）
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8str.data(),
		static_cast<int>(utf8str.size()), nullptr, 0);
	if (len == 0) {
		throw std::runtime_error("MultiByteToWideChar failed to calculate size.");
	}

	std::wstring result(len, L'\0');
	int converted = MultiByteToWideChar(CP_UTF8, 0, utf8str.data(),
		static_cast<int>(utf8str.size()),
		result.data(), len);
	if (converted == 0) {
		throw std::runtime_error("MultiByteToWideChar conversion failed.");
	}
	return result;  // C++11 起保证返回的 string 不含多余空字符
}

// 将 UTF‑16 std::wstring 转换为 UTF‑8 std::string
inline std::string dto_string(const std::wstring& wstr) {
	if (wstr.empty()) return {};

	// 计算所需缓冲区大小（含终止符）
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.data(),
		static_cast<int>(wstr.size()), nullptr, 0,
		nullptr, nullptr);
	if (len == 0) {
		throw std::runtime_error("WideCharToMultiByte failed to calculate size.");
	}

	std::string result(len, '\0');
	int converted = WideCharToMultiByte(CP_UTF8, 0, wstr.data(),
		static_cast<int>(wstr.size()),
		result.data(), len,
		nullptr, nullptr);
	if (converted == 0) {
		throw std::runtime_error("WideCharToMultiByte conversion failed.");
	}
	return result;
}

DYC_END