#pragma once

#include "dycLogger/Logger.h"
#include <random>
#include <map>
#include <stdexcept>

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

DYC_END