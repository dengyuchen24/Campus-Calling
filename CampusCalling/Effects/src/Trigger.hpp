// Trigger.hpp
#pragma once
#include <vector>
#include <map>

class Effect;
using TriggerTiming = unsigned int;

namespace Timing {
	constexpr TriggerTiming ON_ATTACK = 1;
	constexpr TriggerTiming ON_DAMAGED = 2;
	constexpr TriggerTiming ON_HEAL = 3;
	constexpr TriggerTiming ON_TURN_START = 4;
	constexpr TriggerTiming ON_TURN_END = 5;
	constexpr TriggerTiming ON_CARD_USED = 6;
}

class TriggerSystem {
public:
	void registerEffect(TriggerTiming timing, Effect* effect) {
		auto it = listeners.find(timing);
		if (it == listeners.end()) listeners[timing] = { effect };
		else it->second.push_back(effect);
	}
	void unregisterEffect(Effect* effect) {
		for (auto& [t, vec] : listeners)
			vec.erase(std::remove(vec.begin(), vec.end(), effect), vec.end());
	}
	void trigger(TriggerTiming timing, EffectData* data = nullptr) {
		auto it = listeners.find(timing);
		if (it == listeners.end()) return;
		for (auto* eff : it->second) {
			if (eff) eff->trigger(data);  // 传递数据
		}
	}
private:
	std::map<TriggerTiming, std::vector<Effect*>> listeners;
};