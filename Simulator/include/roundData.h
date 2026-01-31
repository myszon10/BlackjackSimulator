#pragma once
#include <vector>
#include "hand.h"
using namespace std;

enum Phase {
	Betting,
	InitialDeal,	// Both hands have 2 cards (dealer has 1 invisible)
	InsuranceOffer, // Possible if dealer has a visible Ace
	PlayerTurn,		// Dealer hole card is still hidden 
	DealerTurn,		// Dealer hole card revealed and draws until stop condition
	Settlement,		// No more cards are drawn
	Finished
};

enum Action { Hit, Stand, Double, Split, TakeInsurance, DeclineInsurance };

struct RoundState {
	Phase phase;

	vector<Hand> playerHands;
	vector<int> handBets; // bet per hand (after doubles etc.)
	vector<bool> handDone;
	vector<bool> handBlackjack; // true -> 2-card 21 (pays 3:2)
	vector<bool> handSplitAces;
	int activeHand = 0;

	Hand dealerHand;

	int baseBet = 0;
	int insuranceBet = 0; // == 0 -> no insurance
	bool dealerBlackjack;
};

struct RoundResult {
	int unitsWon; 
	int unitsBet;
	enum Outcome { PlayerBlackjack, PlayerWon, PlayerLost, Draw } outcome;
};
