#pragma once

#include "../dycLogger/Logger.h"
#include "../Effects/EffectSystem.hpp"
#include "../Cards/cards.h"
#include "char_reader.h"

#include <any>
#include <map>
#include <string>
#include <random>

// 0.0f ~ 1.0f 随机浮点数
inline float random_float_01()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	return dis(gen);
}

namespace dyc
{
	class Card;

	enum class Subject : int
	{
		CHINESE = 1,
		MATH = 2,
		ENGLISH = 3,
		PHYSICS =4,
		CHEMISTRY = 5,
		HISTORY = 6,
		POLITICS = 7,
		BIOLOGY = 8,
		GEOGRAPHY = 9
	};

	enum class ExtraKey : int
	{

	};

	class Character : public EffectTarget, public WndCard
	{
	protected:
		std::map<ExtraKey, std::any> mExtra;
		float mSpeedBonus = 0.0f;
		int mMoveVal = 1000;
		int cur_health = 10000;
		float strength = 1.0f;
		int mRound = 0;
		bool isMyTurn = false;  // 用于避免OnMyTurn里循环触发
	public:
		using WndCard::WndCard;

		void SetExtra(ExtraKey key, std::any val);

		// 获取可修改的指针
		template <typename T>
		T* GetExtraPtrAs(ExtraKey key)
		{
			auto it = mExtra.find(key);
			if (it == mExtra.end()) {
				
				return nullptr;
			}
			return std::any_cast<T>(&it->second);
		}

		// 获取mExtra的指针 const 版本
		template <typename T>
		const T* GetExtraPtrAs(ExtraKey key) const
		{
			auto it = mExtra.find(key);
			if (it == mExtra.end()) {
				return nullptr;
			}
			return std::any_cast<T>(&it->second);
		}

		std::wstring GetName();
		int GetSpeed();

		void SetSpeedBonus(float deltaSpeed);
		int GetMoveVal() const;
		void RecalculateMoveVal();

		int MaxHealth();
		int GetHealth() const;
		void SetHealth(int h);
		void TakeDamage(int dmg);
		bool IsDead() const;
		float GetStrength() const;

		// 处理自己的回合，但是用update函数处理玩家操作
		virtual void OnMyTurn() {}
	};

	class Student : public Character
	{
	public:
		Card* mEquipment = nullptr;
		float max_focus_bonus_ = 0.0f;  // 最大专注度加成（health）
		float lucky_rate_ = 0.05f;  // 幸运一击率
		float lucky_dmg_ = 0.5f;  // 幸运一击伤害
		float defense_bonus_ = 0.0f;
		float heal_bonus_ = 0.0f;
		float be_healed_bonus_ = 0.0f;
		float break_val_bonus_ = 0.0f;
		_Student* student = nullptr;
		Student(const std::string& name);
	};

	class Teacher : public Character
	{
	public:
		_Teacher* teacher = nullptr;
		Teacher(const std::string& name);
	};
}

#include "characters/CaiShuYou/CaiShuYou.h"