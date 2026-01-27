#include <vector>
#include "card.h"

static Suit suits[] = { Hearts, Diamonds, Clubs, Spades };
static Rank ranks[] = { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

class Deck {
public:
	std::vector<Card> Cards;
	Deck();
	Card DrawCard();
	void Shuffle();
	void ResetDeck();	
	void PrintDeck();
};