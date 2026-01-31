#include "gameEngine.h"
using namespace std;

void GameEngine::ApplyPlayerAction(RoundState& s, Action a, Shoe& shoe, int& totalBet, PlayerSession& session, CounterEvents* events) {
	auto draw = [&](Shoe& shoe, CounterEvents* events) -> DrawResult {
		DrawResult dr = shoe.DrawCard();
		if (events && dr.reshuffled) {
			events->OnReshuffle();
		}
		return dr;
		};

	auto markBlackjackIfTwoCard21 = [&](int handIndex) {
		Hand& hh = s.playerHands[handIndex];
		if (hh.CardCount() == 2 && hh.CalculateValue() == 21) {
			s.handBlackjack[handIndex] = true;
			s.handDone[handIndex] = true;
		}
		};

	Hand& h = s.playerHands[s.activeHand];
	int& bet = s.handBets[s.activeHand];

	switch (a) {
	case Hit: {
		if (s.handSplitAces[s.activeHand]) {
			s.handDone[s.activeHand] = true;
			break;
		}

		auto dr = draw(shoe, events);
		h.AddCard(dr.card);
		if (events) events->OnCardRevealed(dr.card);

		if (h.IsBust() || h.CalculateValue() == 21) 
			s.handDone[s.activeHand] = true;
		break;
	}
	case Stand:
		s.handDone[s.activeHand] = true;
		break;
	case Double: {
		if (s.handSplitAces[s.activeHand]) {
			s.handDone[s.activeHand] = true;
			break;
		}

		if (session.GetPlayerBalance() >= bet) {
			session.UpdatePlayerBalance(-bet);
			totalBet += bet;
			bet *= 2;

			auto dr = draw(shoe, events);
			h.AddCard(dr.card);
			if (events) events->OnCardRevealed(dr.card);

			s.handDone[s.activeHand] = true;
		}
		else {
			s.handDone[s.activeHand] = true;
		}
		break;
	}
	case Split: {
		const int idx = s.activeHand;

		// Copy bet value (NOT reference)
		const int betValue = s.handBets[idx];

		if (session.GetPlayerBalance() < betValue) {
			s.handDone[idx] = true;
			break;
		}

		if (s.playerHands[idx].CardCount() != 2 ||
			s.playerHands[idx].GetCard(0).GetRank() != s.playerHands[idx].GetCard(1).GetRank()) {
			s.handDone[idx] = true;
			break;
		}

		if ((int)s.playerHands.size() >= Rules::maxHands) {
			s.handDone[idx] = true;
			break;
		}

		// Pay additional bet
		session.UpdatePlayerBalance(-betValue);
		totalBet += betValue;

		// Move one card into a new hand
		Card moved = s.playerHands[idx].PopLastCard();
		Hand newHand;
		newHand.AddCard(moved);

		// Push new hand state (this may reallocate!)
		s.playerHands.push_back(newHand);
		s.handBets.push_back(betValue);
		s.handDone.push_back(false);
		s.handBlackjack.push_back(false);
		s.handSplitAces.push_back(false);

		const int newIdx = (int)s.playerHands.size() - 1;

		// Re-acquire references AFTER push_backs
		Hand& h0 = s.playerHands[idx];
		Hand& h1 = s.playerHands[newIdx];

		// Deal one card to each split hand
		auto dr1 = draw(shoe, events);
		h0.AddCard(dr1.card);
		if (events) events->OnCardRevealed(dr1.card);

		auto dr2 = draw(shoe, events);
		h1.AddCard(dr2.card);
		if (events) events->OnCardRevealed(dr2.card);

		// Mark 2-card 21 as blackjack on both hands
		markBlackjackIfTwoCard21(idx);
		markBlackjackIfTwoCard21(newIdx);

		// Split Aces restriction (one card only)
		if (h0.GetCard(0).GetRank() == Ace) {
			s.handSplitAces[idx] = true;
			s.handSplitAces[newIdx] = true;
			s.handDone[idx] = true;
			s.handDone[newIdx] = true;
		}

		break;
	}

	case TakeInsurance: {
		int cost = s.baseBet / 2;
		if (session.GetPlayerBalance() >= cost) {
			session.UpdatePlayerBalance(-cost);
			s.insuranceBet = cost;
			totalBet += cost;
		}
		else {
			s.insuranceBet = 0;
		}
		break;
	}
	case DeclineInsurance:
		s.insuranceBet = 0;
		break;
	}
}

