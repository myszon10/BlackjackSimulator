#include <vector>
#include "card.h"

class Hand {
public:
	Hand();
	void ClearHand();
	void AddCard(Card added);
	int CalculateValue();
	bool IsBust();
	void PrintHand();
private:
	std::vector<Card> _cards;
};