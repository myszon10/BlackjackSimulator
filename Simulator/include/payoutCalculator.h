#include "rules.h"
#include "roundData.h"

class PayoutCalculator {
public:
	static RoundResult CalculatePayout(RoundState& s) {
		RoundResult result;
		result.unitsWon = 0;

		// Handle insurance bet
		if (s.insuranceBet > 0 && s.dealerBlackjack) {
			result.unitsWon += s.insuranceBet * 2; // pays 2:1 
		}
		// Determine main outcome
		if (s.playerBlackjack && !s.dealerBlackjack) {
			result.unitsWon += s.bet * Rules::BlackjackPayout;
			result.outcome = RoundResult::PlayerBlackjack;
		} 
		else if (s.dealerBlackjack && !s.playerBlackjack) {
			result.outcome = RoundResult::PlayerLost;
		} 
		else if (s.playerHand.IsBust()) {
			result.outcome = RoundResult::PlayerLost;
		} 
		else if (s.dealerHand.IsBust()) {
			result.outcome = RoundResult::PlayerWon;
		} 
		else {
			int playerValue = s.playerHand.CalculateValue();
			int dealerValue = s.dealerHand.CalculateValue();
			if (playerValue > dealerValue) {
				result.unitsWon += s.bet;
				result.outcome = RoundResult::PlayerWon;
			} 
			else if (playerValue < dealerValue) {
				result.outcome = RoundResult::PlayerLost;
			} 
			else {
				result.outcome = RoundResult::Draw;
			}
		}
		return result;
	}
};