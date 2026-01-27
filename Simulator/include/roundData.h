#include <vector>
#include "hand.h"

enum Phase {
	Betting,
	InitialDeal,	// Both hands have 2 cards (dealer has 1 invisible)
	InsuranceOffer, // Possible if dealer has a visible Ace
	PlayerTurn,		// Dealer hole card is still hidden 
	DealerTurn,		// Dealer hole card revealed and draws until stop condition
	Settlement,		// No more cards are drawn
	Finished
};

enum Action { Hit, Stand, Double, TakeInsurance, DeclineInsurance };

struct RoundState {
	Phase phase;
	Hand playerHand;
	Hand dealerHand;
	int bet;
	int insuranceBet; // == 0 -> no insurance
	bool playerDone;  // true -> player will not act anymore (stood, busted, doubled down)
};
