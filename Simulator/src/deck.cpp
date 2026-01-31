#include <cstdlib>
#include "deck.h"
using namespace std;

Deck::Deck() {
	Cards.clear();
	
	for (int i = 0; i < SUIT_COUNT; i++) {
		for (int j = 0; j < RANK_COUNT; j++) {
			Card card(suits[i], ranks[j]);
			Cards.push_back(card);
		}
	}

	this->Shuffle();
}

Card Deck::DrawCard() {
	if (Cards.empty()) throw runtime_error("No cards left");
	Card c = Cards.back();
	Cards.pop_back();
	return c;
}

void Deck::Shuffle() {
	int n = (int)Cards.size();

	while (n > 1) {
		n--;
		int k = rand() % (n + 1);
		Card tmp = Cards[k];
		Cards[k] = Cards[n];
		Cards[n] = tmp;
	}
}

void Deck::ResetDeck() {
	for (int i = 0; i < SUIT_COUNT; i++) {
		for (int j = 0; j < RANK_COUNT; j++) {
			Card card(suits[i], ranks[j]);
			Cards.push_back(card);
		}
	}

	this->Shuffle();
}

void Deck::PrintDeck() {
	for (Card card : Cards) {
		cout << card << endl;
	}
}