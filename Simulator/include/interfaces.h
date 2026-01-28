#pragma once
#include <vector>
#include <algorithm>
#include "roundData.h"

class IActionPolicy {
public:
	virtual Action SelectAction(RoundState& state, const std::vector<Action>& legalActions) = 0;
	virtual ~IActionPolicy() = default;
};

class IBettingPolicy {
public:
	virtual int DetermineBetAmount(RoundState& state, int playerBalance) = 0;
	virtual ~IBettingPolicy() = default;
};

/*
Player strategies (TODO):
- Always hit while <= 17
- Always hit until 12+
- Random decisions
- Basic strategy
- Card counter
*/