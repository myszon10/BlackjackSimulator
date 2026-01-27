#include <vector>
#include "roundData.h"

class ActionRules {
public:
	std::vector<Action> GetLegalActions(RoundState& s) {
		switch (s.phase) {
		case Betting:
		case InitialDeal:
		case DealerTurn:
		case Settlement:
		case Finished:
			return {};
		case InsuranceOffer:
			return { TakeInsurance, DeclineInsurance };
		case PlayerTurn:
			if (s.playerDone || s.playerHand.IsBust() || s.playerHand.CalculateValue() == 21) {
				return {};
			}
			if (s.playerHand.CardCount() == 2) {
				return { Hit, Stand, Double };
			}
			return { Hit, Stand };
		default:
			return {};
		}
	}
};