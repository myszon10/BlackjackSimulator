#include <cmath>
#include "shoe.h"
#include "deck.h"
#include "rules.h"
using namespace std;

Shoe::Shoe(int numberOfDecks) : _numberOfDecks(numberOfDecks) { 
	ResetShoe();
}

void Shoe::ResetShoe() {
	_cards.clear();

	for (int i = 0; i < _numberOfDecks; i++) {
		Deck singleDeck{};
		_cards.insert(_cards.begin(), singleDeck.Cards.begin(), singleDeck.Cards.end());
	}

	this->ShuffleShoe();
}

void Shoe::ShuffleShoe() {
	int n = _cards.size();

	while (n > 1) {
		n--;
		int k = rand() % (n + 1);
		Card tmp = _cards[k];
		_cards[k] = _cards[n];
		_cards[n] = tmp;
	}
}

Card Shoe::DrawCard() {
	if ((double)_drawIndex >= _cards.size() * Rules::ReshuffleWhenPlayed) {
		this->ResetShoe();
		_drawIndex = 0;
	}

	Card drawnCard = _cards[_drawIndex++];
	return drawnCard;
}

int Shoe::GetDecksRemaining() {
	return (int)ceil((double)_cards.size() / 52);
}

unordered_map<Rank, double> Shoe::CalculateCardProbabilities() {
	unordered_map<Rank, double> remainingCards;
	unordered_map<Rank, double> probabilities;

	for (int i = 0; i < RANK_COUNT; i++) {
		remainingCards[ranks[i]] = _numberOfDecks * SUIT_COUNT;
	}

	// iterating through cards already drawn
	for (int i = 0; i < _drawIndex; i++) { 
		Card currentCard = _cards[i];
		remainingCards[currentCard.GetRank()]--;
	}

	int totalRemainingCards = _cards.size() - _drawIndex;

	for (int i = 0; i < RANK_COUNT; i++) {
		probabilities[ranks[i]] = (double)remainingCards[ranks[i]] / totalRemainingCards;
	}

	return probabilities;
}

void Shoe::PrintShoe() {
	int cnt = 0;
	for (Card c : _cards) {
		cout << ++cnt << ": " << c << endl;
	}
}