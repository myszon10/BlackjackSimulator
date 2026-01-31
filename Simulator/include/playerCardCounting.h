#pragma once
#include "interfaces.h"
#include "rules.h"
#include "hiLoCounter.h"
#include "shoe.h"
#include "playerBasicStrategy.h"
#include <algorithm>
#include <cmath>
using namespace std;

class CardCounterActions : public IActionPolicy {
	/*
	This player uses basic strategy for Hit/Stand/Double decisions
	Insurance: Only if true count is at leat at 3
	Doubling down: according to basic strategy rules
	*/
public:
	CardCounterActions(HiLoCounter& counter, Shoe& shoe) : _counter(counter), _shoe(shoe) { }

	Action SelectAction(RoundState& state, const std::vector<Action>& legalActions) override {
		if (state.phase == InsuranceOffer) {
			double trueCount = _counter.TrueCount(_shoe.GetDecksRemaining());
			if (trueCount >= 3) {
				return TakeInsurance;
			}
			else {
				return DeclineInsurance;
			}
		}

		// Otherwise, use basic strategy
		return _basic.SelectAction(state, legalActions);
	}

private:
	HiLoCounter& _counter;
	Shoe& _shoe;
	BasicStrategyActions _basic;
};

class CardCounterBets : public IBettingPolicy {
	/*
	Betting pattern:
	- true count = 2 -> 2x base
	- true count = 3 -> 4x base
	- true count >= 4 -> 8x base
	*/
public:
	CardCounterBets(int initialBankroll, HiLoCounter& counter, Shoe& shoe) 
		: _initialBankroll(initialBankroll), _counter(counter), _shoe(shoe) { }

	int DetermineBetAmount(RoundState& state, int playerBalance) override {
		(void)state; // unused

		int base = max(1, _initialBankroll / Rules::playerD);
		double trueCount = _counter.TrueCount(_shoe.GetDecksRemaining());
		int trueCountInt = (int)floor(trueCount);

		int mult = 1;
		if (trueCountInt == 2) mult = 2;
		else if (trueCountInt == 3) mult = 4;
		else if (trueCountInt >= 4) mult = 8;

		int bet = base * mult;
		bet = max(1, min(bet, playerBalance)); // at least 1, at most player's balance)
		return bet;
	}

private:
	int _initialBankroll;
	HiLoCounter& _counter;
	Shoe& _shoe;
};