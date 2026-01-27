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
private:
	std::vector<Card> _cards;
};