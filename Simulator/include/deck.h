#include <vector>
#include "card.h"

class Deck {
public:
	std::vector<Card> Cards;
	Deck();
	Card DrawCard();
	void Shuffle();
	void ResetDeck();	
	void PrintDeck(); // used for debugging
};