#include "gameEngine.h"

void GameEngine::ApplyPlayerAction(RoundState& s, Action a, Shoe& shoe, int& totalBet) {
	Card newCard;
	
	switch (a) {
	case Hit:
		newCard = shoe.DrawCard();
		s.playerHand.AddCard(newCard);

		if (s.playerHand.IsBust() || s.playerHand.CalculateValue() == 21) {
			s.playerDone = true;
		}
		break;

	case Stand:
		s.playerDone = true;
		break;

	case Double:
		totalBet += s.bet;
		s.bet *= 2;
		newCard = shoe.DrawCard();
		s.playerHand.AddCard(newCard);
		s.playerDone = true;
		break;

	case TakeInsurance:
		s.insuranceBet = s.bet / 2;
		totalBet += s.insuranceBet;
		break;

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
	PlayerSession& session) {
	int totalBetAmount = 0;

	// Place bet
	s.bet = betDecider.DetermineBetAmount(s, session.GetPlayerBalance());
	totalBetAmount += s.bet;

	// Initialize round
	s.playerHand.ClearHand();
	s.dealerHand.ClearHand();
	s.playerDone = false;
	s.playerBlackjack = false;
	s.dealerBlackjack = false;
	s.insuranceBet = 0;
	s.phase = InitialDeal;

	// Round loop
	while (s.phase != Finished) {
		Action a;
		switch (s.phase) {
		// Dealing starting cards
		case InitialDeal:
			s.playerHand.AddCard(shoe.DrawCard());
			s.dealerHand.AddCard(shoe.DrawCard()); // upcard
			s.playerHand.AddCard(shoe.DrawCard());
			s.dealerHand.AddCard(shoe.DrawCard()); // hole card

			if (s.playerHand.CalculateValue() == 21) {
				s.playerDone = true;
				s.playerBlackjack = true;
				s.phase = Settlement;
				break;
			}
			else if (s.dealerHand.CalculateValue() == 21) {
				s.dealerBlackjack = true;
				s.playerDone = true;
				s.phase = Settlement;
				break;
			}

			if (s.dealerHand.GetLastCard().IsAce()) {
				s.phase = InsuranceOffer;
			}
			else {
				s.phase = PlayerTurn;
			}
			break;

		// Insurance - if dealer has ace, can bet half the pot on insurance 
		// if dealer gets a blackjack
		case InsuranceOffer:
			a = actionDecider.SelectAction(s, ActionRules::GetLegalActions(s, session));
			this->ApplyPlayerAction(s, a, shoe, totalBetAmount);
			s.phase = PlayerTurn;
			break;

		case PlayerTurn:
			while (!s.playerDone) {
				auto legal = ActionRules::GetLegalActions(s, session);
				if (legal.empty()) {
					break;
				}

				a = actionDecider.SelectAction(s, legal);
				this->ApplyPlayerAction(s, a, shoe, totalBetAmount);
			}
			s.phase = DealerTurn;
			break;

		case DealerTurn:
			while (s.dealerHand.ShouldHitDealer()) {
				s.dealerHand.AddCard(shoe.DrawCard());
			}

			if (s.dealerHand.CalculateValue() == 21) {
				s.dealerBlackjack = true;
			}

			s.phase = Settlement;
			break;

		case Settlement:
			RoundResult result = PayoutCalculator::CalculatePayout(s);
			result.unitsBet = totalBetAmount;
			s.phase = Finished;

			return result;
		}
	}
}