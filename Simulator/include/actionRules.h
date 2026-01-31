#pragma once
#include <vector>
#include "roundData.h"
#include "playerSession.h"
#include "rules.h"

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
			if (session.GetPlayerBalance() >= s.baseBet / 2) return { TakeInsurance, DeclineInsurance };
			return { DeclineInsurance };
		case PlayerTurn: {
			if (s.playerHands.empty()) return {};

			Hand& h = s.playerHands[s.activeHand];
			if (s.handDone[s.activeHand] || h.IsBust() || h.CalculateValue() == 21) {
				return {};
			}

			std::vector<Action> actions = { Hit, Stand };

			const int bet = s.handBets[s.activeHand];
			const bool canAfford = session.GetPlayerBalance() >= bet;

			// Check for double down possibility (2 cars only, enough balance, not on split-aces hand)
			if (h.CardCount() == 2 && canAfford && !s.handSplitAces[s.activeHand]) {
				actions.push_back(Double);
			}

			// Splitting: 2 cards of same rank, must afford extra bet, not on split-aces hand
			const bool isPair = (h.CardCount() == 2 && h.GetCard(0).GetRank() == h.GetCard(1).GetRank());

			if (isPair && canAfford && (int)s.playerHands.size() < Rules::maxHands && !s.handSplitAces[s.activeHand]) {
				actions.push_back(Split); // Placeholder for Split action
			}

			// Hit/Stand available only if not split-aces
			if (s.handSplitAces[s.activeHand]) {
				return { Stand };
			}

			return actions;
		}
			
		default:
			return {};
		}
	}
};