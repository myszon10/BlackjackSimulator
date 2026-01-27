#include <iostream>
#include "windows.h"
#include "card.h"
using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);

	Suit suits[] = {Hearts, Diamonds, Clubs, Spades};
	Rank ranks[] = { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			Card card(suits[i], ranks[j]);
			cout << card << " (Value: " << card.GetCardValue() << ")" << endl;
		}
		cout << endl;
	}
}