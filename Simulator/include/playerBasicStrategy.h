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
		// Split check first if available
		const bool canSplit = find(legalActions.begin(), legalActions.end(), Split) != legalActions.end();
		if (canSplit) {
			Action sp = DecideSplit(state);
			if (sp == Split) return Split;
		}

		Hand& h = state.playerHands[state.activeHand];
		const int total = h.CalculateValue();
		const int up = state.dealerHand.GetFirstCard().GetCardValue();
		const bool canDouble = find(legalActions.begin(), legalActions.end(), Double) != legalActions.end();

		if (h.isSoft) return DecideSoft(total, up, canDouble, legalActions);
		return DecideHard(total, up, canDouble, legalActions);
	}
private:
	Action DecideSplit(RoundState& state) {
		Hand& h = state.playerHands[state.activeHand];
		if (h.CardCount() != 2) return Hit;

		Rank r = h.GetCard(0).GetRank();
		int up = state.dealerHand.GetFirstCard().GetCardValue();

		// Always split A,A and 8,8
		if (r == Ace || r == Eight) {
			return Split;
		}

		// Never split 10-value and 5,5
		if (r == Ten || r == Jack || r == Queen || r == King || r == Five) {
			return Hit;
		}

		// 2,2 and 3,3: split vs 2-7 else hit
		if (r == Two || r == Three) {
			if (up >= 2 && up <= 7) 
				return Split;

			return Hit;
		}

		// 4,4: split vs 5-6 else hit
		if (r == Four) {
			if (up == 5 || up == 6) 
				return Split;

			return Hit;
		}

		// 6,6: split vs 2-6 else hit
		if (r == Six) {
			if (up >= 2 && up <= 6) 
				return Split;

			return Hit;
		}

		// 7,7: split vs 2-7 else hit
		if (r == Seven) {
			if (up >= 2 && up <= 7) 
				return Split;

			return Hit;
		}

		// 9,9: split vs 2-6 and 8-9; stand vs 7,10,A
		if (r == Nine) {
			if ((up >= 2 && up <= 6) || up == 8 || up == 9) 
				return Split;

			return Stand;
		}

		return Hit;

	}

	Action DecideHard(int total, int up, bool canDouble, const vector<Action>& legalActions) {
		(void)legalActions;
		
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