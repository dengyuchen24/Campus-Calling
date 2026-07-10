#pragma once

#include "../dycLogger/Logger.h"
#include "../Effects/EffectSystem.hpp"
#include "../Cards/cards.h"

#include <any>
#include <map>
#include <set>
#include <string>
#include <vector>
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
		MATH,
		ENGLISH,
		PHYSICS,
		CHEMISTRY,
		HISTORY,
		POLITICS,
		BIOLOGY,
		GEOGRAPHY
	};

	enum class ExtraKey : int
	{

	};

	class Character : public EffectTarget, public WndCard
	{
	protected:
		std::wstring mName;
		std::map<ExtraKey, std::any> mExtra;
		int mSpeed = 100;
		float mSpeedBonus = 0.0f;
		int mMoveVal = 1000;
		int max_health = 10000;
		int cur_health = 10000;
		float strength = 1.0f;
		std::set<Subject> mSubjects;
		int mRound = 0;
		bool isMyTurn = false;  // 用于避免OnMyTurn里循环触发
	public:
		using WndCard::WndCard;

		virtual std::string Description() = 0;

		void SetExtra(ExtraKey key, std::any val)
		{
			mExtra[key] = std::move(val);
		}

		// 获取可修改的指针
		template <typename T>
		T* GetExtraPtrAs(ExtraKey key)
		{
			auto it = mExtra.find(key);
			if (it == mExtra.end()) {
				LOG_COUT("[ERROR] Key not found: " << (int)key);
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

		std::wstring GetName() const { return mName; }
		void SetSpeedBonus(float deltaSpeed) { mSpeedBonus += deltaSpeed; }
		int GetMoveVal() const { return mMoveVal; }
		void RecalculateMoveVal()
		{
			int mv = mMoveVal;
			mMoveVal += static_cast<int>
			(1000 / (mSpeed + static_cast<int>(mSpeed * mSpeedBonus)));
			if (mv == mMoveVal) ++mMoveVal;
		}

		// 处理自己的回合，但是用update函数处理玩家操作
		virtual void OnMyTurn() = 0;
	};

	class Student : public Character
	{
	protected:
		Card* mEquipment = nullptr;
		float max_focus_bonus_ = 0.0f;  // 最大专注度加成（health）
		int stealth_ = 0;  // 隐蔽值（攻击力）
		float lucky_rate_ = 0.5f;  // 幸运一击率
		float lucky_dmg_ = 0.5f;  // 幸运一击伤害
		int defense_ = 0;
		float defense_bonus_ = 0.0f;
		float heal_bonus_ = 0.0f;
		float be_healed_bonus_ = 0.0f;
		float break_val_ = 0.3f;  // 击破效率：对敌方造成伤害的break_val_会用于击破弱点
		float break_val_bonus_ = 0.0f;
	};

	class Teacher : public Character
	{
	protected:
		int max_weakness = 100000;
		int cur_weakness = 100000;
	};
}

#include "characters/CaiShuYou/CaiShuYou.h"