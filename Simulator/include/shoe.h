#pragma once
#include <vector>
#include <unordered_map>
#include "card.h"
#include "rules.h"

class Shoe {
public:
	Shoe(int numberOfDecks);
	void ResetShoe();
	void ShuffleShoe();
	Card DrawCard();
	int GetDecksRemaining(); // Approx. how many decks still remaining
	void PrintShoe(); // used for debugging
private:
	std::vector<Card> _cards;
	int _numberOfDecks = Rules::NumberOfDecks;
	int _drawIndex; // to maintain O(1) for drawing from the beginning
};