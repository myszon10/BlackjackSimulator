#pragma once
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
	Card PopLastCard();
	Card GetCard(int index);
	bool ShouldHitDealer();
	Card GetFirstCard();
	bool isSoft = true;
private:
	std::vector<Card> _cards;
};