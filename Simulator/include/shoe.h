#include <vector>
#include <map>
#include <card.h>

class Shoe {
public:
	Shoe(int numberOfDecks);
	void ResetShoe();
	void ShuffleShoe();
	Card DrawCard();
	int GetDecksRemaining(); // Approx. how many decks still remaining
	std::map<Rank, double> CalculateCardProbabilities();
	void PrintShoe(); // used for debugging
private:
	std::vector<Card> _cards;
	int _numberOfDecks;
	int _drawIndex; // to maintain O(1) for drawing from the beginning
};