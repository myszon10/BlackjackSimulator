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
	bool ShouldHitDealer();
	int GetFirstCardValue(); // dealer's value visible to the player
private:
	std::vector<Card> _cards;
};