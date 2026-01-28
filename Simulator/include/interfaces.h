#include <vector>
#include "roundData.h"

class IActionPolicy {
public:
	virtual Action SelectAction(const RoundState& state, const std::vector<Action>& legalActions) = 0;
	virtual ~IActionPolicy() = default;
};

class IBettingPolicy {
public:
	virtual int DetermineBetAmount(const RoundState& state, int playerBalance) = 0;
	virtual ~IBettingPolicy() = default;
};