#pragma once

#include "dycLogger/Logger.h"
#include <random>
#include <map>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <concepts>
#include <fstream>

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

DYC_END