#pragma once
#include "interfaces.h"
#include "rules.h"
using namespace std;

class Player12Actions : public IActionPolicy {
	/*
	This player imitates a cautious average casino player. Always hits while his hand value is <= 12
	Insurance: Never
	Doubling down:
		- player 11 vs dealer 2-10
		- player 10 vs dealer 2-9
		- player 9 vs dealer 3-6
	*/
public:
	Action SelectAction(RoundState& state, const std::vector<Action>& legalActions) override {
		int playerHandValue = state.playerHands[0].CalculateValue();
		int dealerValue = state.dealerHand.GetFirstCard().GetCardValue();

		// Double down decision
		if (find(legalActions.begin(), legalActions.end(), Double) != legalActions.end()) {
			if (playerHandValue == 11) {
				return Double;
			}
			else if (playerHandValue == 10 && dealerValue >= 2 && dealerValue <= 9) {
				return Double;
			}
			else if (playerHandValue == 9 && dealerValue >= 3 && dealerValue <= 6) {
				return Double;
			}
		}

		// Hit or Stand decision
		if (playerHandValue <= 12) {
			return Hit;
		}
		else {
			return Stand;
		}
	}
};

class Player12Bets : public IBettingPolicy {
	/*
	Betting pattern:
	- base bet -> initial bankroll // D
	- final bet -> base bet + randint(-R, R)
	*/
public:
	Player12Bets(int initialBankroll) : _initialBankroll(initialBankroll) {}

	int DetermineBetAmount(RoundState& state, int playerBalance) override {
		int baseBet = _initialBankroll / D;
		int variation = rand() % (2 * R + 1) - R; // randint(-R, R)

		return baseBet + variation;
	}

private:
	int _initialBankroll;
	int D = Rules::playerD;
	int R = Rules::playerR;
};