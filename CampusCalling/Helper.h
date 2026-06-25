#pragma once

#include "dycLogger/Logger.h"
#include <random>
#include <map>
#include <stdexcept>
#include <string>
#include <Windows.h>

DYC_BEGIN

constexpr sf::Vector2i nullvec2i = sf::Vector2i(-1, -1);

// 使用 Windows API 将 std::wstring 转换为 UTF-8 编码的 std::string
inline std::string WStrToStr(const std::wstring& wstr)
{
	if (wstr.empty())
	{
		return "";
	}

	// 计算所需缓冲区大小
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	if (size_needed <= 0)
	{
		return "";
	}

	// 分配缓冲区并执行转换
	std::string str(size_needed, 0);
	int result = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);

	if (result <= 0)
	{
		return "";
	}

	return str;
}

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