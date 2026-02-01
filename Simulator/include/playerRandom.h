#pragma once
#include <random>
#include "interfaces.h"
#include "rules.h"
#include "rng.h"
using namespace std;

class PlayerRandomActions : public IActionPolicy {
	/*
	This player always makes a random choice
	Insurance: Never
	Doubling down: doubles down with the probability defined in Rules::randomDoubleDownChance
	*/
public:
	Action SelectAction(RoundState& state, const std::vector<Action>& legalActions) override {
		// Double down decision
		if (find(legalActions.begin(), legalActions.end(), Double) != legalActions.end()) {
			if (RNG::Chance(Rules::randomDoubleDownChance)) return Double;
		}
		// Hit or Stand decision
		return RNG::Chance(Rules::randomHitChance) ? Hit : Stand;
	}
private:
	// Returns 1 with probability p, otherwise 0
	int randomWithProb(double p) {
		double rndDouble = (double)rand() / RAND_MAX;
		return rndDouble < p;
	}
};

class PlayerRandomBets : public IBettingPolicy {
	/*
	Betting pattern:
	- base bet -> initial bankroll // D
	- final bet -> base bet + randint(-R, R)
	*/
public:
	PlayerRandomBets(int initialBankroll) : _initialBankroll(initialBankroll) {}

	int DetermineBetAmount(RoundState& state, int playerBalance) override {
		int baseBet = _initialBankroll / D;
		int variation = RNG::Int(-R, R);

		return baseBet + variation;
	}

private:
	int _initialBankroll;
	int D = Rules::playerD;
	int R = Rules::playerR;
};