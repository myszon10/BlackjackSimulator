#include <vector>
#include "card.h"

class Hand {
public:
	Hand();
	void ClearHand();
	void AddCard(Card added);
	int CalculateValue();
	bool IsBust();
	bool Soft17();
	void PrintHand();
	int CardCount();
	Card GetLastCard();
	bool ShouldHitDealer();
private:
	std::vector<Card> _cards;
};