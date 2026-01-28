#include <vector>
#include "roundData.h"
#include "playerSession.h"

class ActionRules {
public:
	static std::vector<Action> GetLegalActions(RoundState& s, PlayerSession& session) {
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
			if (s.playerHand.CardCount() == 2 && session.GetPlayerBalance() >= s.bet) {
				return { Hit, Stand, Double };
			}
			return { Hit, Stand };
		default:
			return {};
		}
	}
};