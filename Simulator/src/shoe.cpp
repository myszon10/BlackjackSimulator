#include <cmath>
#include "shoe.h"
#include "deck.h"
#include "rules.h"
#include "rng.h"
using namespace std;

Shoe::Shoe(int numberOfDecks) : _numberOfDecks(numberOfDecks) { 
	_drawIndex = 0;
	ResetShoe();
}

void Shoe::ResetShoe() {
	_drawIndex = 0;
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
		int k = RNG::Int(0, n);
		Card tmp = _cards[k];
		_cards[k] = _cards[n];
		_cards[n] = tmp;
	}
}

DrawResult Shoe::DrawCard() {
	bool reshuffled = false;

	if ((double)_drawIndex >= _cards.size() * Rules::ReshuffleWhenPlayed) {
		this->ResetShoe();
		_drawIndex = 0;
		reshuffled = true;
	}

	Card drawnCard = _cards[_drawIndex++];
	return { drawnCard, reshuffled };
}

int Shoe::GetDecksRemaining() {
	int remaining = (int)_cards.size() - _drawIndex;
	if (remaining <= 0) return 0;
	return (int)ceil((double)remaining / 52.0);
}


void Shoe::PrintShoe() {
	int cnt = 0;
	for (Card c : _cards) {
		cout << ++cnt << ": " << c << endl;
	}
}