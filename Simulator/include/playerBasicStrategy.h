#pragma once
#include "interfaces.h"
#include "rules.h"
#include <algorithm>
using namespace std;

class BasicStrategyActions : public IActionPolicy {
	/*
	This player uses basic strategy for decision making.
	Insurance: Never
	Doubling down: according to basic strategy rules
	*/
public:
	Action SelectAction(RoundState& state, const std::vector<Action>& legalActions) override {
		const int total = state.playerHand.CalculateValue();
		const int up = state.dealerHand.GetFirstCardValue();
		const bool canDouble = find(legalActions.begin(), legalActions.end(), Double) != legalActions.end();

		if (state.playerHand.isSoft) {
			return DecideSoft(total, up, canDouble, legalActions);
		}
		else {
			return DecideHard(total, up, canDouble, legalActions);
		}
	}
private:
	Action DecideHard(int total, int up, bool canDouble, const vector<Action>& legalActions) {
		// Stand on 17+
		if (total >= 17) return Stand;

		// 13-16: stand vs 2-6, else hit
		if (total >= 13 && total <= 16) {
			if (up >= 2 && up <= 6) return Stand;
			return Hit;
		}

		// 12: stand vs 4-6, else hit
		if (total == 12) {
			if (up >= 4 && up <= 6) return Stand;
			return Hit;
		}

		// 11: double vs 2-A 
		if (total == 11) {
			if (canDouble) return Double;
			return Hit;
		}

		// 10: double vs 2-9 else hit
		if (total == 10) {
			if (up >= 2 && up <= 9 && canDouble) return Double;
			return Hit;
		}

		// 9: double vs 3-6 else hit
		if (total == 9) {
			if (up >= 3 && up <= 6 && canDouble) return Double;
			return Hit;
		}

		// 8 or less: hit
		return Hit;
	}

	Action DecideSoft(int total, int up, bool canDouble, const vector<Action>& legalActions) {
		// Soft 19+ always stand
		if (total >= 19) return Stand;

		// Soft 18 rules:
		// - double vs 3-6 (if allowed), otherwise stand
		// - stand vs 2,7,8
		// - hit vs 9,10,A
		if (total == 18) {
			if (up >= 3 && up <= 6) {
				if (canDouble) return Double;
				return Stand;
			}
			if (up == 2 || up == 7 || up == 8) return Stand;
			return Hit; // 9,10,A
		}

		// Soft 17: double vs 3-6 else hit
		if (total == 17) {
			if (up >= 3 && up <= 6 && canDouble) return Double;
			return Hit;
		}

		// Soft 15-16: double vs 4-6 else hit
		if (total == 15 || total == 16) {
			if (up >= 4 && up <= 6 && canDouble) return Double;
			return Hit;
		}

		// Soft 13-14: double vs 5-6 else hit
		if (total == 13 || total == 14) {
			if (up >= 5 && up <= 6 && canDouble) return Double;
			return Hit;
		}

		return Hit;
	}
};

class BasicStrategyBets : public IBettingPolicy {
	// Always bets the base amount: initial bankroll // D
public:
	BasicStrategyBets(int initialBankroll) : _initialBankroll(initialBankroll) {}

	int DetermineBetAmount(RoundState& state, int playerBalance) override {
		int base = _initialBankroll / Rules::playerD;
		base = max(1, min(base, playerBalance));
		return base;
	}

private:
	int _initialBankroll;
};