RoundResult GameEngine::playRound(
	RoundState& s, 
	Shoe& shoe, 
	IActionPolicy& actionDecider, 
	IBettingPolicy& betDecider,
	PlayerSession& session,
	CounterEvents* events) {

	// Helper lambda function to call OnReshuffle event
	auto draw = [&](Shoe& shoe, CounterEvents* events) -> DrawResult {
			DrawResult dr = shoe.DrawCard();
			if (events && dr.reshuffled) {
				events->OnReshuffle();
			}
			return dr;
		};

	auto markBlackjackIfTwoCard21 = [&](int handIndex) {
		Hand& hh = s.playerHands[handIndex];
		if (hh.CardCount() == 2 && hh.CalculateValue() == 21) {
			s.handBlackjack[handIndex] = true;
			s.handDone[handIndex] = true;
		}
		};

	int totalBetAmount = 0;

	// Place bet
	s.baseBet = betDecider.DetermineBetAmount(s, session.GetPlayerBalance());
	s.baseBet = max(1, min(s.baseBet, session.GetPlayerBalance()));

	session.UpdatePlayerBalance(-s.baseBet);
	totalBetAmount += s.baseBet;

	// Initialize round
	s.dealerHand.ClearHand();
	s.insuranceBet = 0;
	s.dealerBlackjack = false;
	s.phase = InitialDeal;

	// Initialize player: 1 hand
	s.playerHands.clear();
	s.handBets.clear();
	s.handDone.clear();
	s.handBlackjack.clear();
	s.handSplitAces.clear();

	s.playerHands.push_back(Hand());
	s.handBets.push_back(s.baseBet);
	s.handDone.push_back(false);
	s.handBlackjack.push_back(false);
	s.handSplitAces.push_back(false);
	s.activeHand = 0;

	DrawResult p1, p2, up, hole;

	// Round loop
	while (s.phase != Finished) {
		Action a;

		switch (s.phase) {
		// Dealing starting cards
		case InitialDeal:
			// Drawing cards
			p1 = draw(shoe, events); s.playerHands[0].AddCard(p1.card);
			if (events) events->OnCardRevealed(p1.card);

			up = draw(shoe, events); s.dealerHand.AddCard(up.card);
			if (events) events->OnCardRevealed(up.card);

			p2 = draw(shoe, events); s.playerHands[0].AddCard(p2.card);
			if (events) events->OnCardRevealed(p2.card);

			hole = draw(shoe, events); s.dealerHand.AddCard(hole.card);

			// Player 2 card blackjack check
			markBlackjackIfTwoCard21(0);

			// If dealer shows Ace, offer insurance first
			if (s.dealerHand.GetFirstCard().IsAce()) {
				s.phase = InsuranceOffer;
			}
			else {
				// No insurance possible => check dealer blackjack immediately
				if (s.dealerHand.CalculateValue() == 21) {
					s.dealerBlackjack = true;
					s.phase = Settlement;
				}
				else if (s.handBlackjack[0]) {
					// player blackjack but dealer not blackjack => settle
					s.phase = Settlement;
				}
				else {
					s.phase = PlayerTurn;
				}
			}
			break;

		// Insurance - if dealer has ace, can bet half the pot on insurance 
		// if dealer gets a blackjack
		case InsuranceOffer:
			// Insurance decision uses activeHand = 0
			s.activeHand = 0;
			a = actionDecider.SelectAction(s, ActionRules::GetLegalActions(s, session));
			this->ApplyPlayerAction(s, a, shoe, totalBetAmount, session, events);

			// After insurance decision, dealer checks hole for blackjack now
			if (s.dealerHand.CalculateValue() == 21) {
				s.dealerBlackjack = true;

				if (events) {
					Card holeCard = s.dealerHand.GetLastCard();
					events->OnCardRevealed(holeCard);
				}

				s.phase = Settlement;
			}
			else if (s.handBlackjack[0]) {
				s.phase = Settlement;
			}
			else {
				s.phase = PlayerTurn;
			}
			break;

		case PlayerTurn:
			// Play each hand in order (splits can add new hands)
			for (s.activeHand = 0; s.activeHand < (int)s.playerHands.size(); s.activeHand++) {
				while (!s.handDone[s.activeHand]) {
					auto legal = ActionRules::GetLegalActions(s, session);
					if (legal.empty()) {
						break;
					}

					a = actionDecider.SelectAction(s, legal);
					this->ApplyPlayerAction(s, a, shoe, totalBetAmount, session, events);
				}
			}

			s.phase = DealerTurn;
			break;

		case DealerTurn:
			// Reveal dealoer hole card
			if (events) {
				Card holeCard = s.dealerHand.GetLastCard();
				events->OnCardRevealed(holeCard);
			}

			// Dealer draws
			while (s.dealerHand.ShouldHitDealer()) {
				auto dr = draw(shoe, events);
				s.dealerHand.AddCard(dr.card);
				if (events) events->OnCardRevealed(dr.card);
			}

			s.phase = Settlement;
			break;

		case Settlement: {
			RoundResult result = PayoutCalculator::CalculatePayout(s);
			result.unitsBet = totalBetAmount;
			s.phase = Finished;

			return result;
		}

		default:
			break;
		}
	}

	RoundResult fallback{};
	fallback.unitsWon = 0;
	fallback.unitsBet = totalBetAmount;
	fallback.outcome = RoundResult::Draw;
	return fallback;
}