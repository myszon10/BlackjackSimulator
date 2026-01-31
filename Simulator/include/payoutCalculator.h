#pragma once
#include "rules.h"
#include "roundData.h"

class PayoutCalculator {
public:
	static RoundResult CalculatePayout(RoundState& s) {
		RoundResult result;
		result.unitsWon = 0;
		result.unitsBet = 0;

		// Total bet = all hand bets + insurance bet
		for (int b : s.handBets) {
			result.unitsBet += b;
		}
		result.unitsBet += s.insuranceBet;

		// Insurance
		if (s.insuranceBet > 0 && s.dealerBlackjack) {
			result.unitsWon += s.insuranceBet * 3; // pays 2:1 
		}

		// Dealer blackjack: only hands flagged blackjack push, others lose
		if (s.dealerBlackjack) {
			for (size_t i = 0; i < s.playerHands.size(); i++) {
				int bet = s.handBets[i];
				if (s.handBlackjack[i]) {
					result.unitsWon += bet; // push
				}
			}
			result.outcome = (result.unitsWon == result.unitsBet) ? RoundResult::Draw : RoundResult::PlayerLost;
			return result;
		}

		const bool dealerBust = s.dealerHand.IsBust();
		const int dealerValue = s.dealerHand.CalculateValue();

		bool anyBlackjackWin = false;

		// Evaluate each hand
		for (size_t i = 0; i < s.playerHands.size(); i++) {
			Hand& h = s.playerHands[i];
			int bet = s.handBets[i];

			if (s.handBlackjack[i]) {
				anyBlackjackWin = true;
				result.unitsWon += (int)std::llround(bet * (1.0 + Rules::BlackjackPayout));
				continue;
			}

			if (h.IsBust()) continue;

			if (dealerBust) {
				result.unitsWon += 2 * bet;
				continue;
			}

			int pv = h.CalculateValue();
			if (pv > dealerValue) result.unitsWon += 2 * bet;
			else if (pv == dealerValue) result.unitsWon += bet;
		}

		if (anyBlackjackWin) result.outcome = RoundResult::PlayerBlackjack;
		else if (result.unitsWon > result.unitsBet) result.outcome = RoundResult::PlayerWon;
		else if (result.unitsWon < result.unitsBet) result.outcome = RoundResult::PlayerLost;
		else result.outcome = RoundResult::Draw;

		return result;
	}
};